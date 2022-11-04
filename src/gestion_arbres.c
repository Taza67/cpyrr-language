/* 
 * Module associé à la gestion des arbres
 ** Fichier Source
 */

#include "gestion_arbres.h"

extern FILE *yyout;

/****************************************************************************************************************/
                                    /*FONCTION DE CREATION D'UN ARBRE VIDE*/
/****************************************************************************************************************/

// Créé un arbre vide
arbre arbre_vide() {
    return NULL;
}

/****************************************************************************************************************/
                                    /*FONCTION DE TEST D'UN ARBRE VIDE*/
/****************************************************************************************************************/

// Teste si l'arbre est vide
int estvide(arbre a) {
    return (a == arbre_vide());
}

/****************************************************************************************************************/
                                        /*FONCTION DE CREATION D'UN NOEUD*/
/****************************************************************************************************************/

// Initialise un noeud avec les valeurs données
arbre creer_noeud(arbre fils, int type_noeud, int num_lexico, int num_decl, arbre frere) {
    arbre new_noeud;
    // Allocation de mémoire
    new_noeud = (arbre)malloc(sizeof(noeud));
    if (new_noeud == NULL) erreur("creer_noeud(arbre fils, int type_noeud, int num_lexico, int num_decl, arbre frere) : échec de l'allocation de mémoire à <new_noeud> !\n");
    // Initialisations
    new_noeud->fils = fils;
    new_noeud->type_noeud = type_noeud;
    new_noeud->num_lexico = num_lexico;
    new_noeud->num_decl = num_decl;
    new_noeud->frere = frere;
    // Retour
    return new_noeud;
}

/****************************************************************************************************************/
                                        /*FONCTION D'AJOUT DE FILS*/
/****************************************************************************************************************/

// Ajoute un fils à un noeud
arbre ajouter_fils(arbre noeud, arbre fils) {
    if (estvide(noeud))
        noeud->fils = fils;
    else
        erreur("ajouterfils(arbre noeud, arbre fils) : le fils du noeud n'est pas vide !\n");
    return noeud;
}

/****************************************************************************************************************/
                                        /*FONCTION D'AJOUT DE FRERE*/
/****************************************************************************************************************/

// Ajoute un frere à un noeud
arbre ajouter_frere(arbre noeud,arbre frere) {
    if(estvide(noeud->frere))
        noeud->frere = frere;
    else 
        erreur("ajouterfrere(arbre noeud, arbre frere) : le frere du noeud n'est pas vide !\n");
    return noeud;
}
/****************************************************************************************************************/
                                        /*FONCTION D'AFFICHAGE D'UN ARBRE*/
/****************************************************************************************************************/



// Affiche l'arbre
void affichage_arbre(arbre a, int prof) {
    for (int i = 0; i < prof; i++)
        fputs("    ", yyout);
    affichage_noeud(a);
    if (a->fils) affichage_arbre(a->fils, prof + 1);
    if (a->frere) affichage_arbre(a->frere, prof);
}

// Affiche les infos sur un noeud
void affichage_noeud(arbre a) {
    fprintf(yyout, "[%s]", type_noeud_string(a->type_noeud));
    switch (a->type_noeud) {
        case A_IDF:
            fprintf(yyout, " ---- [lexeme : %s] ---- [declaration : %d]", lexeme(a->num_lexico), a->num_decl);
            break;
        case A_CSTE_E:
            fprintf(yyout, " ---- [valeur : %d]", a->num_lexico);
            break;
        case A_CSTE_R:
            fprintf(yyout, " ---- [valeur : %s]", lexeme(a->num_lexico));
            break;
        case A_CSTE_S:
            fprintf(yyout, " ---- [valeur : %s]", lexeme(a->num_lexico));
            break;
        case A_TRUE:
        case A_FALSE:
            fprintf(yyout, " ---- [valeur : %s]", a->num_lexico == 0 ? "vrai" : "faux");
            break;
        case A_IND_TAB:
            fprintf(yyout, " ---- [dimension : %d] ---- [type : %d]", a->num_lexico, a->num_decl);
            break;
        case A_POINT_STRUCT:
            fprintf(yyout, " ---- [lexeme structure : %s] ---- [lexeme champ : %s]", lexeme(a->num_lexico), lexeme(a->num_decl));
            break;
    }
    fprintf(yyout, "\n\n");
}

// Renvoie une chaine de caractère représentant le type du noeud
char * type_noeud_string(int type_noeud) {
    char *tab_corresp[40] = { "A_IDF","A_CSTE_E","A_CSTE_R", "A_CSTE_S", "A_PLUS",
                              "A_MOINS", "A_MULT", "A_DIV","A_LISTE_INST",
                              "A_LISTE_P", "A_AFFEC","A_ITE", "A_WHILE", "A_FOR",
                              "A_APPEL_FCT", "A_APPEL_PROC", "A_MOD", "A_OR", "A_AND", "A_NOT",
                              "A_SUP", "A_SUP_EGAL", "A_INF","A_INF_EGAL", "A_EGAL_EGAL",
                              "A_NOT_EGAL", "A_TRUE", "A_FALSE", "A_INST_VIDE", "A_INST_RET",
                              "A_IT", "A_EI", "A_INC_FOR", "A_COND_FOR", "A_INIT_FOR", "A_IND_TAB",
                              "A_POINT_STRUCT", "A_CSTE_C", "A_LECTURE", "A_ECRITURE" };
    return tab_corresp[type_noeud];
}