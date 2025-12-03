#include <time.h>
#include <ctype.h>
#include "ansi.h"
#include "ansi_extended.h"
#include "tp1.h"
#include "extras_tp1.h"
#include "lista.h"
#include "menu.h"
#define CANT_COLUMNAS_PREDET 6
#define CANT_FILAS_PREDET 3
#define CARTA_ANCHO 9
#define CARTA_ALTO 5
#define ESPACIO 2

typedef struct carta {
	struct pokemon *pokemon_carta;
	bool revelada;
	bool eliminada;
} carta_t;

carta_t *crear_carta(struct pokemon *pokemon)
{
	if (pokemon == NULL)
		return NULL;

	carta_t *nueva_carta = malloc(sizeof(carta_t));
	if (nueva_carta == NULL)
		return NULL;

	nueva_carta->pokemon_carta = pokemon;
	nueva_carta->eliminada = false;
	nueva_carta->revelada = false;

	return nueva_carta;
}

void mezclar_fisher_yates(size_t *posiciones_elegidas, size_t cantidad)
{
	size_t i, j, tmp;

	if (posiciones_elegidas == NULL || cantidad < 2)
		return;

	for (i = 0; i < cantidad; i++) {
		j = (size_t)rand() % (i + 1);
		tmp = posiciones_elegidas[i];
		posiciones_elegidas[i] = posiciones_elegidas[j];
		posiciones_elegidas[j] = tmp;
	}
}

bool actualizar_jugadas(char *jugadas[5], size_t *jugada_numero,
			unsigned int fila_cart0, unsigned int col_cart0,
			unsigned int fila_cart1, unsigned int col_cart1,
			bool acierto)
{
	if(jugadas == NULL || jugada_numero == NULL)
		return false;

	char char_fila_carta0 = 'A' + (char)fila_cart0;
	char char_fila_carta1 = 'A' + (char)fila_cart1;
	size_t tamaño_buffer = sizeof(char) *50;
	char *nueva_jugada = malloc(tamaño_buffer);
	if(nueva_jugada == NULL)
		return false;

	char de_si_acerto = (acierto) ? 'V' : 'X'; //Por limitaciones de char, imagine a la V como ✓ 
	if(acierto) {
		snprintf(nueva_jugada, tamaño_buffer, "Jugada [%ld]: Cartas %c%i - %c%i" ANSI_COLOR_BOLD ANSI_COLOR_GREEN " %c" ANSI_COLOR_RESET,
			*jugada_numero, char_fila_carta0, col_cart0+1, char_fila_carta1, col_cart1+1, de_si_acerto);
	} else {
		snprintf(nueva_jugada, tamaño_buffer, "Jugada [%ld]: Cartas %c%i - %c%i" ANSI_COLOR_BOLD ANSI_COLOR_RED " %c" ANSI_COLOR_RESET,
			*jugada_numero, char_fila_carta0, col_cart0+1, char_fila_carta1, col_cart1+1, de_si_acerto);
	}

	if(jugadas[4] != NULL)
		free(jugadas[4]);

	for(int i = 4; i > 0; i--) {
		jugadas[i] = jugadas[i-1];
	}

	jugadas[0] = nueva_jugada;
	(*jugada_numero)++;

	return true;
}

//Devuelve una lista con las cartas aleatoriamente
bool elegir_pokemons(lista_t *pokemons, lista_t *pokemons_elegidos, unsigned short cant_filas, unsigned short cant_columnas)
{
	if (pokemons == NULL || pokemons_elegidos == NULL ||
	    lista_cantidad(pokemons) <= 1)
		return false;

	size_t cantidad = lista_cantidad(pokemons);
	size_t total = cant_filas * cant_columnas;
	size_t cantidad_a_elegir = (total % 2 == 0) ?
					   total / 2 :
					   (total-1) / 2;
	if (cantidad < cantidad_a_elegir)
		return false;

	size_t i;
	struct pokemon *tmp = NULL;

	size_t *posiciones_elegidas =
		malloc(sizeof(size_t) * cantidad_a_elegir);
	if (posiciones_elegidas == NULL)
		return false;

	for (i = 0; i < cantidad_a_elegir; i++)
		posiciones_elegidas[i] = i;

	mezclar_fisher_yates(posiciones_elegidas, cantidad_a_elegir);

	for (i = 0; i < cantidad_a_elegir; i++) {
		tmp = lista_buscar_elemento(pokemons, posiciones_elegidas[i]);
		lista_agregar(pokemons_elegidos, tmp);
	}

	free(posiciones_elegidas);
	return true;
}

