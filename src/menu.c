#include "menu.h"

typedef struct opciones {
        char tecla;
        char *texto;
} opcion_t;

struct menu {
        char *titulo;
        lista_t *opciones;
};

/////////////////////////////////////////////////////////////////
/////////////////                               /////////////////
/////////////////       Funciones internas      /////////////////
/////////////////                               /////////////////
/////////////////////////////////////////////////////////////////

static char *copiar_string(char *string)
{
	if (string == NULL)
		return NULL;

	char *copia = malloc((strlen(string) + 1) * sizeof(char));
	if (copia == NULL)
		return NULL;

	strcpy(copia, string);
	return copia;
}

static opcion_t *nueva_opcion(char *texto, char tecla)
{
        if(texto == NULL)
                return NULL;
                
        opcion_t *nueva_opcion = malloc(sizeof(opcion_t));
        if(nueva_opcion == NULL)
                return NULL;

        nueva_opcion->tecla = tecla;
        nueva_opcion->texto = copiar_string(texto);
        if(nueva_opcion->texto == NULL) {
                free(nueva_opcion);
                return NULL;
        }

        return nueva_opcion;
}

static void liberar_opcion(void *opcion) {
        if(opcion == NULL)
                return;

        opcion_t *oopcion = opcion;
        free(oopcion->texto);
        free(oopcion);
}

/////////////////////////////////////////////////////////////////
/////////////////                               /////////////////
/////////////////       Funciones publicas      /////////////////
/////////////////                               /////////////////
/////////////////////////////////////////////////////////////////

menu_t *crear_menu(char *titulo)
{
        menu_t *nuevo_menu = malloc(sizeof(menu_t));
        if(nuevo_menu == NULL)
                return NULL;

        nuevo_menu->opciones = lista_crear();
        if(nuevo_menu->opciones == NULL) {
                free(nuevo_menu);
                return NULL;
        }

        if(titulo != NULL) {
                nuevo_menu->titulo = copiar_string(titulo);
                if(nuevo_menu->titulo == NULL) {
                        lista_destruir(nuevo_menu->opciones);
                        free(nuevo_menu);
                        return NULL;
                }
        } else {
                nuevo_menu->titulo = NULL;
        }

        return nuevo_menu;        
}

bool menu_agregar_opcion(menu_t *menu, char *texto, char tecla)
{
        if(menu == NULL || texto == NULL || tecla == 0) 
                return false;

        opcion_t *opcion = nueva_opcion(texto, tecla);
        if(opcion == NULL)
                return false;

        bool estado = lista_agregar(menu->opciones, opcion);
        if(estado == false)
                liberar_opcion(opcion);

        return estado;
}

char mostrar_menu(const menu_t *menu)
{
        printf(ANSI_CLEAR_SCREEN "\n");

        if(menu == NULL) {
                printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "Hubo un error con el menu!" ANSI_COLOR_RESET "\n");
                return 0;
        }

        char input = 0;
        int limpiador;
        if(menu->titulo != NULL)
                printf(ANSI_COLOR_BOLD ANSI_BG_BLUE ANSI_COLOR_YELLOW "%s\n" ANSI_COLOR_RESET, menu->titulo);
        
        lista_iterador_t *iterador = lista_iterador_crear(menu->opciones);

        opcion_t *opcion_actual = lista_iterador_obtener_actual(iterador);
        
        while(opcion_actual != NULL) {
                printf(ANSI_BG_RED ANSI_COLOR_BOLD ANSI_COLOR_BLACK"- %c: %s\n" ANSI_BG_RESET ANSI_COLOR_RESET,opcion_actual->tecla, opcion_actual->texto);
                lista_iterador_siguiente(iterador);                
                opcion_actual = lista_iterador_obtener_actual(iterador);
        }
        
        scanf(" %c", &input);
        lista_iterador_destruir(iterador);
        while((limpiador = getchar()) != '\n' && limpiador != EOF);

        return (char)input;
}

void destruir_menu(menu_t *menu)
{
        if(menu == NULL)
                return;

        lista_destruir_todo(menu->opciones, liberar_opcion);
        free(menu->titulo);
        free(menu);
}
