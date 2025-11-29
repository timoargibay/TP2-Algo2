#ifndef PILA_H_
#define PILA_H_
#include <stdlib.h>
#include <stdbool.h>

typedef struct pila pila_t;

pila_t *pila_crear();

bool pila_apilar(pila_t *pila, void *elemento);

void *pila_desapilar(pila_t *pila);

void *pila_ver_primero(pila_t *pila);

size_t pila_cantidad(pila_t *pila);

void pila_destruir(pila_t *pila);

#endif // PILA_H_
