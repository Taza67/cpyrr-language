/* 
 * Module associé à la gestion des erreurs
 ** Fichier entête
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

// Prototypes des fonctions
void viderBuffer();
// Affiche un message d'erreur et provoque la fermeture forcée du programme
void erreur(char * message, ...);
// Affiche un message d'avertissement
void avertissement(char * message, ...);
// Affiche la notice d'utilisation du programme
void usage(char * programme, char * message, ...);