#include "pa2m.h"
#include "src/menu.h"

void prueba_crear_menu()
{
	menu_t *prueba =
		crear_menu("Prueba De Menus!!", ANSI_BG_GREEN ANSI_COLOR_BLACK);
	if (prueba == NULL) {
		pa2m_afirmar(false, "Fallo al crear menu! (NULL)");
		return;
	}

	menu_agregar_opcion(prueba, "Se ve bien este menu!", 'S',
			    ANSI_COLOR_GREEN);
	menu_agregar_opcion(
		prueba, "No se ve bien el menu :( (como ves esto entonces?)",
		'N', ANSI_COLOR_RED);
	char input = mostrar_menu(prueba);
	destruir_menu(prueba);
	pa2m_afirmar(input == 's', "Se muestra un menu correctamente");
}
int main()
{
	prueba_crear_menu();
	pa2m_mostrar_reporte();
}