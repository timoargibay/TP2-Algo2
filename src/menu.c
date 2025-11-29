#include "menu.h"
#include "lista.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct menu {
	char *titulo;
	lista_t *opciones;
};

static char *copiar_string(const char *string)
{
	if (string == NULL)
		return NULL;

	char *copia = malloc((strlen(string) + 1) * sizeof(char));
	if (copia == NULL)
		return NULL;

	strcpy(copia, string);
	return copia;
}

menu_t *menu_crear(char *titulo)
{
	menu_t *menu = malloc(sizeof(menu_t));
	if (menu == NULL)
		return NULL;

	menu->titulo = copiar_string(titulo);

	menu->opciones = lista_crear();
	if (menu->opciones == NULL) {
		free(menu);
		return NULL;
	}

	return menu;
}

char *menu_mostrar_titulo(menu_t *menu)
{
	if (menu == NULL)
		return NULL;

	return menu->titulo;
}

bool menu_agregar_opcion(menu_t *menu, const char *texto)
{
	if (menu == NULL || texto == NULL)
		return false;

	char *texto_agregar = copiar_string(texto);
	if (texto_agregar == NULL)
		return false;

	if (lista_agregar(menu->opciones, texto_agregar) == false)
		return false;

	return true;
}

char *menu_get_opcion(const menu_t *menu, size_t pos)
{
	if (menu == NULL || pos >= lista_cantidad(menu->opciones))
		return NULL;

	return lista_buscar_elemento(menu->opciones, pos);
}

size_t menu_cantidad(const menu_t *menu)
{
	return lista_cantidad(menu->opciones);
}

void menu_destruir(menu_t *menu)
{
	if (menu == NULL)
		return;

	lista_destruir_todo(menu->opciones, free);
	free(menu->titulo);
	free(menu);
}