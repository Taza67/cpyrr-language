/* 
 * Module associé à l'association
 ** Fichier entête
 */

#include "erreur.h"
#include "table_declarations.h"
#include "table_representation.h"
#include "gestion_arbres.h"
#include "pile_execution.h"

// Prototypes des fonctions
// Vérifie si une déclaration de type ne fait pas conflit avec une précédente de la même région
int verifier_surcharge_type(char * lexeme, int num_region);
// Vérifie si une déclaration de variable ne fait pas conflit avec une précédente du même type et de la même région
int verifier_surcharge_var(char * lexeme, int type, int num_region);
// Vérifie si une déclaration de fonction/procedure ne fait pas conflit avec une précédente de même structure
int verifier_surcharge_fonc_proc(char *lexeme, int num_region, int types_param[], int nombre_params, int nature);
// Renvoie l'index dans la table de représentation du champ d'une structure
int index_rep_struct(int num_decl, int num_lexico_champ);
// Récupère les infos sur une fonction/procédure
void recuperer_infos_fonc_proc(int num_decl, int *type_retourne, int *nombre_params, int types_param[]);
// Renvoie le numéro de la dernière bonne déclaration associée à un numéro lexico d"un type
int declaration_type(char *lexeme, int num_region);
// Renvoie le numéro de la dernière bonne déclaration associée à un numéro lexico d"une variable
int declaration_var(char *lexeme, int num_region, int nature_type);
// Renvoie le numéro de la dernière bonne déclaration associée à un numéro lexico d'une fonction/procédure
int declaration_fonc_proc(char *lexeme, int num_region, int types_param[], int nombre_params);
// Renvoie le numéro de déclaration du type d'un arbre représentant un idf
int calculer_type_idf(arbre a);
// Renvoie le numéro de déclaration du type de la valeur retournée par une fonction
int calculer_type_fonct(int num_decl);
// Renvoie le numéro de déclaration du type d'une variable case d'un tableau
int calculer_type_case(int num_decl);
// Renvoie le numéro de déclaration du type d'une variable champ d'une structure
int calculer_type_champ(int index_rep);
// Renvoie le numéro déclaration du type d'une expression représenté par un arbre
int calculer_type(arbre a);
// Vérifie la concordance des types des opérances d'une expression représenté par un arbre et renvoie le numéro déclaration de son type
int verifier_concordance_type(arbre a);
// Vérifie si une expression est possible avec un type donné
int verifier_type(int type, arbre a);
// Renvoie une chaine de caractère représentant un type
char * type_string(int num_decl);
// Renvoie une chaine de caractère réprésentant une liste de types
char * liste_types_string(int liste_types[TAILLE_MAX_TABLEAU_PARAMS], int taille);