#include "liste-noeud.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

Noeud *creer_noeud(unsigned char caractere, size_t occurrences, Noeud *eG, Noeud *eD) {
    Noeud *noeud = (Noeud *) malloc(sizeof(Noeud));
    noeud->c = caractere;
    noeud->occ = occurrences;
    noeud->eG = eG;
    noeud->eD = eD;
    return noeud;
}

ListeNoeud* creer_LN(Noeud *info) {
    ListeNoeud *liste = (ListeNoeud *) malloc(sizeof(ListeNoeud));
    liste->info = info;
    liste->succ = NULL;
    return liste;
}

Noeud *trouve_LN(ListeNoeud *listeNoeud, unsigned char caractere) {
    // On ne peut pas trouver de Noeud dans une liste qui n'en possède pas.
    if (listeNoeud == NULL) {
        return NULL;
    }

    int trouve = 0;
    while (!trouve && listeNoeud != NULL) {
        trouve = listeNoeud->info->c == caractere;
        if (!trouve) {
            listeNoeud = listeNoeud->succ;
        }
    }

    return trouve ? listeNoeud->info : NULL;
}

ListeNoeud* insertion_LN(ListeNoeud *listeNoeud, Noeud *noeud) {
    // Gardien : Si le Noeud est NULL, il n'y a rien à insérer
    if (noeud == NULL) {
        return NULL;
    }
    ListeNoeud *element = creer_LN(noeud);
    // Gardien : on crée la liste si elle n'existe pas.
    if (listeNoeud == NULL) {
        return element;
    }

    ListeNoeud *returnValue = NULL;
    if (noeud->occ > listeNoeud->info->occ) {
        // Insertion après le début, il faut chercher où insérer le Noeud.
        while (listeNoeud->succ != NULL && listeNoeud->succ->info->occ < noeud->occ) {
            listeNoeud = listeNoeud->succ;
        }
        ListeNoeud *tmp = listeNoeud->succ;
        listeNoeud->succ = element;
        listeNoeud->succ->succ = tmp;
    }
    else {
        // Insertion au début, on crée un nouvel élément dans la liste et 
        // remplace l'autre noeud au début de la liste.
        element->succ = listeNoeud;
        returnValue = element;
    }

    return returnValue;
}

void insertion_fin_LN(ListeNoeud *listeNoeud, Noeud *noeud) {
    if (listeNoeud == NULL) {
        return;
    }

    while (listeNoeud->succ != NULL) {
        listeNoeud = listeNoeud->succ;
    }
    listeNoeud->succ = creer_LN(noeud);
}

ListeNoeud* suppression_LN(ListeNoeud *liste, Noeud *noeud) {
    // Gardien : dans ces deux cas, on ne peut rien faire
    if (liste == NULL || noeud == NULL) {
        return NULL;
    }

    ListeNoeud *returnValue = NULL;
    if (liste->info != noeud) {
        while (liste->succ != NULL && liste->succ->info != noeud) {
            liste = liste->succ;
        }
        if (liste->succ != NULL) {
            ListeNoeud *tmp = liste->succ;
            liste->succ = liste->succ->succ;
            free(tmp);
        }
    }
    else {
        returnValue = liste->succ;
        free(liste);
    }
    return returnValue;
}

ListeNoeud* suppression_debut_LN(ListeNoeud *liste) {
    ListeNoeud *debut = liste->succ;
    free(liste);
    return debut;
}

ListeNoeud* tri_LN(ListeNoeud *liste) {
    if (liste == NULL || liste->succ == NULL) {
        return liste;
    }

    ListeNoeud *nouvelleListe = NULL;

    while (liste != NULL) {
        ListeNoeud* top = insertion_LN(nouvelleListe, liste->info);
        if (top != NULL) nouvelleListe = top;
        liste = liste->succ;
    }

    return nouvelleListe;
}

int est_feuille(Noeud *noeud) {
    return (noeud->eG == NULL && noeud->eD == NULL);
}

size_t max_char(ListeNoeud *liste) {
    if (liste == NULL) {
        return 0;
    }

    size_t max = 0;
    while (liste != NULL) {
        if ((size_t) liste->info->c > max) {
            max = (size_t) liste->info->c;
        }
        liste = liste->succ;
    }

    return max;
}

void libere_LN(ListeNoeud *liste) {
    while (liste != NULL) {
        ListeNoeud *tmp = liste;
        liste = liste->succ;
        free(tmp);
    }
}

size_t taille_LN(ListeNoeud *liste) {
    if (liste == NULL) {
        return 0;
    }

    size_t cpt = 0;
    while (liste != NULL) {
        liste = liste->succ;
        ++cpt;
    }
    return cpt;
}

void afficher_noeud(Noeud *noeud) {
    if (noeud->c != '\0') {
        printf("%c, %li\n", noeud->c, noeud->occ);
    }
    else {
        printf("%li\n", noeud->occ);
    }
}

void afficher_LN(ListeNoeud *listeNoeud) {
    if (listeNoeud == NULL) {
        printf("[]\n");
        return;
    }

    printf("[\n");
    size_t cpt = 0;
    while (listeNoeud != NULL) {
        printf("\t%li : ", cpt++);
        afficher_noeud(listeNoeud->info);
        listeNoeud = listeNoeud->succ;
    }
    printf("]\n");
}