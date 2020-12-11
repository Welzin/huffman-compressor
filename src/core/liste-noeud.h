#ifndef __LISTE_NOEUD_H__
#define __LISTE_NOEUD_H__

/*
 * Fichier : liste-noeud.h
 * Auteur : Welzin
 * Projet Huffman
 *
 * Ce fichier contient la définition d'un Noeud, d'une liste de Noeuds, et
 * toutes les fonctions associées à une liste de Noeuds.
 * J'ai préféré faire des listes chaînées à des tableaux pour avoir une 
 * meilleure optimisation de la mémoire, et pouvoir avoir un nombre de noeuds
 * pratiquement illimité (à part par la mémoire allouée au programme).
 */
#include <stddef.h>

/** Structure qui décrit le Noeud d'un arbre.
 *
 *  Il contient une liste d'entiers, qui représente tous les caractères 
 *  présents dans les enfants, le nombre d'occurrences totales de tous ces
 *  caractères, et des pointeurs vers ses enfants.
 */
typedef struct noeud {
    unsigned char c;    //!< Défini seulement si c'est une feuille. 
    size_t occ;         //!< Nombre total d'occurrences
    struct noeud *eG;   //!< Enfant gauche
    struct noeud *eD;   //!< Enfant droit
} Noeud;

/** Structure qui décrit une liste simplement chaînée de Noeuds.
 *
 *  Pour chaque élément de la structure, nous avons une information qui
 *  contient les données de l'élément actuel, ainsi que le successeur qui
 *  emmène vers le prochain élément de la liste.
 */
typedef struct listenoeud {
    Noeud *info;
    struct listenoeud *succ;
} ListeNoeud;

/** Crée un nouveau Noeud
 *
 *  Instancie un Noeud, c prend la valeur de `caracteres`, occ la valeur 
 *  d'`occurrences`, et les deux enfants de `eG` et `eD`.
 *
 *  @param caractere Caractère à enregistrer
 *  @param occurrences Nombre total d'occurrence de ces caractères
 *  @param eG Enfant gauche du noeud à créer
 *  @param eD Enfant droit du noeud à créer
 *  @return Le pointeur vers le noeud qui vient d'être créé.
 */
Noeud *creer_noeud(unsigned char caractere, size_t occurrences, Noeud *eG, Noeud *eD);

/** Crée un nouvelle Liste de Noeuds
 *
 *  Instancie une Liste de Noeuds, avec le premier élément qui prend la valeur
 *  `info` et le successeur NULL.
 *
 *  @param info La valeur du premier élément de la liste.
 *  @return Le pointeur vers la Liste de Noeuds crée.
 */
ListeNoeud* creer_LN(Noeud *info);

/** Essaie de trouver un Noeud dans la liste qui aurait la même ListeInt fournie
 *  
 *  Cherche dans la liste de Noeuds le Noeud avec une liste de caractères 
 *  similaires à celle recherchée.
 *
 *  @param listeNoeud La liste dans laquelle chercher
 *  @param caractere Le caractère à trouver dans un Noeud.
 *  @return Si le Noeud a été trouvé, retourne le pointeur vers le Noeud. Sinon,
 *          retourne NULL.
 */
Noeud* trouve_LN(ListeNoeud *listeNoeud, unsigned char caractere);

/** Ajoute `noeud` à la bonne place dans `listeNoeud`
 *
 *  Insère le Noeud de manière à ce que la liste soit triée selon le nombre 
 *  d'occurrences de manière croissante.
 *
 *  @param listeNoeud La liste à agrandir
 *  @param noeud Le noeud à ajouter
 *  @return L'objet si c'est inséré au début, NULL sinon
 */
ListeNoeud* insertion_LN(ListeNoeud *listeNoeud, Noeud *noeud);

/** Ajoute `noeud` à la fin dans `listeNoeud`
 *
 *  @param listeNoeud La liste à agrandir
 *  @param noeud Le noeud à ajouter
 */
void insertion_fin_LN(ListeNoeud *listeNoeud, Noeud *noeud);

/** Supprime `noeud` donné de `liste`.
 *
 *  Supprime la liaison entre le parent et le successeur de `noeud`.
 *  Le Noeud n'est pas libéré en mémoire, juste supprimé de la liste.
 *
 *  @param liste Liste de Noeuds à mettre à jour.
 *  @param noeud Le noeud à supprimer.
 *  @return Si le noeud à supprimer est la tête, retourne la nouvelle tête. NULL sinon.
 */
ListeNoeud* suppression_LN(ListeNoeud *liste, Noeud *noeud);

/** Supprime le premier Noeud de `liste`
 *
 *  Supprime et libère le premier élément de la liste.
 *
 *  @param liste Liste de Noeuds à mettre à jour.
 *  @return Le nouveau premier Noeud.
 */
ListeNoeud* suppression_debut_LN(ListeNoeud *liste);

/** Trie la liste de Noeuds
 *
 *  Trie la liste de Noeuds donnée dans une nouvelle liste.
 *
 *  @param liste La liste à trier
 *  @return Liste de Noeuds triée
 */
ListeNoeud* tri_LN(ListeNoeud *liste);

/** Vérifie si le Noeud donné est une feuille.
 *
 *  @param noeud Le Noeud à vérifier.
 *  @return 1 si noeud est une feuille, 0 sinon.
 */
int est_feuille(Noeud *noeud);

/** Calcule le caractère avec la plus haute valeur ASCII du tableau
 *
 *  @param liste Liste de Noeuds à tester
 *  @return La valeur ASCII la plus haute de tous les caractères
 */
size_t max_char(ListeNoeud *liste);

/** Détruit une liste.
 *
 *  Libère la mémoire prise par la liste donnée. Ne détruit pas les données.
 *
 *  @param liste La liste à détruire.
 */
void libere_LN(ListeNoeud *liste);

/** Calcule la taille d'une Liste de Noeuds
 *
 *  O(n), parcours la liste jusqu'à avoir NULL, il est déconseillé d'utiliser cette
 *  fonction dans une boucle car assez gourmande.
 *
 *  @param liste La liste sur laquelle calculer la taille.
 *  @return Taille de la liste.
 */
size_t taille_LN(ListeNoeud *liste);

/** Affiche correctement un Noeud
 *
 *  Affiche la liste d'Entiers ainsi que le nombre d'occurrences.
 *
 *  @param noeud Le Noeud à afficher.
 */
void afficher_noeud(Noeud *noeud);

/** Affiche correctement une Liste de Noeuds
 *
 *  Pour chaque élément, affiche son indice ainsi que son contenu.
 *
 *  @param listeNoeud La liste de Noeuds à afficher
 */
void afficher_LN(ListeNoeud *listeNoeud);

#endif