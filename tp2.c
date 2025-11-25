#include "src/juego.h"

bool mostrar_pokemon(struct pokemon *pokemon_seleccionado)
{
	char *tipo_pokemon = malloc(16 * sizeof(char));
	if (tipo_pokemon == NULL)
		return false;

	switch (pokemon_seleccionado->tipo) {
	case TIPO_AGUA:
		strcpy(tipo_pokemon, "Agua");
		break;
	case TIPO_ELEC:
		strcpy(tipo_pokemon, "Electrico");
		break;
	case TIPO_FANT:
		strcpy(tipo_pokemon, "Fantasma");
		break;
	case TIPO_FUEG:
		strcpy(tipo_pokemon, "Fuego");
		break;
	case TIPO_LUCH:
		strcpy(tipo_pokemon, "Lucha");
		break;
	case TIPO_NORM:
		strcpy(tipo_pokemon, "Normal");
		break;
	case TIPO_PLAN:
		strcpy(tipo_pokemon, "Planta");
		break;
	case TIPO_PSI:
		strcpy(tipo_pokemon, "Psiquico");
		break;

	default:
		strcpy(tipo_pokemon, "INVALIDO");
		break;
	}

	printf("------------------------------\n");
	printf("ID: %i\nNombre: %s\nTipo: %s\nAtaque: %i\nDefensa: %i\nVelocidad: %i",
	       pokemon_seleccionado->id, pokemon_seleccionado->nombre,
	       tipo_pokemon, pokemon_seleccionado->ataque,
	       pokemon_seleccionado->defensa, pokemon_seleccionado->velocidad);
	printf("\n------------------------------\n");

	free(tipo_pokemon);
	return true;
}

char *copiar_string(char *string)
{
	if (string == NULL)
		return NULL;

	char *copia = malloc((strlen(string) + 1) * sizeof(char));
	if (copia == NULL)
		return NULL;

	strcpy(copia, string);
	return copia;
}

void destruir_pokemon_lista(void *pokemon)
{
	if (pokemon == NULL)
		return;

	struct pokemon *pokemon_a_liberar = pokemon;
	free(pokemon_a_liberar->nombre);
	free(pokemon_a_liberar);
}

char fsubmenu_mostrar()
{
	menu_t *submenu_mostrar = menu_crear("Mostrar...", NULL);
	if (submenu_mostrar == NULL)
		return 0;

	menu_agregar_opcion(submenu_mostrar, "Mostrar por nombre", 'N', NULL);
	menu_agregar_opcion(submenu_mostrar, "Mostrar por ID", 'I', NULL);
	menu_agregar_opcion(submenu_mostrar, "Volver al menu anterior", 'A',
			    NULL);

	char input = menu_mostrar(submenu_mostrar, true);
	menu_destruir(submenu_mostrar);
	return input;
}

char fsubmenu_buscar()
{
	menu_t *submenu_buscar = menu_crear("Buscar...", NULL);
	if (submenu_buscar == NULL)
		return 0;

	menu_agregar_opcion(submenu_buscar, "Buscar por nombre", 'N', NULL);
	menu_agregar_opcion(submenu_buscar, "Buscar por ID", 'I', NULL);
	menu_agregar_opcion(submenu_buscar, "Volver al menu anterior", 'A',
			    NULL);
	char input = menu_mostrar(submenu_buscar, true);
	menu_destruir(submenu_buscar);
	return input;
}

char fmenu_principal()
{
	menu_t *menu_principal =
		menu_crear("Pokedex trivia (TP2)", ANSI_COLOR_RED);
	menu_agregar_opcion(menu_principal, "Jugar", 'J', NULL);
	menu_agregar_opcion(menu_principal, "Jugar con semilla", 'S', NULL);
	menu_agregar_opcion(menu_principal, "Cargar Archivo", 'C', NULL);
	menu_agregar_opcion(menu_principal, "Buscar Pokemon...", 'B', NULL);
	menu_agregar_opcion(menu_principal, "Mostrar Pokemon...", 'M', NULL);
	menu_agregar_opcion(menu_principal, "Salir", 'Q', NULL);
	char input = menu_mostrar(menu_principal, true);
	menu_destruir(menu_principal);
	return input;
}

/* Originalmente esta funcion solo era una linea (su return) pero por errores con valgrind
	*decidi hacer una copia real de cada pokemon en vez de solamente poner un puntero al pokemon original
	*en los nodos, asi tp1_destruir y los destructores de tp_lista hacen su trabajo por separado bien.
	*Tambien es la razon por la que incorporé una funcion que copie un string
*/
bool copia_agrega_pokemon(struct pokemon *el_pokemon, void *una_lista)
{
	struct pokemon *pokemon_lista = malloc(sizeof(struct pokemon));
	if (pokemon_lista == NULL)
		return false;

	*pokemon_lista = *el_pokemon;
	pokemon_lista->nombre = copiar_string(el_pokemon->nombre);

	if (pokemon_lista->nombre == NULL) {
		free(pokemon_lista);
		return false;
	}

	return lista_agregar(una_lista, pokemon_lista);
}

