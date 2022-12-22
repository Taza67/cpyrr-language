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
int declaration_type(char *lexeme, int num_region) {
    int ret = -1, it = num_lexico(lexeme), test_nat, test_region, trouve;
    test_nat = test_region = trouve = 0;
    if (it == -1) erreur("declaration_type(char *lexeme, int num_region) : "
                         "Le type %s n'existe pas !\n", lexeme);
    while(it != -1) {
        int nat = table_declarations[it].nature;
        if (nat == N_BASE || nat == N_STRUCTURE || nat == N_TABLEAU) {
            test_nat = 1;
            if (table_declarations[it].region == num_region || table_regions[table_declarations[it].region].NIS < table_regions[num_region].NIS) {
                test_region = 1;
                trouve = 1;
                ret = it;
            }
        }
        it = table_declarations[it].suivant;
    }
    if (!trouve && test_nat == 0)
        erreur("declaration_type(char *lexeme, int num_region) : "
               "Le lexeme %s n'est ni un type simple, ni un type structure, ni un type tableau !\n", lexeme);
    if (!trouve && test_region == 0)
        erreur("declaration_type(char *lexeme, int num_region) : "
               "Le type %s n'est pas accessible depuis la région %d !\n, lexeme, num_region", lexeme, num_region);
    return ret;
}

/****************************************************************************************************************/
                        /*FONCTION DE RECHERCHE DE LA DERNIERE DECLARATION D'UNE VARIABLE*/
/****************************************************************************************************************/

// Renvoie le numéro de la dernière bonne déclaration associée à un numéro lexico d"une variable
int declaration_var(char *lexeme, int num_region, int nature_type) {
    int ret = -1, it = num_lexico(lexeme), test_nature_type, test_region, trouve, test_nat;
    test_nature_type = test_region = trouve = test_nat = 0;
    if (it == -1) erreur("declaration_var(char *lexeme, int num_region, int nature_type) : "
                          "La variable %s n'existe pas !\n", lexeme);
    while(it != -1) {
        int nat = table_declarations[it].nature;
        if (nat == N_VARIABLE || nat == N_PARAMETRE) {
            test_nat = 1;
            int dec_t = table_declarations[it].description;
            if (table_declarations[dec_t].nature == nature_type) {
                test_nature_type = 1;
                if (table_declarations[it].region == num_region || (table_regions[table_declarations[it].region].NIS < table_regions[num_region].NIS)) {
                    test_region = 1;
                    trouve = 1;
                    ret = it;
                }
            }
        }
        it = table_declarations[it].suivant;
    }
    if (!trouve && test_nat == 0)
        erreur("declaration_var(char *lexeme, int num_region, int nature_type) : "
               "Le lexeme %s n'est ni une variable, ni un paramètre !\n", lexeme);
    if (!trouve && test_nature_type == 0)
        erreur("declaration_var(char *lexeme, int num_region, int nature_type) : "
               "La variable %s n'existe pas sous cette nature de type %s !\n", lexeme, nature_type);
    if (!trouve && test_region == 0)
        erreur("declaration_var(char *lexeme, int num_region, int nature_type) : "
               "La variable %s n'est pas accessible depuis la région %d !\n", lexeme, num_region);
    return ret;
}

/****************************************************************************************************************/
                        /*FONCTION DE RECHERCHE DE LA DERNIERE DECLARATION D'UNE FONCTION*/
/****************************************************************************************************************/