void imprimir_carta_oculta(char salida[CARTA_ALTO][CARTA_ANCHO + 1])
{
	snprintf(salida[0], CARTA_ANCHO + 1, "+-------+");
	snprintf(salida[1], CARTA_ANCHO + 1, "|       |");
	snprintf(salida[2], CARTA_ANCHO + 1, "|  ???  |");
	snprintf(salida[3], CARTA_ANCHO + 1, "|       |");
	snprintf(salida[4], CARTA_ANCHO + 1, "+-------+");
}

void imprimir_carta_revelada(const char *texto,
			     char salida[CARTA_ALTO][CARTA_ANCHO + 1])
{
	snprintf(salida[0], CARTA_ANCHO + 1, "+-------+");
	snprintf(salida[1], CARTA_ANCHO + 1, "|%-7s|", "");
	snprintf(salida[2], CARTA_ANCHO + 1, "|%-7s|", texto);
	snprintf(salida[3], CARTA_ANCHO + 1, "|%-7s|", "");
	snprintf(salida[4], CARTA_ANCHO + 1, "+-------+");
}

void mostrar_tablero(lista_t *cartas, size_t filas, size_t columnas,
		     bool revelar, char *ultimas[])
{
	if (cartas == NULL)
		return;

	size_t padding = 5;
	char diseño[CARTA_ALTO][CARTA_ANCHO + 1];

	printf(ANSI_CLEAR_SCREEN ANSI_CURSOR_TOP "     ");
	for (size_t col = 1; col <= columnas; col++)
		printf("    %zu      ", col);
	printf("\n");

	size_t i, j, k;
	for (i = 0; i < filas; i++) {
		printf("  %c \n", (int)('A' + i));
		for (j = 0; j < CARTA_ALTO; j++) {
			printf("%*s", (int)padding, "");

			for (k = 0; k < columnas; k++) {
				carta_t *carta_actual = lista_buscar_elemento(
					cartas, i * columnas + k);
				if (carta_actual == NULL)
					continue;
				else if (revelar)
					imprimir_carta_revelada(
						carta_actual->pokemon_carta
							->nombre,
						diseño);
				else if (carta_actual->eliminada)
					imprimir_carta_revelada("", diseño);
				else if (carta_actual->revelada)
					imprimir_carta_revelada(
						carta_actual->pokemon_carta
							->nombre,
						diseño);
				else
					imprimir_carta_oculta(diseño);

				printf("%s", diseño[j]);

				if (k != columnas - 1)
					printf("%*s", ESPACIO, "");
			}
			printf("\n");
		}
	}

	if(ultimas == NULL)
		return;

	for(i = 0; i < 5; i++) {
		if(ultimas[i] != NULL)
			printf("%s\n", ultimas[i]);
	}
}

bool leer_coordenada(const char *input, unsigned int *fila, unsigned int *columna, unsigned int cant_filas, unsigned int cant_columnas)
{
	if (!input || !fila || !columna)
		return false;

	if (strlen(input) < 2)
		return false;

	char letra = input[0];
	char col_char = input[1];

	if (letra >= 'A' && letra <= 'Z')
		letra = letra - 'A' + 'a';

	if (letra < 'a' || letra >= 'a' + cant_filas)
		return false;

	if (col_char < '1' || col_char > '0' + cant_columnas)
		return false;

	*fila = (unsigned int)letra - 'a';
	*columna = (unsigned int)col_char - '1';

	return true;
}

