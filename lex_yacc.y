%{    
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/stat.h>
    #include "../inc/erreur.h"
    #include "../inc/association.h"
    #include "../inc/table_lexicographique.h"
    #include "../inc/table_declarations.h"
    #include "../inc/table_representation.h"
    #include "../inc/table_regions.h"
    #include "../inc/gestion_arbres.h"
    #include "../inc/pile.h"
    #include "../inc/machine_virtuelle.h"
    #include "../inc/gestion_fichiers.h"

    int yylex();
    int yyerror(const char *s);
    
    // Lex
    extern char *yytext;
    extern FILE *yyin;
    extern FILE *yyout;
    extern FILE* log_error;
    extern int yyleng;
    extern int num_ligne, num_caractere, num_mot;

    // table_declarations
    extern element_table_declarations table_declarations[TAILLE_MAX_TABLE_DECL];
    extern element_table_regions table_regions[TAILLE_MAX_TABLE_REGIONS];

    // table_representation
    extern int case_vide;
    extern dimensions_tab tab_dim[TAILLE_MAX_TABLEAU_DIMENSIONS];
    extern params_tab tab_param[TAILLE_MAX_TABLEAU_PARAMS];
    extern champs_tab tab_champ[TAILLE_MAX_TABLEAU_CHAMPS];
    extern int table_representation[TAILLE_MAX_TABLE_REPRESENTATION];               // Table de représentation

    // Arbre abstrait
    pile regions;

    // Yacc
    // // Déclarations
    int num_region = -1, description = -1, execution = 0, nature = N_VIDE;
    int nombre_parametres = 0;
    int nombre_dimensions = 0;
    int nombre_champs = 0;
    int deplacement = -1;
    int NIS = -1;
    int taille_region = 0;

    pile nums_decl;
    pile nums_lexicos;
    
    // // Instructions
    pile nombres_args;
    pile nums_dimensions;
    // // // Appels de fonctions/procédures
    int types_param[TAILLE_MAX_PILE][TAILLE_MAX_TABLEAU_PARAMS];
    int num_appel = -1;

    // Machine virtuelle
    extern pile regions_appelantes;
    extern pile pile_execution;
%}

%union  {
    int int_valeur;
    char * string_valeur;
    arbre arbre_abstrait_valeur;
}

// Terminaux

// Programme
%token PROG
// Blocs
%token DEBUT FIN
// Variables
%token VARIABLE DEUX_POINTS TYPE 
%token <string_valeur> IDF
// Tableaux
%token TABLEAU DE POINT_POINT CROCHET_OUVRANT CROCHET_FERMANT
// Structures
%token STRUCT FSTRUCT POINT_STRUCT
// Types simples
%token ENTIER REEL CARACTERE BOOLEEN
// Séparateurs
%token POINT_VIRGULE VIRGULE OPAFF
// Fonction / procédures
%token PROCEDURE FONCTION RETOURNE
%token PARENTHESE_OUVRANTE PARENTHESE_FERMANTE
// Instruction vidc
%token VIDE
// Mots-clés réservés
%token SI ALORS SINON TANT_QUE FAIRE SINON_SI FOR
// Opérateurs arithmétiques
%token PLUS MOINS MULT DIV MODULO
%token PLUS_AUG MOINS_AUG MULT_AUG DIV_AUG MOD_AUG
// Opérateurs log_erroriques
%token ET OU NOT
// Opérateurs de comparaison
%token SUP SUP_EGAL INF INF_EGAL EGAL NOT_EGAL
// Constantes
%token <int_valeur> CSTE_ENTIERE CSTE_CARACTERE 
%token <string_valeur> CSTE_REELLE CSTE_CHAINE 
%token <string_valeur> VRAI FAUX
//Entrées/Sorties
%token <string_valeur> LIRE ECRIRE

// Types

//// Terminaux

//// Non-terminaux

%type <arbre_abstrait_valeur> bool expression_comparaison expression_booleenne expression_booleenne2 expression_booleenne3 
expression_arithmetique expression_arithmetique2 expression_arithmetique3 expression affectation lecture liste_variables 
ecriture suite_ecriture instruction condition suite_condition_sinon_si suite_condition_sinon_si2 tant_que appel liste_arguments 
liste_args un_arg resultat_retourne liste_instructions suite_liste_inst variable boucle_for_c instruction_for instruction_for2
condition_for indiceS_tab indice_tab pointeur_struct pointeurS_struct une_variable affectation_augmentee



%%

//////////////////////////////////////////////////////////////////////////////////////////////////
/* ****************************************PROGRAMME******************************************* */

programme                                           :   PROG corps
                                                    ;

corps                                               :                                                                                   {
                                                                                                                                        NIS++;
                                                                                                                                        num_region++;
                                                                                                                                        if (num_region == 0) deplacement = NIS;
                                                                                                                                        table_regions[num_region].NIS = NIS;
                                                                                                                                        empiler(num_region, regions);
                                                                                                                                        }
                                                        liste_declarations liste_instructions                                           {
                                                                                                                                        NIS--;
                                                                                                                                        int num = depiler(regions);
                                                                                                                                        table_regions[num].arbre_abstrait = $3;
                                                                                                                                        }

                                                    ;

