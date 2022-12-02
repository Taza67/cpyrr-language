/* 
 * Module associé à la gestion des fichiers
 ** Fichier source
 */

#include "gestion_fichiers.h"

/****************************************************************************************************************/
                                        /*FONCTIONS DE GESTION DES FICHIERS*/
/****************************************************************************************************************/

// Vérifie si un nom correspond au nom d'un dossier

int verifier_dossier(char * nom_fichier) {
    DIR *is_dossier = NULL;
    int retour = 0;
    // Si l'ouverture se fait, c'est un dossier
    if ((is_dossier = opendir(nom_fichier)) != NULL) {
        closedir(is_dossier);
        retour = 1;
    }
    return retour;
}

// Vérifie si un nom correspond au nom d'un fichier

int verifier_fichier(char * nom_fichier) {
    FILE *is_fichier = NULL;
    int retour = 0;
    // Si l'ouverture se fait, c'est un fichier
    if ((is_fichier = fopen(nom_fichier, "r")) != NULL) {
        fclose(is_fichier);
        retour = 1;
    }
    return retour;
}

// Crée le chemin d'un fichier à partir de son nom et celui du dossier parent

char * creer_chemin_fichier(char *nom_dossier_parent, char *nom_fichier) {
    int longueur_chemin = strlen(nom_dossier_parent) + strlen(nom_fichier) + 1;
    char *chemin = (char*)allocation_mem_init0(longueur_chemin, sizeof(char));
    strcpy(chemin, nom_dossier_parent);
    strcat(chemin, "/");
    strcat(chemin, nom_fichier);
    return chemin;
}

// Modifie le nom d'un fichier si le dernier est déjà attribué

