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

void evaluer_lecture(arbre a) {
    arbre it_a;
    elem_pile_exec_t elem;
    int dep;
    for (it_a = a->fils; it_a != arbre_vide(); it_a = it_a->fils->frere) {
        int type = calculer_type(it_a->fils);
        switch (type) {
        case T_INT:
            int val_i;
            fprintf(stderr, "Entrez un entier : ");
            if (scanf("%d", &val_i) != 1) erreur("evaluer_lecture(arbre a) : lecture d'un entier pour la variable %s échouée !\n", lexeme(it_a->fils->num_lexico));
            dep = calculer_deplacement(it_a->fils);
            elem.type = T_INT;
            elem.elem.int_valeur = val_i;
            pile_execution[dep] = elem;
            break;
        case T_FLOAT:
            float val_f;
            fprintf(stderr, "Entrez un réel : ");
            if (scanf("%f", &val_f) != 1) erreur("evaluer_lecture(arbre a) : lecture d'un réel pour la variable %s échouée !\n", lexeme(it_a->fils->num_lexico));
            dep = calculer_deplacement(it_a->fils);
            elem.type = T_FLOAT;
            elem.elem.float_valeur = val_f;
            pile_execution[dep] = elem;
            break;
        case T_CHAR:
            char val_c;
            fprintf(stderr, "Entrez un caractère : ");
            if (scanf("%c", &val_c) != 1) erreur("evaluer_lecture(arbre a) : lecture d'un caractère pour la variable %s échouée !\n", lexeme(it_a->fils->num_lexico));
            dep = calculer_deplacement(it_a->fils);
            elem.type = T_CHAR;
            elem.elem.char_valeur = val_c;
            pile_execution[dep] = elem;
            break;
        case T_BOOL:
            char val_b[1024];
            fprintf(stderr, "Entrez un booleen (Valeurs possibles : \"vrai\", \"faux\") : ");
            if (scanf("%s", val_b) != 1) erreur("evaluer_lecture(arbre a) : lecture d'un booleen pour la variable %s échouée !\n", lexeme(it_a->fils->num_lexico));
            else if (strcmp("vrai", val_b) != 0 && strcmp("faux", val_b) != 0)
                erreur("evaluer_lecture(arbre a) : lecture d'un booleen pour la variable %s échouée !\n", lexeme(it_a->fils->num_lexico));
            dep = calculer_deplacement(it_a->fils);
            elem.type = T_BOOL;
            elem.elem.bool_valeur = (strcmp("vrai", val_b) == 0) ? 1 : 0;
            pile_execution[dep] = elem;
            break;
        case T_STRING:
            char val_s[4096];
            fprintf(stderr, "Entrez une chaine de caractère (Taille max : 4095) : ");
            if (scanf("%s", val_s) != 1) erreur("evaluer_lecture(arbre a) : lecture d'une chaine de caractère pour la variable %s échouée ! (Attention : taille max de la chaine = 4096)\n", lexeme(it_a->fils->num_lexico));
            dep = calculer_deplacement(it_a->fils);
            elem.type = T_STRING;
            elem.elem.string_valeur = val_s;
            pile_execution[dep] = elem;
            break;
        }
        viderBuffer();
    }
}

// Evalue la fonction ecriture

