#ifndef __HUF_H__
#define __HUF_H__

/*
 * Fichier : huf.h
 * Auteur : Welzin
 * Projet Huffman - Compression
 *
 * Ce fichier contient toutes les fonctions nécessaires à la compression d'un
 * fichier, notamment les fonctions d'écritures dans le fichier de destination.
 */
#include "../core/liste-noeud.h"
#include "../core/codage.h"

/** Énumération qui liste les possibilités de ce que renvoie la compression
 *
 *  La fonction de compression peut être sortie dans plusieurs états différents.
 *  Cette énumération donne tous les états possibles.
 */
typedef enum state {
    Good,       //!< Réussite de la compression
    Size,       //!< La compression à réussie, mais le fichier est trop gros
    Failure,    //!< Compression échouée
} State;

/** Enregistre la fréquence de chaque caractère dans le fichier de destination.
 *
 *  Pour chaque feuille, enregistre son caractère et son nombre d'occurrences.
 *  Au final, le fichier sera composé d'un header avec au tout début le nombre
 *  de caractères total du header, puis le tableau des fréquences.
 *
 *  @param feuilles Toutes les feuilles de l'arbre 
 *  @param dst Nom du fichier de destination
 *  @return 1 si l'enregistrement a réussi, 0 sinon.
 */
int enregistrer_frequences(ListeNoeud *feuilles, const char* dst);

/** Écrit le nombre de bit total dans le fichier dst
 *
 *  Calcule le nombre de bits totals du fichier final et l'écrit juste après
 *  le tableau des fréquences.
 *
 *  @param racine Racine de l'arbre, avec le nombre d'occurrences totales.
 *  @param tableau Tableau d'Encodage pour calculer la longueur moyenne.
 *  @param dst Fichier de destination.
 */
void ecrire_taille(Noeud *racine, Encodage *tableau, const char* dst);

/** Compresse le fichier src dans dst.
 *
 *  Grâce au tableau d'encodages, compresse le fichier src dans dst avec pour 
 *  chaque caractère du fichier source un accès et une écriture en O(1) à son 
 *  encodage.
 *
 *  @param tableau Le tableau d'Encodage de chaque caractère.
 *  @param src Nom du fichier source.
 *  @param dst Nom du fichier de destination.
 *  @return État de la compression.
 *  @sa State
 */
State compresser(Encodage *tableau, const char* src, const char* dst);

#endif