/*
 * Fichier : main.c
 * Auteur : Welzin
 * Projet Huffman - Compression
 *
 * Ce fichier contient l'entrée du programme (la fonction main).
 * Je pense que c'est une des manière les plus optimisées de faire. En effet,
 * la compression est en O(n), où n est la taille du fichier qu'on veut compresser.
 * C'est du O(n) car l'arbre n'est pas parcouru dans la boucle de compression,
 * donc un certains temps d'opération est économisé.
 *
 * Je ne vois qu'une seule autre optimisation à potentiellement implémenter :
 * récupérer le contenu du fichier dans une chaîne de caractères, et opérer sur 
 * celle-ci pour tout le reste du programme. Cependant, ça dépendrait de la taille
 * du fichier ; s'il est trop gros, il vaut mieux le parcourir 2 fois, qu'allouer
 * plusieurs gigas en mémoire. Ça demanderait quelques changements, bien sûr
 * faisables assez facilement, car le code est flexible, mais aussi beaucoup 
 * d'ajustements, et le code serait peut-être moins clair, donc je vais préférer
 * la clarté à l'optimisation totale.
 */
#include "huf.h"
#include "../core/utils.h"
#include "../core/arbre.h"
#include "../core/liste-noeud.h"
#include "../core/codage.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 3 || argc > 5) {
        affiche_type(Error);
        printf("Impossible d'appeler le programme %s avec %i arguments. Utilisation : %s source dest\n", argv[0], argc, argv[0]);
        exit(EXIT_FAILURE);
    }

    int silent = 0, skip = 0;
    int argsAvantFichiers = 0;
    if (chaines_egales(argv[1], "--skip") || chaines_egales(argv[1], "--silent")) {
        argsAvantFichiers = 1;
        if (chaines_egales(argv[1], "--skip")) {
            skip = 1;
            if (chaines_egales(argv[2], "--silent")) {
                silent = 1;
                argsAvantFichiers = 2;
            }
        } 
        else {
            silent = 1;
            if (chaines_egales(argv[2], "--skip")) {
                skip = 1;
                argsAvantFichiers = 2;
            }
        }
    }

    if (argc - 1 < argsAvantFichiers + 2) {
        affiche_type(Error);
        printf("Vous avez spécifié des options mais pas le nom des fichiers sources et destination.\n");
        exit(EXIT_FAILURE);
    }

    const char* src = argv[argsAvantFichiers + 1];
    const char* real_dst = argv[argsAvantFichiers + 2];

    // Création du nom du fichier de destination.
    char dst[strlen(real_dst) > 22 ? strlen(real_dst) : 22];
    strcpy(dst, real_dst);

    // Si le fichier source et de destination est le même, il faut créer un fichier temporaire pour ne pas perdre les données.
    if (chaines_egales(src, real_dst)) {
        affiche_type(Warning);
        printf("La source et la destination sont le même fichier. Voulez-vous tout de même continuer (le fichier source sera écrasé) ? [y/n] ");
        char response;
        scanf("%c", &response);
        if (response == 'O' || response == 'o' || response == 'y' || response == 'Y') {
            affiche_type(Info);
            printf("Le fichier %s sera bien écrasé.\n", src);

            // Création du nom du fichier temporaire et copie dans le variable de destination
            char tmpName[22];
            strcpy(tmpName, "tmp_build_file_XXXXXX\0");
            mkstemp(tmpName);
            strcpy(dst, tmpName);
        }
        else {
            affiche_type(Info);
            printf("Arrêt du programme.\n");
            exit(EXIT_SUCCESS);
        }
    }

    // Étape 1 : Calcul de la fréquence de chaque caractère du fichier
    ListeNoeud *feuilles = calcul_frequence(src);
    size_t maxChar = max_char(feuilles);

    // Si la liste des feuilles n'a pas pu être calculée, c'est que le fichier 
    // est vide, donc il n'y a rien à compresser.
    if (feuilles == NULL) {
        affiche_type(Error);
        printf("Impossible de compresser, le fichier %s est vide.\n", src);
        exit(EXIT_FAILURE);
    }

    // Étape 2 : Enregistrement de la fréquence de chaque caractère.
    int resultat = enregistrer_frequences(feuilles, dst);

    if (!resultat) {
        affiche_type(Error);
        printf("Un problème est survenu lors de l'écriture dans le fichier de destination. Arrêt du programme.");
        exit(EXIT_FAILURE);
    }

    // Affichage de la liste des caractères et de leur probabilité d'apparition
    if (!silent) {
        affiche_type(Info);
        afficher_LN(feuilles);
    }

    // Étape 3 : Création de l'arbre
    Noeud *racine = creer_arbre(feuilles);

    if (!silent) {
        affiche_type(Info);
        printf("\n");
        afficher_arbre(racine, 0);
    }

    // Étape 4 : Création du tableau d'encodage
    Encodage *tableau = creer_encodage(racine, maxChar);

    if (!silent) {
        affiche_type(Info);
        printf("\n");
        affiche_encodage(racine, tableau);
        affiche_moyenne(racine, tableau);
    }

    // Étape 5 : Ajouter la taille totale du fichier après le tableau des fréquences
    ecrire_taille(racine, tableau, dst); 

    // Étape 6 : Compression du fichier
    State resultatCompression = compresser(tableau, src, dst);

    // Libération de la mémoire
    free(tableau);
    dealloue(racine);

    // Vérification que tout s'est bien passé à la compression.
    if (resultatCompression == Failure) {
        affiche_type(Error);
        printf("Il y a eu un problème lors de la compression.\n");
        exit(EXIT_FAILURE);
    }
    if (resultatCompression == Size) {
        if (skip) {
            remove(dst);
            affiche_type(Info);
            printf("Fichier compressé plus gros que l'original. Arrêt du programme.\n");
            exit(2);
        }
        affiche_type(Warning);
        printf("Le fichier compressé est plus gros que le fichier source. Voulez-vous quand même le garder ? [y/n] ");
        char c;
        // Besoin de l'espace au cas où
        scanf(" %c", &c);
        if (!(c == 'O' || c == 'o' || c == 'y' || c == 'Y')) {
            remove(dst);
            affiche_type(Info);
            printf("Arrêt du programme.\n");
            exit(EXIT_SUCCESS);
        }
    }

    // Si un fichier temporaire a été créé, on écrase le fichier source
    if (!chaines_egales(dst, real_dst)) {
        rename(dst, real_dst);
    }

    affiche_type(Info);
    printf("Compression terminée avec succès.\n");

    return EXIT_SUCCESS;
}