void evaluer_ecriture(arbre a) {
    arbre it_a;
    elem_pile_exec_t elem;
    char * format = lexeme(a->fils->num_lexico);
    int compteurs[5], compteur_total = 0, nb_args,
        taille_format = strlen(format);
    // Initialisations
    for (int i = 0; i < 5; i++) compteurs[i] = 0;
    nb_args = a->num_lexico;
    // Vérification des arguments
    for (it_a = a->fils->frere; it_a != arbre_vide(); it_a = it_a->fils->frere) {
        elem = evaluer_expression(it_a->fils);
        if (elem.type == -1) erreur("evaluer_ecriture(arbre a) : l'argument %s n'a pas été initialisé !\n", lexeme(it_a->fils->num_lexico));
    }
    // Parcours du format
    for (int i = 1; i < taille_format - 1; i++) {
        char c = format[i];
        if (c == '%') {
            c = format[++i];
            if (c == '\0') break;
            switch (c) {
                case 'd': compteurs[T_INT]++; compteur_total++; break;
                case 'f': compteurs[T_FLOAT]++; compteur_total++; break;
                case 'c': compteurs[T_CHAR]++; compteur_total++; break;
                case 's': compteurs[T_STRING]++; compteur_total++; break;
                case 'b': compteurs[T_BOOL]++; compteur_total++; break;
            }
        }
    }
    if (nb_args != compteur_total) erreur("evaluer_ecriture(arbre a) : le nombre d'arguments ne correspond pas au nombre de formats attendant une valeur ! "
                                          "Il y a %d formats et %d arguments.\n", compteur_total, nb_args);
    it_a = a->fils->frere;
    for (int i = 1; i < taille_format - 1; i++) {
        char c = format[i];
        if (c == '%') {
            c = format[++i];
            switch (c) {
                case 'd':
                    elem = evaluer_expression(it_a->fils);
                    if (elem.type != T_INT) erreur("evaluer_ecriture(arbre a) : le type de l'argument ne correspond pas au type du format (format : %s, argument : %s) !\n", type_string(T_INT), type_string(elem.type));
                    printf("%d", elem.elem.int_valeur);
                    it_a = it_a->fils->frere;
                    break;
                case 'f':
                    elem = evaluer_expression(it_a->fils);
                    if (elem.type != T_FLOAT) erreur("evaluer_ecriture(arbre a) : le type de l'argument ne correspond pas au type du format (format : %s, argument : %s) !\n", type_string(T_FLOAT), type_string(elem.type));
                    printf("%f", elem.elem.float_valeur);
                    it_a = it_a->fils->frere;
                    break;
                case 'c':
                    elem = evaluer_expression(it_a->fils);
                    if (elem.type != T_CHAR) erreur("evaluer_ecriture(arbre a) : le type de l'argument ne correspond pas au type du format (format : %s, argument : %s) !\n", type_string(T_CHAR), type_string(elem.type));
                    printf("%c", elem.elem.char_valeur);
                    it_a = it_a->fils->frere;
                    break;
                case 's':
                    elem = evaluer_expression(it_a->fils);
                    if (elem.type != T_STRING) erreur("evaluer_ecriture(arbre a) : le type de l'argument ne correspond pas au type du format (format : %s, argument : %s) !\n", type_string(T_STRING), type_string(elem.type));
                    printf("%s", elem.elem.string_valeur);
                    it_a = it_a->fils->frere;
                    break;
                case 'b':
                    elem = evaluer_expression(it_a->fils);
                    if (elem.type != T_BOOL) erreur("evaluer_ecriture(arbre a) : le type de l'argument ne correspond pas au type du format (format : %s, argument : %s) !\n", type_string(T_BOOL), type_string(elem.type));
                    printf("%s", (elem.elem.bool_valeur) == 0 ? "faux" : "vrai");
                    it_a = it_a->fils->frere;
                    break;
                default:
                    printf("%c%c", format[i - 1], c);
            }
        } else if (c == '\\') {
            c = format[++i];
            switch (c) {
                case 'n': printf("\n"); break;
                case 't': printf("\t"); break;
                default:
                    printf("%c%c", format[i - 1], c);
            }
        } else
            printf("%c", c);
    }
}

/****************************************************************************************************************/
                                            /*AUTRES FONCTIONS*/
/****************************************************************************************************************/

// Retourne une chaine de caractère sans les guillemets

char * enlever_quotes(char * chaine) {
    char * ret;
    int taille = strlen(chaine) - 2;
    if (chaine[0] == '\"' && chaine[taille + 1] == '\"') {
        ret = allocation_mem_init0(taille, sizeof(char));
        for (int i = 1; i < taille + 1; i++)
            ret[i - 1] = chaine[i];
        return ret;
    } else
        return chaine;
}