/* ****************************************FIN_PROGRAMME*************************************** */
//////////////////////////////////////////////////////////////////////////////////////////////////
/* ****************************************DECLARATIONS**************************************** */

liste_declarations                                  :   liste_declarations_types liste_declarations_variables
                                                        liste_declarations_procedures_fonctions
                                                    ;

// Règles - Déclaration de types

liste_declarations_types                            :   
                                                    |   liste_declarations_types declaration_type POINT_VIRGULE
                                                    ;

declaration_type                                    :   TYPE IDF                                                                        {
                                                                                                                                        verifier_surcharge_type($2, sommet(regions));
                                                                                                                                        empiler(inserer_lexeme($2), nums_lexicos);
                                                                                                                                        }
                                                        DEUX_POINTS suite_declaration_type
                                                    ;

suite_declaration_type                              :   STRUCT liste_champs FSTRUCT                                                     {
                                                                                                                                        int num_decl = inserer_declaration(depiler(nums_lexicos), sommet(regions), N_STRUCTURE, case_vide);
                                                                                                                                        inserer_representation_structure(nombre_champs);
                                                                                                                                        inserer_execution_declaration(num_decl, execution);
                                                                                                                                        execution = 0; nombre_champs = 0;
                                                                                                                                        }


                                                    |   TABLEAU                                                                         {
                                                                                                                                        empiler(inserer_declaration(depiler(nums_lexicos), sommet(regions), N_TABLEAU, case_vide), nums_decl);
                                                                                                                                        execution = 1;
                                                                                                                                        }    
                                                        dimension DE nom_type                                                           {
                                                                                                                                        execution *= table_declarations[sommet(nums_decl)].execution;
                                                                                                                                        inserer_representation_tableau(depiler(nums_decl), nombre_dimensions);
                                                                                                                                        inserer_execution_declaration(depiler(nums_decl), execution);
                                                                                                                                        execution = -1; nombre_dimensions = 0;
                                                                                                                                        }
                                                    ;

// // Structures

liste_champs                                        :   un_champ
                                                    |   liste_champs POINT_VIRGULE un_champ
                                                    ;

un_champ                                            :   IDF                                                                             { empiler(inserer_lexeme($1), nums_lexicos); }
                                                        DEUX_POINTS nom_type                                                            {              
                                                                                                                                        tab_champ[nombre_champs].num_lexico = depiler(nums_lexicos);
                                                                                                                                        tab_champ[nombre_champs].type = sommet(nums_decl);
                                                                                                                                        tab_champ[nombre_champs].execution = execution;
                                                                                                                                        execution += table_declarations[sommet(nums_decl)].execution;
                                                                                                                                        nombre_champs++;
                                                                                                                                        depiler(nums_decl);
                                                                                                                                        }
                                                    ;

// // Tableaux

dimension                                           :   CROCHET_OUVRANT liste_dimensions CROCHET_FERMANT
                                                    ;

liste_dimensions                                    :   une_dimension
                                                    |   liste_dimensions VIRGULE une_dimension
                                                    ; 
une_dimension                                       :   CSTE_ENTIERE POINT_POINT CSTE_ENTIERE                                           {
                                                                                                                                        execution *= $3 - $1 + 1;
                                                                                                                                        tab_dim[nombre_dimensions].borne_inf = $1;
                                                                                                                                        tab_dim[nombre_dimensions].borne_sup = $3;
                                                                                                                                        nombre_dimensions++;
                                                                                                                                        }
                                                    ;

// Fin Règles - Déclaration de types

// Règles - Déclaration de variables

liste_declarations_variables                        :   
                                                    |   liste_declarations_variables declaration_variable POINT_VIRGULE
                                                    ;

declaration_variable                                :   VARIABLE IDF                                                                    { empiler(inserer_lexeme($2), nums_lexicos); }
                                                        DEUX_POINTS nom_type                                                            {
                                                                                                                                        int num_decl;
                                                                                                                                        verifier_surcharge_var($2, sommet(nums_decl), sommet(regions));
                                                                                                                                        num_decl = inserer_declaration(depiler(nums_lexicos), sommet(regions), N_VARIABLE, sommet(nums_decl));
                                                                                                                                        execution = deplacement;
                                                                                                                                        inserer_execution_declaration(num_decl, execution);
                                                                                                                                        deplacement += table_declarations[sommet(nums_decl)].execution;
                                                                                                                                        table_regions[sommet(regions)].taille += table_declarations[sommet(nums_decl)].execution;
                                                                                                                                        depiler(nums_decl);
                                                                                                                                        }
                                                    ;
// Fin Règles - Déclaration de variables

// Règles - Déclaration de procédures ou fonctions

liste_declarations_procedures_fonctions             :   
                                                    |   liste_declarations_procedures_fonctions declaration_procedure_fonction
                                                    ;

declaration_procedure_fonction                      :   declaration_fonction
                                                    |   declaration_procedure
                                                    ;

