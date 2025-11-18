#include "tp1.h"
#include "extras_tp1.h"

/**
 * Dado un puntero a archivo, leé una linea y devuelve un puntero a la misma
 */
char *leer_linea_archivo(FILE *archivo)
{
	size_t capacidad = 4;
	int longitud = 0;
	int caracter; //Uso int y no char por problemas para detectar EOF y similares problemas con FGETC
	char *buffer = malloc(capacidad * sizeof(char));

	if (buffer == NULL)
		return NULL;

	while ((caracter = fgetc(archivo)) != EOF && caracter != '\n') {
		if (longitud >= capacidad - 1) {
			capacidad *= 2;
			char *tmp = realloc(buffer, capacidad * sizeof(char));
			if (tmp == NULL) {
				free(buffer);
				return NULL;
			} else {
				buffer = tmp;
			}
		}
		buffer[longitud++] = (char)caracter;
	}

	if (longitud == 0 && caracter == EOF) {
		free(buffer);
		return NULL;
	}

	buffer[longitud] = '\0';
	return buffer;
}

/**
 *Lee la linea por columna
*/
char *leer_parte(char *linea, long unsigned int *inicio)
{
	size_t largo_columna = strcspn(&linea[*inicio], ",\n\0");
	char *columna = malloc((largo_columna + 1) * sizeof(char));

	if (columna == NULL)
		return NULL;

	memcpy(columna, &linea[*inicio], largo_columna);
	columna[largo_columna] = '\0';
	*inicio += largo_columna;

	if (linea[*inicio] == ',')
		(*inicio)++;

	return columna;
}

/**
 *Dada un tipo de pokemon, verifica que este sea valido
 *correr al cargar pokemon, antes de validar_carga_pokemon
 */
enum tipo_pokemon validar_tipo_pokemon(char *tipo)
{
	if (strcmp(tipo, "ELEC") == 0)
		return TIPO_ELEC;
	else if (strcmp(tipo, "FUEG") == 0)
		return TIPO_FUEG;
	else if (strcmp(tipo, "PLAN") == 0)
		return TIPO_PLAN;
	else if (strcmp(tipo, "AGUA") == 0)
		return TIPO_AGUA;
	else if (strcmp(tipo, "NORM") == 0)
		return TIPO_NORM;
	else if (strcmp(tipo, "FANT") == 0)
		return TIPO_FANT;
	else if (strcmp(tipo, "PSI") == 0)
		return TIPO_PSI;
	else if (strcmp(tipo, "LUCH") == 0)
		return TIPO_LUCH;
	else
		return -1;
}

//Funcion no presente en la firma, solo se usa en validar_carga_pokemon
bool stat_valido(int stat)
{
	if (stat > 100 || stat < 0)
		return false;
	else
		return true;
}

//Valida un pokemon y que este esté bien//
bool validar_carga_pokemon(struct pokemon un_pokemon)
{
	if (un_pokemon.id < 0)
		return false;

	if (un_pokemon.nombre == NULL)
		return false;

	if (un_pokemon.tipo == -1)
		return false;

	if (!stat_valido(un_pokemon.ataque))
		return false;

	if (!stat_valido(un_pokemon.defensa))
		return false;

	if (!stat_valido(un_pokemon.velocidad))
		return false;

	return true;
}

/*
 *Dado un string con un numero, lo convierte a int 
*/
int strtnum(char *string)
{
	char *fin_linea;
	long valor_obtenido = strtol(string, &fin_linea, 10);

	if (*fin_linea ==
	    '\0') //Si strtol anduvo bien, obtengo '\0', si obtengo otro char, la columna es invalida o hubo un drama
		return (int)valor_obtenido;
	else
		return -1;
}

/**
 *Dada un tipo de pokemon, verifica que este sea valido
 *correr al cargar pokemon, antes de validar_carga_pokemon
 */
char *enum_a_string_tipo_pokemon(enum tipo_pokemon tipo)
{
	switch (tipo) {
	case TIPO_ELEC:
		return "ELEC";

	case TIPO_AGUA:
		return "AGUA";

	case TIPO_FANT:
		return "FANT";

	case TIPO_FUEG:
		return "FUEG";

	case TIPO_LUCH:
		return "LUCH";

	case TIPO_NORM:
		return "NORM";

	case TIPO_PLAN:
		return "PLAN";

	case TIPO_PSI:
		return "PSI";

	default:
		return NULL;
	}
}

/*
 *Dado un pokemon y un struct de destino, hace una copia completa a destino, 
  * null si hay error
 */
struct pokemon *copiar_pokemon(struct pokemon *destino, struct pokemon *fuente)
{
	*destino = *fuente;

	if (fuente->nombre != NULL) {
		destino->nombre =
			malloc((strlen(fuente->nombre) + 1) * sizeof(char));
	}

	if (destino->nombre != NULL)
		strcpy(destino->nombre, fuente->nombre);

	return destino;
}

//Funcion no presente en la firma, solo se usa acá para ordenar
void swap_elementos(struct pokemon *lista, size_t posicion)
{
	struct pokemon tmp = lista[posicion - 1];
	lista[posicion - 1] = lista[posicion];
	lista[posicion] = tmp;
}

//Ordena una lista ascendentemente por ID
void ordenar_lista(struct pokemon *lista_pokemon, size_t cantidad_elementos)
{
	bool lista__esta_ordenada = false;
	size_t i;

	while (!lista__esta_ordenada) {
		lista__esta_ordenada = true;
		for (i = 1; i < cantidad_elementos; i++) {
			if (lista_pokemon[i - 1].id > lista_pokemon[i].id) {
				swap_elementos(lista_pokemon, i);
				lista__esta_ordenada = false;
			}
		}
		cantidad_elementos--;
	}
}

//Busca el id mas chico en una lista
size_t buscar_por_menor_id(struct pokemon *lista, size_t cantidad,
			   size_t arranca_desde_aca)
{
	size_t i = arranca_desde_aca;
	size_t pos_id_minimo = i;
	for (i = arranca_desde_aca; i < cantidad; i++) {
		if (lista[i].id < lista[pos_id_minimo].id)
			pos_id_minimo = i;
	}

	return (size_t)lista[pos_id_minimo].id;
}

//Verifica si una lista ya esta ordenada
bool lista_ordenada(struct pokemon *lista, size_t tamaño)
{
	int i;
	bool ordenada = true;
	for (i = 0; i < tamaño - 1 && ordenada; i++) {
		if ((lista[i].id <= lista[i + 1].id) == false)
			ordenada = false;
	}

	return ordenada;
}

//Ordena una lista alfabeticamente por nombre
int ordenar_alfabeticamente(struct pokemon *lista, size_t cantidad)
{
	bool ordenada = false;
	size_t posicion;

	if (cantidad == 0 || lista == NULL)
		return 1;

	while (ordenada == false) {
		ordenada = true;
		for (posicion = 1; posicion < cantidad; posicion++) {
			if (strcmp(lista[posicion - 1].nombre,
				   lista[posicion].nombre) > 0) {
				swap_elementos(lista, posicion);
				ordenada = false;
			}
		}
		cantidad--;
	}

	return 0;
}