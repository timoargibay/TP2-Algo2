#include <stdlib.h>
#include "extras_abb.h"

nodo_t *crear_nodo(void *dato)
{
	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (nuevo_nodo == NULL)
		return NULL;

	nuevo_nodo->dato = dato;
	nuevo_nodo->izq = nuevo_nodo->der = NULL;

	return nuevo_nodo;
}

nodo_t *nodo_insertar(nodo_t *nodo, nodo_t *nuevo_nodo,
		      int (*cmp)(const void *, const void *))
{
	if (nodo == NULL)
		return nuevo_nodo;

	int comparacion = cmp(nuevo_nodo->dato, nodo->dato);

	if (comparacion < 0) {
		nodo->izq = nodo_insertar(nodo->izq, nuevo_nodo, cmp);
	} else {
		nodo->der = nodo_insertar(nodo->der, nuevo_nodo, cmp);
	}

	return nodo;
}

bool es_hoja(nodo_t *nodo)
{
	if (nodo == NULL)
		return true;

	if (nodo->izq == NULL && nodo->der == NULL)
		return true;

	return false;
}

void *liberar_nodo(abb_t *abb, nodo_t *nodo)
{
	if (abb == NULL || nodo == NULL || abb->cantidad == 0)
		return NULL;

	void *dato = nodo->dato;
	free(nodo);
	abb->cantidad--;
	return dato;
}

bool buscar_padre_y_predecesor(nodo_t *nodo, nodo_t **padre_predecesor,
			       nodo_t **predecesor_inmediato)
{
	if (nodo == NULL)
		return false;

	*predecesor_inmediato = nodo;

	while ((*predecesor_inmediato)->der != NULL) {
		*padre_predecesor = *predecesor_inmediato;
		*predecesor_inmediato = (*predecesor_inmediato)->der;
	}

	return true;
}

void *dato_existe_recursivamente(nodo_t *nodo, void *dato,
				 int (*cmp)(const void *, const void *),
				 bool dev_nodo, nodo_t **padre_opc)
{
	if (nodo == NULL)
		return NULL;

	int comparacion = cmp(dato, nodo->dato);

	if (comparacion == 0) {
		if (dev_nodo == true)
			return nodo;
		else
			return nodo->dato;
	}

	if (padre_opc != NULL)
		*padre_opc = nodo;

	if (comparacion < 0)
		return dato_existe_recursivamente(nodo->izq, dato, cmp,
						  dev_nodo, padre_opc);
	else
		return dato_existe_recursivamente(nodo->der, dato, cmp,
						  dev_nodo, padre_opc);
}

size_t trabajar_elementos_recursivamente(nodo_t *nodo,
					 bool (*f)(void *, void *),
					 enum abb_recorrido tipo_recorrido,
					 void *extra,
					 bool *continuar_recursividad)
{
	if (nodo == NULL || f == NULL)
		return 0;

	size_t elementos_afectados = 0;
	switch (tipo_recorrido) {
	case ABB_INORDEN:
		elementos_afectados += trabajar_elementos_recursivamente(
			nodo->izq, f, tipo_recorrido, extra,
			continuar_recursividad);

		if (*continuar_recursividad == false)
			return elementos_afectados;

		elementos_afectados++;
		if (f(nodo->dato, extra) == false) {
			*continuar_recursividad = false;
			return elementos_afectados;
		}

		elementos_afectados += trabajar_elementos_recursivamente(
			nodo->der, f, tipo_recorrido, extra,
			continuar_recursividad);
		break;

	case ABB_PREORDEN:
		if (*continuar_recursividad == false)
			return elementos_afectados;

		elementos_afectados++;
		if (f(nodo->dato, extra) == false) {
			*continuar_recursividad = false;
			return elementos_afectados;
		}
		elementos_afectados += trabajar_elementos_recursivamente(
			nodo->izq, f, tipo_recorrido, extra,
			continuar_recursividad);
		elementos_afectados += trabajar_elementos_recursivamente(
			nodo->der, f, tipo_recorrido, extra,
			continuar_recursividad);
		break;

	case ABB_POSTORDEN:
		elementos_afectados += trabajar_elementos_recursivamente(
			nodo->izq, f, tipo_recorrido, extra,
			continuar_recursividad);
		elementos_afectados += trabajar_elementos_recursivamente(
			nodo->der, f, tipo_recorrido, extra,
			continuar_recursividad);

		if (*continuar_recursividad == false)
			return elementos_afectados;

		elementos_afectados++;
		if (f(nodo->dato, extra) == false) {
			*continuar_recursividad = false;
			return elementos_afectados;
		}
		break;
	}

	return elementos_afectados;
}

size_t vectorizar_recursivamente(nodo_t *nodo, size_t *posicion, size_t cant,
				 void **vector,
				 enum abb_recorrido tipo_recorrido)
{
	size_t elementos_afectados = 0;
	if (nodo == NULL || *posicion >= cant || vector == NULL)
		return 0;

	switch (tipo_recorrido) {
	case ABB_PREORDEN:
		if (*posicion < cant) {
			vector[(*posicion)++] = nodo->dato;
			elementos_afectados++;
		}
		elementos_afectados += vectorizar_recursivamente(
			nodo->izq, posicion, cant, vector, tipo_recorrido);
		elementos_afectados += vectorizar_recursivamente(
			nodo->der, posicion, cant, vector, tipo_recorrido);
		break;

	case ABB_INORDEN:
		elementos_afectados += vectorizar_recursivamente(
			nodo->izq, posicion, cant, vector, tipo_recorrido);
		if (*posicion < cant) {
			vector[(*posicion)++] = nodo->dato;
			elementos_afectados++;
		}
		elementos_afectados += vectorizar_recursivamente(
			nodo->der, posicion, cant, vector, tipo_recorrido);
		break;

	case ABB_POSTORDEN:
		elementos_afectados += vectorizar_recursivamente(
			nodo->izq, posicion, cant, vector, tipo_recorrido);
		elementos_afectados += vectorizar_recursivamente(
			nodo->der, posicion, cant, vector, tipo_recorrido);

		if (*posicion < cant) {
			vector[(*posicion)++] = nodo->dato;
			elementos_afectados++;
		}
		break;
	}

	return elementos_afectados;
}

void destruir_todo_recursivamente(nodo_t *nodo, void (*destructor)(void *))
{
	if (nodo == NULL)
		return;

	destruir_todo_recursivamente(nodo->izq, destructor);
	destruir_todo_recursivamente(nodo->der, destructor);

	if (destructor != NULL)
		destructor(nodo->dato);

	free(nodo);
}