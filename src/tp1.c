#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tp1.h"
#include "extras_tp1.h"

struct tp1 {
	struct pokemon *pokemons;
	size_t cantidad;
};

/**
 * Lee el archivo indicado y devuelve la estructura tp1 con los pokemones.
 * En caso de error devuelve NULL.
*/
tp1_t *tp1_leer_archivo(const char *nombre)
{
	if (nombre == NULL)
		return NULL;

	struct pokemon *tmp_ptr;
	char *buffer_linea;
	tp1_t *lista = malloc(sizeof(tp1_t)); //
	size_t capacidad_lista = 1;
	lista->cantidad = 0;
	struct pokemon tmp_pokemon;

	FILE *archivo = fopen(nombre, "r");

	if (archivo == NULL || lista == NULL) {
		free(lista);
		return NULL;
	}

	lista->pokemons = malloc(sizeof(struct pokemon));

	if (lista->pokemons == NULL) {
		free(lista);
		return NULL;
	}

	long unsigned int posicion_linea = 0;

	char *id_sin_procesar;
	char *tipo_sin_procesar;
	char *ataque_sin_procesar;
	char *defensa_sin_procesar;
	char *velocidad_sin_procesar;

	while ((buffer_linea = leer_linea_archivo(archivo)) != NULL) {
		posicion_linea = 0;

		id_sin_procesar = leer_parte(buffer_linea, &posicion_linea);
		tmp_pokemon.nombre = leer_parte(buffer_linea, &posicion_linea);
		tipo_sin_procesar = leer_parte(buffer_linea, &posicion_linea);
		ataque_sin_procesar = leer_parte(buffer_linea, &posicion_linea);
		defensa_sin_procesar =
			leer_parte(buffer_linea, &posicion_linea);
		velocidad_sin_procesar =
			leer_parte(buffer_linea, &posicion_linea);

		tmp_pokemon.id = strtnum(id_sin_procesar);
		tmp_pokemon.tipo = validar_tipo_pokemon(tipo_sin_procesar);
		tmp_pokemon.ataque = strtnum(ataque_sin_procesar);
		tmp_pokemon.defensa = strtnum(defensa_sin_procesar);
		tmp_pokemon.velocidad = strtnum(velocidad_sin_procesar);

		free(id_sin_procesar);
		free(tipo_sin_procesar);
		free(ataque_sin_procesar);
		free(defensa_sin_procesar);
		free(velocidad_sin_procesar);

		if (validar_carga_pokemon(tmp_pokemon) == true &&
		    tp1_buscar_id(lista, tmp_pokemon.id) == NULL) {
			if (lista->cantidad >= capacidad_lista) {
				capacidad_lista *= 2;
				tmp_ptr =
					realloc(lista->pokemons, //
						capacidad_lista *
							sizeof(struct pokemon));

				if (tmp_ptr == NULL) {
					free(lista);
					return NULL;
				}

				lista->pokemons = tmp_ptr;
			}

			lista->pokemons[lista->cantidad] = tmp_pokemon;
			lista->pokemons[lista->cantidad].nombre =
				malloc((strlen(tmp_pokemon.nombre) + 1) * //
				       sizeof(char));
			if (lista->pokemons[lista->cantidad].nombre != NULL)
				strcpy(lista->pokemons[lista->cantidad].nombre,
				       tmp_pokemon.nombre);
			lista->cantidad++;
		}
		free(buffer_linea);
		free(tmp_pokemon.nombre);
	}

	fclose(archivo);
	ordenar_lista(lista->pokemons, lista->cantidad);
	return lista;
}

/**
 * Devuelve la cantidad de pokemones leidos correctamente.
 * En caso de error devuelve 0.
*/
size_t tp1_cantidad(tp1_t *tp1)
{
	int i = 0;
	size_t leidos_bien = 0;

	if (tp1 == NULL)
		return 0;

	for (i = 0; i < tp1->cantidad; i++) {
		if (validar_carga_pokemon(tp1->pokemons[i])) {
			leidos_bien++;
		}
	}
	return leidos_bien;
}

