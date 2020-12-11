#include "arbre.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

ListeNoeud* calcul_frequence(const char* filename) {
    // Ouverture du fichier source
    FILE *fd = fopen(filename, "rb");

    if (fd == NULL) {
        affiche_type(Error);
        printf("Le programme ne peut pas accéder au fichier source %s !\n", filename);
        return NULL;
    }

    int caracteres[256];
    for (size_t i = 0; i < 256; ++i) {
        caracteres[i] = 0;
    }

    unsigned char c = 0;
    while (fread(&c, 1, 1, fd) == 1) {
        caracteres[(int)c]++;
    }

    fclose(fd);

    // Parcours caractère par caractère
    ListeNoeud *listeNoeud = NULL;
    for (size_t i = 0; i < 256; ++i) {
        if (caracteres[i] != 0) {
            ListeNoeud* res = insertion_LN(listeNoeud, creer_noeud((unsigned char) i, caracteres[i], NULL, NULL));
            if (res != NULL) {
                listeNoeud = res;
            }
        }
    }

    return listeNoeud;
}

ListeNoeud* reconstruction_frequence(const char* filename, size_t *bits, size_t *indice) {
    // Ouverture du fichier source
    FILE *fd = fopen(filename, "r");

    if (fd == NULL) {
        affiche_type(Error);
        printf("Le programme ne peut pas accéder au fichier source %s !\n", filename);
        return NULL;
    }

    // Récupération de la taille du tableau de fréquence.
    size_t taille = 0; 

    // Si !good, soit on n'a pas trouvé de délimiteur, soit le format n'est pas
    // bon, dans les deux cas, on arrête ici.
    if(!recuperer_occurrences(fd, &taille)) {
        affiche_type(Error);
        printf("Le fichier %s n'est pas un fichier compressé par cet algorithme.\n", filename);
        return NULL;
    }

    // Création d'un noeud au début car on insère à la fin.
    ListeNoeud *feuilles = creer_LN(creer_noeud('\0', 0, NULL, NULL));

    // Finalement, on peut lire le tableau des fréquences.
    size_t cpt = 0;
    while (cpt < taille) {
        // Récupération du caractère, 
        unsigned char c = (unsigned char) fgetc(fd); 

        // Nombre d'occurrences
        size_t occ = 0;
        if (!recuperer_occurrences(fd, &occ)) {
            affiche_type(Error);
            printf("Le fichier %s est corrompu.\n", filename);
            return NULL;
        }

        insertion_fin_LN(feuilles, creer_noeud(c, occ, NULL, NULL));

        ++cpt;
    }

    // Suppression du noeud NULL ajouté.
    free(feuilles->info);
    feuilles = suppression_debut_LN(feuilles);

    // Récupération du nombre de bits total pertinents :
    if(!recuperer_occurrences(fd, bits)) {
        affiche_type(Error);
        printf("Le fichier %s n'est pas un fichier compressé par cet algorithme.\n", filename);
        return NULL;
    }

    *indice = ftell(fd);
    fclose(fd);

    return feuilles;
}

Noeud* creer_arbre(ListeNoeud *feuilles) {
    // Gardien : Si la liste est nulle, il n'y a rien a faire
    if (feuilles == NULL) {
        return NULL;
    }

    // Si elle ne comporte qu'un seul élément, on est obligés de créer un faux 
    // Noeud pour que quelque chose soit marqué dans le fichier.
    if (feuilles->succ == NULL) {
        return creer_noeud(feuilles->info->c, feuilles->info->occ, NULL, feuilles->info);;
    }

    while (feuilles->succ != NULL) {
        // Récupération des deux Noeuds avec le moins d'occurrences
        Noeud *eG = feuilles->info;
        Noeud *eD = feuilles->succ->info;

        // Retirer 2 fois au début, vu que c'est les 2 premiers éléments de la liste.
        feuilles = suppression_debut_LN(feuilles);
        feuilles = suppression_debut_LN(feuilles);

        // Création du parent et insertion dans la liste triée. Le caractère 
        // n'est pas important. On met quand même à \0 au lieu de ne pas l'initialiser. 
        Noeud *parent = creer_noeud('\0', eG->occ + eD->occ, eG, eD);
        ListeNoeud *res = insertion_LN(feuilles, parent);

        // Si insertion_LN a retourné quelque chose, alors il y a une nouvelle tête de liste.
        if (res != NULL) {
            feuilles = res;
        }
    }
    Noeud *racine = feuilles->info;

    // Obligé de libérer la mémoire ici ; cette feuille risque d'être perdue si 
    // on passe par res != NULL dans l'itération. 
    free(feuilles);

    // Il n'y a plus qu'un seul élément dans la liste, c'est notre racine.
    return racine;
}

void dealloue(Noeud *racine) {
    if (racine == NULL) {
        return;
    }

    dealloue(racine->eG);
    dealloue(racine->eD);

    free(racine);
}

void afficher_arbre(Noeud *noeud, size_t profondeur) {
    // Gardien: si le noeud n'a rien à afficher, on ne fait rien
    if (noeud == NULL) {
        return;
    }

    // On affiche déjà le sous-noeud droit, car l'arbre est affiché de gauche
    // à droite, donc par rotation, les noeuds en haut sont les noeuds droits
    // de l'arbre
    afficher_arbre(noeud->eD, profondeur + 1);

    // On affiche le noeud courant avec la profondeur actuelle
    for(size_t i = 0; i < profondeur; ++i) {
        printf("\t");
    }
    afficher_noeud(noeud);

    // Affichage du noeud de gauche
    afficher_arbre(noeud->eG, profondeur + 1);
}

