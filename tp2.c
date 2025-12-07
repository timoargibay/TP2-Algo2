#include "src/juego.h"
#include "src/cola.h"

/////////////////////////////////////////////////////////////
////////					     ////////
////////	Funciones de utilidad interna        ////////
////////					     ////////
/////////////////////////////////////////////////////////////

void limpiar_buffer_stdin()
{
	int input = 8;
	while ((input = getchar()) != '\n' && input != 0)
		;
}

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

//Hace una copia real en el heap de el string provisto, hay que liberarlo con free al finalizar
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

void destruir_pokemon(void *pokemon)
{
	if (pokemon == NULL)
		return;

	struct pokemon *pokemon_a_liberar = pokemon;
	free(pokemon_a_liberar->nombre);
	free(pokemon_a_liberar);
}

//Hace una copia real completa de un pokemon provisto y lo guarda en el TDA lista provisto
//Pensada para iteradores internos
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

//TP1_con_cada_pokemon requiere esto para funcionar
bool dummy_true(struct pokemon *un_pokemon, void *un_dato)
{
	return true;
}

//Copa de copia_agrega_pokemon pero usando el tda cola para mejor eficiencia
bool encola_copia_pokemon(struct pokemon *el_pokemon, void *una_cola)
{
	if (una_cola == NULL)
		return false;

	struct pokemon *pokemon_lista = malloc(sizeof(struct pokemon));
	if (pokemon_lista == NULL)
		return false;

	*pokemon_lista = *el_pokemon;
	pokemon_lista->nombre = copiar_string(el_pokemon->nombre);

	if (pokemon_lista->nombre == NULL) {
		free(pokemon_lista);
		return false;
	}

	return cola_encolar(una_cola, pokemon_lista);
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

unsigned int llamar_juego_con_semilla()
{
	int semilla = -1;
	bool ingreso_correcto = false;
	while (ingreso_correcto == false && semilla < 0) {
		printf(ANSI_ALTERNATIVE_SCREEN ANSI_CURSOR_TOP
		       "\nIngrese la semilla (debe ser mayor a 0): ");
		char *inputs_varios = leer_linea_archivo(stdin);
		printf("\n");
		semilla = strtnum(inputs_varios);
		free(inputs_varios);
	}

	return (unsigned int)semilla;
}

void jugar_archivo_propio(unsigned int semilla, unsigned short filas,
			  unsigned short columnas)
{
	char input = 0;
	tp1_t *carga_manual = NULL;
	while (carga_manual == NULL && input != 'n') {
		printf(ANSI_ALTERNATIVE_SCREEN ANSI_CURSOR_TOP
		       "Ingrese la ruta del archivo: ");
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
	juego(pokemons_archivo, semilla, filas, columnas);
	tp1_destruir(carga_manual);
	lista_destruir_todo(pokemons_archivo, destruir_pokemon);
}

void fcomo_se_juega()
{
	printf(ANSI_ALTERNATIVE_SCREEN ANSI_CLEAR_SCREEN ANSI_CURSOR_TOP);
	printf(ANSI_BG_MAGENTA ANSI_COLOR_BOLD ANSI_COLOR_YELLOW
	       "===¿Como se juega?===\n" ANSI_BG_RESET);

	printf(ANSI_COLOR_GREEN
	       "\nEs simple!\n\nSe trata de un juego al estilo memotest, es muy posible q lo hayas jugado en tu infancia.\n"
	       "Para arrancar, se juega de a dos, pero si sos vos solo no decimos nada ;)\n"
	       "Se muestra un tablero con una cantidad par de pokemons, vos tenes q memorizar lo que se muestra lo mejor posible ya que luego las cartas\n"
	       "se dan vuelta y no las podes volver a mirar (al menos, no todas al mismo tiempo),\ndependeras de memorizar las jugadas tuyas y de tu oponente para"
	       "saber donde estan las parejas.\nPara elegir cada carta, se muestran coordenadas como en un mapa,\ndonde cada direccion se corresponde a cada carta.\n\n"
	       "En este juego, las letras se corresponden a cada fila y los numeros a cada columna, entonces si elegis la carta B3, estarias eligiendo la carta de la\n"
	       "segunda fila y tercer columna, que, en caso de ya estar revelada O eliminada, el juego te pedira que elijas de vuelta una que este disponible.\n\n"
	       "Esto se repite, de a 1 turno por jugador, hasta q no queden mas cartas, al finalizar, se mostraran los puntajes,\nsiendo el jugador con mas puntos el ganador.\n\n"
	       "Opcionalmente, si te gusto el orden de las cartas (¿?) podes ver la semilla usada e ingresarla manualmente para jugar la misma partida de vuelta\n" ANSI_COLOR_RESET);

	printf("\n\n\n\n\nPresiona Enter para continuar...");
	getchar();
}

void configurar_dimensiones_juego(unsigned short *filas,
				  unsigned short *columnas)
{
	if (filas == NULL || columnas == NULL)
		return;

	menu_t *config =
		menu_crear(ANSI_COLOR_BOLD ANSI_BG_RED
			   "Configuracion" ANSI_BG_RESET ANSI_COLOR_RESET);
	if (config == NULL)
		return;

	size_t i;
	bool salir = false;
	menu_agregar_opcion(config, "F) Cambiar cantidad de filas");
	menu_agregar_opcion(config, "C) Cambiar cantidad columnas");
	menu_agregar_opcion(config, "A) Ir a menu anterior");
	char input = 0;
	short valor_nuevo_preprocesado;
	char *input_crudo = NULL;

	while (salir == false) {
		printf(ANSI_ALTERNATIVE_SCREEN ANSI_CLEAR_SCREEN
			       ANSI_CURSOR_TOP);

		printf("%s\n", menu_mostrar_titulo(config));
		for (i = 0; i < menu_cantidad(config); i++)
			printf("%s\n", menu_get_opcion(config, i));

		printf("\nCantidad actual de filas: %i\nCantidad actual de columnas: %i\n",
		       *filas, *columnas);
		input = tolower_propio((char)getchar());
		getchar();

		switch (input) {
		case 'f':
			valor_nuevo_preprocesado = 0;
			while (valor_nuevo_preprocesado < 2 ||
			       valor_nuevo_preprocesado > 9) {
				printf(ANSI_CLEAR_SCREEN ANSI_CURSOR_TOP
				       "Ingrese la cantidad de filas: ");
				input_crudo = leer_linea_archivo(stdin);
				valor_nuevo_preprocesado =
					(short int)strtnum(input_crudo);
				free(input_crudo);

				if (valor_nuevo_preprocesado < 2 ||
				    valor_nuevo_preprocesado > 9) {
					printf("ERROR: Valor debe ser mayor a 2 y menor a 10.");
					getchar();
				} else {
					*filas = (unsigned short)
						valor_nuevo_preprocesado;
				}
			}
			break;
		case 'c':
			valor_nuevo_preprocesado = 0;
			while (valor_nuevo_preprocesado < 2 ||
			       valor_nuevo_preprocesado > 9) {
				printf(ANSI_CLEAR_SCREEN ANSI_CURSOR_TOP
				       "Ingrese la cantidad de filas: ");
				input_crudo = leer_linea_archivo(stdin);
				valor_nuevo_preprocesado =
					(short int)strtnum(input_crudo);
				free(input_crudo);
				if (valor_nuevo_preprocesado < 2 ||
				    valor_nuevo_preprocesado > 9) {
					printf("ERROR: Valor debe ser mayor a 2 y menor a 9.");
					getchar();
				} else {
					*columnas = (unsigned short)
						valor_nuevo_preprocesado;
				}
			}
			break;
		case 'a':
			salir = true;
			break;
		}
	}
	menu_destruir(config);
}

////////////////////////////////////////////////////////
////////					////////
////////	Funciones de menues buscar	////////
////////					////////
////////////////////////////////////////////////////////

char fsubmenu_buscar()
{
	menu_t *submenu_buscar = menu_crear("Buscar...");
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

////////////////////////////////////////////////////////
////////					////////
////////	Funciones de mostrar menu	////////
////////					////////
////////////////////////////////////////////////////////
char fsubmenu_mostrar()
{
	menu_t *submenu_mostrar = menu_crear("Mostrar...");
	if (submenu_mostrar == NULL)
		return 0;

	menu_agregar_opcion(submenu_mostrar, "N) Mostrar por nombre");
	menu_agregar_opcion(submenu_mostrar, "I) Mostrar por ID");
	menu_agregar_opcion(submenu_mostrar, "A) Volver al menu anterior");

	printf(ANSI_ALTERNATIVE_SCREEN ANSI_CURSOR_TOP);
	for (size_t i = 0; i < menu_cantidad(submenu_mostrar); i++) {
		char *opcion_actual = menu_get_opcion(submenu_mostrar, i);
		printf("%s\n", opcion_actual);
	}
	char input = tolower_propio((char)getchar());
	printf(ANSI_CURSOR_HOME ANSI_NORMAL_SCREEN);
	menu_destruir(submenu_mostrar);
	return input;
}

void menues_mostrar_nombre(tp1_t *pokemons_tp)
{
	printf(ANSI_NORMAL_SCREEN ANSI_CLEAR_SCREEN);
	if (pokemons_tp == NULL)
		return;

	cola_t *pokemons_por_nombre = cola_crear();
	if (pokemons_por_nombre == NULL)
		return;

	tp1_con_cada_pokemon(pokemons_tp, dummy_true,
			     "ordenar_alfabeticamente");
	tp1_con_cada_pokemon(pokemons_tp, encola_copia_pokemon,
			     pokemons_por_nombre);

	if (cola_cantidad(pokemons_por_nombre) != tp1_cantidad(pokemons_tp)) {
		printf("\nHubo un problema!\n");
		tp1_destruir(pokemons_tp);
		struct pokemon *poke = NULL;
		while ((poke = cola_desencolar(pokemons_por_nombre)) != NULL)
			destruir_pokemon(poke);

		cola_destruir(pokemons_por_nombre);
		return;
	}

	struct pokemon *poke_actual = NULL;

	while ((poke_actual = cola_desencolar(pokemons_por_nombre)) != NULL) {
		mostrar_pokemon(poke_actual);
		destruir_pokemon(poke_actual);
	}

	cola_destruir(pokemons_por_nombre);
	printf("Presiona enter para continuar....");
	getchar();
	printf(ANSI_CLEAR_SCREEN);
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

	char input;
	bool salir = false;

	while (salir == false) {
		input = fsubmenu_mostrar();
		getchar();
		switch (input) {
		case 'n':
			menues_mostrar_nombre(pokemons_tp);
			break;
		case 'i':
			menues_mostrar_id(pokemons_lista);
			break;
		case 'a':
			salir = true;
			break;
		}
	}
}

/////////////////////////////////////////////

char fmenu_principal(unsigned int semilla)
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
	//Nuevas opciones
	menu_agregar_opcion(menu_principal, "E) Cambiar estilos");
	menu_agregar_opcion(menu_principal, "H) ¿Como se juega?");
	menu_agregar_opcion(menu_principal, "A) Ajustes de juego");
	//Fin nuevas opciones
	menu_agregar_opcion(menu_principal, "Q) Salir");

	char *titulo = menu_mostrar_titulo(menu_principal);
	printf(ANSI_ALTERNATIVE_SCREEN ANSI_CURSOR_TOP);
	printf("%s\n", titulo);
	for (size_t i = 0; i < menu_cantidad(menu_principal); i++) {
		char *opcion_actual = menu_get_opcion(menu_principal, i);
		printf("%s\n", opcion_actual);
	}
	printf("\nUsando semilla: " ANSI_COLOR_BOLD ANSI_COLOR_GREEN
	       "%i" ANSI_BG_RESET ANSI_COLOR_RESET "\n",
	       semilla);
	char input = tolower_propio((char)getchar());
	printf(ANSI_CURSOR_HOME ANSI_NORMAL_SCREEN);
	menu_destruir(menu_principal);
	return input;
}

////////////////////////////////////
////////		    ////////
////////	Main	    ////////
////////		    ////////
////////////////////////////////////
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
	unsigned int semilla = (unsigned int)time(NULL);
	unsigned short cantidad_columnas = 6;
	unsigned short cantidad_filas = 3;

	while (terminar == false) {
		input = fmenu_principal(semilla);
		getchar();
		switch (input) {
		case 'j':
			juego(pokemons_lista, semilla, cantidad_filas,
			      cantidad_columnas);
			break;
		case 's':
			semilla = llamar_juego_con_semilla();
			break;
		case 'c':
			jugar_archivo_propio(semilla, cantidad_filas,
					     cantidad_columnas);
			break;
		case 'b':
			menues_buscar(pokemons_lista);
			break;
		case 'm':
			menues_mostrar(pokemons_1, pokemons_lista);
			break;
		case 'e':
			//cambiar colores
			break;
		case 'h':
			fcomo_se_juega();
			break;
		case 'a':
			configurar_dimensiones_juego(&cantidad_filas,
						     &cantidad_columnas);
			break;
		case 'q':
			terminar = true;
			break;
		}
	}

	tp1_destruir(pokemons_1);
	lista_destruir_todo(pokemons_lista, destruir_pokemon);
	return 0;
}