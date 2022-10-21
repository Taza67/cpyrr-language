/* 
 * Module associé à la table lexicographique
 ** Fichier Source
 */

#include "table_lexicographique.h"

extern FILE *yyout;

int ligne_vide = 0; // indice d'insertion de la prochaine ligne vide dans la table lexicographique
int table_hash_code[TAILLE_MAX_HC]; // Table de hash-code
element_table_lexico lexico[TAILLE_MAX_TABLE_LEXICO]; // Table lexico

/****************************************************************************************************************/
                                    /*FONCTION D'INITIALISATION DE LA TABLE DE HASHAGE*/
/****************************************************************************************************************/

// Initialise la table des hash-codes

void init_table_hashage() {
    // Initialisation
    for (int i = 0; i < TAILLE_MAX_HC; i++) {
        table_hash_code[i] = -1;
    }
}

/****************************************************************************************************************/
                                    /*FONCTION D'INITIALISATION DE LA TABLE LEXICO*/
/****************************************************************************************************************/

// Initialise la table lexico

void init_table_lexico(){
    // Initialisation
    for (int i = 0; i < TAILLE_MAX_TABLE_LEXICO; i++) {
        lexico[i].longueur = -1;
        lexico[i].suivant = -1;
    }
    // Types de base -> Ordre d'insertion important !
    inserer_lexeme("entier");
    inserer_lexeme("reel");
    inserer_lexeme("char");
    inserer_lexeme("bool");
    inserer_lexeme("string");
}

/****************************************************************************************************************/
                                            /*FONCTION DE HASHAGE*/
/****************************************************************************************************************/

// Renvoie le hash-code d'un lexème

int hashage(char *lexeme) {
    int res = 0, total = 0,
        lgr_lexeme = strlen(lexeme);
    
    for (int i = 0; i < lgr_lexeme; i++)
        total += lexeme[i];
    res = total % 32;

    return res;
}

/****************************************************************************************************************/
                                    /*FONCTION D'INSERTION DE LEXEME*/
/****************************************************************************************************************/

// Insère un lexème dans la table lexico s'il n'y existe pas déjà

int inserer_lexeme(char *lexeme) {
    int hashcode = hashage(lexeme),
        lgr_lexeme = strlen(lexeme),
        ret = ligne_vide,
        num_lexico = table_hash_code[hashcode]; // Récup du num lexico associé au hash-code (= -1 ou numero lexico)

    if (num_lexico == -1) // Hash-code non repéré ?
        // Indice lexico associé à hash-code = indice du nouveau lexème = ligne_vide
        table_hash_code[hashcode] = ligne_vide;
    else { // Hash-code déjà présent ?
        while (num_lexico != -1) { // Tant qu'il y a un lexeme avec qui comparer
            if (lexico[num_lexico].longueur == lgr_lexeme) // Même longueur ?
                if (strcmp(lexico[num_lexico].lexeme, lexeme) == 0) // Même lexème ?
                    return num_lexico; // Pas d'ajout + retour de l'indice lexico de ce lexème
            if (lexico[num_lexico].suivant == -1) break;
            num_lexico = lexico[num_lexico].suivant;
        }
        // Insertion dans la classe du hash-code
        lexico[num_lexico].suivant = ligne_vide;
    }
    // Insertion dans la table lexico
    strcpy(lexico[ligne_vide].lexeme, lexeme);
    lexico[ligne_vide].longueur = lgr_lexeme;
    ligne_vide++;
    
    return ret;
}

/****************************************************************************************************************/
                                    /*FONCTION D'INSERTION DE LEXEME*/
/****************************************************************************************************************/

// Retourne le numéro lexico de [lexeme] s'il existe, -1 sinon

