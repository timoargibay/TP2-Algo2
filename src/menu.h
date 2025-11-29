#ifndef MENU_H_
#define MENU_H_
#include "lista.h"
#include <stdlib.h>
#include <string.h>

typedef struct menu menu_t;

menu_t *menu_crear();

char *menu_mostrar_titulo(menu_t *menu);

bool menu_agregar_opcion(menu_t *menu, const char *texto);

char *menu_get_opcion(const menu_t *menu, size_t pos);

size_t menu_cantidad(const menu_t *menu);

void menu_destruir(menu_t *menu);

#endif