declaration_procedure                               :   PROCEDURE IDF                                                                   {
                                                                                                                                        taille_region = 0;
                                                                                                                                        nombre_parametres = 0;
                                                                                                                                        deplacement = NIS + 1 + 1;
                                                                                                                                        }
                                                        liste_parametres                                                                {
                                                                                                                                        int types_param[TAILLE_MAX_TABLEAU_PARAMS], num_lexico, num_decl;
                                                                                                                                        for (int i = 0; i < nombre_parametres; i++)
                                                                                                                                            types_param[i] = tab_param[i].type;
                                                                                                                                        verifier_surcharge_fonc_proc($2, sommet(regions), types_param, nombre_parametres, N_PROCEDURE);
                                                                                                                                        num_lexico = inserer_lexeme($2);
                                                                                                                                        num_decl = inserer_declaration(num_lexico, sommet(regions), N_PROCEDURE, case_vide);
                                                                                                                                        inserer_execution_declaration(num_decl, num_region + 1);
                                                                                                                                        for (int i = 0; i < nombre_parametres; i++) {
                                                                                                                                            num_decl = inserer_declaration(tab_param[i].num_lexico, num_region + 1, N_PARAMETRE, tab_param[i].type);
                                                                                                                                            inserer_execution_declaration(num_decl, tab_param[i].execution);
                                                                                                                                        }
                                                                                                                                        inserer_representation_procedure(nombre_parametres);
                                                                                                                                        table_regions[num_region + 1].taille = taille_region;
                                                                                                                                        }
                                                        corps 
                                                    ;

declaration_fonction                                :   FONCTION IDF                                                                    {
                                                                                                                                        taille_region = 0;
                                                                                                                                        nombre_parametres = 0;
                                                                                                                                        deplacement = NIS + 1 + 1;
                                                                                                                                        }
                                                        liste_parametres RETOURNE type_simple                                           {
                                                                                                                                        int types_param[TAILLE_MAX_TABLEAU_PARAMS], num_lexico, num_decl;
                                                                                                                                        for (int i = 0; i < nombre_parametres; i++)
                                                                                                                                            types_param[i] = tab_param[i].type;
                                                                                                                                        verifier_surcharge_fonc_proc($2, sommet(regions), types_param, nombre_parametres, N_FONCTION);
                                                                                                                                        num_lexico = inserer_lexeme($2);
                                                                                                                                        num_decl = inserer_declaration(num_lexico, sommet(regions), N_FONCTION, case_vide);
                                                                                                                                        inserer_execution_declaration(num_decl, num_region + 1);
                                                                                                                                        for (int i = 0; i < nombre_parametres; i++) {
                                                                                                                                            num_decl = inserer_declaration(tab_param[i].num_lexico, num_region + 1, N_PARAMETRE, tab_param[i].type);
                                                                                                                                            inserer_execution_declaration(num_decl, tab_param[i].execution);
                                                                                                                                        }
                                                                                                                                        inserer_representation_fonction(depiler(nums_decl), nombre_parametres);
                                                                                                                                        table_regions[num_region + 1].taille = taille_region;
                                                                                                                                        }
                                                        corps
                                                    ;
                                                    
liste_parametres                                    :   PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE
                                                    ;

liste_param                                         :   un_param
                                                    |   liste_param VIRGULE un_param
                                                    ;
                        
un_param                                            :   
                                                    |   IDF                                                                             { empiler(inserer_lexeme($1), nums_lexicos); }
                                                        DEUX_POINTS type_simple                                                         {
                                                                                                                                        execution = deplacement;
                                                                                                                                        deplacement += table_declarations[sommet(nums_decl)].execution;
                                                                                                                                        taille_region += table_declarations[sommet(nums_decl)].execution;
                                                                                                                                        tab_param[nombre_parametres].num_lexico = depiler(nums_lexicos);
                                                                                                                                        tab_param[nombre_parametres].type = sommet(nums_decl);
                                                                                                                                        tab_param[nombre_parametres].execution = execution;
                                                                                                                                        nombre_parametres++;
                                                                                                                                        depiler(nums_decl);
                                                                                                                                        }
                                                    ;

// Fin Règles - Déclaration de procédures ou fonctions

// Règles - Types symples

nom_type                                            :   type_simple
                                                    |   IDF                                                                             { empiler(declaration_type($1, sommet(regions)), nums_decl); }
                                                    ;

type_simple                                         :   ENTIER                                                                          { empiler(0, nums_decl); }
                                                    |   REEL                                                                            { empiler(1, nums_decl); }
                                                    |   BOOLEEN                                                                         { empiler(3, nums_decl); }
                                                    |   CARACTERE                                                                       { empiler(2, nums_decl); }
                                                    ;

// Fin Règles - Types simples

/* ****************************************FIN_DECLARATIONS************************************ */
//////////////////////////////////////////////////////////////////////////////////////////////////
/* ****************************************INSTRUCTIONS**************************************** */

liste_instructions                                  :   DEBUT suite_liste_inst FIN                                                      { $$ = $2; }
                                                    ;

suite_liste_inst                                    :   instruction                                                                     { $$ = $1; }
                                                    |   instruction suite_liste_inst                                                    { 
                                                                                                                                        ajouter_frere($1->fils, $2);
                                                                                                                                        $$ = $1;
                                                                                                                                        }
                                                    ;

