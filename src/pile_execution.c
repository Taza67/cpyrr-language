/* 
 * Module associé à la pile d'exécution
 ** Fichier Source
 */

#include "pile_execution.h"

extern FILE* yyout;

elem_pile_exec_t pile_execution[TAILLE_PILE_EXEC];                                  // Pile d'exécution
int base_courante = 0,                                                         // Sommet de la pile : indice de la derniere case
    ex_base_courante = 0;                                                      // Ex base courante
int ex_NIS = 0;
pile bases_courantes;
extern element_table_regions table_regions[TAILLE_MAX_TABLE_REGIONS];

/****************************************************************************************************************/
                            /*FONCTION D'INITIALISATION DE LA PILE D'EXECUTION*/
/****************************************************************************************************************/

// Initialise la pile d'exécution

void init_pile_execution() {
    initialiser(bases_courantes);
    for (int i = 0; i < TAILLE_PILE_EXEC; i++)
        pile_execution[i].type = -1;
}

/****************************************************************************************************************/
                        /*FONCTION D'INSERTION D'UNE REGION DANS LA PILE D'EXECUTION*/
/****************************************************************************************************************/

// Réserve l'espace dans la pile pour une région et renvoie l'indice de départ de la zone de données

