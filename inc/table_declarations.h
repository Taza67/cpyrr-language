/* 
 * Module associé à la table des déclarations
 ** Fichier En-tête
 */

#ifndef _TABLE_DECLARATIONS_H_
#define _TABLE_DECLARATIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table_regions.h"
#include "pile_execution.h"
#include "machine_virtuelle.h"

// Taille maximale d'une table des déclarations (taille table primaire = 2000 = taille table lexico)
#define TAILLE_MAX_TABLE_DECL 3000
// Indice de début de la zone de débordement (taille zone de debordement = 1000)
#define INDICE_DEBUT_ZONE_DEBORDEMENT 2000

// Nature
enum {
    N_VIDE = -1, N_BASE = 0,
    N_STRUCTURE = 1, N_TABLEAU = 2,
    N_VARIABLE = 3, N_PARAMETRE = 4,
    N_PROCEDURE = 5, N_FONCTION = 6
};

// Structure d'une déclaration dans la table des déclarations
typedef struct {
    int nature;
    int suivant;
    int region;
    int description;
    int execution;
} element_table_declarations;

// Prototypes des fonctions
// Initialise la table des déclarations
void init_table_declarations();
// Calcule et retourne l'indice où sera ajoutée la nouvelle déclaration (Réalise aussi le chainage)
int calculer_indice_ajout(int num_lexico);
// Insère une déclaration dans la table
int inserer_declaration(int num_lexico, int num_region, int nature, int description);
// Met à jour la valeur du champ execution de la déclaration d'indice_decl
void inserer_execution_declaration(int indice_decl, int execution);
// Affiche la table des déclarations
void affichage_table_declarations();
// Calcule le déplacement d'une variable
int calculer_deplacement(arbre a);
// Calcule le numéro de région d'une variable
int calculer_numero_region(arbre a);

#endif
