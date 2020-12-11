#ifndef __DEHUF_H__
#define __DEHUF_H__

/*
 * Fichier : dehuf.h
 * Auteur : Welzin
 * Projet Huffman - Décompression
 *
 * Ce fichier contient la fonction nécessaire pour déchiffrer la compression
 * d'un fichier - compressé avec la partie `huf` de ce projet.
 */

#include "../core/liste-noeud.h"

/** Affiche sur la sortie standard le fichier décompressé.
 *
 *  Pour chaque bit de donnée, parcours l'arbre et affiche le caractère associé
 *  sur la sortie standard.
 *
 *  @param filename Nom du fichier source.
 *  @param racine Racine de l'arbre binaire.
 *  @param indice Indice de début du fichier binaire.
 *  @param bits Nombre total de bits pertinents.
 *  @return 1 si la décompression à réussi, 0 sinon.
 */
int decompresser(const char* filename, Noeud *racine, size_t indice, size_t bits);


#endif