/**
 * Guarda en el archivo indicado los pokemones contenidos en la estructura tp1 
 * de manera tal que tp1_leer_archivo pueda volver a leerlo correctamente.
 *
 * Devuelve NULL en caso de error o el tp1 pasado por parámetro en caso de exito.
*/
tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre)
{
	if (tp1 == NULL || nombre == NULL)
		return NULL;

	FILE *archivo = fopen(nombre, "w");
	int pos_pokemon = 0;

	if (archivo == NULL)
		return NULL;

	for (pos_pokemon = 0; pos_pokemon < tp1->cantidad; pos_pokemon++) {
		char *string_tipo = enum_a_string_tipo_pokemon(
			tp1->pokemons[pos_pokemon].tipo);

		fprintf(archivo, "%i,%s,%s,%i,%i,%i\n",
			tp1->pokemons[pos_pokemon].id,
			tp1->pokemons[pos_pokemon].nombre, string_tipo,
			tp1->pokemons[pos_pokemon].ataque,
			tp1->pokemons[pos_pokemon].defensa,
			tp1->pokemons[pos_pokemon].velocidad);
	}

	fclose(archivo);
	return tp1;
}

/**
 * Dados 2 tp1, devuelve un nuevo tp1 que es la unión de ambos.
 * No se deben incluír pokemones repetidos. En caso de repeticiones se debe
 * preferir el pokemon del primer tp1.
 *
 * En caso de error devuelve NULL.
*/
tp1_t *tp1_union(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (un_tp == NULL || otro_tp == NULL)
		return NULL;

	size_t pos_l1, pos_l2, pos_l3;
	pos_l1 = pos_l2 = pos_l3 = 0;
	tp1_t *lista_unida = malloc(sizeof(tp1_t));

	if (lista_unida == NULL)
		return NULL;

	lista_unida->cantidad = un_tp->cantidad + otro_tp->cantidad;
	lista_unida->pokemons =
		malloc(lista_unida->cantidad * sizeof(struct pokemon));

	while (pos_l1 < un_tp->cantidad && pos_l2 < otro_tp->cantidad) {
		if (un_tp->pokemons[pos_l1].id < otro_tp->pokemons[pos_l2].id) {
			copiar_pokemon(&lista_unida->pokemons[pos_l3],
				       &un_tp->pokemons[pos_l1]);
			pos_l1++;
		} else if (un_tp->pokemons[pos_l1].id >
			   otro_tp->pokemons[pos_l2].id) {
			copiar_pokemon(&lista_unida->pokemons[pos_l3],
				       &otro_tp->pokemons[pos_l2]);
			pos_l2++;
		} else {
			copiar_pokemon(&lista_unida->pokemons[pos_l3],
				       &un_tp->pokemons[pos_l1]);
			pos_l1++;
			pos_l2++;
		}
		pos_l3++;
	}

	while (pos_l1 < un_tp->cantidad) {
		copiar_pokemon(&lista_unida->pokemons[pos_l3],
			       &un_tp->pokemons[pos_l1]);
		pos_l1++;
		pos_l3++;
	}

	while (pos_l2 < otro_tp->cantidad) {
		copiar_pokemon(&lista_unida->pokemons[pos_l3],
			       &otro_tp->pokemons[pos_l2]);
		pos_l2++;
		pos_l3++;
	}

	lista_unida->cantidad = pos_l3;
	return lista_unida;
}

/**
 * Dados 2 tp1, devuelve un nuevo tp1 que es la intersección de ambos.
 * 
 * En caso de error devuelve NULL.
*/
tp1_t *tp1_interseccion(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (un_tp == NULL || otro_tp == NULL)
		return NULL;

	size_t i, j;

	tp1_t *tp_interseccion = malloc(sizeof(tp1_t));

	if (tp_interseccion == NULL)
		return NULL;

	tp_interseccion->cantidad = 0;

	tp_interseccion->pokemons = malloc(
		(un_tp->cantidad + otro_tp->cantidad) * sizeof(struct pokemon));

	if (tp_interseccion->pokemons == NULL) {
		free(tp_interseccion);
		return NULL;
	}

	for (i = 0; i < un_tp->cantidad; i++) {
		for (j = 0; j < otro_tp->cantidad; j++) {
			if (un_tp->pokemons[i].id == otro_tp->pokemons[j].id)
				copiar_pokemon(
					&tp_interseccion->pokemons
						 [tp_interseccion->cantidad++],
					&un_tp->pokemons[i]);
		}
	}

	return tp_interseccion;
}