instruction                                         :   affectation POINT_VIRGULE                                                       { $$ = creer_noeud($1, A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    |   condition                                                                       { $$ = creer_noeud($1, A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    |   tant_que                                                                        { $$ = creer_noeud($1, A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    |   boucle_for_c                                                                    { $$ = creer_noeud($1, A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    |   appel POINT_VIRGULE                                                             { $$ = creer_noeud($1, A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    |   VIDE POINT_VIRGULE                                                              { $$ = creer_noeud(creer_noeud(arbre_vide(), A_INST_VIDE, -1, -1, arbre_vide()), A_LISTE_INST, -1, -1, arbre_vide()); } 
                                                    |   RETOURNE resultat_retourne POINT_VIRGULE                                        { $$ = creer_noeud(creer_noeud($2, A_INST_RET, -1, -1, arbre_vide()), A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    |   lecture POINT_VIRGULE                                                           { $$ = creer_noeud($1, A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    |   ecriture POINT_VIRGULE                                                          { $$ = creer_noeud($1, A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    |   affectation_augmentee POINT_VIRGULE                                             { $$ = creer_noeud($1, A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    ;

// Règles - Affectation

// // Affectation classique

affectation                                         :   variable OPAFF expression                                                       {
                                                                                                                                        $1 = ajouter_frere($1, $3);
                                                                                                                                        $$ = creer_noeud($1, A_AFFEC, -1, -1, arbre_vide());
                                                                                                                                        }
                                                    ;

// // Affectation augmentée

affectation_augmentee                               :   variable PLUS_AUG expression                                                    {
                                                                                                                                        arbre a = creer_noeud($1->fils, $1->type_noeud, $1->num_lexico, $1->num_decl, $1->frere);
                                                                                                                                        $3 = creer_noeud(ajouter_frere(a, $3), A_PLUS, -1, -1, arbre_vide());
                                                                                                                                        $1 = ajouter_frere($1, $3);
                                                                                                                                        $$ = creer_noeud($1, A_AFFEC, -1, -1, arbre_vide());
                                                                                                                                        }
                                                    |   variable MOINS_AUG expression                                                   {
                                                                                                                                        arbre a = creer_noeud($1->fils, $1->type_noeud, $1->num_lexico, $1->num_decl, $1->frere);
                                                                                                                                        $3 = creer_noeud(ajouter_frere(a, $3), A_MOINS, -1, -1, arbre_vide());
                                                                                                                                        $1 = ajouter_frere($1, $3);
                                                                                                                                        $$ = creer_noeud($1, A_AFFEC, -1, -1, arbre_vide());
                                                                                                                                        }
                                                    |   variable MULT_AUG expression                                                    {
                                                                                                                                        arbre a = creer_noeud($1->fils, $1->type_noeud, $1->num_lexico, $1->num_decl, $1->frere);
                                                                                                                                        $3 = creer_noeud(ajouter_frere(a, $3), A_MULT, -1, -1, arbre_vide());
                                                                                                                                        $1 = ajouter_frere($1, $3);
                                                                                                                                        $$ = creer_noeud($1, A_AFFEC, -1, -1, arbre_vide());
                                                                                                                                        }
                                                    |   variable DIV_AUG expression                                                     {
                                                                                                                                        arbre a = creer_noeud($1->fils, $1->type_noeud, $1->num_lexico, $1->num_decl, $1->frere);
                                                                                                                                        $3 = creer_noeud(ajouter_frere(a, $3), A_DIV, -1, -1, arbre_vide());
                                                                                                                                        $1 = ajouter_frere($1, $3);
                                                                                                                                        $$ = creer_noeud($1, A_AFFEC, -1, -1, arbre_vide());
                                                                                                                                        }
                                                    |   variable MOD_AUG expression                                                     {
                                                                                                                                        arbre a = creer_noeud($1->fils, $1->type_noeud, $1->num_lexico, $1->num_decl, $1->frere);
                                                                                                                                        $3 = creer_noeud(ajouter_frere(a, $3), A_MOD, -1, -1, arbre_vide());
                                                                                                                                        $1 = ajouter_frere($1, $3);
                                                                                                                                        $$ = creer_noeud($1, A_AFFEC, -1, -1, arbre_vide());
                                                                                                                                        }

// Fin Règles - Affectation

// Règles - condition, boucle

// // IF THEN, ELSE IF, ELSE

condition                                           :   SI expression_booleenne ALORS liste_instructions suite_condition_sinon_si       { $$ = creer_noeud(ajouter_frere($2, ajouter_frere($4, $5)), A_ITE, -1, -1, arbre_vide()); }
                                                    |   SI expression_booleenne ALORS liste_instructions                                { $$ = creer_noeud(ajouter_frere($2, $4), A_IT, -1, -1, arbre_vide()); }
                                                    ;

suite_condition_sinon_si                            :   suite_condition_sinon_si2 suite_condition_sinon_si                              { $$ = ajouter_frere($1, $2); }
                                                    |   suite_condition_sinon_si2                                                       { $$ = $1; }
                                                    ;

suite_condition_sinon_si2                           :   SINON_SI expression_booleenne ALORS liste_instructions                          { $$ = creer_noeud(ajouter_frere($2, $4), A_EI, -1, -1, arbre_vide()); }
                                                    |   SINON liste_instructions                                                        { $$ = $2; }
                                                    ;

// // TANT QUE

tant_que                                            :   TANT_QUE expression_booleenne FAIRE liste_instructions                          { $$ = creer_noeud(ajouter_frere($2, $4), A_WHILE, -1, -1, arbre_vide()); }
                                                    ;

// // Boucle for de style c

boucle_for_c                                        :   FOR PARENTHESE_OUVRANTE instruction_for POINT_VIRGULE condition_for 
                                                        POINT_VIRGULE instruction_for PARENTHESE_FERMANTE liste_instructions            {
                                                                                                                                        $7 = creer_noeud($7, A_INC_FOR, -1, -1, $9);
                                                                                                                                        $5 = creer_noeud($5, A_COND_FOR, -1, -1, $7);
                                                                                                                                        $3 = creer_noeud($3, A_INIT_FOR, -1, -1, $5);
                                                                                                                                        $$ = creer_noeud($3, A_FOR, -1, -1, arbre_vide());
                                                                                                                                        }
                                                    ;

instruction_for                                     :   instruction_for VIRGULE instruction_for2                                        {
                                                                                                                                        ajouter_frere($1->fils, $3);
                                                                                                                                        $$ = $1;
                                                                                                                                        }
                                                    |   instruction_for2                                                                { $$ = $1; }
                                                    ;

instruction_for2                                    :                                                                                   { $$ = arbre_vide(); }
                                                    |   affectation                                                                     { $$ = creer_noeud($1, A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique                                                         { $$ = creer_noeud($1, A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    |   affectation_augmentee                                                           { $$ = creer_noeud($1, A_LISTE_INST, -1, -1, arbre_vide()); }
                                                    ;

condition_for                                       :                                                                                   { $$ = arbre_vide(); }
                                                    |   expression_booleenne                                                            { $$ = $1; }
                                                    ;
// Règles - Appel de fonctions/procédures

appel                                               :   IDF                                                                             {
                                                                                                                                        empiler(0, nombres_args);
                                                                                                                                        num_appel++;
                                                                                                                                        }
                                                        liste_arguments                                                                 {
                                                                                                                                        int nd = declaration_fonc_proc($1, sommet(regions), types_param[num_appel], sommet(nombres_args)),
                                                                                                                                            nat = (table_declarations[nd].nature == N_FONCTION) ? A_APPEL_FCT : A_APPEL_PROC;
                                                                                                                                        $$ = creer_noeud($3, nat, num_lexico($1), nd, arbre_vide());
                                                                                                                                        num_appel--;
                                                                                                                                        depiler(nombres_args);
                                                                                                                                        }
                                                    ;

liste_arguments                                     :   PARENTHESE_OUVRANTE liste_args PARENTHESE_FERMANTE                              { 
                                                                                                                                        $$ = $2;
                                                                                                                                        }
                                                    ;

liste_args                                          :   un_arg VIRGULE liste_args                                                       {
                                                                                                                                        ajouter_frere($1->fils, $3);
                                                                                                                                        $$ = $1;
                                                                                                                                        }
                                                    |   un_arg                                                                          { $$ = $1; }
                                                    ;

un_arg                                              :                                                                                   { $$ = arbre_vide(); }
                                                    |   expression                                                                      {
                                                                                                                                        types_param[num_appel][sommet(nombres_args)] = calculer_type($1);
                                                                                                                                        empiler(depiler(nombres_args) + 1, nombres_args);
                                                                                                                                        $$ = creer_noeud($1, A_LISTE_P, -1, -1, arbre_vide());
                                                                                                                                        }
                                                    ;
                                                    
// Fin Règles - Appel de fonctions/procédures



// Règles - Retour

resultat_retourne                                   :                                                                                   { $$ = arbre_vide(); }
                                                    |   expression                                                                      { $$ = $1; }
                                                    ;

// Fin Règles - Retour

/* ****************************************FIN_INSTRUCTIONS************************************ */
//////////////////////////////////////////////////////////////////////////////////////////////////
/* ****************************************VARIABLES******************************************* */

variable                                            :   IDF                                                                             {
                                                                                                                                        int num_decl = declaration_var($1, sommet(regions), N_BASE);
                                                                                                                                        $$ = creer_noeud(arbre_vide(), A_IDF, num_lexico($1), num_decl, arbre_vide());
                                                                                                                                        }
                                                    |   IDF                                                                             {
                                                                                                                                        empiler(num_lexico($1), nums_lexicos);
                                                                                                                                        empiler(declaration_var($1, sommet(regions), N_STRUCTURE), nums_decl);
                                                                                                                                        empiler(table_declarations[sommet(nums_decl)].description, nums_decl);
                                                                                                                                        }
                                                        pointeurS_struct                                                                {
                                                                                                                                        depiler(nums_decl);
                                                                                                                                        $$ = creer_noeud($3, A_IDF, depiler(nums_lexicos), depiler(nums_decl), arbre_vide());
                                                                                                                                        }
                                                    |   IDF                                                                             { 
                                                                                                                                        empiler(0, nums_dimensions);
                                                                                                                                        empiler(num_lexico($1), nums_lexicos);
                                                                                                                                        empiler(declaration_var($1, sommet(regions), N_TABLEAU), nums_decl);
                                                                                                                                        empiler(table_declarations[sommet(nums_decl)].description, nums_decl);
                                                                                                                                        }
                                                        indiceS_tab                                                                     {
                                                                                                                                        depiler(nums_decl);
                                                                                                                                        $$ = creer_noeud($3, A_IDF, depiler(nums_lexicos), depiler(nums_decl), arbre_vide());
                                                                                                                                        depiler(nums_dimensions);
                                                                                                                                        }
                                                    ;

// Règles - Élément d'un tableau

indiceS_tab                                         :   indice_tab indiceS_tab                                                          {
                                                                                                                                        ajouter_frere($1->fils, $2);
                                                                                                                                        $$ = $1;
                                                                                                                                        }
                                                    |   indice_tab                                                                      { $$ = $1; }
                                                    |   indice_tab                                                                      { empiler(table_representation[table_declarations[$1->num_decl].description], nums_decl); }
                                                        pointeurS_struct                                                                {
                                                                                                                                        $$ = ajouter_frere($1, $3);
                                                                                                                                        depiler(nums_decl);
                                                                                                                                        }
                                                    ;


indice_tab                                          :   CROCHET_OUVRANT expression_arithmetique CROCHET_FERMANT                         {
                                                                                                                                        int num_dimension = depiler(nums_dimensions);
                                                                                                                                        empiler(++num_dimension, nums_dimensions);
                                                                                                                                        $$ = creer_noeud($2, A_IND_TAB, num_dimension, sommet(nums_decl), arbre_vide());
                                                                                                                                        }
                                                    ;
                                       

// Fin Règles - Élément d'un tableau

// Règles - Élément d'une structure

pointeurS_struct                                    :   pointeur_struct pointeurS_struct                                                {
                                                                                                                                        ajouter_fils($1, $2);
                                                                                                                                        $$ = $1;
                                                                                                                                        }
                                                    |   pointeur_struct                                                                 { $$ = $1; }
                                                    |   pointeur_struct                                                                 { empiler(table_representation[$1->num_decl + 1], nums_decl); }
                                                        indiceS_tab                                                                     {
                                                                                                                                        $$ = ajouter_frere($1, $3);
                                                                                                                                        depiler(nums_decl);
                                                                                                                                        }
                                                    ;

pointeur_struct                                     :   POINT_STRUCT IDF                                                                { $$ = creer_noeud(arbre_vide(), A_POINT_STRUCT, num_lexico($2), index_rep_struct(sommet(nums_decl), num_lexico($2)), arbre_vide()); }
                                                    ;  
                        

// Fin Règles - Élément d'une structure



/* ****************************************FIN_VARIABLES*************************************** */
//////////////////////////////////////////////////////////////////////////////////////////////////
/* ****************************************EXPRESSIONS***************************************** */

expression                                          :   expression_arithmetique                                                         { $$ = $1; }
                                                    |   expression_booleenne                                                            { $$ = $1; }
                                                    ;
                                            

// Règles - Expressions arithmétiques

expression_arithmetique                             :   expression_arithmetique PLUS expression_arithmetique2                           { $$ = creer_noeud(ajouter_frere($1, $3), A_PLUS, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique MOINS expression_arithmetique2                          { $$ = creer_noeud(ajouter_frere($1, $3), A_MOINS,-1, -1, arbre_vide()); }
                                                    |   expression_arithmetique2                                                        { $$ = $1; }
                                                    ;

expression_arithmetique2                            :   expression_arithmetique2 MULT expression_arithmetique3                          { $$ = creer_noeud(ajouter_frere($1, $3), A_MULT, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique2 DIV expression_arithmetique3                           { $$ = creer_noeud(ajouter_frere($1, $3), A_DIV, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique2 MODULO expression_arithmetique3                        { $$ = creer_noeud(ajouter_frere($1, $3), A_MOD, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique3                                                        { $$ = $1; }
                                                    ;

expression_arithmetique3                            :   PARENTHESE_OUVRANTE expression_arithmetique PARENTHESE_FERMANTE                 { $$ = $2; }
                                                    |   CSTE_ENTIERE                                                                    { $$ = creer_noeud(arbre_vide(), A_CSTE_E, $1, -1, arbre_vide()); }
                                                    |   CSTE_REELLE                                                                     { $$ = creer_noeud(arbre_vide(), A_CSTE_R, inserer_lexeme($1), -1, arbre_vide() ); }
                                                    |   CSTE_CARACTERE                                                                  { $$ = creer_noeud(arbre_vide(), A_CSTE_C, $1, -1, arbre_vide()); }
                                                    |   CSTE_CHAINE                                                                     { $$ = creer_noeud(arbre_vide(), A_CSTE_S, inserer_lexeme($1), -1, arbre_vide()); }
                                                    |   appel                                                                           { $$ = $1; }
                                                    |   variable                                                                        { $$ = $1; }
                                                    ;

// Fin Règles - Expressions arithmétiques

// Règles - Expressions booléennes

expression_booleenne                                :   expression_booleenne OU expression_booleenne2                                   { $$ = creer_noeud(ajouter_frere($1, $3), A_OR, -1, -1, arbre_vide()); }
                                                    |   expression_booleenne2                                                           { $$ = $1;}
                                                    ;

expression_booleenne2                               :   expression_booleenne2 ET expression_booleenne3                                  { $$ = creer_noeud(ajouter_frere($1, $3), A_AND, -1, -1, arbre_vide()); }
                                                    |   expression_booleenne3                                                           { $$ = $1; }
                                                    ;

expression_booleenne3                               :   PARENTHESE_OUVRANTE expression_booleenne PARENTHESE_FERMANTE                    { $$ = $2; }
                                                    |   NOT PARENTHESE_OUVRANTE expression_booleenne PARENTHESE_FERMANTE                { $$ = creer_noeud(0, A_NOT, -1, -1, arbre_vide()); }
                                                    |   expression_comparaison                                                          { $$ = $1;}                                                                     
                                                    ;

// Fin Règles - Expressions booléennes

// Règles - Expressions de comparaison
// // Attention on ne peut pas faire si (x) faire ... même si x est une variable booleenne, il faut faire si (x == vrai) faire ... (Recommandé par le prof)
expression_comparaison                              :   expression_arithmetique SUP expression_arithmetique                             { $$ = creer_noeud(ajouter_frere($1, $3), A_SUP, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique SUP_EGAL expression_arithmetique                        { $$ = creer_noeud(ajouter_frere($1, $3), A_SUP_EGAL, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique INF expression_arithmetique                             { $$ = creer_noeud(ajouter_frere($1, $3), A_INF, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique INF_EGAL expression_arithmetique                        { $$ = creer_noeud(ajouter_frere($1, $3), A_INF_EGAL, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique EGAL expression_arithmetique                            { $$ = creer_noeud(ajouter_frere($1, $3), A_EGAL_EGAL, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique NOT_EGAL expression_arithmetique2                       { $$ = creer_noeud(ajouter_frere($1, $3), A_NOT_EGAL, -1, -1, arbre_vide()); }
                                                    |   PARENTHESE_OUVRANTE expression_booleenne PARENTHESE_FERMANTE EGAL bool          { $$ = creer_noeud(ajouter_frere($2, $5), A_EGAL_EGAL, -1, -1, arbre_vide()); }
                                                    |   PARENTHESE_OUVRANTE expression_booleenne PARENTHESE_FERMANTE NOT_EGAL bool      { $$ = creer_noeud(ajouter_frere($2, $5), A_NOT_EGAL, -1, -1, arbre_vide()); }
                                                    |   bool EGAL PARENTHESE_OUVRANTE expression_booleenne PARENTHESE_FERMANTE          { $$ = creer_noeud(ajouter_frere($1, $4), A_EGAL_EGAL, -1, -1, arbre_vide()); }
                                                    |   bool NOT_EGAL PARENTHESE_OUVRANTE expression_booleenne PARENTHESE_FERMANTE      { $$ = creer_noeud(ajouter_frere($1, $4), A_NOT_EGAL, -1, -1, arbre_vide()); }
                                                    |   bool EGAL bool                                                                  { $$ = creer_noeud(ajouter_frere($1, $3), A_EGAL_EGAL, -1, -1, arbre_vide()); }
                                                    |   bool NOT_EGAL bool                                                              { $$ = creer_noeud(ajouter_frere($1, $3), A_NOT_EGAL, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique EGAL bool                                               { $$ = creer_noeud(ajouter_frere($1, $3), A_EGAL_EGAL, -1, -1, arbre_vide()); }
                                                    |   expression_arithmetique NOT_EGAL bool                                           { $$ = creer_noeud(ajouter_frere($1, $3), A_NOT_EGAL, -1, -1, arbre_vide()); }
                                                    |   bool EGAL expression_arithmetique                                               { $$ = creer_noeud(ajouter_frere($1, $3), A_EGAL_EGAL, -1, -1, arbre_vide()); }
                                                    |   bool NOT_EGAL expression_arithmetique                                           { $$ = creer_noeud(ajouter_frere($1, $3), A_NOT_EGAL, -1, -1, arbre_vide()); }
                                                    |   bool                                                                            { $$ = $1; }
                                                    ;

bool                                                :   VRAI                                                                            { $$ = creer_noeud(arbre_vide(), A_TRUE, 1, -1, arbre_vide()); }
                                                    |   FAUX                                                                            { $$ = creer_noeud(arbre_vide(), A_FALSE, 0, -1, arbre_vide()); }
                                                    ;

// Fin Règles - Expressions de comparaison

/* ****************************************FIN_EXPRESSIONS************************************* */
//////////////////////////////////////////////////////////////////////////////////////////////////


/* ****************************************DEBUT_ENTREE/SORTIE********************************* */
//////////////////////////////////////////////////////////////////////////////////////////////////

lecture                                             :   LIRE                                                                            { empiler(0, nombres_args); }
                                                        PARENTHESE_OUVRANTE liste_variables PARENTHESE_FERMANTE                         { $$ = creer_noeud($4, A_LECTURE, depiler(nombres_args), -1, arbre_vide()); }
                                                    ;

liste_variables                                     :   une_variable VIRGULE liste_variables                                            {
                                                                                                                                        ajouter_frere($1->fils, $3);
                                                                                                                                        $$ = $1;
                                                                                                                                        }
                                                    |   une_variable                                                                    { $$ = $1; }
                                                    ;

une_variable                                        :                                                                                   { $$ = arbre_vide(); }
                                                    |   variable                                                                        {
                                                                                                                                        $$ = creer_noeud($1, A_LISTE_P, -1, -1, arbre_vide());
                                                                                                                                        empiler(depiler(nombres_args) + 1, nombres_args);
                                                                                                                                        }
                                                    ;

ecriture                                            :   ECRIRE                                                                          { empiler(0, nombres_args); }
                                                        PARENTHESE_OUVRANTE CSTE_CHAINE suite_ecriture PARENTHESE_FERMANTE              {
                                                                                                                                        $5 = creer_noeud(arbre_vide(), A_LISTE_P, inserer_lexeme($4), -1, $5);
                                                                                                                                        $$ = creer_noeud($5, A_ECRITURE, depiler(nombres_args), -1, arbre_vide());
                                                                                                                                        }
                                                    ;

suite_ecriture                                      :                                                                                   { $$ = arbre_vide(); }
                                                    |   VIRGULE expression suite_ecriture                                               {
                                                                                                                                        $2 = creer_noeud($2, A_LISTE_P, -1, -1, arbre_vide());
                                                                                                                                        ajouter_frere($2->fils, $3);
                                                                                                                                        $$ = $2;
                                                                                                                                        empiler(depiler(nombres_args) + 1, nombres_args);
                                                                                                                                        }
                                                    ;

/* ****************************************FIN_ENTREE/SORTIE*********************************** */
//////////////////////////////////////////////////////////////////////////////////////////////////

%%

int yyerror(const char *s) {
    erreur("Syntaxe incorrecte à la ligne %d, caractère %d, mot %d : %s !\n", num_ligne, num_caractere, num_mot, yytext);
    return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
    // Vérification des arguments
    if (argc < 2 || argc > 4) usage(argv[0], "Le nombre d'arguments %d n'est pas correct !\n", argc);
    if (strcmp(argv[1], "-h") == 0) usage(argv[0], "");
    else if (argc < 3) usage(argv[0], "Le nombre d'arguments %d n'est pas correct !\n", argc);
    else if (strcmp(argv[1], "-c") != 0) usage(argv[0], "L'option %s n'est pas disponible !\n", argv[1]);
    // Ouverture des fichiers d'entrée/sortie
    yyin = fopen(argv[2], "r");
    if (yyin == NULL) erreur("Échec de l'ouverture du fichier d'entrée \"%s\" !\n", argv[2]);
    if (argc == 4) {
        if (verifier_fichier(argv[3])) demander_confirmation_ecrasement(argv[3]);
        yyout = fopen(argv[3], "w+");
        if (yyout == NULL) avertissement("Échec de l'ouverture du fichier de sortie \"%s\" !\n", argv[3]);
    }
    if (yyout == NULL) {
        char * default_rep = "default";
        char * default_file;
        if (!verifier_dossier(default_rep)) mkdir(default_rep, S_IRWXU);
        default_file = creer_chemin_fichier(default_rep, "default");
        default_file = renvoyer_nom_modifie(default_file);
        yyout = fopen(default_file, "w+");
        if (yyout == NULL) erreur("Échec de l'ouverture du fichier de sortie par défaut \"%s\" !\n", default_file);
    }
    /* Ouverture d'un fichier "log_error" pour les erreurs
    log_error = fopen("log_error", "a+");
    if (log_error == NULL) {
        avertissement("Échec de l'ouverture du fichier \"log_error\" pour les erreurs/avertissements !\n");
        log_error = stderr;
    }
    // */
    // Initialisations
    init_table_hashage();                   // Initialisation de la table des hash-codes
    init_table_lexico();                    // Initialisation de la table lexico
    init_table_declarations();              // Initialisation de la table des déclarations
    init_table_representation();            // Initialisation de la table de représentation
    init_table_regions();                   // Initialisation de la table des régions
    init_pile_execution();

    initialiser(regions);
    initialiser(pile_execution);
    initialiser(nums_dimensions);
    initialiser(nombres_args);
    initialiser(nums_decl);
    initialiser(nums_lexicos);

    // Compilation
    //// Analyse
    yyparse();
    // Affichages
    //// Affichade de la table lexico
    affichage_table_lexico();
    //// Affichage de la table des déclarations
    affichage_table_declarations();
    //// Affichage de la table de représentation
    affichage_table_representation();
    //// Affichage de la table des régions
    affichage_table_regions();
    //// Affichage des arbres abstraits
    affichage_regions_arbres();

    // Exécution
    //// Machine virtuelle
    initialiser(regions_appelantes);
    inserer_region(0);
    evaluer_instruction(table_regions[0].arbre_abstrait);
    //// Affichage de la pile d'exécution
    affichage_pile_execution();
    
    // Fermeture normale
    fclose(yyin);
    fclose(yyout);
    if (log_error != stderr) fclose(log_error);
    exit(EXIT_SUCCESS);
}