#ifndef __UTILS_H__
#define __UTILS_H__

/*
 * Fichier : utils.h
 * Auteur : Welzin
 * Projet Huffman
 *
 * Ce fichier contient les définitions de fonctions utilitaires, utilisées 
 * par le compresseur ainsi que le décompresseur.
 */

/*
 * Définition des macros à utiliser dans la compression & décompression
 */
#define DELIM ';'

#include <stddef.h>
#include <stdio.h>

/** Énumération des différents types d'affichage.
 *
 *  L'énumération Type contient les différentes possibilités d'affichage avant
 *  un message. 
 *
 *  @sa affiche_type(Type)
 */
typedef enum type {
    Success,
    Info,
    Warning,
    Error,
    Debug,
} Type;

/** Affiche le type demandé, avec un affichage différent par type.
 *  
 *  Affiche le type entre crochets, avec une certaine couleur :
 *      - Vert pour le succès
 *      - Bleu clair pour l'information
 *      - Jaune pour l'avertissement
 *      - Rouge pour l'erreur
 *      - Bleu foncé pour le debug
 *  Ça permet d'avoir un affichage console mieux compréhensible.
 */
void affiche_type(Type type);

/** Teste l'égalité du contenu de deux chaînes de caractères.
 *
 *  @param c1 Une des chaînes à comparer
 *  @param c2 L'autre chaîne à comparer
 *  @return 0 si les deux chaînes sont différentes, 1 si elles sont identiques.
 */
int chaines_egales(const char* c1, const char* c2);

/** Transforme un entier en chaîne de caractères.
 *
 *  @param entier L'entier à transformer
 *  @return Chaîne de Caractères correspondant à l'entier.
 */
char *itoa(size_t entier);

/** Récupère un nombre d'un tableau de fréquence.
 *
 *  Dans un tableau de fréquence, un certain formattage est requis. On se sert
 *  de ce formattage pour récupérer le nombre d'occurrences (donc d'un nombre
 *  délimité par DELIM). Si DELIM n'est pas trouvé, ou que les caractères ne
 *  sont pas des nombres, retourne 0.
 *
 *  @param fd Le fichier dans lequel lire.
 *  @param out La variable où écrire le résultat.
 *  @return 1 si ça c'est bien passé, 0 sinon.
 */
int recuperer_occurrences(FILE *fd, size_t *out);

#endif