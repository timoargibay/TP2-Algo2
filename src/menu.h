#ifndef MENU_H
#define MENU_H
#include "ansi.h"
#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct menu menu_t;

//Crea menu con un titulo, titulo puede ser NULL
menu_t *crear_menu(const char *titulo, const char *estilo_titulo);

//Agrega las opciones al menu
bool menu_agregar_opcion(menu_t *menu, const char *texto, char tecla, char *estilo_opcion);

//Agrega titulo si no habia, si habia lo cambia, y si titulo es NULL se lo borra
bool menu_modificar_titulo(menu_t *menu, const char *titulo, const char *estilo_titulo);

//Muestra el menu y devuelve la tecla que el usuario oprima
char mostrar_menu(menu_t *menu);

//Borra una opcion del menu 
bool menu_borrar_opcion(menu_t *menu, char tecla_opcion_a_borrar);

//Modifica el texto de una opcion si la hay
bool menu_modificar_texto_opcion(menu_t *menu, char tecla_opcion, const char *nuevo_texto);

//Devuelve la cantidad de opciones que hay en el menu
size_t menu_cantidad_opciones(menu_t *menu);

//Libera toda la memoria asociada al menu
void destruir_menu(menu_t *menu);

#endif // MENU_H