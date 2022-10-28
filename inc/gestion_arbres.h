/* 
 * Module associé à la gestion des arbres
 ** Fichier En-tête
 */

#ifndef _GESTION_ARBRES_H_
#define _GESTION_ARBRES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "erreur.h"
#include "table_lexicographique.h"

// Constantes pour les types des noeuds
enum {
    A_IDF, A_CSTE_E, A_CSTE_R, A_CSTE_S, A_PLUS, A_MOINS, A_MULT, A_DIV,
    A_LISTE_INST, A_LISTE_P, A_AFFEC, A_ITE, A_WHILE, A_FOR, A_APPEL_FCT,
    A_APPEL_PROC, A_MOD, A_OR, A_AND, A_NOT, A_SUP, A_SUP_EGAL, A_INF,
    A_INF_EGAL, A_EGAL_EGAL, A_NOT_EGAL, A_TRUE, A_FALSE, A_INST_VIDE,
    A_INST_RET, A_IT, A_EI, A_INC_FOR, A_COND_FOR, A_INIT_FOR, A_IND_TAB,
    A_POINT_STRUCT, A_CSTE_C, A_LECTURE, A_ECRITURE
};

// Structure d'un noeud dans l'arbre abstrait
typedef struct noeud {
    struct noeud *fils;
    int type_noeud;
    int num_lexico;
    int num_decl;
    struct noeud *frere;
} noeud;

// Type arbre
typedef noeud* arbre;

// Prototypes des fonctions
// Créé un arbre vide
arbre arbre_vide();
// Teste si l'arbre est vide
int estvide(arbre a);
// Creer un noeud
arbre creer_noeud(arbre fils, int type_noeud, int num_lexico, int num_decl, arbre frere);
// Ajoute un fils à un noeud
arbre ajouter_fils(arbre noeud, arbre fils);
// Ajoute un frere à un noeud
arbre ajouter_frere(arbre noeud, arbre frere);
// Affiche l'arbre
void affichage_arbre(arbre a, int prof);
// Affiche un noeud
void affichage_noeud(arbre a);
// Renvoie une chaine de caractère représentant le type du noeud
char * type_noeud_string(int type_noeud);

#endif