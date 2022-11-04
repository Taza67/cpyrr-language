/* 
 * Module associé à la table des régions
 ** Fichier En-tête
 */

#ifndef _TABLE_REGIONS_H_
#define _TABLE_REGIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_arbres.h"

// Taille maximale d'une table des régions
#define TAILLE_MAX_TABLE_REGIONS 20

// Structure d'une région dans la table des régions
typedef struct {
    int taille;
    int NIS;
    arbre arbre_abstrait;
} element_table_regions;

// Prototypes des fonctions
// Initialise la table des régions
void init_table_regions();
// Affiche la table des régions
void affichage_table_regions();
// Affiche les arbres de la table
void affichage_regions_arbres();
// Affiche l'arbre d'une une région
void affichage_region(int num_region);
#endif