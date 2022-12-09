/* 
 * Module associé à la machine virtuelle
 ** Fichier en-têt
 */

#ifndef _MACHINE_VIRTUELLE_H_
#define _MACHINE_VIRTUELLE_H_

#include "table_lexicographique.h"
#include "table_declarations.h"
#include "table_representation.h"
#include "table_regions.h"
#include "pile_execution.h"
#include "association.h"
#include "allocation.h"
#include "pile.h"

// Évalue un arbre représentatif d'une instruction
void evaluer_instruction(arbre a);
// Évalue un arbre représentatif d'une expression
elem_pile_exec_t evaluer_expression(arbre a);
// Evalue une fonction / procédure
void evaluer_fonc_proc(arbre a);
// Evalue la fonction lecture
void evaluer_lecture(arbre a);
// Evalue la fonction ecriture
void evaluer_ecriture(arbre a);
// Retourne une chaine de caractère sans les guillemets
char * enlever_quotes(char * chaine);

#endif