#include "pa2m.h"
#include "src/menu.h"

void prueba_mostrar_menu()
{
	menu_t *prueba =
		menu_crear("Prueba De Menus!!", ANSI_BG_GREEN ANSI_COLOR_BLACK);
	if (prueba == NULL) {
		pa2m_afirmar(false, "Fallo al crear menu! (NULL)");
		return;
	}

	menu_agregar_opcion(prueba, "Se ve bien este menu!", 'S',
			    ANSI_COLOR_GREEN);
	menu_agregar_opcion(
		prueba, "No se ve bien el menu :( (como ves esto entonces?)",
		'N', ANSI_COLOR_RED);

	char input = menu_mostrar(prueba, true);
	menu_destruir(prueba);
	pa2m_afirmar(input == 's', "Se muestra un menu correctamente");
}

void prueba_menu_null()
{
	menu_t *prueba = menu_crear(NULL, NULL);
	menu_destruir(prueba);
	pa2m_afirmar(prueba != NULL,
		     "Al crear un menu sin titulo ni estilo, no hay problemas");
}

void prueba_agregar_opcion_null()
{
	pa2m_afirmar(menu_agregar_opcion(NULL, "Pruebaaa", 'K',
					 ANSI_COLOR_BLUE) == false,
		     "Agregar opcion a menu NULL da false");
}

void prueba_modificar_titulo()
{
	pa2m_afirmar(menu_modificar_titulo(NULL, "Hola", NULL) == false,
		     "Modificar titulo de menu NULL da false");
}

void prueba_mostrar_menu_null()
{
	pa2m_afirmar(menu_mostrar(NULL, false) == 0,
		     "Mostrar menu NULL devuelve 0");
}

void prueba_borrar_menu_null_no_falla()
{
	menu_destruir(NULL);
	pa2m_afirmar(true, "No crashea destruir un menu NULL");
}

int main()
{
	pa2m_nuevo_grupo("Pruebas a TDA Menu");
	prueba_mostrar_menu();
	prueba_menu_null();
	prueba_agregar_opcion_null();
	prueba_modificar_titulo();
	prueba_mostrar_menu_null();
	prueba_borrar_menu_null_no_falla();
	pa2m_mostrar_reporte();
}