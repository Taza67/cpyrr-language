%{
    #include <stdlib.h>
    #include <string.h>
    #include "../inc/gestion_arbres.h"
    #include "lex_yacc.tab.h"
    #define ERREUR 999
    int num_ligne = 0,
        num_mot = 1,
        num_caractere = 1,
        commentaire = 0;
    void comptage() {
        num_mot++; 
        num_caractere += yyleng;
    }
%}

/* Commentaire */
SPECIAUX[(\/)(\\)(\')(\")(0)(n)(t)]


%%

\/\/[^\n]*[\n]              {num_ligne++;}
(\/\*)                      {commentaire = 1;}
(\*\/)                      {commentaire = 0;}

prog                        {comptage(); if (!commentaire) return PROG;}
debut                       {comptage(); if (!commentaire) return DEBUT;}
fin                         {comptage(); if (!commentaire) return FIN;}

var                         {comptage(); if (!commentaire) return VARIABLE;}
\:                          {comptage(); if (!commentaire) return DEUX_POINTS;}
type                        {comptage(); if (!commentaire) return TYPE;}

tableau                     {comptage(); if (!commentaire) return TABLEAU;}
de                          {comptage(); if (!commentaire) return DE;}
\.\.                        {comptage(); if (!commentaire) return POINT_POINT;}
\,                          {comptage(); if (!commentaire) return VIRGULE;}
\[                          {comptage(); if (!commentaire) return CROCHET_OUVRANT;}
\]                          {comptage(); if (!commentaire) return CROCHET_FERMANT;}

struct                      {comptage(); if (!commentaire) return STRUCT;}
fstruct                     {comptage(); if (!commentaire) return FSTRUCT;}
\.                          {comptage(); if (!commentaire) return POINT_STRUCT;}

entier                      {comptage(); if (!commentaire) return ENTIER;}
reel                        {comptage(); if (!commentaire) return REEL;}
char                        {comptage(); if (!commentaire) return CARACTERE;}
bool                        {comptage(); if (!commentaire) return BOOLEEN;}

\;                          {comptage(); if (!commentaire) return POINT_VIRGULE;}
=                           {comptage(); if (!commentaire) return OPAFF;}

procedure                   {comptage(); if (!commentaire) return PROCEDURE;}
fonction                    {comptage(); if (!commentaire) return FONCTION;}
retourne                    {comptage(); if (!commentaire) return RETOURNE;}

\(                          {comptage(); if (!commentaire) return PARENTHESE_OUVRANTE;}
\)                          {comptage(); if (!commentaire) return PARENTHESE_FERMANTE;}

vide                        {comptage(); if (!commentaire) return VIDE;}

si                          {comptage(); if (!commentaire) return SI;}
alors                       {comptage(); if (!commentaire) return ALORS;}
sinon                       {comptage(); if (!commentaire) return SINON;}
tant_que                    {comptage(); if (!commentaire) return TANT_QUE;}
faire                       {comptage(); if (!commentaire) return FAIRE;}
sinon_si                    {comptage(); if (!commentaire) return SINON_SI;}
for                         {comptage(); if (!commentaire) return FOR;}

\+                          {comptage(); if (!commentaire) return PLUS;}
\-                          {comptage(); if (!commentaire) return MOINS;}
\/                          {comptage(); if (!commentaire) return DIV;}
\*                          {comptage(); if (!commentaire) return MULT;}
\%                          {comptage(); if (!commentaire) return MODULO;}

\+=                         {comptage(); if (!commentaire) return PLUS_AUG;}
\-=                         {comptage(); if (!commentaire) return MOINS_AUG;}
\/=                         {comptage(); if (!commentaire) return DIV_AUG;}
\*=                         {comptage(); if (!commentaire) return MULT_AUG;}
\%=                         {comptage(); if (!commentaire) return MOD_AUG;}

!                           {comptage(); if (!commentaire) return NOT;}
&&                          {comptage(); if (!commentaire) return ET;}
\|\|                        {comptage(); if (!commentaire) return OU;}
vrai                        {comptage(); if (!commentaire) { yylval.string_valeur = strdup(yytext); return VRAI; }}
faux                        {comptage(); if (!commentaire) { yylval.string_valeur = strdup(yytext); return FAUX; }}

\>                          {comptage(); if (!commentaire) return SUP;}
\>\=                        {comptage(); if (!commentaire) return SUP_EGAL;}
\<                          {comptage(); if (!commentaire) return INF;}
\<\=                        {comptage(); if (!commentaire) return INF_EGAL;}
\=\=                        {comptage(); if (!commentaire) return EGAL;}
\!\=                        {comptage(); if (!commentaire) return NOT_EGAL;}

lire                        {comptage(); if (!commentaire) { yylval.string_valeur = strdup(yytext); return LIRE; }}
ecrire                      {comptage(); if (!commentaire) { yylval.string_valeur = strdup(yytext); return ECRIRE; }}

(0|-?([1-9][0-9]*))         {comptage(); if (!commentaire) { yylval.int_valeur = atoi(yytext); return CSTE_ENTIERE; }}
(0|-?([1-9][0-9]*\.[0-9]+)) {comptage(); if (!commentaire) { yylval.string_valeur = strdup(yytext); return CSTE_REELLE; }}
\'([^\']|(\\{SPECIAUX}))\'  {comptage(); if (!commentaire) { yylval.int_valeur = yytext[1]; return CSTE_CARACTERE; }}
\"([^\"]|(\\{SPECIAUX}))*\" {comptage(); if (!commentaire) { yylval.string_valeur = strdup(yytext); return CSTE_CHAINE; }}

[a-zA-Z][a-zA-Z0-9]*        {comptage(); if (!commentaire) { yylval.string_valeur = strdup(yytext); return IDF; }}

[ \t\r]                     {num_caractere++;}
[\n]                        {num_caractere = 0; num_mot = 0; num_ligne++;}
.                           {return ERREUR;}

%%