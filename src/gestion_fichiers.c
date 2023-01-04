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
char * renvoyer_nom_modifie(char * nom) {
    /* *Déclaration de variables */
    int i = 0, taille = strlen(nom) + 2, stop = 0;
    char * new_name = (char*)allocation_mem_init0(taille, sizeof(char));
    strcpy(new_name, nom);
    for (i = 0; stop != 1; i++) {
        int verif = verifier_dossier(new_name) || verifier_fichier(new_name);
        if (verif == 1) {
            new_name[taille - 2] = i + '0';
            new_name[taille - 1] = '\0';
        } else
            stop = 1;
    }
    return new_name;
}

// Demande une confirmation à l'utilisateur sur l'écrasement du fichier
void demander_confirmation_ecrasement(char * nom_fichier) {
    int patience = 5;
    char c = 0;
    do {
        // Demande de confirmation
        avertissement("Un fichier du nom %s existe déjà !\n  Si le programme se poursuit, ce fichier sera écrasé."
                      " Êtes-vous sûr de continuer ? -> ", nom_fichier);
        c = getchar();
        if (c != 'O' && c != 'N') patience--;
        viderBuffer();
    } while (c != 'O' && c != 'N' && patience != 0);
    // Réponse
    if (c == 'N' || patience == 0)
        erreur("L'utilisateur ne souhaite pas écraser le fichier existant !\n");
}
        