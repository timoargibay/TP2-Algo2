#ifndef MENU_H
#define MENU_H
#include "ansi.h"
#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct menu menu_t;

//Crea menu con un titulo
menu_t *crear_menu(char *titulo);

//Agrega las opciones al menu
bool menu_agregar_opcion(menu_t *menu, char *texto, char tecla);

//Muestra el menu y devuelve la tecla que el usuario oprima
char mostrar_menu(const menu_t *menu);

//Libera toda la memoria asociada al menu
void destruir_menu(menu_t *menu);

#endif // MENU_H