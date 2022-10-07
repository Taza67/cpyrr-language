/* 
 * Module associé à la gestion des erreurs
 ** Fichier source
 */

#include "erreur.h"

FILE* log_error;

void viderBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF)
        c = getchar();
}

/****************************************************************************************************************/
                                        /*FONCTIONS DE GESTION D'ERREURS*/
/****************************************************************************************************************/

// Affiche un message d'erreur et provoque la fermeture forcée du programme
void erreur(char * message, ...) {
    time_t now;
    time(&now);
    struct tm *now_s = localtime(&now);
    va_list arg;
    va_start(arg, message);
    if (log_error != stderr)
        fprintf(log_error, "- [%02d/%02d/%d -- %02d:%02d:%02d] Erreur -> ", 
                now_s->tm_mday, now_s->tm_mon, now_s->tm_year + 1900,now_s->tm_hour, now_s->tm_min, now_s->tm_sec);
    else
        fprintf(log_error, "- Erreur -> ");
    vfprintf(log_error, message, arg);
    if (message[strlen(message) - 2] != '\n')
        fprintf(log_error, "\n");
    va_end(arg);
    exit(EXIT_FAILURE);
}

// Affiche un message d'avertissement
void avertissement(char * message, ...) {
    time_t now;
    time(&now);
    struct tm *now_s = localtime(&now);
    va_list arg;
    va_start(arg, message);
    if (log_error != stderr)
        fprintf(log_error, "- [%02d/%02d/%d -- %02d:%02d:%02d] Avertissement -> ", 
                now_s->tm_mday, now_s->tm_mon, now_s->tm_year + 1900,now_s->tm_hour, now_s->tm_min, now_s->tm_sec);
    else
        fprintf(log_error, "- Avertissement -> ");
    vfprintf(log_error, message, arg);
    va_end(arg);
}

// Affiche la notice d'utilisation du programme
void usage(char * programme, char * message, ...) {
    va_list arg;
    va_start(arg, message);
    if (strlen(message) != 0) {
        fprintf(stderr, "- Message -> ");
        vfprintf(stderr, message, arg);
        if (message[strlen(message) - 2] != '\n')
            fprintf(log_error, "\n");
        fprintf(stderr, "\n");
    }
    va_end(arg);
    fprintf(stderr, "- Objet : Compilateur du langage CPYRR\n"
                    "- Usage : %s <option> <fichier_entree> [<fichier_sortie>]\n"
                    "+ - Arguments :\n"
                    "+ + - <option>         :   - { -c } : compile le programme donné.\n"
                    "                           - { -h } : affiche la notice d'utilisation du programme.\n"
                    "+ + - <fichier_entree> :   code source en langage CPYRR, accessible en lecture.\n"
                    "+ + - <fichier_sortie> :   - nom du fichier de sortie qui contiendra le texte intermédiare.\n"
                    "                           - [optionnel] : si non indiqué, un fichier par défaut sera généré.\n",
                    programme);
    exit(EXIT_FAILURE);
}