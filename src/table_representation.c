/* 
 * Module associé à la table de représentation des types et des entêtes de sous-programmes
 ** Fichier En-tête
 */

#include "table_representation.h"

extern FILE *yyout;

int case_vide = 0;
int table_representation[TAILLE_MAX_TABLE_REPRESENTATION];      // Table de représentation
dimensions_tab tab_dim[TAILLE_MAX_TABLEAU_DIMENSIONS];          // Tableau de structures dimensions_tab pour gérer les bornes des tableaux
params_tab tab_param[TAILLE_MAX_TABLEAU_PARAMS];                // Tableau de structures params_tab pour gérer les paramètres des fonctions/procédures
champs_tab tab_champ[TAILLE_MAX_TABLEAU_CHAMPS];                // Tableau de structures champs_tab pour gérer les champs des structures

/****************************************************************************************************************/
                            /*FONCTION D'INITIALISATION DE LA TABLE DE REPRESENTATION*/
/****************************************************************************************************************/

// Initialise la table de représentation

void init_table_representation() {
    // Initialisation
    for (int i = 0; i < TAILLE_MAX_TABLE_REPRESENTATION; i++)
        table_representation[i] = -1;
}

/****************************************************************************************************************/
                                    /*FONCTION D'INSERTION POUR LES STRUCTURES*/
/****************************************************************************************************************/

// Insère la représentation d'une structure dans la table

void inserer_representation_structure(int nombre_champs) {
    table_representation[case_vide++] = nombre_champs;
    for (int i = 0; i < nombre_champs; i++) {
        table_representation[case_vide++] = tab_champ[i].num_lexico;
        table_representation[case_vide++] = tab_champ[i].type;
        table_representation[case_vide++] = tab_champ[i].execution;
    }
}

/****************************************************************************************************************/
                                    /*FONCTION D'INSERTION POUR LES TABLEAUX*/
/****************************************************************************************************************/

// Insère la représentation du tableau dans la table

