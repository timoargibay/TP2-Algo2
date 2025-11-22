#include "src/menu.h"
#include "src/tp1.h"
#include "src/extras_tp1.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argumento[])
{
	tp1_t *pokemons_1 = NULL;
	if (argc >= 1)
		pokemons_1 = tp1_leer_archivo(argumento[1]);
	else
		pokemons_1 = tp1_leer_archivo("pokedex.csv");

	if (pokemons_1 == NULL)
		return 1;

	menu_t *menu_principal =
		crear_menu("Pokedex trivia (TP2)", ANSI_COLOR_RED);
	menu_t *submenu_buscar = crear_menu("Buscar...", NULL);
	menu_t *submenu_mostrar = crear_menu("Mostrar...", NULL);
	if (!menu_principal || !submenu_buscar || !submenu_mostrar) {
		tp1_destruir(pokemons_1);
		free(menu_principal);
		free(submenu_buscar);
		free(submenu_mostrar);
		return 1;
	}

	//Preparo menu principal
	menu_agregar_opcion(menu_principal, "Jugar", 'J', NULL);
	menu_agregar_opcion(menu_principal, "Jugar con semilla", 'S', NULL);
	menu_agregar_opcion(menu_principal, "Cargar Archivo", 'C', NULL);
	menu_agregar_opcion(menu_principal, "Buscar Pokemon...", 'B', NULL);
	menu_agregar_opcion(menu_principal, "Mostrar Pokemon...", 'M', NULL);
	menu_agregar_opcion(menu_principal, "Salir", 'Q', NULL);

	//Preparo submenu de buscar
	menu_agregar_opcion(submenu_buscar, "Buscar por nombre", 'N', NULL);
	menu_agregar_opcion(submenu_buscar, "Buscar por ID", 'I', NULL);
	menu_agregar_opcion(submenu_buscar, "Volver al menu anterior", 'A',
			    NULL);

	//Preparo submenu de mostrar
	menu_agregar_opcion(submenu_mostrar, "Mostrar por nombre", 'N', NULL);
	menu_agregar_opcion(submenu_mostrar, "Mostrar por ID", 'I', NULL);
	menu_agregar_opcion(submenu_mostrar, "Volver al menu anterior", 'A',
			    NULL);

	char input = mostrar_menu(menu_principal);
	char *inputs_varios = NULL;

	switch (input) {
	case 'j':
		/* code */
		break;
	case 's':
		inputs_varios = leer_linea_archivo(stdin);
		//aca falta
	case 'c':
		printf("Ingrese la ruta del archivo: ");
		inputs_varios = leer_linea_archivo(stdin);
		printf("\n");
		tp1_t *carga_manual = tp1_leer_archivo(inputs_varios);
		if (carga_manual == NULL)
			printf("No se pudo leer el archivo. Queres intentar de vuelta?");

		break;
	case 'b':
		input = mostrar_menu(submenu_buscar);
		break;
	case 'm':
		input = mostrar_menu(submenu_mostrar);
		break;
	case 'q':
		tp1_destruir(pokemons_1);
		free(menu_principal);
		free(submenu_buscar);
		free(submenu_mostrar);
		return 0;
	default:
		input = mostrar_menu(menu_principal);
		break;
	}
}