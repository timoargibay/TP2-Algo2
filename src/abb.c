#include "abb.h"
#include "abb_estructura_privada.h"
#include "extras_abb.h"

abb_t *abb_crear(int (*comparador)(const void *, const void *))
{
	if (comparador == NULL)
		return NULL;

	abb_t *abb = malloc(sizeof(abb_t));
	if (abb == NULL)
		return NULL;

	abb->raiz = NULL;
	abb->cantidad = 0;
	abb->comparador = comparador;

	return abb;
}

bool abb_insertar(abb_t *abb, void *dato)
{
	if (abb == NULL)
		return false;

	nodo_t *nuevo_nodo = crear_nodo(dato);
	if (nuevo_nodo == NULL)
		return false;

	nodo_t *se_inserto = NULL;

	if (abb->raiz == NULL) {
		abb->raiz = nuevo_nodo;
		abb->cantidad++;
		return true;
	}

	se_inserto = nodo_insertar(abb->raiz, nuevo_nodo, abb->comparador);
	if (se_inserto != NULL) {
		abb->cantidad++;
		return true;
	}

	return false;
}

bool abb_existe(abb_t *abb, void *dato)
{
	if (dato == NULL)
		return true;

	if (abb == NULL)
		return false;

	return dato_existe_recursivamente(abb->raiz, dato, abb->comparador,
					  false, NULL) != NULL;
}

void *abb_buscar(abb_t *abb, void *dato)
{
	if (abb == NULL || abb->raiz == NULL || dato == NULL)
		return NULL;

	return dato_existe_recursivamente(abb->raiz, dato, abb->comparador,
					  false, NULL);
}

void *abb_eliminar(abb_t *abb, void *dato)
{
	if (abb == NULL || abb->raiz == NULL)
		return NULL;

	nodo_t *padre = NULL;
	nodo_t *nodo = dato_existe_recursivamente(
		abb->raiz, dato, abb->comparador, true, &padre);
	nodo_t *hijo = NULL;
	if (nodo == NULL)
		return NULL;

	//Nodo hoja
	if (es_hoja(nodo)) {
		if (padre == NULL)
			abb->raiz = NULL;
		else if (padre->izq == nodo)
			padre->izq = NULL;
		else
			padre->der = NULL;

		return liberar_nodo(abb, nodo);
	}

	//Tiene un solo hijo
	if (nodo->izq == NULL || nodo->der == NULL) {
		hijo = (nodo->izq != NULL) ? nodo->izq : nodo->der;

		if (padre == NULL)
			abb->raiz = hijo;
		else if (padre->izq == nodo)
			padre->izq = hijo;
		else
			padre->der = hijo;

		return liberar_nodo(abb, nodo);
	}

	//Tiene 2 hijos
	nodo_t *padre_predecesor = nodo;
	nodo_t *predecesor = NULL;
	void *dato_a_borrar = nodo->dato;
	buscar_padre_y_predecesor(nodo->izq, &padre_predecesor, &predecesor);
	nodo->dato = predecesor->dato;

	if (padre_predecesor == nodo)
		padre_predecesor->izq = predecesor->izq;
	else
		padre_predecesor->der = predecesor->izq;

	liberar_nodo(abb, predecesor);
	return dato_a_borrar;
}

void *abb_raiz(abb_t *abb)
{
	if (abb == NULL || abb->raiz == NULL)
		return NULL;

	return abb->raiz->dato;
}

size_t abb_cantidad(abb_t *abb)
{
	if (abb == NULL)
		return 0;

	return abb->cantidad;
}

bool abb_esta_vacio(abb_t *abb)
{
	if (abb == NULL || abb->cantidad == 0 || abb->raiz == NULL)
		return true;

	return false;
}

size_t abb_con_cada_elemento(abb_t *abb, enum abb_recorrido tipo_recorrido,
			     bool (*f)(void *, void *), void *extra)
{
	if (abb == NULL || f == NULL)
		return 0;

	bool recursividad = true;
	return trabajar_elementos_recursivamente(abb->raiz, f, tipo_recorrido,
						 extra, &recursividad);
}

size_t abb_vectorizar(abb_t *abb, enum abb_recorrido tipo_recorrido,
		      size_t cant, void **vector)
{
	if (abb == NULL || vector == NULL || cant == 0)
		return 0;

	size_t posicion = 0;
	return vectorizar_recursivamente(abb->raiz, &posicion, cant, vector,
					 tipo_recorrido);
}

void abb_destruir(abb_t *abb)
{
	if (abb == NULL)
		return;

	destruir_todo_recursivamente(abb->raiz, NULL);
	free(abb);
}

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
	if (abb == NULL)
		return;

	destruir_todo_recursivamente(abb->raiz, destructor);
	free(abb);
}
