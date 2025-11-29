#include "pa2m.h"
#include "src/juego.h"

void prueba_juego_null()
{
	pa2m_afirmar(juego(NULL, 0) == 1,
		     "Si no se envian pokemons a juego, falla");
}

void prueba_elegir_cartas_null()
{
	pa2m_afirmar(elegir_carta(NULL, 3, 3, NULL, false) == NULL,
		     "Si no se envian cartas a elegir, da NULL");
}

void prueba_leer_coordenadas_null()
{
	size_t fila, columna;
	fila = 3;
	columna = 3;
	pa2m_afirmar(leer_coordenada(NULL, &fila, &columna) == false,
		     "Si no se envian input, no se lee carta");
}

void prueba_elegir_pokemons_null()
{
	unsigned int semilla = 8808;
	pa2m_afirmar(elegir_pokemons(NULL, NULL, &semilla) == false,
		     "Si no se envian pokemons, no se mezcla nada");
}

void pruebas_crear_carta_null()
{
	pa2m_afirmar(crear_carta(NULL) == NULL,
		     "Si no se envia pokemon, no se crea carta");
}

//=====

void prueba_juego_sin_semilla()
{
	pa2m_afirmar(juego(NULL, (unsigned int)0) == 1,
		     "Si la semilla es 0 y no hay pokemons, igualmente falla.");
}

void prueba_juego_lista_vacia()
{
	lista_t *pokemons = lista_crear();
	pa2m_afirmar(juego(pokemons, 0) == 1,
		     "Si la lista de pokemons está vacía, el juego falla.");

	lista_destruir(pokemons);
}

void prueba_elegir_pokemons_lista_vacia()
{
	lista_t *pokemons = lista_crear();
	lista_t *elegidos = lista_crear();
	unsigned int semilla = 1234;

	pa2m_afirmar(elegir_pokemons(pokemons, elegidos, &semilla) == false,
		     "No se pueden elegir pokemons de una lista vacía.");

	lista_destruir(pokemons);
	lista_destruir(elegidos);
}

void prueba_elegir_pokemons_sin_semilla()
{
	lista_t *pokemons = lista_crear();
	lista_t *elegidos = lista_crear();

	pa2m_afirmar(elegir_pokemons(pokemons, elegidos, NULL) == false,
		     "Si no se envía semilla, falla la selección.");

	lista_destruir(pokemons);
	lista_destruir(elegidos);
}

void prueba_leer_coordenada_sin_fila()
{
	size_t c = 0;
	pa2m_afirmar(leer_coordenada("A1", NULL, &c) == false,
		     "No se puede leer coordenada si fila es NULL.");
}

void prueba_leer_coordenada_sin_columna()
{
	size_t f = 0;
	pa2m_afirmar(leer_coordenada("A1", &f, NULL) == false,
		     "No se puede leer coordenada si columna es NULL.");
}

void prueba_leer_coordenada_formato_invalido()
{
	size_t f = 0, c = 0;
	pa2m_afirmar(leer_coordenada("ZZZ", &f, &c) == false,
		     "No debe aceptar coordenadas con formato inválido.");
}

void prueba_leer_coordenada_fuera_rango()
{
	size_t f = 0, c = 0;
	pa2m_afirmar(leer_coordenada("Z9", &f, &c) == false,
		     "No debe aceptar coordenadas fuera de rango.");
}

int main()
{
	pa2m_nuevo_grupo("============== Punteros NULL ===============");
	prueba_juego_null();
	prueba_elegir_cartas_null();
	prueba_leer_coordenadas_null();
	prueba_elegir_pokemons_null();
	pruebas_crear_carta_null();

	pa2m_nuevo_grupo("============== Pruebas casos borde ==============");
	prueba_juego_sin_semilla();
	prueba_juego_lista_vacia();
	prueba_elegir_pokemons_lista_vacia();
	prueba_elegir_pokemons_sin_semilla();
	prueba_leer_coordenada_sin_fila();
	prueba_leer_coordenada_sin_columna();
	prueba_leer_coordenada_formato_invalido();
	prueba_leer_coordenada_fuera_rango();

	pa2m_mostrar_reporte();

	return pa2m_mostrar_reporte();
}
