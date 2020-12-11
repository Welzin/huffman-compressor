#include "codage.h"

#include <stdlib.h>
#include <stdio.h>

Encodage* creer_encodage(Noeud *racine, size_t maxCharValue) {
    Encodage *encodages = (Encodage *) malloc((maxCharValue + 1) * sizeof(Encodage));
    Encodage encodage;
    encodage.longueur = 0;
    encodage.binaire = (unsigned char) 0;

    // Instanciation de tous les éléments du tableau à l'encodage nul 
    for (size_t i = 0; i <= maxCharValue; ++i) {
        encodages[i] = encodage;
    }

    // Peuplage du tableau avec les vraies valeurs
    encodage_aux(encodage, racine, encodages);
    return encodages;
}

void encodage_aux(Encodage encodage, Noeud *noeud, Encodage *tableau) {
    // Gardien
    if (noeud == NULL) {
        return;
    }

    if (est_feuille(noeud)) {
        tableau[noeud->c] = encodage;
    }
    else {
        encodage.longueur += 1;
        encodage.binaire <<= 1;
        encodage_aux(encodage, noeud->eG, tableau);
        encodage.binaire |= 1;
        encodage_aux(encodage, noeud->eD, tableau);
    }
}

void affiche_encodage(Noeud *noeud, Encodage *encodages) {
    if (noeud == NULL) {
        return;
    }

    if (est_feuille(noeud)) {
        Encodage actuel = encodages[noeud->c];
        printf("codeChar(%c) = ", noeud->c);
        for (size_t i = actuel.longueur; i > 0; --i) {
            size_t bits = i - 1;
            printf("%lld", (actuel.binaire & (1 << bits)) >> bits);
        }
        printf("\n");
    }
    else {
        affiche_encodage(noeud->eG, encodages);
        affiche_encodage(noeud->eD, encodages);
    }
}

void affiche_moyenne(Noeud *racine, Encodage *encodages) {
    printf("Longueur moyenne de codage : %.2f bits\n", (float)calcul_total(racine, encodages) / racine->occ);
}

size_t calcul_total(Noeud *noeud, Encodage *encodages) {
    if (noeud == NULL) {
        return 0;
    }

    if (est_feuille(noeud)) {
        return encodages[noeud->c].longueur * noeud->occ;
    }
    else {
        return calcul_total(noeud->eG, encodages) + calcul_total(noeud->eD, encodages);
    }
}