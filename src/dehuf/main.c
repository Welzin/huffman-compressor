/*
 * Fichier : main.c
 * Auteur : Welzin
 * Projet Huffman - Décompression
 *
 * Ce fichier contient l'entrée du programme (la fonction main).
 * La consigne demande d'envoyer le contenu du fichier décompressé sur la sortie
 * standard, mais avoir la possibilité d'un autre fichier de destination serait 
 * bienvenu. 
 *
 * Cependant, je vois l'intérêt que propose de « juste » afficher le contenu sur
 * la console, de toute façon, c'est facilement enregistrable dans un fichier.
 * Je ne vais donc pas le faire dans cette partie, mais plutôt automatiser ça
 * par un script bash.
 */
#include "dehuf.h"
#include "../core/utils.h"
#include "../core/arbre.h"

#include <stdlib.h>

int main(int argc, char **argv) {
    // 2 arguments : nom du programme + nom du fichier.
    if (argc != 2) {
        affiche_type(Error);
        printf("Impossible d'appeler le programme %s avec %i arguments. Utilisation : %s fichier\n", argv[0], argc, argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* src = argv[1];

    // Étape 1 : Récupération des feuilles grâce au tableau des fréquences
    size_t indice, bits = 0;
    ListeNoeud *feuilles = reconstruction_frequence(src, &bits, &indice);
    if (feuilles == NULL) {
        exit(EXIT_FAILURE);
    }

    // Étape 2 : Reconstruction de l'arbre
    Noeud *racine = creer_arbre(feuilles);

    // Étape 3 : Décompression du fichier
    int resultat = decompresser(src, racine, indice, bits);
    dealloue(racine);

    if (!resultat) {
        affiche_type(Error);
        printf("Impossible de décompresser le fichier.\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}