carta_t *elegir_carta(lista_t *cartas, 
			unsigned int *fila_elegida, unsigned int *columna_elegida,
			unsigned int filas, unsigned int columnas,
			carta_t *eleccion_previa, bool *turno)
{
	if (!cartas || !turno)
		return NULL;

	char jugador1[] = "Jugador 1";
	char jugador2[] = "Jugador 2";
	char *le_toca = (*turno) ? jugador2 : jugador1;
	bool seleccion_valida = false;
	char *input = NULL;
	unsigned int fila = 999;
	unsigned int columna = 999;
	carta_t *carta_elegida = NULL;

	while (seleccion_valida == false) {
		free(input);
		printf("\nSeleccione una carta %s (ej: A1, B2, C3): ", le_toca);
		input = leer_linea_archivo(stdin);
		printf(ANSI_CLEAR_LINE);

		if (input == NULL ||
		    leer_coordenada(input, &fila, &columna, filas, columnas) == false) {
			printf("Error: Coord invalida :(");
			continue;
		}

		if (fila >= filas || columna >= columnas) {
			printf("Fuera de rango!");
			continue;
		}

		carta_elegida = lista_buscar_elemento(cartas, (size_t)fila * columnas + columna);
		if (carta_elegida == NULL) {
			printf("Error, esa carta no existe o no la encontramos :(");
			continue;
		}

		if (carta_elegida->eliminada) {
			printf("Esa carta ya fue eliminada!");
			continue;
		}

		if (carta_elegida == eleccion_previa) {
			printf("No podes elegir la misma carta!");
			continue;
		}

		seleccion_valida = true;
	}

	if(fila_elegida != NULL && columna_elegida != NULL) {
		*fila_elegida = fila;
		*columna_elegida = columna;
	}

	free(input);
	return carta_elegida;
}

