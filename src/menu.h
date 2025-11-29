#ifndef MENU_H_
#define MENU_H_
#include "lista.h"
#include <stdlib.h>
#include <string.h>
#include "ansi.h"
#include "ansi_extended.h"

typedef struct menu menu_t;

//Crea el TDA menu
menu_t *menu_crear(char *titulo);

//Devuelve el string del titulo, con sus estilos si los hubiera
char *menu_mostrar_titulo(menu_t *menu);

//Modifica el titulo del menu por el que ingreses
bool menu_modificar_titulo(char *nuevo_titulo_y_estilos); ////////////

//Agrega una opcion al menu
bool menu_agregar_opcion(menu_t *menu, const char *texto);

//Devuelve la opcion elegida (la posicion va al mismo orden en q se agregaron las opciones)
char *menu_get_opcion(const menu_t *menu, size_t pos);

//Devuelve en O(1) la cantidad de opciones q tiene el menu
size_t menu_cantidad(const menu_t *menu);

//Libera toda la memoria asociada al menu
void menu_destruir(menu_t *menu);

#endif