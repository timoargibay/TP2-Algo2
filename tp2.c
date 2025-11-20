#include "src/menu.h"
#include "src/tp1.h"
#include <stdio.h>
#include <ctype.h>
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

        //Armo menu principal
        menu_t *menu_principal = crear_menu("Pokedex trivia (TP2)", ANSI_COLOR_RED);
        if(menu_principal == NULL) {
                tp1_destruir(pokemons_1);
                return 1;
        }

        menu_agregar_opcion(menu_principal, "Jugar", 'J', NULL);
        menu_agregar_opcion(menu_principal, "Jugar con semilla", 'S', NULL);
        menu_agregar_opcion(menu_principal, "Cargar Archivo", 'C', NULL);
        menu_agregar_opcion(menu_principal, "Buscar Pokemon...", 'B', NULL);
        menu_agregar_opcion(menu_principal, "Mostrar Pokemon...", 'M', NULL);
        menu_agregar_opcion(menu_principal, "Salir", 'Q', NULL);  
        
        menu_t *menu_buscar = crear_menu("Buscar...", NULL);
        if(menu_buscar == NULL) {
                tp1_destruir(pokemons_1);
                return 1;
        }
        menu_agregar_opcion(menu_buscar, "Buscar por nombre", 'N', NULL);
        menu_agregar_opcion(menu_buscar, "Buscar por ID", 'I', NULL);
        menu_agregar_opcion(menu_buscar, "Volver al menu anterior", 'A', NULL);

        menu_t *menu_mostrar = crear_menu("Mostrar...", NULL);
        if(menu_mostrar == NULL) {
                tp1_destruir(pokemons_1);
                return 1;
        }
        menu_agregar_opcion(menu_mostrar, "Mostrar por nombre", 'N', NULL);
        menu_agregar_opcion(menu_mostrar, "Mostrar por ID", 'I', NULL);
        menu_agregar_opcion(menu_mostrar, "Volver al menu anterior", 'A', NULL);



        char input = mostrar_menu(menu_principal);
}