int juego(lista_t *pokemons_disponibles, unsigned int semilla, unsigned short user_cant_filas, unsigned short user_cant_columnas)
{
	unsigned short cant_filas = (user_cant_filas != 0) ? user_cant_filas : CANT_FILAS_PREDET;
	unsigned short cant_columnas = (user_cant_columnas != 0) ? user_cant_columnas : CANT_COLUMNAS_PREDET;
	char jugador1[] = "Jugador 1";
	char jugador2[] = "Jugador 2";
	size_t j1_puntaje = 0;
	size_t j2_puntaje = 0;
	bool turno = false; //false: 1  | true: 2
	lista_t *pokemons_elegidos = lista_crear();
	lista_t *cartas = lista_crear();
	lista_t *cartas_mezcladas = lista_crear();
	struct pokemon *pokemon_actual = NULL;
	carta_t *carta_actual = NULL;
	size_t i;
	size_t cartas_eliminadas = 0;

	unsigned int semilla_actual = (semilla != 0) ? semilla :
						       (unsigned int)time(NULL);

	if (pokemons_elegidos == NULL || cartas == NULL ||
	    cartas_mezcladas == NULL) {
		lista_destruir(pokemons_elegidos);
		lista_destruir(cartas);
		lista_destruir(cartas_mezcladas);
		return 1;
	}

	if (elegir_pokemons(pokemons_disponibles, pokemons_elegidos, cant_filas, cant_columnas) == false) {
		lista_destruir(pokemons_elegidos);
		lista_destruir(cartas);
		lista_destruir(cartas_mezcladas);
		return 1;
	}

	for (i = 0; i < lista_cantidad(pokemons_elegidos); i++) {
		pokemon_actual = lista_buscar_elemento(pokemons_elegidos, i);
		carta_actual = crear_carta(pokemon_actual);
		if (carta_actual != NULL)
			lista_agregar(cartas, carta_actual);
	}

	//Crea la pareja de las primeras cartas
	for (i = 0; i < lista_cantidad(pokemons_elegidos); i++) {
		pokemon_actual = lista_buscar_elemento(pokemons_elegidos, i);
		carta_actual = crear_carta(pokemon_actual);
		if (carta_actual != NULL)
			lista_agregar(cartas, carta_actual);
	}

	size_t cantidad_cartas = lista_cantidad(cartas);

	size_t array_pos_cartas[cantidad_cartas];
	for (i = 0; i < cantidad_cartas; i++)
		array_pos_cartas[i] = i;

	mezclar_fisher_yates(array_pos_cartas, cantidad_cartas);
	for (i = 0; i < cantidad_cartas; i++) {
		lista_agregar(cartas_mezcladas,
			      lista_buscar_elemento(cartas,
						    array_pos_cartas[i]));
	}

	if (lista_cantidad(cartas_mezcladas) != cantidad_cartas) {
		lista_destruir(pokemons_elegidos);
		lista_destruir(cartas_mezcladas);
		lista_destruir_todo(cartas, free);
		return 1;
	}

	char *basura = NULL;
	carta_t *carta1 = NULL;
	carta_t *carta2 = NULL;
	
	unsigned int fila_elegida0;
	unsigned int columna_elegida0;
	unsigned int fila_elegida1;
	unsigned int columna_elegida1;
	size_t jugada_nro = 1;
	char *ultimas_jugadas[5] = {0};
	bool fue_acierto;

	printf(ANSI_ALTERNATIVE_SCREEN);
	mostrar_tablero(cartas_mezcladas, cant_filas, cant_columnas, true, NULL);
	printf("\nMemorizenlas!!\n"
	       "Semilla actual: %i\n"
	       "Presione ENTER para continuar...\n",
	       semilla_actual);
	basura = leer_linea_archivo(stdin);
	free(basura);

	while (cartas_eliminadas < lista_cantidad(cartas)) {
		mostrar_tablero(cartas_mezcladas, cant_filas, cant_columnas,
				false, ultimas_jugadas);
		carta1 = elegir_carta(cartas_mezcladas, &fila_elegida0, &columna_elegida0, cant_filas,
				      cant_columnas, NULL, &turno);
		carta1->revelada = true;
		mostrar_tablero(cartas_mezcladas, cant_filas, cant_columnas,
				false, ultimas_jugadas);
		carta2 = elegir_carta(cartas_mezcladas, &fila_elegida1, &columna_elegida1, cant_filas,
				      cant_columnas, carta1, &turno);
		carta2->revelada = true;
		mostrar_tablero(cartas_mezcladas, cant_filas, cant_columnas,
				false, ultimas_jugadas);

		if (carta1->pokemon_carta->id == carta2->pokemon_carta->id) {
			printf("\n" ANSI_COLOR_GREEN
			       "¡Correcto! :D\n" ANSI_COLOR_RESET);
			carta1->eliminada = carta2->eliminada = true;
			cartas_eliminadas += 2;
			if (turno)
				j2_puntaje++;
			else
				j1_puntaje++;

			fue_acierto = true;
		} else {
			printf("\n" ANSI_COLOR_RED "Incorrecto :(\n"
			       "Las cartas no coinciden.\n" ANSI_COLOR_RESET);
			carta1->revelada = carta2->revelada = false;
			fue_acierto = false;
		}

		printf("\nPresione ENTER para continuar...");
		basura = leer_linea_archivo(stdin);
		free(basura);
		turno = (turno) ? false : true;
		actualizar_jugadas(ultimas_jugadas, &jugada_nro, fila_elegida0, columna_elegida0, fila_elegida1, columna_elegida1, fue_acierto);
	}
	char *ganador = (j1_puntaje > j2_puntaje) ? jugador1 : jugador2;
	char *perdedor = (j1_puntaje < j2_puntaje) ? jugador1 : jugador2;
	size_t *puntaje_ganador = (j1_puntaje > j2_puntaje) ? &j1_puntaje :
							      &j2_puntaje;
	size_t *puntaje_perdedor = (j2_puntaje > j1_puntaje) ? &j1_puntaje :
							       &j2_puntaje;
	printf(ANSI_CLEAR_SCREEN ANSI_CURSOR_TOP
	       "Terminaron! Felicitaciones :) \n*escena de evangelion de todos aplaudiendo*\n");
	if(j1_puntaje == j2_puntaje) {
		printf("Resultados:\n" ANSI_COLOR_GREEN "Empate!! Con un puntaje de %ld" ANSI_COLOR_RESET "\n", j1_puntaje);
	} else {
		printf("Resultados: \n" ANSI_COLOR_GREEN
			"Gano: %s con un puntaje de %ld\n" ANSI_COLOR_RED
			"Perdio: %s con un puntaje de %ld\n" ANSI_COLOR_RESET
			"Jugaste en la semilla:" ANSI_BG_GREEN ANSI_COLOR_BOLD
			"%i" ANSI_BG_RESET ANSI_COLOR_RESET "\n",
			ganador, *puntaje_ganador, perdedor, *puntaje_perdedor,
			semilla_actual);
	}
	getchar();
	printf(ANSI_NORMAL_SCREEN);

	for(i = 0; i < 5; i++) {
		free(ultimas_jugadas[i]);
	}
	lista_destruir(pokemons_elegidos);
	lista_destruir(cartas_mezcladas);
	lista_destruir_todo(cartas, free);

	return 0;
}