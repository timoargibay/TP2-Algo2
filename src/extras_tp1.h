#ifndef EXTRAS_TP1_H_
#define EXTRAS_TP1_H_
#include "tp1.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Dado un puntero a archivo, leé una linea y devuelve un puntero a la misma
 */
char *leer_linea_archivo(FILE *archivo);

/**
 *Lee la linea por columna
*/
char *leer_parte(char *linea, long unsigned int *inicio);

/**
 *Dada un tipo de pokemon, verifica que este sea valido
 */
enum tipo_pokemon validar_tipo_pokemon(char *tipo);

//Valida un pokemon y que este esté bien//
bool validar_carga_pokemon(struct pokemon un_pokemon);

/*
 *Dado un string con un numero, lo convierte a int, si falla, devuelve -1
*/
int strtnum(char *string);

/*
 * Agarra un enum y lo convierte a su string correspondiente 
*/
char *enum_a_string_tipo_pokemon(enum tipo_pokemon tipo);

/*
 *Dado un pokemon y un struct de destino, hace una copia completa a destini
 */
struct pokemon *copiar_pokemon(struct pokemon *destino, struct pokemon *fuente);

/*
 * Dada una lista desordenada la ordena por ID usando bubble sort
*/
void ordenar_lista(struct pokemon *lista_pokemon, size_t cantidad_elementos);

/*
 * Dado un puntero a lista de pokemons, busca el de menor id y lo retorna
*/
size_t buscar_por_menor_id(struct pokemon *lista, size_t cantidad,
			   size_t arranca_desde_aca);
/*
 * Revisa una lista y determina si esta ordenada
 */
bool lista_ordenada(struct pokemon *lista, size_t tamaño);

/*
 * Recibe una lista y la ordena alfabeticamente usando strcmp
*/
int ordenar_alfabeticamente(struct pokemon *lista, size_t cantidad);
#endif // EXTRAS_TP1_H_