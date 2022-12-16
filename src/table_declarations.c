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
void affichage_table_declarations() {
    char * natures[50] = {"Base    ", "Structure", "Tableau  ", "Variable", "Paramètre", "Procédure", "Fonction"};
    fprintf(yyout, "Affichage de la table de déclarations :\n\n");
    for(int i = 0; i < TAILLE_MAX_TABLE_DECL; i++)
        if (table_declarations[i].nature != N_VIDE)
            fprintf(yyout, "Indice :\t%d\t||\tNature :\t%s\t||\tSuivant :\t%4d\t||\tRégion :\t%3d\t||\tdescription :\t%3d\t||\tExécution :\t%3d\t\n",
                i, natures[table_declarations[i].nature], table_declarations[i].suivant, table_declarations[i].region,
                table_declarations[i].description, table_declarations[i].execution);
    fprintf(yyout, "\n\n");
}

/****************************************************************************************************************/
                                /*FONCTION DE CALCUL DU DEPLACEMENT D'UNE VARIABLE*/
/****************************************************************************************************************/

// Calcule le déplacement d'une variable
int calculer_deplacement(arbre a) {
    int num_reg_var = table_declarations[a->num_decl].region,
        nis_reg_var = table_regions[num_reg_var].NIS,
        nis_reg_app = table_regions[sommet(regions_appelantes)].NIS,
        dec = nis_reg_app - nis_reg_var,
        dep = 0, ind, ind_rep, borne_inf, borne_sup;
    switch (a->type_noeud) {
        case A_IDF:
            if (num_reg_var == 0)
                dep = table_declarations[a->num_decl].execution;
            else if (num_reg_var == sommet(regions_appelantes))
                dep = sommet(bases_courantes) - 1 + nis_reg_var + table_declarations[a->num_decl].execution;
            else 
                dep = pile_execution[sommet(bases_courantes) + dec].elem.int_valeur + nis_reg_var + table_declarations[a->num_decl].execution;
            if (!estvide(a->fils)) {
                dep += calculer_deplacement(a->fils);
            }
            break;
        case A_POINT_STRUCT:
            ind_rep = a->num_decl;
            dep += table_representation[ind_rep + 2];
            if (!estvide(a->fils))
                dep += calculer_deplacement(a->fils);
            if (!estvide(a->frere))
                dep += calculer_deplacement(a->frere);
            break;
        case A_IND_TAB:
            elem_pile_exec_t elem = evaluer_expression(a->fils);
            if (elem.type != T_INT)
                erreur("calculer_deplacement(arbre a) : l'indice d'un tableau doit toujours être un entier (entree : %s) !\n", type_string(elem.type));
            ind = elem.elem.int_valeur;
            ind_rep = table_declarations[a->num_decl].description + (2 * a->num_lexico);
            borne_inf = table_representation[ind_rep];
            borne_sup = table_representation[ind_rep + 1];
            if (ind < borne_inf || ind > borne_sup)
                erreur("calculer_deplacement(arbre a) : indice du tableau incorrect (indice : %d, borne inférieure : %d, borne supérieure : %d) !\n", ind, borne_inf, borne_sup);
            dep = ind - borne_inf;
            if (a->fils->frere != arbre_vide())
                dep += dep * (borne_sup - borne_inf) + calculer_deplacement(a->fils->frere);
            if (!estvide(a->frere))
                dep += calculer_deplacement(a->frere);
            break;
        default:
            erreur("calculer_deplacement(arbre a) : type du noeud incorrect %s !\n", type_noeud_string(a->type_noeud));
    }
    return dep;
}