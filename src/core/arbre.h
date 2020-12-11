#ifndef __ARBRE_H__
#define __ARBRE_H__

/*
 * Fichier : arbre.h
 * Auteur : Welzin
 * Projet Huffman
 *
 * Ce fichier contient toutes les fonctions nécessaires pour gérer un arbre, ce
 * qui inclus :
 *      - Le construire
 *      - Le détruire
 *      - Le parcourir
 */
#include "liste-noeud.h"

#include <stddef.h>

/** Compte la fréquence de chaque caractère d'un fichier.
 * 
 *  Crée une liste de Noeuds associés au nombre d'occurrences de chaque 
 *  caractère dans le fichier donné. 
 *
 *  @param filename Nom du fichier source
 *  @return Liste de Noeuds qui seront les feuilles de l'arbre. S'il y a une 
 *          erreur lors de la création, renvoie NULL.
 */
ListeNoeud* calcul_frequence(const char* filename);

/** Reconstruit les feuilles d'un arbre d'après un tableau de fréquences.
 *
 *  Lis le contenu de `filename`, et, si un tableau des fréquences est trouvé
 *  au début du fichier, construit les feuilles de l'arbre grâce au tableau.
 *
 *  @param filename Nom du fichier compressé.
 *  @param bits Nombre de bits total sur lequel est codé le fichier.
 *  @param indice Renvoie l'indice où le binaire commence.
 *  @return Liste de Noeuds contenant les feuilles de l'arbre. Peut renvoyer 
 *          NULL si le fichier donné n'est pas compressé.
 */
ListeNoeud* reconstruction_frequence(const char* filename, size_t *bits, size_t *indice);

/** Crée un arbre à partir des feuilles données
 *
 *  À partir des feuilles calculées grâce au calcul_fréquence, crée un arbre
 *  binaire pour faire un codage huffman.
 *
 *  @param feuilles La liste de Noeuds qui décrivent la fréquence des caractères
 *  @return La racine de l'arbre
 */
Noeud* creer_arbre(ListeNoeud *feuilles);

/** Désalloue toute la mémoire attribuée à un arbre
 *
 *  @param racine Racine de l'arbre à désallouer
 */
void dealloue(Noeud *racine);

/** Affiche correctement un arbre binaire.
 *
 *  Affiche chaque noeud d'un arbre avec ses enfants. À chaque profondeur de 
 *  l'arbre, l'indentation augmente de 1.
 *
 *  @param noeud Le Noeud à partir duquel afficher
 *  @param profondeur La profondeur actuelle du Noeud de l'arbre
 */
void afficher_arbre(Noeud *noeud, size_t profondeur);

#endif