// Renvoie le numéro de la dernière bonne déclaration associée à un numéro lexico d'une fonction/procédure
int declaration_fonc_proc(char *lexeme, int num_region, int types_param[], int nombre_params) {
    int ret = -1, it = num_lexico(lexeme), test_nat, test_nb_param, test_types_param, test_region, nb_p, trouve, nb_p_bis,
        types_param_attente[TAILLE_MAX_TABLEAU_PARAMS];
    test_nb_param = test_types_param = test_region = trouve = test_nat = 0;
    if (it == -1) erreur("declaration_fonc(char *lexeme, int num_region, int types_param[], int nombre_params) : "
                         "La fonction/procédure %s n'existe pas !\n", lexeme);
    while (it != -1) {
        int nat = table_declarations[it].nature;
        if (nat == N_FONCTION || nat == N_PROCEDURE) {
            int ind_rep = table_declarations[it].description,
                i;
            test_nat = 1;
            if (nat == N_FONCTION) ind_rep++;
            nb_p = table_representation[ind_rep];
            if (nb_p == nombre_params) {
                int types_ok = 1;
                test_nb_param = 1;
                for (i = 0, nb_p_bis = 0, ind_rep += 2; i < nb_p; ind_rep += 2, i++, nb_p_bis++) {
                    types_param_attente[i] = table_representation[ind_rep];
                    types_ok = types_ok && types_param[i] == types_param_attente[i];
                }
                if (types_ok) {
                    test_types_param = 1;
                    if (table_declarations[it].region == num_region || table_regions[table_declarations[it].region].NIS < table_regions[num_region].NIS) {
                        test_region = 1;
                        trouve = 1;
                        ret = it;
                    }
                }
            }
        }
        it = table_declarations[it].suivant;
    }
    if (!trouve && test_nat == 0) 
        erreur("declaration_fonc(char *lexeme, int num_region, int types_param[], int nombre_params) : "
               "Le lexeme %s n'est pas une fonction/procédure !\n", lexeme);
    if (!trouve && test_nb_param == 0) 
        erreur("declaration_fonc(char *lexeme, int num_region, int types_param[], int nombre_params) : "
               "La fonction/procédure %s requiert un nombre d'arguments différent (Attente : %d, Entrée : %d) !\n", lexeme, nb_p, nombre_params);
    if (!trouve && test_types_param == 0) 
        erreur("declaration_fonc(char *lexeme, int num_region, int types_param[], int nombre_params) : "
               "La fonction/procédure %s requiert des arguments de types différents !\nAttente : %s \nEntrée : %s\n", lexeme, liste_types_string(types_param_attente, nb_p_bis), liste_types_string(types_param, nombre_params));
    if (!trouve && test_region == 0) 
        erreur("declaration_fonc(char *lexeme, int num_region, int types_param[], int nombre_params) : "
               "La fonction/procédure %s n'est pas accessible depuis la région %d !\n", lexeme, num_region);
    return ret;
}

/****************************************************************************************************************/
                            /*FONCTIONS DE CALCUL DU TYPE DE VARIABLES/FONCTIONS*/
/****************************************************************************************************************/

// Renvoie le numéro de déclaration du type d'un arbre représentant un idf
int calculer_type_idf(arbre a) {
    int type = -1;
    switch (a->type_noeud) {
        case A_IDF:
            type = table_declarations[a->num_decl].description;
            if (!estvide(a->fils)) return calculer_type_idf(a->fils);
            return type;
        case A_IND_TAB:
            type = table_representation[table_declarations[a->num_decl].description];
            if (!(estvide(a->frere))) return calculer_type_idf(a->frere);
            return type;
        case A_POINT_STRUCT:
            type = table_representation[a->num_decl + 1];
            if (!(estvide(a->frere))) return calculer_type_idf(a->frere);
            if (!(estvide(a->fils))) return calculer_type_idf(a->fils);
            return type;
        default:
            erreur("calculer_type_idf(arbre a) : le type %s n'est pas valable dans cette fonction !\n", type_noeud_string(a->type_noeud));
    }
    return -1;
}

// Renvoie le numéro de déclaration du type de la valeur retournée par une fonction
int calculer_type_fonct(int num_decl) {
    int type = -1;
    if (num_decl != -1)
        type = table_representation[table_declarations[num_decl].description];
    else erreur("calculer_type_fonct(int num_decl) : Le numéro de déclaration de la fonction %d est incorrect !\n", num_decl);
    return type;
}

/****************************************************************************************************************/
                                /*FONCTION DE CALCUL DU TYPE D'UNE EXPRESSION*/
/****************************************************************************************************************/

