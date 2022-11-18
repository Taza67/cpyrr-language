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
int inserer_region(int num_region) {
    int b_c = base_courante,
        NIS = table_regions[num_region].NIS;

    if (num_region > 0) {
        // Chainage dynamique
        pile_execution[b_c].type = T_INT;
        pile_execution[b_c].elem.int_valeur = ex_base_courante;
        b_c++;
        
        // Chainage statique
        if (NIS > ex_NIS) {                                             // NIS croît
            pile_execution[b_c] = pile_execution[b_c - 1];
            b_c++;
            int  i = 1;
            while (i < NIS) {
                pile_execution[b_c] = pile_execution[ex_base_courante + i];
                i++;
                b_c++;
            }
        } else if (NIS == ex_NIS) {                                     // NIS stagne
            int i = 0;
            while (i < NIS) {
                pile_execution[b_c] = pile_execution[ex_base_courante + i];
                i++;
                b_c++;
            }
        } else {                                                        // NIS décroît
            int i = ex_NIS - NIS;
            while (i < ex_NIS) {
                pile_execution[b_c] = pile_execution[ex_base_courante + NIS + i];
                i++;
                b_c++;
            }
        }
    }
    ex_NIS = NIS;
    ex_base_courante = base_courante;
    base_courante = b_c + table_regions[num_region].taille;

    empiler(ex_base_courante, bases_courantes);

    return b_c;
}

/****************************************************************************************************************/
                            /*FONCTION D'AFFICHAGE DE LA PILE D'EXECUTION*/
/****************************************************************************************************************/

// Affiche la pile d'exécution
void affichage_pile_execution() {
    fprintf(yyout, "Affichage de la pile d'exécution :\n\n");
    for (int i = 0; i < base_courante; i++) {
        switch (pile_execution[i].type) {
        case T_INT:
            fprintf(yyout, "|__%4d__|    [%10d]    |\n", i, pile_execution[i].elem.int_valeur);
            break;
        case T_FLOAT:
            fprintf(yyout, "|__%4d__|    [%10.5f]    |\n", i, pile_execution[i].elem.float_valeur);
            break;
        case T_CHAR:
            fprintf(yyout, "|__%4d__|    [         %c]    |\n", i, pile_execution[i].elem.char_valeur);
            break;
        case T_BOOL:
            fprintf(yyout, "|__%4d__|    [%10s]    |\n", i, (pile_execution[i].elem.bool_valeur == 0) ? "false" : "true");
            break;
        case T_STRING:
            fprintf(yyout, "|__%4d__|    [%10s]    |\n", i, pile_execution[i].elem.string_valeur);
            break;
        case -1:
            fprintf(yyout, "|__%4d__|    [          ]    |\n", i);
            break;
        default:
            erreur("affichage_pile_execution() : type %d de l'élément incorrect", pile_execution->type);
        }
    }
    fprintf(yyout, "\n\n");
}