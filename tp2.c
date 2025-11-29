#include "src/juego.h"

char tolower_propio(char l)
{
	if (l >= 'A' && l <= 'Z')
		return l + 32;

	return l;
}

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
	menu_t *submenu_mostrar = menu_crear("Mostrar...");
	if (submenu_mostrar == NULL)
		return 0;

	menu_agregar_opcion(submenu_mostrar, "N) Mostrar por nombre");
	menu_agregar_opcion(submenu_mostrar, "I) Mostrar por ID");
	menu_agregar_opcion(submenu_mostrar, "A) Volver al menu anterior");

	printf(ANSI_ALTERNATIVE_SCREEN);
	for (size_t i = 0; i < menu_cantidad(submenu_mostrar); i++) {
		char *opcion_actual = menu_get_opcion(submenu_mostrar, i);
		printf("%s\n", opcion_actual);
	}
	char input = tolower_propio((char)getchar());
	printf(ANSI_NORMAL_SCREEN);
	menu_destruir(submenu_mostrar);
	return input;
}

char fsubmenu_buscar()
{
	menu_t *submenu_buscar = menu_crear("Buscar...", NULL);
	if (submenu_buscar == NULL)
		return 0;

	menu_agregar_opcion(submenu_buscar, "N) Buscar por nombre");
	menu_agregar_opcion(submenu_buscar, "I) Buscar por ID");
	menu_agregar_opcion(submenu_buscar, "A) Volver al menu anterior");

	printf(ANSI_ALTERNATIVE_SCREEN ANSI_RESET_SCREEN ANSI_CURSOR_TOP);
	for (size_t i = 0; i < menu_cantidad(submenu_buscar); i++) {
		char *opcion_actual = menu_get_opcion(submenu_buscar, i);
		printf("%s\n", opcion_actual);
	}
	char input = tolower_propio((char)getchar());
	printf(ANSI_CURSOR_HOME ANSI_NORMAL_SCREEN);
	menu_destruir(submenu_buscar);
	return input;
}

char fmenu_principal()
{
	menu_t *menu_principal =
		menu_crear(ANSI_COLOR_RED ANSI_BG_YELLOW
			   "======Pokedex trivia (TP2)======" ANSI_COLOR_RESET
				   ANSI_BG_RESET);
	menu_agregar_opcion(menu_principal, "J) Jugar");
	menu_agregar_opcion(menu_principal, "S) Jugar con semilla");
	menu_agregar_opcion(menu_principal, "C) Cargar Archivo");
	menu_agregar_opcion(menu_principal, "B) Buscar Pokemon...");
	menu_agregar_opcion(menu_principal, "M) Mostrar Pokemon...");
	menu_agregar_opcion(menu_principal, "Q) Salir");

	char *titulo = menu_mostrar_titulo(menu_principal);
	printf(ANSI_ALTERNATIVE_SCREEN ANSI_RESET_SCREEN ANSI_CURSOR_TOP);
	printf("%s\n", titulo);
	for (size_t i = 0; i < menu_cantidad(menu_principal); i++) {
		char *opcion_actual = menu_get_opcion(menu_principal, i);
		printf("%s\n", opcion_actual);
	}
	char input = tolower_propio((char)getchar());
	printf(ANSI_CURSOR_HOME ANSI_NORMAL_SCREEN);
	menu_destruir(menu_principal);
	return input;
}

