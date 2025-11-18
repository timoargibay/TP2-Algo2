#include "pa2m.h"
#include "src/menu.h"

void prueba_crear_menu()
{
        menu_t *prueba = crear_menu("Prueba De Menus!!");
        if(prueba == NULL) {
                pa2m_afirmar(false, "Fallo al crear menu! (NULL)");
                return;
        }

        menu_agregar_opcion(prueba, "Se ve bien este menu!", 'S');
        menu_agregar_opcion(prueba, "No se ve bien el menu :( (como ves esto entonces?)", 'N');
        char input = mostrar_menu(prueba);
        destruir_menu(prueba);
        pa2m_afirmar(input == 'S' || input == 's', "Se muestra un menu correctamente");
}
int main() {
        prueba_crear_menu();
        pa2m_mostrar_reporte();
}