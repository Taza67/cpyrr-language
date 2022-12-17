/* 
 * Module associé à l'association
 ** Fichier source
 */

#include "association.h"

extern element_table_declarations table_declarations[TAILLE_MAX_TABLE_DECL];    // Table des déclarations
extern int table_representation[TAILLE_MAX_TABLE_REPRESENTATION];               // Table de représentation
extern element_table_regions table_regions[TAILLE_MAX_TABLE_REGIONS];           // Table des régions


/****************************************************************************************************************/
                                /*FONCTIONS DE VÉRIFICATION DE SURCHARGE*/
/****************************************************************************************************************/

// Vérifie si une déclaration de type ne fait pas conflit avec une précédente de la même région

int verifier_surcharge_type(char * lexeme, int num_region) {
    int it = num_lexico(lexeme);
    while (it != -1) {
        int nat = table_declarations[it].nature;
        if (nat == N_BASE)
            erreur("verifier_surcharge_type(char * lexeme, int num_region) : interdiction de redéfinir un type de base (type : %s) !\n", lexeme);
        if (nat == N_STRUCTURE || nat == N_TABLEAU)
            if (table_declarations[it].region == num_region)
                erreur("verifier_surcharge_type(char * lexeme, int num_region) : une déclaration du type %s existe déjà dans la région %d !\n", lexeme, num_region);
        it = table_declarations[it].suivant;
    }
    return 1;
}

// Vérifie si une déclaration de variable ne fait pas conflit avec une précédente du même type et de la même région

int verifier_surcharge_var(char * lexeme, int type, int num_region) {
    int it = num_lexico(lexeme);
    while (it != -1) {
        int nat = table_declarations[it].nature;
        if (nat == N_VARIABLE || nat == N_PARAMETRE) {
            if (table_declarations[it].description == type)
                if (table_declarations[it].region == num_region)
                    erreur("verifier_surcharge_var(char * lexeme, int type, int num_region) : une déclaration de la variable %s existe déjà avec le type %s dans la région %d !\n", lexeme, type_string(type), num_region);
        }
        it = table_declarations[it].suivant;
    }
    return 1;
}

// Vérifie si une déclaration de fonction/procedure ne fait pas conflit avec une précédente de même structure

int verifier_surcharge_fonc_proc(char *lexeme, int num_region, int types_param[], int nombre_params, int nature) {
    int it = num_lexico(lexeme), nb_p, nb_p_bis,
        types_param_attente[TAILLE_MAX_TABLEAU_PARAMS];
    while (it != -1) {
        int nat = table_declarations[it].nature;
        if (nat == nature) {
            int ind_rep = table_declarations[it].description,
                i;
            if (nat == N_FONCTION) ind_rep++;
            nb_p = table_representation[ind_rep];
            if (nb_p == nombre_params) {
                int types_ok = 1;
                for (i = 0, nb_p_bis = 0, ind_rep += 2; i < nb_p; ind_rep += 2, i++, nb_p_bis++) {
                    types_param_attente[i] = table_representation[ind_rep];
                    types_ok = types_ok && types_param[i] == types_param_attente[i];
                }
                if (types_ok)
                    if (table_declarations[it].region == num_region)
                        erreur("verifier_surcharge_fonc_proc(char *lexeme, int num_region, int types_param[], int nombre_params) : "
                               "Une %s %s existe déjà avec la même structure ! \nStructure : %s%s\n", (nature == N_FONCTION) ? "fonction" : "procédure", lexeme, lexeme, liste_types_string(types_param_attente, nb_p_bis));
            }
        }
        it = table_declarations[it].suivant;
    }
    return 1;
}

/****************************************************************************************************************/
                    /*FONCTION DE RECUPERATION DE L'INDICE DE REPRESENTATION D'UN CHAMP DE STRUCTURE*/
/****************************************************************************************************************/

// Renvoie l'index dans la table de représentation du champ d'une structure

int index_rep_struct(int num_decl, int num_lexico_champ) {
    int ind_rep = table_declarations[num_decl].description,
        nb_champs = table_representation[ind_rep++];
    for (int i = 0; i < nb_champs; i++, ind_rep += 3)
        if (table_representation[ind_rep] == num_lexico_champ) {
            return ind_rep;
        }
    return -1;
}

/****************************************************************************************************************/
                        /*FONCTION DE RECUPERATION DE LA DESCRIPTION D'UNE FONCTION/PROCEDURE*/
/****************************************************************************************************************/

// Récupère les infos sur une fonction/procédure

void recuperer_infos_fonc_proc(int num_decl, int *type_retourne, int *nombre_params, int types_param[]) {
    int ind_rep = table_declarations[num_decl].description,
        nat = table_declarations[num_decl].nature, i;
    *type_retourne = (nat == N_FONCTION) ? table_representation[ind_rep++] : -1;
    *nombre_params = table_representation[ind_rep++];
    for (i = 0, ind_rep++; i < *nombre_params; i++, ind_rep += 2)
        types_param[i] = table_representation[ind_rep];
}

/****************************************************************************************************************/
                        /*FONCTION DE RECHERCHE DE LA DERNIERE DECLARATION D'UN TYPE*/
/****************************************************************************************************************/

// Renvoie le numéro de la dernière bonne déclaration associée à un numéro lexico d"un type

