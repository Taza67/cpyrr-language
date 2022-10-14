
#include "allocation.h"

void * allocation_mem(size_t nobjets, size_t taille) {
    void * pt;
    pt = malloc (nobjets * taille);
    if (pt == NULL)
        erreur("Impossible d'allouer la memoire %d * %d !\n", nobjets, taille);
    return pt;
}

void * allocation_mem_init0(size_t nobjets, size_t taille) {
    void * pt;
    pt = calloc(nobjets, taille);
    if (pt == NULL)
        erreur("Impossible d'allouer la mémoire %d * %d !\n", nobjets, taille);
    return pt;
}

