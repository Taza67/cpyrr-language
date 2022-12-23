/* 
 * Module associé à la machine virtuelle
 ** Fichier Source
 */

#include "machine_virtuelle.h"

extern element_table_lexico lexico[TAILLE_MAX_TABLE_LEXICO];                    // Table lexico
extern element_table_declarations table_declarations[TAILLE_MAX_TABLE_DECL];    // Table des déclarations
extern int table_representation[TAILLE_MAX_TABLE_REPRESENTATION];               // Table de représentation
extern element_table_regions table_regions[TAILLE_MAX_TABLE_REGIONS];           // Table des régions
extern elem_pile_exec_t pile_execution[TAILLE_PILE_EXEC];                            // Pile d'exécution
extern pile bases_courantes;
pile regions_appelantes;

int retourne_trouve = 0;
elem_pile_exec_t ret;
int region_courante = -1;

/* 
enum {
    A_LISTE_P, A_IND_TAB,
    A_POINT_STRUCT
}; */

/****************************************************************************************************************/
                                /*FONCTION D'EVALUATION D'UNE INSTRUCTION*/
/****************************************************************************************************************/

// Évalue un arbre représentatif d'une instruction

elem_pile_exec_t evaluer_expression(arbre a) {
    elem_pile_exec_t elem_pile;
    int dep, type_operandes;
    if (estvide(a)) { 
        elem_pile.type = -1;
        return elem_pile;
    }
    switch (a->type_noeud) {
        case A_IDF:
            dep = calculer_deplacement(a);
            return pile_execution[dep];
        case A_CSTE_E:
            elem_pile.type = T_INT;
            elem_pile.elem.int_valeur = a->num_lexico;
            return elem_pile;
        case A_CSTE_R:
            elem_pile.type = T_FLOAT;
            elem_pile.elem.float_valeur = atof(lexico[a->num_lexico].lexeme);
            return elem_pile;
        case A_CSTE_C:
            elem_pile.type = T_CHAR;
            elem_pile.elem.char_valeur = a->num_lexico;
            return elem_pile;
        case A_CSTE_S:
            elem_pile.type = T_STRING;
            elem_pile.elem.string_valeur = enlever_quotes(strdup(lexico[a->num_lexico].lexeme));
            return elem_pile;
        case A_TRUE:
        case A_FALSE:
            elem_pile.type = T_BOOL;
            elem_pile.elem.bool_valeur = a->num_lexico;
            return elem_pile;
        case A_PLUS:
            elem_pile.type = calculer_type(a);
            switch (elem_pile.type) {
            case T_INT:
                elem_pile.elem.int_valeur = evaluer_expression(a->fils).elem.int_valeur + evaluer_expression(a->fils->frere).elem.int_valeur;
                return elem_pile;
            case T_FLOAT:
                elem_pile.elem.float_valeur = evaluer_expression(a->fils).elem.float_valeur + evaluer_expression(a->fils->frere).elem.float_valeur;
                return elem_pile;
            case T_CHAR:
                elem_pile.elem.char_valeur = evaluer_expression(a->fils).elem.char_valeur + evaluer_expression(a->fils->frere).elem.char_valeur;
                return elem_pile;
            case T_STRING:
                char *g = enlever_quotes(evaluer_expression(a->fils).elem.string_valeur),
                     *d = enlever_quotes(evaluer_expression(a->fils->frere).elem.string_valeur),
                     *r = allocation_mem_init0(strlen(g) + strlen(d) + 1, sizeof(char));
                strcat(r, g);
                strcat(r, d);
                elem_pile.elem.string_valeur = r;
                return elem_pile;
            }
        case A_MOINS:
            elem_pile.type = calculer_type(a);
            switch (elem_pile.type) {
            case T_INT:
                elem_pile.elem.int_valeur = evaluer_expression(a->fils).elem.int_valeur - evaluer_expression(a->fils->frere).elem.int_valeur;
                return elem_pile;
            case T_FLOAT:
                elem_pile.elem.float_valeur = evaluer_expression(a->fils).elem.float_valeur - evaluer_expression(a->fils->frere).elem.float_valeur;
                return elem_pile;
            case T_CHAR:
                elem_pile.elem.char_valeur = evaluer_expression(a->fils).elem.char_valeur - evaluer_expression(a->fils->frere).elem.char_valeur;
                return elem_pile;
            }
        case A_MULT:
            elem_pile.type = calculer_type(a);
            switch (elem_pile.type) {
            case T_INT:
                elem_pile.elem.int_valeur = evaluer_expression(a->fils).elem.int_valeur * evaluer_expression(a->fils->frere).elem.int_valeur;
                return elem_pile;
            case T_FLOAT:
                elem_pile.elem.float_valeur = evaluer_expression(a->fils).elem.float_valeur * evaluer_expression(a->fils->frere).elem.float_valeur;
                return elem_pile;
            }
        case A_DIV:
            elem_pile.type = calculer_type(a);
            switch (elem_pile.type) {
            case T_INT:
                int diviseur_i = evaluer_expression(a->fils->frere).elem.int_valeur,
                    dividende_i = evaluer_expression(a->fils).elem.int_valeur;
                if (diviseur_i == 0) erreur("evaluer_expression(arbre a) : division de %d par %d", dividende_i, diviseur_i);
                elem_pile.elem.int_valeur = dividende_i / diviseur_i;
                return elem_pile;
            case T_FLOAT:
                float diviseur_f = evaluer_expression(a->fils).elem.float_valeur,
                      dividende_f = evaluer_expression(a->fils->frere).elem.float_valeur;
                if (diviseur_f == 0) erreur("evaluer_expression(arbre a) : division de %f par %f", dividende_f, diviseur_f);
                elem_pile.elem.int_valeur = dividende_f / diviseur_f;
                return elem_pile;
            }
        case A_MOD:
            elem_pile.type = calculer_type(a);
            elem_pile.elem.int_valeur = evaluer_expression(a->fils).elem.int_valeur % evaluer_expression(a->fils->frere).elem.int_valeur;
            return elem_pile;
        case A_OR:
            elem_pile.type = calculer_type(a);
            elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.bool_valeur || evaluer_expression(a->fils->frere).elem.bool_valeur;
            return elem_pile;
        case A_AND:
            elem_pile.type = calculer_type(a);
            elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.bool_valeur && evaluer_expression(a->fils->frere).elem.bool_valeur;
            return elem_pile;
        case A_NOT:
            elem_pile.type = calculer_type(a);
            elem_pile.elem.bool_valeur = !evaluer_expression(a->fils).elem.bool_valeur;
            return elem_pile;
        case A_SUP:
            elem_pile.type = T_BOOL;
            type_operandes = calculer_type(a);
            switch (type_operandes) {
            case T_INT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.int_valeur > evaluer_expression(a->fils->frere).elem.int_valeur;
                return elem_pile;
            case T_FLOAT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.float_valeur > evaluer_expression(a->fils->frere).elem.float_valeur;
                return elem_pile;
            case T_CHAR:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.char_valeur > evaluer_expression(a->fils->frere).elem.char_valeur;
                return elem_pile;
            case T_STRING:
                char *g = enlever_quotes(evaluer_expression(a->fils).elem.string_valeur),
                     *d = enlever_quotes(evaluer_expression(a->fils->frere).elem.string_valeur);
                elem_pile.elem.bool_valeur = strcmp(g, d) > 0;
                return elem_pile;
            }
        case A_SUP_EGAL:
            elem_pile.type = T_BOOL;
            type_operandes = calculer_type(a);
            switch (type_operandes) {
            case T_INT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.int_valeur >= evaluer_expression(a->fils->frere).elem.int_valeur;
                return elem_pile;
            case T_FLOAT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.float_valeur >= evaluer_expression(a->fils->frere).elem.float_valeur;
                return elem_pile;
            case T_CHAR:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.char_valeur >= evaluer_expression(a->fils->frere).elem.char_valeur;
                return elem_pile;
            case T_STRING:
                char *g = enlever_quotes(evaluer_expression(a->fils).elem.string_valeur),
                     *d = enlever_quotes(evaluer_expression(a->fils->frere).elem.string_valeur);
                elem_pile.elem.bool_valeur = strcmp(g, d) >= 0;
                return elem_pile;
            }
        case A_INF:
            elem_pile.type = T_BOOL;
            type_operandes = calculer_type(a);
            switch (type_operandes) {
            case T_INT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.int_valeur < evaluer_expression(a->fils->frere).elem.int_valeur;
                return elem_pile;
            case T_FLOAT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.float_valeur < evaluer_expression(a->fils->frere).elem.float_valeur;
                return elem_pile;
            case T_CHAR:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.char_valeur < evaluer_expression(a->fils->frere).elem.char_valeur;
                return elem_pile;
            case T_STRING:
                char *g = enlever_quotes(evaluer_expression(a->fils).elem.string_valeur),
                     *d = enlever_quotes(evaluer_expression(a->fils->frere).elem.string_valeur);
                elem_pile.elem.bool_valeur = strcmp(g, d) < 0;
                return elem_pile;
            }
        case A_INF_EGAL:
            elem_pile.type = T_BOOL;
            type_operandes = calculer_type(a);
            switch (type_operandes) {
            case T_INT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.int_valeur <= evaluer_expression(a->fils->frere).elem.int_valeur;
                return elem_pile;
            case T_FLOAT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.float_valeur <= evaluer_expression(a->fils->frere).elem.float_valeur;
                return elem_pile;
            case T_CHAR:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.char_valeur <= evaluer_expression(a->fils->frere).elem.char_valeur;
                return elem_pile;
            case T_STRING:
                char *g = enlever_quotes(evaluer_expression(a->fils).elem.string_valeur),
                     *d = enlever_quotes(evaluer_expression(a->fils->frere).elem.string_valeur);
                elem_pile.elem.bool_valeur = strcmp(g, d) <= 0;
                return elem_pile;
            }
        case A_EGAL_EGAL:
            elem_pile.type = T_BOOL;
            type_operandes = calculer_type(a);
            switch (type_operandes) {
            case T_INT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.int_valeur == evaluer_expression(a->fils->frere).elem.int_valeur;
                return elem_pile;
            case T_FLOAT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.float_valeur == evaluer_expression(a->fils->frere).elem.float_valeur;
                return elem_pile;
            case T_CHAR:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.char_valeur == evaluer_expression(a->fils->frere).elem.char_valeur;
                return elem_pile;
            case T_BOOL:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.bool_valeur == evaluer_expression(a->fils->frere).elem.bool_valeur;
                return elem_pile;
            case T_STRING:
                char *g = enlever_quotes(evaluer_expression(a->fils).elem.string_valeur),
                     *d = enlever_quotes(evaluer_expression(a->fils->frere).elem.string_valeur);
                elem_pile.elem.bool_valeur = strcmp(g, d) == 0;
                return elem_pile;
            }
        case A_NOT_EGAL:
            elem_pile.type = T_BOOL;
            type_operandes = calculer_type(a);
            switch (type_operandes) {
            case T_INT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.int_valeur != evaluer_expression(a->fils->frere).elem.int_valeur;
                return elem_pile;
            case T_FLOAT:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.float_valeur != evaluer_expression(a->fils->frere).elem.float_valeur;
                return elem_pile;
            case T_CHAR:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.char_valeur != evaluer_expression(a->fils->frere).elem.char_valeur;
                return elem_pile;
            case T_BOOL:
                elem_pile.elem.bool_valeur = evaluer_expression(a->fils).elem.bool_valeur != evaluer_expression(a->fils->frere).elem.bool_valeur;
                return elem_pile;
            case T_STRING:
                char *g = enlever_quotes(evaluer_expression(a->fils).elem.string_valeur),
                     *d = enlever_quotes(evaluer_expression(a->fils->frere).elem.string_valeur);
                elem_pile.elem.bool_valeur = strcmp(g, d) != 0;
                return elem_pile;
            }
        case A_APPEL_FCT:
            evaluer_fonc_proc(a);
            elem_pile = ret;
            return elem_pile;
        case A_APPEL_PROC:
            evaluer_fonc_proc(a);
            elem_pile.type = -1;
            return elem_pile;
        case A_LECTURE:
            evaluer_lecture(a);
            elem_pile.type = -1;
            return elem_pile;
        case A_ECRITURE:
            evaluer_ecriture(a);
            elem_pile.type = -1;
            return elem_pile;
    }
    elem_pile.type = -1;
    return elem_pile;
}

/****************************************************************************************************************/
                                /*FONCTION D'EVALUATION DE FONCTIONS/PROCEDURES*/
/****************************************************************************************************************/

// Evalue une fonction / procédure

