/* 
 * Module associé au TAD pile
 ** Fichier En-tête
 */

#ifndef _PILE_H_
#define _PILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table_regions.h"

#define TAILLE_MAX_PILE 100

// Type pile
typedef int pile[100 + 1];

// Prototypes des fonctions
// Initialise une pile
int initialiser(pile p);
// Empile un élément dans la pile
int empiler(int elem, pile p);
// Vérifie si la pile est vide
int pile_estvide(pile p);
// Dépile la pile
int depiler(pile p);
// Renvoie le sommet de la pile
int sommet(pile p);
// Affiche la pile
int afficher(pile p); 

#endif