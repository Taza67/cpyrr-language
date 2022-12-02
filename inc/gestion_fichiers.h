/* 
 * Module associé à la gestion des fichiers
 ** Fichier en-têt
 */

#ifndef _GESTION_FICHIERS_H_
#define _GESTION_FICHIERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "allocation.h"
#include "erreur.h"

// Prototypes des fonctions
// Vérifie si un nom correspond au nom d'un dossier
int verifier_dossier(char * nom_fichier);
// Vérifie si un nom correspond au nom d'un fichier
int verifier_fichier(char * nom_fichier);
// Crée le chemin d'un fichier à partir de son nom et celui du dossier parent
char * creer_chemin_fichier(char *nom_dossier_parent, char *nom_fichier);
// Modifie le nom d'un fichier si le dernier est déjà attribué
char * renvoyer_nom_modifie(char * nom);
// Demande une confirmation à l'utilisateur sur l'écrasement du fichier
void demander_confirmation_ecrasement(char * nom_fichier);
#endif