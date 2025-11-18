#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"

struct nodo {
	void *dato;
	struct nodo *siguiente;
};

typedef struct nodo nodo_t;

struct lista {
	nodo_t *primero;
	nodo_t *ultimo;
	size_t cantidad;
};

struct lista_iterador {
	size_t posicion;
	nodo_t *nodo;
	lista_t *lista;
};

/**
 * Crea un lista vacía.
* Devuelve NULL en caso de error.
 */
lista_t *lista_crear()
{
	lista_t *nueva_lista = malloc(sizeof(lista_t));

	if (nueva_lista == NULL)
		return NULL;

	nueva_lista->cantidad = 0;
	nueva_lista->primero = NULL;
	nueva_lista->ultimo = NULL;

	return nueva_lista;
}

/**
 * Devuelve true si la lista está vacía. Una lista nula está vacía siempre.
*/
bool lista_vacia(lista_t *lista)
{
	return lista == NULL ||
	       (lista->cantidad == 0 && lista->primero == NULL);
}

/**
 * Devuelve la cantidad de elementos en la lista o 0 en caso de error.
 */
size_t lista_cantidad(lista_t *lista)
{
	if (lista == NULL)
		return 0;

	return lista->cantidad;
}

/**
 * Agrega un elemento al final de la lista.
 *
 * Devuelve true si pudo agregar el elemento.
 */
bool lista_agregar(lista_t *lista, void *dato)
{
	if (lista == NULL)
		return false;

	nodo_t *nodo_nuevo = malloc(sizeof(nodo_t));

	if (nodo_nuevo == NULL)
		return false;

	nodo_nuevo->dato = dato;
	nodo_nuevo->siguiente = NULL;

	if (lista->primero == NULL) {
		lista->primero = nodo_nuevo;
		lista->ultimo = nodo_nuevo;
	} else {
		lista->ultimo->siguiente = nodo_nuevo;
		lista->ultimo = nodo_nuevo;
	}

	lista->cantidad++;

	return true;
}

/**
 * Agrega un elemento en la posición dada de la lista.
 *
 * Si se intenta insertar en una posición que no existe no lo inserta y
 * devuelve false.
 */
bool lista_insertar(lista_t *lista, void *elemento, size_t posicion)
{
	if (lista == NULL || lista->cantidad <= posicion)
		return false;

	size_t i = 0;
	nodo_t *actual = NULL;
	nodo_t *nodo_nuevo = malloc(sizeof(nodo_t));

	if (lista->primero != NULL)
		actual = lista->primero;

	if (nodo_nuevo == NULL)
		return false;

	nodo_nuevo->dato = elemento;
	nodo_nuevo->siguiente = NULL;

	if (posicion == 0) {
		nodo_nuevo->siguiente = lista->primero;
		lista->primero = nodo_nuevo;

		if (lista->cantidad == 0)
			lista->ultimo = nodo_nuevo;
	} else if (lista->cantidad == posicion) {
		lista->ultimo->siguiente = nodo_nuevo;
		lista->ultimo = nodo_nuevo;
	} else {
		for (i = 0; i < posicion - 1; i++) {
			actual = actual->siguiente;
		}
		nodo_nuevo->siguiente = actual->siguiente;
		actual->siguiente = nodo_nuevo;
	}

	lista->cantidad++;
	return true;
}

/**
 * Elimina un elemento de la lista. Devuelve el elemento.
 *
 * En caso de error devuelve NULL
 */
void *lista_eliminar_elemento(lista_t *lista, size_t posicion)
{
	if (lista == NULL || lista->primero == NULL ||
	    lista->cantidad <= posicion)
		return NULL;

	size_t i;
	nodo_t *actual = lista->primero;
	nodo_t *anterior = NULL;
	void *dato = NULL;

	if (posicion == 0) {
		if (lista->cantidad == 1) {
			lista->primero = NULL;
			lista->ultimo = NULL;
		} else {
			lista->primero = actual->siguiente;
		}
	}

	else {
		for (i = 0; i < posicion; i++) {
			anterior = actual;
			actual = actual->siguiente;
		}
		if (actual == lista->ultimo)
			lista->ultimo = anterior;
	}

	dato = actual->dato;
	free(actual);

	lista->cantidad--;
	return dato;
}

/**
 * Busca un elemento en la lista y devuelve la posición del mismo.
 *
 * Si no existe el elemento devuelve -1
 */
