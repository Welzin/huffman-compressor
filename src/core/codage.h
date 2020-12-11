#ifndef __CODAGE_H__
#define __CODAGE_H__

/*
 * Fichier : codage.h
 * Auteur : Welzin
 * Projet Huffman
 *
 * Ce fichier défini une structure qui permet de définir et garder l'encodage
 * de chaque caractère de l'arbre. De cette manière, l'arbre ne sera parcouru
 * qu'une seule fois pour chaque caractère et pas à chaque itération de la 
 * compression. De plus, en gardant ces objets dans un tableau, on aura une
 * complexité d'accès d'O(1) contrairement aux listes, bien que de la mémoire
 * sera prise pour rien, la compression et décompression seront beaucoup
 * plus rapides.
 */
#include "liste-noeud.h"

#include <stddef.h>

/** Structure qui décrit la manière d'encoder un caractère
 *
 *  Dans cette structure, seulement la longueur du caractère en binaire et sa
 *  valeur en binaire sont gardées. En effet, de cette manière, 
 */
typedef struct encodage {
    size_t longueur;
    unsigned long long binaire;
} Encodage; 

/** Crée l'encodage de chaque caractère de l'arbre.
 *
 *  Crée un tableau de `maxCharValue` éléments. C'est pour s'assurer qu'on prend
 *  le moins de mémoire possible. Remplit le tableau aux indices des caractères
 *  sur chaque feuille de l'arbre en enregistrant l'encodage binaire. La longueur
 *  de chaque encodage est aussi enregistrée, de telle sorte à ce qu'il y ait
 *  une vraie compression. 
 *
 *  @param racine Le Noeud racine de l'arbre huffman.
 *  @param maxCharValue Le caractère avec l'indice ASCII maximum.
 *  @return Tableau d'Encodages de taille `maxCharValue`.
 */
Encodage* creer_encodage(Noeud *racine, size_t maxCharValue);

/** Peuple récursivement le tableau d'Encodage
 *
 *  Mets à jour l'Encodage actuel et rappelle la fonction sur les enfants. Si
 *  c'est une feuille, sauvegarde l'encodage à l'emplacement du caractère du 
 *  noeud actuel. 
 *
 *  @param encodage État actuel de l'encodage.
 *  @param noeud Noeud actuel.
 *  @param tableau Le tableau d'Encodage.
 */
void encodage_aux(Encodage encodage, Noeud *noeud, Encodage *tableau);

/** Affiche l'encodage de chaque caractère en format lisible.
 * 
 *  Fonction récursive qui affiche le code binaire associé au caractère de
 *  chaque feuille.
 *
 *  @param noeud La racine de l'arbre.
 *  @param encodages Le tableau des encodages.
 */
void affiche_encodage(Noeud *noeud, Encodage *encodages);

/** Affiche la longueur moyenne d'encodage des caractères.
 *
 *  @param racine La racine de l'arbre.
 *  @param encodages Le tableau des encodages.
 */
void affiche_moyenne(Noeud *racine, Encodage *encodages);

/** Calcul la somme des longueurs de l'encodage des différents caractères.
 *
 *  @param racine La racine de l'arbre.
 *  @param encodages Le tableau des encodages.
 */
size_t calcul_total(Noeud *racine, Encodage *encodages);

#endif