#ifndef JUEGO_H_
#define JUEGO_H_

#include <time.h>
#include <ctype.h>
#include "ansi.h"
#include "ansi_extended.h"
#include "tp1.h"
#include "extras_tp1.h"
#include "lista.h"
#include "menu.h"
#define CANT_COLUMNAS 6
#define CANT_FILAS 3
#define CARTA_ANCHO 9
#define CARTA_ALTO 5
#define ESPACIO 2
#define ANCHO_IMAGINARIO 100

//Nota del editor: En casos de uso real, estimo que todas estas funciones, a excepcion de juego() serian static y no declaradas aca
//Pero para facilitar las pruebas de estabilidad y casos borde, es necesario

typedef struct carta carta_t;

carta_t *crear_carta(struct pokemon *pokemon);

bool elegir_pokemons(lista_t *pokemons, lista_t *pokemons_elegidos);

void imprimir_carta_oculta(char salida[CARTA_ALTO][CARTA_ANCHO + 1]);

void imprimir_carta_revelada(const char *texto,
			     char salida[CARTA_ALTO][CARTA_ANCHO + 1]);

void mostrar_tablero(lista_t *cartas, size_t filas, size_t columnas,
		     bool revelar);

bool leer_coordenada(const char *input, size_t *fila, size_t *columna);

carta_t *elegir_carta(lista_t *cartas, size_t filas, size_t columnas,
		      carta_t *eleccion_previa, bool *turno);

int juego(lista_t *pokemons_disponibles, unsigned int semilla,
	  unsigned short user_cant_filas, unsigned short user_cant_columnas);

#endif