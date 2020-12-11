#include "dehuf.h"

#include <stdio.h>

int decompresser(const char* filename, Noeud *racine, size_t indice, size_t bits) {
    if (racine == NULL) {
        return 0;
    }

    FILE *fd = fopen(filename, "rb");
    if (fd == NULL) {
        return 0;
    }

    // On bouge le curseur à l'indice donné
    fseek(fd, indice, SEEK_SET);
    Noeud *noeud = racine;
    size_t cpt = 0;
    unsigned char c;
    while(fread(&c, 1, 1, fd) == 1) {
        // Parcours de l'arbre bit par bit. 
        for (int i = 7; i >= 0; --i) {
            int test = c & (1 << i);
            // Si test, ça veut dire que le bit (i + 1) est à 1, on va donc à
            // droite.
            if (test) {
                if (noeud->eD != NULL) {
                    noeud = noeud->eD;
                }
            }
            // Sinon, c[i + 1] == 0, donc on va à gauche.
            else {
                if (noeud->eG != NULL) {
                    noeud = noeud->eG;
                }
            }

            if (est_feuille(noeud)) {
                printf("%c", noeud->c);
                noeud = racine;
            }

            // Si le compte de bits est supérieur au nombre intéressant, on 
            // arrête le programme.
            ++cpt;
            if (cpt >= bits) {
                break;
            }
        }
    }

    fclose(fd);
    return 1;
}