/**
* Dados 2 tp1, devuelve un nuevo tp1 que es la diferencia de ambos.
* La diferencia se define como los pokemones que están en el primer tp1
* pero no en el segundo.
*
* En caso de error devuelve NULL.
*/
tp1_t *tp1_diferencia(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (un_tp == NULL || otro_tp == NULL || un_tp == otro_tp)
		return NULL;

	size_t i;
	tp1_t *tp_diferencia = malloc(sizeof(tp1_t));
	struct pokemon *encontrado = NULL;
	struct pokemon *copia_correcta = NULL;

	if (tp_diferencia == NULL)
		return NULL;

	tp_diferencia->cantidad = 0;
	tp_diferencia->pokemons =
		malloc((un_tp->cantidad) * sizeof(struct pokemon));

	if (tp_diferencia->pokemons == NULL)
		return NULL;

	for (i = 0; i < un_tp->cantidad; i++) {
		encontrado = tp1_buscar_id(otro_tp, un_tp->pokemons[i].id);
		if (encontrado == NULL) {
			copia_correcta = copiar_pokemon(
				&tp_diferencia
					 ->pokemons[tp_diferencia->cantidad++],
				&un_tp->pokemons[i]);
			if (copia_correcta == NULL) {
				tp1_destruir(tp_diferencia);
				return NULL;
			}
		}
	}

	return tp_diferencia;
}

/**
* Busca un pokemon por nombre.
* En caso de no encontrarlo devuelve NULL.
*/
struct pokemon *tp1_buscar_nombre(tp1_t *tp, const char *nombre)
{
	if (tp == NULL || nombre == NULL)
		return NULL;

	int i;
	for (i = 0; i < tp->cantidad; i++) {
		if (strcmp(nombre, tp->pokemons[i].nombre) == 0)
			return &tp->pokemons[i];
	}

	return NULL;
}

/**
* Busca un pokemon por id.
* En caso de no encontrarlo devuelve NULL.
*/
struct pokemon *tp1_buscar_id(tp1_t *tp, int id)
{
	if (tp == NULL)
		return NULL;

	int i;
	for (i = 0; i < tp->cantidad; i++) {
		if (tp->pokemons[i].id == id)
			return &tp->pokemons[i];
	}

	return NULL;
}

/**
 * Aplica la función f a cada pokemon por orden de id (de menor a mayor).
 * La función deja de aplicarse si f devuelve false o se terminaron los pokemones.
 * 
 * Devuelve la cantidad de pokemones sobre los cuales se aplicó la función f.
 */
size_t tp1_con_cada_pokemon(tp1_t *un_tp, bool (*f)(struct pokemon *, void *),
			    void *extra)
{
	size_t pokemones_trabajados = 0;
	struct pokemon *tmp_poke = NULL;
	if (un_tp == NULL || f == NULL || un_tp->pokemons == NULL)
		return 0;

	if (extra != NULL &&
	    strcmp((char *)extra, "ordenar_alfabeticamente") == 0)
		ordenar_alfabeticamente(un_tp->pokemons, un_tp->cantidad);

	if (un_tp->cantidad == 0)
		return 0;

	while (pokemones_trabajados < un_tp->cantidad &&
	       f((tmp_poke = &un_tp->pokemons[pokemones_trabajados++]),
		 extra) == true)
		;

	return pokemones_trabajados;
}
/**
 * Libera toda la memoria asociada al tp1
 */
void tp1_destruir(tp1_t *tp1)
{
	if (tp1 != NULL) {
		int i = 0;
		for (i = 0; i < tp1->cantidad; i++) {
			free(tp1->pokemons[i].nombre);
		}

		free(tp1->pokemons);
		free(tp1);
	}
}