#ifndef ABB_H_
#define ABB_H_
#include <stdlib.h>
#include <stdbool.h>

enum abb_recorrido { ABB_INORDEN, ABB_PREORDEN, ABB_POSTORDEN };

typedef struct abb abb_t;

abb_t *abb_crear(int (*comparador)(const void *, const void *));

bool abb_insertar(abb_t *abb, void *dato);

bool abb_existe(abb_t *abb, void *dato);

void *abb_buscar(abb_t *abb, void *dato);

void *abb_eliminar(abb_t *abb, void *dato);

void *abb_raiz(abb_t *abb);

size_t abb_cantidad(abb_t *abb);

bool abb_esta_vacio(abb_t *abb);

size_t abb_con_cada_elemento(abb_t *abb, enum abb_recorrido tipo_recorrido,
			     bool (*f)(void *, void *), void *extra);

size_t abb_vectorizar(abb_t *abb, enum abb_recorrido tipo_recorrido,
		      size_t cant, void **vector);

void abb_destruir(abb_t *abb);

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *));

#endif // ABB_H_