lista_t *array_a_lista(tp1_t *array_pokemons)
{
	if (array_pokemons == NULL)
		return NULL;
	lista_t *nueva_lista = lista_crear();
	if (nueva_lista == NULL)
		return NULL;

	size_t pokemons_agregados = tp1_con_cada_pokemon(
		array_pokemons, copia_agrega_pokemon, nueva_lista);

	if (pokemons_agregados != tp1_cantidad(array_pokemons)) {
		lista_destruir(nueva_lista);
		nueva_lista = NULL;
	}

	return nueva_lista;
}

int main(int argc, char *argumento[])
{
	tp1_t *pokemons_1 = NULL;
	if (argc >= 2)
		pokemons_1 = tp1_leer_archivo(argumento[1]);
	else
		pokemons_1 = tp1_leer_archivo("pokedex.csv");

		
	if (pokemons_1 == NULL)
		return 1;
		
	lista_t *pokemons_lista = array_a_lista(pokemons_1);
	if(pokemons_lista == NULL) {
		tp1_destruir(pokemons_1);
		return 1;
	}
	
	char input = fmenu_principal();
	char *inputs_varios = NULL;
	tp1_t *carga_manual = NULL;

	bool terminar = false;


	while(terminar == false) {
		switch (input) {
		case 'j':
			juego(pokemons_lista, 0);
	
			break;
		case 's':
			inputs_varios = leer_linea_archivo(stdin);
			juego(pokemons_lista, inputs_varios);
			break;
		case 'c':
			do {
				printf(ANSI_RESET_SCREEN
				       "Ingrese la ruta del archivo: ");
				inputs_varios = leer_linea_archivo(stdin);
				printf("\n");
				carga_manual = tp1_leer_archivo(inputs_varios);
				free(inputs_varios);
				if (carga_manual == NULL) {
					printf("No se pudo leer el archivo. Queres intentar de vuelta? S/N");
					input = leer_un_char(stdin);
				}
			} while (carga_manual == NULL && input != 's');
			break;
		case 'b':
			input = fsubmenu_buscar();
			switch (input) {
			case 'n':
				printf("Ingrese el nombre: ");
				inputs_varios = leer_linea_archivo(stdin);
				printf("\n");
				bool se_encontro = false;
				for(size_t i = 0; i < lista_cantidad(pokemons_lista); i++) {
					struct pokemon *poke_actual = lista_buscar_elemento(pokemons_lista, i);
					if(poke_actual != NULL && strcmp(poke_actual->nombre, inputs_varios) == 0) {
						mostrar_pokemon(poke_actual);
						se_encontro = true;
					}
				}
				if(se_encontro == false)
					printf("\nNo se encontro ese pokemon en la pokedex cargada :(\n");
				
				free(inputs_varios);
				printf("Presiona enter para continuar....");
				leer_linea_archivo(stdin);
				printf("\n");
				break;
			case 'i':
				printf("Ingrese el ID: ");
				inputs_varios = leer_linea_archivo(stdin);
				printf("\n");
				int id = strtnum(inputs_varios);
				bool se_encontro = false;
				for(size_t i = 0; i < lista_cantidad(pokemons_lista); i++) {
					struct pokemon *poke_actual = lista_buscar_elemento(pokemons_lista, i);
					if(poke_actual != NULL && poke_actual->id == id) {
						mostrar_pokemon(poke_actual);
						se_encontro = true;
					}
				}
				if(se_encontro == false)
					printf("\nNo se encontro ese pokemon en la pokedex cargada :(\n");
				
				free(inputs_varios);
				printf("Presiona enter para continuar....");
				leer_linea_archivo(stdin);
				printf("\n");
				break;
			case 'a':
				continue;
			}
			break;
		case 'm':
			input = fsubmenu_mostrar();
			switch (input) {
			case 'n':
				tp1_con_cada_pokemon(pokemons_1, NULL, "ordenar_alfabeticamente");
				lista_t *pokemons_por_nombre = array_a_lista(pokemons_1);
				if(pokemons_por_nombre == NULL || lista_cantidad(pokemons_por_nombre) != tp1_cantidad(pokemons_1)) {
					printf("\nHubo un problema!\n");
					tp1_destruir(pokemons_1);
					break;
				} else {
					for(size_t i = 0; i < lista_cantidad(pokemons_por_nombre); i++) {
						struct pokemon *poke_actual = lista_buscar_elemento(pokemons_por_nombre, i);
						if(poke_actual != NULL)
							mostrar_pokemon(poke_actual);
					}
				}
				printf("Presiona enter para continuar....");
				leer_linea_archivo(stdin);
				printf("\n");
				break;
			case 'i':
				for(size_t i = 0; i < lista_cantidad(pokemons_lista); i++) {
					struct pokemon *poke_actual = lista_buscar_elemento(pokemons_lista, i);
					mostrar_pokemon(poke_actual);
				}
				printf("Presiona enter para continuar....");
				leer_linea_archivo(stdin);
				printf("\n");
				
				break;
			case 'a':
				continue;
			}
			break;
		case 'q':
			terminar = true;
			break;	
		}
	}

	lista_destruir_todo(pokemons_lista, destruir_pokemon_lista);
	return 0;
}