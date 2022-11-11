/* 
 * Module associé à la pile d'exécution
 ** Fichier En-tête
 */

#ifndef _PILE_EXECUTION_H_
#define _PILE_EXECUTION_H_

#include "pile.h"
#include "table_regions.h"

#define TAILLE_PILE_EXEC 5000
#define TAILLE_MAX_STRING 1024

// Types

typedef union {
    int int_valeur;
    float float_valeur;
    int bool_valeur;
    char char_valeur;
    char *string_valeur;
} elem_t;

// Type de l'union
enum {
    T_INT, T_FLOAT, T_CHAR, T_BOOL, T_STRING                // Ordre important !
};

typedef struct {
    int type;
    elem_t elem;
} elem_pile_exec_t;

// Prototypes des fonctions
// Initialise la pile d'exécution
void init_pile_execution();
// Insère les données d'une région dans la pile
int inserer_region(int num_region);
// Affiche la pile d'exécution
void affichage_pile_execution();

#endif