// Renvoie le numéro déclaration du type d'une expression représenté par un arbre
int calculer_type(arbre a) {
    int type = -1;
    switch (a->type_noeud) {
    case A_IDF: return calculer_type_idf(a);
    case A_CSTE_E: return T_INT;
    case A_CSTE_R: return T_FLOAT;
    case A_CSTE_C: return T_CHAR;
    case A_TRUE: return T_BOOL;
    case A_FALSE: return T_BOOL;
    case A_CSTE_S: return T_STRING;
    case A_OR:
    case A_AND:
    case A_PLUS:
    case A_MOINS:
    case A_MULT:
    case A_DIV:
    case A_MOD:
    case A_SUP:
    case A_SUP_EGAL:
    case A_INF:
    case A_INF_EGAL:
    case A_EGAL_EGAL:
    case A_NOT_EGAL:
        type = verifier_concordance_type(a);
        if (verifier_type(type, a)) return type;
        break;
    case A_NOT:
        type = calculer_type(a->fils);
        if (verifier_type(type, a)) return type;
        break;
    case A_APPEL_FCT:
        return calculer_type_fonct(a->num_decl);
    default:
        erreur("calculer_type(arbre a) : La nature de l'arbre %d ne correspond pas à celle d'une expression !\n", a->type_noeud);
    }
    erreur("calculer_type(arbre a) : le type %s ne convient pas à l'opération de nature : %s", type_string(type), type_noeud_string(a->type_noeud));
    return type;
}

/****************************************************************************************************************/
                                /*FONCTIONS DE VERIFICATION DU TYPE D'UNE EXPRESSION*/
/****************************************************************************************************************/

// Vérifie la concordance des types des opérances d'une expression représenté par un arbre et renvoie le numéro déclaration de son type 
int verifier_concordance_type(arbre a) {
    int gauche_t, droite_t;
    gauche_t = calculer_type(a->fils);
    droite_t = calculer_type(a->fils->frere);
    if (gauche_t == droite_t) return gauche_t;
    else erreur("verifier_concordance_type(arbre a) : [%s] Les types des opérandes ne concordent pas (g : %s, d : %s) !\n", type_noeud_string(a->type_noeud), type_string(gauche_t), type_string(droite_t));
    return -1;
}

// Vérifie si une expression est possible avec un type donné
int verifier_type(int type, arbre a) {
    switch (a->type_noeud) {
    case A_PLUS: return type == T_INT || type == T_FLOAT || type == T_CHAR || type == T_STRING;
    case A_MOINS: return type == T_INT || type == T_FLOAT || type == T_CHAR;
    case A_MULT: return type == T_INT || type == T_FLOAT;
    case A_DIV: return type == T_INT || type == T_FLOAT;
    case A_MOD: return type == T_INT;
    case A_OR:
    case A_AND:
    case A_NOT: return type == T_BOOL;
    case A_SUP:
    case A_SUP_EGAL:
    case A_INF:
    case A_INF_EGAL: return type == T_INT || type == T_FLOAT || type == T_CHAR || type == T_STRING;
    case A_EGAL_EGAL:
    case A_NOT_EGAL: return type == T_INT || type == T_FLOAT || type == T_CHAR || type == T_STRING || type == T_BOOL;
    default:
        erreur("verifier_type(int type, arbre a) : La nature de l'arbre %s ne correspond pas à celle d'une expression !\n", type_noeud_string(a->type_noeud));
    }
    return 0;
}

/****************************************************************************************************************/
                                    /*FONCTIONS D'AFFICHAGE DE TYPE */
/****************************************************************************************************************/

// Renvoie une chaine de caractère représentant un type
char * type_string(int num_decl) {
    switch (num_decl) {
        case -1: return "null";
        case T_INT: return "entier";
        case T_FLOAT: return "reel";
        case T_CHAR: return "char";
        case T_BOOL: return "booleen";
        case T_STRING: return "string";
        default: return lexeme(num_decl);
    }
}

// Renvoie une chaine de caractère réprésentant une liste de types
char * liste_types_string(int liste_types[TAILLE_MAX_TABLEAU_PARAMS], int taille) {
    char * chaine = allocation_mem_init0(TAILLE_MAX_TABLEAU_PARAMS * 7, sizeof(char));
    strcat(chaine, "( ");
    for (int i = 0; i < taille; i++) {
        chaine = strcat(chaine, type_string(liste_types[i]));
        chaine = strcat(chaine, " ");
    }
    strcat(chaine, ")");
    return chaine;
}