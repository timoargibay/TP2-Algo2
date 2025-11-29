#include "pa2m.h"
#include "src/menu.h"
#include "src/extras_tp1.h"

char tolower_propio(char l)
{
	if (l >= 'A' && l <= 'Z')
		return l + 32;

	return l;
}

void prueba_mostrar_menu()
{
	menu_t *prueba =
		menu_crear(ANSI_BG_GREEN ANSI_COLOR_BLACK "Prueba De Menus!!");
	if (prueba == NULL) {
		pa2m_afirmar(false, "Fallo al crear menu! (NULL)");
		return;
	}

	menu_agregar_opcion(prueba, ANSI_COLOR_GREEN "Se ve bien este menu!");
	menu_agregar_opcion(
		prueba, ANSI_COLOR_RED
		"No se ve bien el menu :( (como ves esto entonces?)" ANSI_COLOR_RESET);

	for (size_t i = 0; i < menu_cantidad(prueba); i++) {
		char *opcion_actual = menu_get_opcion(prueba, i);
		printf("%s\n", opcion_actual);
	}

	char input = tolower_propio((char)getchar());
	getchar();
	menu_destruir(prueba);
	pa2m_afirmar(input == 's', "Se muestra un menu correctamente");
}

void prueba_menu_null()
{
	menu_t *prueba = menu_crear(NULL);
	menu_destruir(prueba);
	pa2m_afirmar(prueba != NULL,
		     "Al crear un menu sin titulo ni estilo, no hay problemas");
}

void prueba_agregar_opcion_null()
{
	pa2m_afirmar(menu_agregar_opcion(NULL, "Pruebaaa") == false,
		     "Agregar opcion a menu NULL da false");
}

//void prueba_modificar_titulo()
//{
//	pa2m_afirmar(menu_modificar_titulo(NULL, "Hola", NULL) == false,
//		     "Modificar titulo de menu NULL da false");
//}

void prueba_mostrar_menu_null()
{
	pa2m_afirmar(menu_get_opcion(NULL, 3) == NULL,
		     "Mostrar opcion menu NULL devuelve NULL");
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
	//prueba_modificar_titulo();
	prueba_mostrar_menu_null();
	prueba_borrar_menu_null_no_falla();
	pa2m_mostrar_reporte();
}