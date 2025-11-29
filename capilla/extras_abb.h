#ifndef EXTRAS_ABB_H_
#define EXTRAS_ABB_H_
#include "abb.h"
#include "abb_estructura_privada.h"
#include <stdbool.h>

nodo_t *crear_nodo(void *dato);

nodo_t *nodo_insertar(nodo_t *nodo, nodo_t *nuevo_nodo,
		      int (*cmp)(const void *, const void *));

bool es_hoja(nodo_t *nodo);

void *liberar_nodo(abb_t *abb, nodo_t *nodo);

bool buscar_padre_y_predecesor(nodo_t *nodo, nodo_t **padre_predecesor,
			       nodo_t **predecesor_inmediato);

void *dato_existe_recursivamente(nodo_t *nodo, void *dato,
				 int (*cmp)(const void *, const void *),
				 bool dev_nodo, nodo_t **padre_opc);

size_t trabajar_elementos_recursivamente(nodo_t *nodo,
					 bool (*f)(void *, void *),
					 enum abb_recorrido tipo_recorrido,
					 void *extra,
					 bool *continuar_recursividad);

size_t vectorizar_recursivamente(nodo_t *nodo, size_t *posicion, size_t cant,
				 void **vector,
				 enum abb_recorrido tipo_recorrido);

void destruir_nodos_recursivamente(nodo_t *nodo, bool recursivamente);

void destruir_todo_recursivamente(nodo_t *nodo, void (*destructor)(void *));

#endif // EXTRAS_ABB_H_