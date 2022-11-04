/* 
 * Module associé à la table des régions
 ** Fichier Source
 */

#include "table_regions.h"

extern FILE *yyout;

element_table_regions table_regions[TAILLE_MAX_TABLE_REGIONS];      // Table des régions

/****************************************************************************************************************/
                            /*FONCTION D'INITIALISATION DE LA TABLE DES REGIONS*/
/****************************************************************************************************************/

// Initialise la table des régions

void init_table_regions() {
    // Initialisation
    for (int i = 0; i < TAILLE_MAX_TABLE_REGIONS; i++) {
        table_regions[i].taille = 0;
        table_regions[i].NIS = -1;
        table_regions[i].arbre_abstrait = arbre_vide();
    }
}

/****************************************************************************************************************/
                                /*FONCTION D'AFFICHAGE DE LA TABLE DES REGIONS*/
/****************************************************************************************************************/

// Affiche la table des régions

