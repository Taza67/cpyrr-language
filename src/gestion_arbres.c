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

