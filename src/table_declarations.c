/* 
 * Module associé à la table des déclarations
 ** Fichier Source
 */

#include "table_declarations.h"

extern FILE* yyout;

element_table_declarations table_declarations[TAILLE_MAX_TABLE_DECL];               // Table des déclarations
int indice_zone_debordement = INDICE_DEBUT_ZONE_DEBORDEMENT;                        // Indice du début de la zone de débordement dans la table des déclarations
extern element_table_regions table_regions[TAILLE_MAX_TABLE_REGIONS];               // Table des régions
extern elem_pile_exec_t pile_execution[TAILLE_PILE_EXEC];                                // Pile d'exécution
extern int table_representation[TAILLE_MAX_TABLE_REPRESENTATION];                   // Table de représentation
extern pile regions_appelantes;
extern pile bases_courantes;

/****************************************************************************************************************/
                            /*FONCTION D'INITIALISATION DE LA TABLE DES DECLARATIONS*/
/****************************************************************************************************************/

// Initialise la table des déclarations

void init_table_declarations() {
    // Initialisation
    for (int i = 0; i < TAILLE_MAX_TABLE_DECL; i++) {
        table_declarations[i].nature = N_VIDE;
        table_declarations[i].suivant = -1;
        table_declarations[i].region = -1;
        table_declarations[i].description = -1;
        table_declarations[i].execution = -1;
    }
    // Types de base -> Ordre d'insertion important !
    inserer_declaration(0, 0, N_BASE, -1); inserer_execution_declaration(0, 1);
    inserer_declaration(1, 0, N_BASE, -1); inserer_execution_declaration(1, 1);
    inserer_declaration(2, 0, N_BASE, -1); inserer_execution_declaration(2, 1);
    inserer_declaration(3, 0, N_BASE, -1); inserer_execution_declaration(3, 1);
    inserer_declaration(4, 0, N_BASE, -1); inserer_execution_declaration(4, 1);
}

/****************************************************************************************************************/
                                    /*FONCTION DE CALCUL DE L'INDICE D'AJOUT*/
/****************************************************************************************************************/

// Calcule et retourne l'indice où sera ajoutée la nouvelle déclaration (Réalise aussi le chainage)

int calculer_indice_ajout(int num_lexico) {
    // Indice de la table où sera ajoutée la déclaration
    int indice_ajout = -1;
    if (table_declarations[num_lexico].nature == N_VIDE)  // Première déclaration ?
        indice_ajout = num_lexico;
    else { // Une déclaration du même nom existe déjà ?
        int it = num_lexico;
        // Recherche de la dernière déclaration du même nom
        while (it != -1) {
            if (table_declarations[it].suivant == -1) break; 
            it = table_declarations[it].suivant;
        }
        indice_ajout = indice_zone_debordement++;
        // Ajout de l'indice de la nouvelle déclaration au chaînage
        table_declarations[it].suivant = indice_ajout;
    }
    return indice_ajout;
}

/****************************************************************************************************************/
                                    /*FONCTION D'INSERTION DE DECLARATION*/
/****************************************************************************************************************/

// Insère une déclaration dans la table

int inserer_declaration(int num_lexico, int num_region, int nature, int description) {
    // Indice de la table où sera ajoutée la déclaration
    int indice_ajout = calculer_indice_ajout(num_lexico);
    // Ajout de la déclaration
    table_declarations[indice_ajout].nature = nature;
    table_declarations[indice_ajout].region = num_region;
    table_declarations[indice_ajout].description = description;
    return indice_ajout;
}

// Met à jour la valeur du champ execution de la déclaration d'indice_decl

void inserer_execution_declaration(int indice_decl, int execution) {
    table_declarations[indice_decl].execution = execution;
}

/****************************************************************************************************************/
                                /*FONCTION D'AFFICHAGE DE LA TABLE DES DECLARATIONS*/
/****************************************************************************************************************/

// Affiche la table des déclarations