/**Originalmente esta funcion solo era una linea (su return) pero por errores con valgrind
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

void llamar_juego_con_semilla(lista_t *pokemons)
{
	int semilla = -1;
	bool ingreso_correcto = false;
	while (ingreso_correcto == false && semilla < 0) {
		printf(ANSI_RESET_SCREEN
		       "\nIngrese la semilla (debe ser mayor a 0): ");
		char *inputs_varios = leer_linea_archivo(stdin);
		printf("\n");
		semilla = strtnum(inputs_varios);
		free(inputs_varios);
	}
	juego(pokemons, (unsigned int)semilla);
}

void jugar_archivo_propio()
{
	char input = 0;
	tp1_t *carga_manual = NULL;
	while (carga_manual == NULL && input != 'n') {
		printf(ANSI_RESET_SCREEN "Ingrese la ruta del archivo: ");
		char *inputs_varios = leer_linea_archivo(stdin);
		printf("\n");
		carga_manual = tp1_leer_archivo(inputs_varios);
		free(inputs_varios);
		if (carga_manual == NULL) {
			printf("No se pudo leer el archivo. Queres intentar de vuelta? S/N: ");
			input = (char)getchar();
		}
	}

	lista_t *pokemons_archivo = lista_crear();
	tp1_con_cada_pokemon(carga_manual, copia_agrega_pokemon,
			     pokemons_archivo);
	juego(pokemons_archivo, 0);
	tp1_destruir(carga_manual);
	lista_destruir_todo(pokemons_archivo, destruir_pokemon_lista);
}

void buscar_por_nombre(lista_t *pokemons)
{
	if (pokemons == NULL)
		return;

	bool se_encontro = false;
	printf(ANSI_RESET_SCREEN "Ingrese el nombre: ");
	char *inputs_varios = leer_linea_archivo(stdin);

	printf("\n");
	for (size_t i = 0; i < lista_cantidad(pokemons); i++) {
		struct pokemon *poke_actual =
			lista_buscar_elemento(pokemons, i);
		if (poke_actual != NULL &&
		    strcmp(poke_actual->nombre, inputs_varios) == 0) {
			mostrar_pokemon(poke_actual);
			se_encontro = true;
		}
	}
	if (se_encontro == false)
		printf("\nNo se encontro ese pokemon en la pokedex cargada :(\n");
	free(inputs_varios);
	printf("Presiona enter para continuar....");
	getchar();
	printf("\n");
}

void buscar_por_id(lista_t *pokemons)
{
	if (pokemons == NULL)
		return;

	printf(ANSI_RESET_SCREEN "Ingrese el ID: ");
	char *inputs_varios = leer_linea_archivo(stdin);
	printf("\n");
	int id = strtnum(inputs_varios);
	bool se_encontro = false;

	for (size_t i = 0; i < lista_cantidad(pokemons); i++) {
		struct pokemon *poke_actual =
			lista_buscar_elemento(pokemons, i);
		if (poke_actual != NULL && poke_actual->id == id) {
			mostrar_pokemon(poke_actual);
			se_encontro = true;
		}
	}

	if (se_encontro == false)
		printf("\nNo se encontro ese pokemon en la pokedex cargada :(\n");

	free(inputs_varios);
	printf("Presiona enter para continuar....");
	getchar();
	printf("\n");
}

void menues_buscar(lista_t *pokemons)
{
	if (pokemons == NULL)
		return;

	bool salir = false;

	while (salir == false) {
		printf(ANSI_RESET_SCREEN);
		char input = fsubmenu_buscar();
		getchar();
		switch (input) {
		case 'n':
			buscar_por_nombre(pokemons);
			break;
		case 'i':
			buscar_por_id(pokemons);
			break;
		case 'a':
			salir = true;
		}
	}
}

void menues_mostrar_nombre(tp1_t *pokemons_tp)
{
	if (pokemons_tp == NULL)
		return;

	tp1_con_cada_pokemon(pokemons_tp, NULL, "ordenar_alfabeticamente");
	lista_t *pokemons_por_nombre = lista_crear();
	if (pokemons_por_nombre == NULL)
		return;

	tp1_con_cada_pokemon(pokemons_tp, copia_agrega_pokemon,
			     pokemons_por_nombre);
	if (lista_cantidad(pokemons_por_nombre) != tp1_cantidad(pokemons_tp)) {
		printf("\nHubo un problema!\n");
		tp1_destruir(pokemons_tp);
	} else {
		for (size_t i = 0; i < lista_cantidad(pokemons_por_nombre);
		     i++) {
			struct pokemon *poke_actual =
				lista_buscar_elemento(pokemons_por_nombre, i);
			if (poke_actual != NULL)
				mostrar_pokemon(poke_actual);
		}
	}
	lista_destruir_todo(pokemons_por_nombre, destruir_pokemon_lista);
	printf("Presiona enter para continuar....");
	getchar();
	printf("\n");
}

void menues_mostrar_id(lista_t *pokemons)
{
	for (size_t i = 0; i < lista_cantidad(pokemons); i++) {
		struct pokemon *poke_actual =
			lista_buscar_elemento(pokemons, i);
		mostrar_pokemon(poke_actual);
	}
	printf("Presiona enter para continuar....");
	getchar();
	printf("\n");
}

void menues_mostrar(tp1_t *pokemons_tp, lista_t *pokemons_lista)
{
	if (pokemons_tp == NULL || pokemons_lista == NULL)
		return;

	char input = fsubmenu_mostrar();
	getchar();
	bool salir = false;

	while (salir == false) {
		switch (input) {
		case 'n':
			menues_mostrar_nombre(pokemons_tp);
		case 'i':
			menues_mostrar_id(pokemons_lista);
		case 'a':
			salir = true;
		}
	}
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
	if (pokemons_lista == NULL) {
		tp1_destruir(pokemons_1);
		return 1;
	}

	bool terminar = false;
	char input;

	while (terminar == false) {
		input = fmenu_principal();
		getchar();
		switch (input) {
		case 'j':
			juego(pokemons_lista, 0);
			printf(ANSI_RESET_SCREEN);
			continue;
		case 's':
			llamar_juego_con_semilla(pokemons_lista);
			break;
		case 'c':
			jugar_archivo_propio();
			continue;
		case 'b':
			menues_buscar(pokemons_lista);
			break;
		case 'm':
			menues_mostrar(pokemons_1, pokemons_lista);
			break;
		case 'q':
			terminar = true;
			break;
		}
	}

	tp1_destruir(pokemons_1);
	lista_destruir_todo(pokemons_lista, destruir_pokemon_lista);
	return 0;
}