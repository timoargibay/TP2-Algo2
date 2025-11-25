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

int juego(lista_t *pokemons_disponibles, unsigned int semilla);

#endif