int lista_buscar_posicion(lista_t *lista, void *elemento,
			  int (*comparador)(const void *, const void *))
{
	if (lista == NULL || comparador == NULL)
		return -1;

	size_t i;
	nodo_t *nodo_actual = lista->primero;

	for (i = 0; i < lista->cantidad; i++) {
		if (comparador(elemento, nodo_actual->dato) == 0)
			return (int)i;

		nodo_actual = nodo_actual->siguiente;
	}

	return -1;
}

/**
 * Dada una posición de la lista, devuelve el elemento en esa posicion. En caso de error devuelve NULL.
 */
void *lista_buscar_elemento(lista_t *lista, size_t posicion)
{
	if (lista == NULL || lista->primero == NULL ||
	    lista->cantidad < posicion)
		return NULL;

	size_t i = 0;
	nodo_t *nodo_actual = lista->primero;
	void *dato = NULL;

	while (nodo_actual != NULL && i < posicion) {
		i++;
		nodo_actual = nodo_actual->siguiente;
	}

	if (i == posicion && nodo_actual != NULL)
		dato = nodo_actual->dato;

	return dato;
}

/**
 * Recorre los elementos de la lista y aplica a cada uno la función f.
 *
 * Cuando la función f devuelve false se deja de recorrer la lista.
 *
 * La función retorna la cantidad de elementos a los cuales se le aplicó f
 */
size_t lista_con_cada_elemento(lista_t *lista, bool (*f)(void *, void *),
			       void *extra)
{
	if (lista == NULL || f == NULL) {
		return 0;
	}

	size_t i = 0;
	nodo_t *nodo_actual = lista->primero;
	bool estado_funcion = true;
	while (i < lista->cantidad && estado_funcion == true) {
		estado_funcion = f(nodo_actual->dato, extra);
		nodo_actual = nodo_actual->siguiente;
		i++;
	}

	return i;
}

/**
 * Destruye la lista aplicando la función destructora a cada elemento.
 */
void lista_destruir_todo(lista_t *lista, void (*destructor)(void *))
{
	if (lista == NULL || lista->cantidad == 0 || lista->primero == NULL ||
	    destructor == NULL)
		return;

	size_t i;
	nodo_t *nodo_actual = lista->primero;
	for (i = 0; i < lista->cantidad; i++) {
		destructor(nodo_actual->dato);
		nodo_actual = nodo_actual->siguiente;
	}

	lista_destruir(lista);
}

/**
* Destruye la lista.
*/
void lista_destruir(lista_t *lista)
{
	if (lista == NULL)
		return;

	nodo_t *proximo_nodo = NULL;
	nodo_t *nodo_actual = NULL;
	if (lista->primero != NULL)
		nodo_actual = lista->primero;

	while (nodo_actual != NULL) {
		proximo_nodo = nodo_actual->siguiente;
		free(nodo_actual);
		nodo_actual = proximo_nodo;
	}

	free(lista);
}

/**
* Crea un iterador externo para la lista.
 */
lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (lista == NULL || lista->cantidad == 0 || lista->primero == NULL)
		return NULL;

	lista_iterador_t *ptr = malloc(sizeof(lista_iterador_t));

	if (ptr == NULL)
		return NULL;

	ptr->lista = lista;
	ptr->posicion = 0;
	ptr->nodo = lista->primero;

	return ptr;
}

/**
 * Devuelve true si aún quedan elementos por iterar.
 */
bool lista_iterador_hay_mas_elementos(lista_iterador_t *it)
{
	if (it == NULL || it->lista == NULL)
		return false;

	if (it->lista->cantidad > it->posicion)
		return true;
	else
		return false;
}

/**
* Avanza el iterador a la siguiente iteración.
 */
void lista_iterador_siguiente(lista_iterador_t *it)
{
	if (it == NULL || it->lista == NULL)
		return;

	if (it->posicion < it->lista->cantidad) {
		it->nodo = it->nodo->siguiente;
		it->posicion++;
	}
}

/**
* Devuelve el elemento de la iteración actual.
 */
void *lista_iterador_obtener_actual(lista_iterador_t *it)
{
	if (it == NULL || it->nodo == NULL)
		return NULL;
	else
		return it->nodo->dato;
}

/**
* Destruye el iterador.
*/
void lista_iterador_destruir(lista_iterador_t *it)
{
	if (it == NULL)
		return;
	else
		free(it);
}
