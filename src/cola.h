#ifndef COLA_H_
#define COLA_H_
#include <stdlib.h>
#include <stdbool.h>

typedef struct cola cola_t;

cola_t *cola_crear();

bool cola_encolar(cola_t *cola, void *elemento);

void *cola_desencolar(cola_t *cola);

void *cola_ver_primero(cola_t *cola);

size_t cola_cantidad(cola_t *cola);

void cola_destruir(cola_t *cola);

#endif // COLA_H_
