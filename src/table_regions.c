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
void affichage_table_regions() {
    fprintf(yyout, "Affichage de la table des régions :\n\n");
    for(int i = 0; i < TAILLE_MAX_TABLE_REGIONS; i++)
        if (table_regions[i].NIS != -1)
            fprintf(yyout, "Indice :\t%3d\t||\tTaille :\t%3d\t||\tNIS :\t%3d\t\n",
                   i, table_regions[i].taille, table_regions[i].NIS);
    fprintf(yyout, "\n\n");
}

// Affiche les arbres des régions de la table
void affichage_regions_arbres() {
    fprintf(yyout, "Affichage des arbres :\n\n");
    for (int i = TAILLE_MAX_TABLE_REGIONS - 1; i >= 0; i--)
        if (table_regions[i].NIS != -1) 
            affichage_region(i);
    fprintf(yyout, "\n\n");
}

// Affiche l'arbre d'une une région
void affichage_region(int num_region) {
    for (int i = 0; i < table_regions[num_region].NIS; i++)
        fputs("    ", yyout);
    fprintf(yyout, "[Région : %2d ]\n", num_region);
    affichage_arbre(table_regions[num_region].arbre_abstrait, table_regions[num_region].NIS);
}