/* 
 * Module associé à la table de représentation des types et des entêtes de sous-programmes
 ** Fichier En-tête
 */

#ifndef _TABLE_REPRESENTATION_TYPES_H__
#define _TABLE_REPRESENTATION_TYPES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Taille maximale d'une table de représentation
#define TAILLE_MAX_TABLE_REPRESENTATION 5000
// Taille maximale d'un tableau contenant la structure dimensions_tab pour gérer les dimensions des tableaux
#define TAILLE_MAX_TABLEAU_DIMENSIONS 50
// Taille maximale d'un tableau contenant la structure params_tab pour gérer les paramètres des fonctions/procédures
#define TAILLE_MAX_TABLEAU_PARAMS 50
// Taille maximale d'un tableau contenant la structure champs_tab pour gérer les champs des structures
#define TAILLE_MAX_TABLEAU_CHAMPS 50


// Structure pour gérer l'insertion des bornes des dimensions des tableaux dans la table de représentation
typedef struct {
    int borne_inf;
    int borne_sup;
} dimensions_tab;

// Structure pour gérer l'insertion des paramètres des fonctions/procédures dans la table de représentation
typedef struct {
    int num_lexico;
    int type;
    int execution;
} params_tab;

// Structure pour gérer l'insertion des champs des structures dans la table de représentation
typedef struct {
    int num_lexico;
    int type;
    int execution;
} champs_tab;

// Prototypes des fonctions
// Initialise la table de représentation
void init_table_representation();
// Insère la représentation d'une structure dans la table
void inserer_representation_structure(int nombre_champs);
// Insère la représentation du tableau dans la table
void inserer_representation_tableau(int type_elements, int nombre_dimensions);
// Insère la représentation d'une procédure dans la table
void inserer_representation_procedure(int nombre_parametres);
// Insère la représentation d'une procédure dans la table
void inserer_representation_fonction(int type_retour, int nombre_parametres);
// Affiche la table de représentation
void affichage_table_representation();

#endif