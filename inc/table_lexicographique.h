/* 
 * Module associé à la table lexicographique
 ** Fichier En-tête
 */

#ifndef _TABLE_LEXICOGRAPHIQUE_H_
#define _TABLE_LEXICOGRAPHIQUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Taille maximale d'une table lexico
#define TAILLE_MAX_TABLE_LEXICO 2000
// Taille maximale d'un lexeme
#define LONGUEUR_MAX_LEXEME 500
// Taille maximale de la table de hashage
#define TAILLE_MAX_HC 32

//  Structure d'un élement dans la table lexico
typedef struct {
    int longueur;
    char lexeme[LONGUEUR_MAX_LEXEME];
    int suivant;
} element_table_lexico;

// Prototypes des fonctions
// Initialise la table des hash-codes
void init_table_hashage();
// Initialise la table lexico
void init_table_lexico();
// Retourne le hash-code du lexème
int hashage(char * lexeme);
// Insère un lexeme dans la table lexicographique
int inserer_lexeme(char * lexeme);
// Retourne le numéro lexico de [lexeme] s'il existe, -1 sinon
int num_lexico(char *lexeme);
// Affiche la table lexico
void affichage_table_lexico();
// Retourne le lexeme correspondant au numéro lexicographique
char * lexeme(int numero_lexico);

#endif