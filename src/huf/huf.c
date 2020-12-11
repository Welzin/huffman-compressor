#include "huf.h"
#include "../core/utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int enregistrer_frequences(ListeNoeud *feuilles, const char* dst) {
    if (feuilles == NULL) {
        return 0;
    }

    FILE *fd = fopen(dst, "w");

    if (fd == NULL) {
        affiche_type(Error);
        printf("Impossible de créer ou d'ouvrir le fichier %s.\n", dst);
        return 0;
    }

    // Compteur de la taille de la chaîne à écrire.
    size_t cpt = 0;

    // Instanciation du tableau de chaînes qui prendra toutes nos fréquences
    size_t taille_tableau = taille_LN(feuilles);
    char *contenu[taille_tableau + 1];
    size_t i = 0;

    while (feuilles != NULL) {
        char *chiffre = itoa(feuilles->info->occ);

        // Taille du chiffre + caractère + 1 séparateur + \0
        size_t taille = strlen(chiffre) + 3;
        char *description = (char *) malloc(taille * sizeof(char)); 
        description[0] = feuilles->info->c;
        for (size_t i = 0; i < strlen(chiffre); ++i) {
            description[i + 1] = chiffre[i];
        }
        description[taille - 2] = DELIM; 
        description[taille - 1] = '\0';

        // On ajoute 1 car on compte juste le nombre de noeuds enregistrés, pas
        // la taille totale de la chaîne
        cpt += /* taille - */ 1;
        contenu[i++] = description;
        feuilles = feuilles->succ;

        free(chiffre);
    }
    contenu[taille_tableau] = NULL;

    // Écriture de la taille de la chaîne dans le fichier
    fprintf(fd, "%li%c", cpt, DELIM);

    // Écriture de la fréquence de chaque caractère dans le fichier
    for (size_t i = 0; contenu[i] != NULL; ++i) {
        // Si le caractère à mettre est \0, on s'arrange pour pouvoir l'écrire
        // lui et son nombre d'occurrences.
        if (contenu[i][0] == '\0') {
            fclose(fd);
            fd = fopen(dst, "ab");
            char w = 0;
            fwrite(&w, 1, 1, fd);
            fclose(fd);
            fd = fopen(dst, "a");
            memmove(contenu[i], contenu[i] + 1, strlen(contenu[i] + 1));
            contenu[i][strlen(contenu[i]) - 1] = '\0';
        }
        fprintf(fd, "%s", contenu[i]);
        free(contenu[i]);
    }

    fclose(fd);

    return 1;
}

void ecrire_taille(Noeud *racine, Encodage *tableau, const char* dst) {
    FILE *fd = fopen(dst, "a");

    if (fd == NULL) {
        affiche_type(Error);
        printf("Impossible de créer ou d'ouvrir le fichier %s.\n", dst);
        return;
    }

    fprintf(fd, "%li%c", calcul_total(racine, tableau), DELIM);
    fclose(fd);
}

State compresser(Encodage *tableau, const char* src, const char* dst) {
    if (tableau == NULL) {
        return Failure;
    }

    FILE *fdSrc = fopen(src, "r");
    FILE *fdDst = fopen(dst, "ab");

    if (fdSrc == NULL || fdDst == NULL) {
        affiche_type(Error);
        printf("Impossible d'accéder aux fichiers %s et %s", src, dst);
        return Failure;
    }

    // Instanciation des variables dont on a besoin dans la boucle principale
    char buffer = (char) 0;
    int c, currentBit = 0;
    while ((c = fgetc(fdSrc)) != EOF) {
        Encodage encodage = tableau[c];
        
        // Calcul des bits à compléter dans le buffer actuel
        int bitsMissing = 8 - currentBit;

        // Si la longueur de l'encodage est inférieur aux nombre de bits 
        // manquants, il n'y a qu'à les ajouter à la fin du buffer actuel.
        if (encodage.longueur <= bitsMissing) {
            buffer <<= encodage.longueur;
            buffer |= encodage.binaire;
            currentBit += encodage.longueur;
        }
        // Sinon, il va falloir ajouter le nombre de bits manquants et remplir
        // le reste sur le(s) prochain(s) bit(s)
        else {
            buffer <<= bitsMissing;
            size_t longueur = encodage.longueur - bitsMissing;
            // Compléter le bit actuel
            buffer |= (encodage.binaire >> longueur);

            // Enregistrer le bit dans le fichier
            fwrite(&buffer, 1, 1, fdDst);
            buffer = (char) 0;

            // Écriture des bits suivants si la longueur est supérieur à 1 octet
            while (longueur > 8) {
                longueur -= 8;
                char byte = (encodage.binaire >> longueur);
                fwrite(&byte, 1, 1, fdDst);
            }

            // Le reste du codage se met sur l'octet suivant
            buffer |= encodage.binaire;
            currentBit = longueur;
        }

        // Si le nombre de bit = 1 octet, on écrit dans le fichier
        if (currentBit == 8) {
            fwrite(&buffer, 1, 1, fdDst);
            currentBit = 0;
            buffer = (char) 0;
        }
    }

    // Si l'octet n'est pas fini, on le complète avec des 0
    if (currentBit != 0) {
        while (currentBit != 8) {
            buffer <<= 1;
            ++currentBit;
        }
        fwrite(&buffer, 1, 1, fdDst);
    }

    size_t tailleSrc = ftell(fdSrc);
    size_t tailleDst = ftell(fdDst);

    fclose(fdSrc);
    fclose(fdDst);

    float gain = 100 - ((float)(100 * tailleDst) / tailleSrc);

    printf("Taille originelle : %li, taille compressée : %li, gain : %.2f%c\n", tailleSrc, tailleDst, gain, '%');

    // Si le gain est négatif, le fichier compressé est plus lourd que le fichier source, on avertit
    // l'utilisateur. Sinon, tout est bon.
    return (gain < 0) ? Size : Good;
}