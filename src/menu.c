#include "menu.h"
#define ANSI_CURSOR_TOP "\033[H"
#define ANSI_ALTERNATIVE_SCREEN "\033[?1049h"
#define ANSI_RETURN_SCREEN "\033[?1049l"

typedef struct opciones {
        char tecla;
        char *texto;
} opcion_t;

struct menu {
        char *titulo;
        lista_t *opciones;
        char *estilos;
};

/////////////////////////////////////////////////////////////////
/////////////////                               /////////////////
/////////////////       Funciones internas      /////////////////
/////////////////                               /////////////////
/////////////////////////////////////////////////////////////////

static char *copiar_string(const char *string)
{
	if (string == NULL)
		return NULL;

	char *copia = malloc((strlen(string) + 1) * sizeof(char));
	if (copia == NULL)
		return NULL;

	strcpy(copia, string);
	return copia;
}

static opcion_t *nueva_opcion(const char *texto, char tecla)
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

menu_t *crear_menu(const char *titulo, const char *estilos)
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

        if(estilos != NULL) {
                nuevo_menu->estilos = copiar_string(estilos);
                if(nuevo_menu->estilos == NULL) {
                        lista_destruir(nuevo_menu->opciones);
                        free(nuevo_menu);
                        return NULL;
                }
        } else {
                nuevo_menu->estilos = NULL;
        }

        return nuevo_menu;        
}

bool menu_agregar_opcion(menu_t *menu, const char *texto, char tecla)
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

bool menu_modificar_titulo(menu_t *menu, const char *titulo)
{
        if(menu == NULL)
                return false;

        if(titulo == NULL) {
                if(menu->titulo != NULL) 
                        free(menu->titulo);

                menu->titulo = NULL;
                return true;
        }

        char *nuevo_titulo = copiar_string(titulo);
        if(nuevo_titulo == NULL)
                return false;

        if(menu->titulo != NULL)
                free(menu->titulo);

        menu->titulo = nuevo_titulo;
        return true;
}

bool menu_editar_estilo(menu_t *menu, const char *estilos_ansi)
{
        if(menu == NULL || estilos_ansi == NULL)
                return false;

        char *nuevo = copiar_string(estilos_ansi);
        if(nuevo == NULL)
                return false;

        if(menu->estilos != NULL)
                free(menu->estilos);

        menu->estilos = nuevo;

        return true;
}

char mostrar_menu(menu_t *menu)
{
        if(menu == NULL) {
                printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "Hubo un error con el menu!" ANSI_COLOR_RESET "\n");
                return 0;
        }
        
        char input[2] = {0};
        int limpiador;
        const char *estilos = (menu->estilos ? menu->estilos : "");
        
        printf(ANSI_ALTERNATIVE_SCREEN
                ANSI_CURSOR_TOP);
        
        if(menu->titulo != NULL)
                printf( "%s\n===%s===\n\n\n" ANSI_COLOR_RESET, estilos, menu->titulo);
        
        lista_iterador_t *iterador = lista_iterador_crear(menu->opciones);
        if(iterador == NULL) {
                printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "Hubo un error con iterador del menu!" ANSI_COLOR_RESET "\n");
                return 0;
        }

        
        opcion_t *opcion_actual = lista_iterador_obtener_actual(iterador);
        
        while(opcion_actual != NULL) {
                printf("%s- %c: %s\n",
                        estilos,
                        opcion_actual->tecla,
                        opcion_actual->texto);

                lista_iterador_siguiente(iterador);                
                opcion_actual = lista_iterador_obtener_actual(iterador);
        }
        
        printf(ANSI_BG_RESET ANSI_COLOR_RESET "\n");
        lista_iterador_destruir(iterador);

        fgets(input, 2, stdin);
        while((limpiador = getchar()) != '\n' && limpiador != EOF);
        printf(ANSI_RETURN_SCREEN);

        if(input[0] == '\n')
                return 0;
        
        return input[0];
}

bool menu_borrar_opcion(menu_t *menu, char tecla_opcion_a_borrar)
{
        if(menu == NULL)
                return false;

        lista_iterador_t *iterador = lista_iterador_crear(menu->opciones);
        if(iterador == NULL)
                return false;

        opcion_t *opcion_actual = lista_iterador_obtener_actual(iterador);
        size_t i;
        size_t cant_opciones = lista_cantidad(menu->opciones);

        for(i = 0; i < cant_opciones && opcion_actual != NULL; i++) {
                if(opcion_actual->tecla == tecla_opcion_a_borrar) {
                        liberar_opcion(lista_eliminar_elemento(menu->opciones, i));
                        lista_iterador_destruir(iterador);
                        return true;
                } else {
                        lista_iterador_siguiente(iterador);
                        opcion_actual = lista_iterador_obtener_actual(iterador);
                }
        }

        lista_iterador_destruir(iterador);
        return false;
}

bool menu_modificar_texto_opcion(menu_t *menu, char tecla_opcion, const char *nuevo_texto)
{
        if(menu == NULL || nuevo_texto == NULL)
                return false;

        lista_iterador_t *iterador = lista_iterador_crear(menu->opciones);
        if(iterador == NULL)
                return false;

        opcion_t *opcion_actual = lista_iterador_obtener_actual(iterador);
        size_t i;
        size_t cant_opciones = lista_cantidad(menu->opciones);

        for(i = 0; i < cant_opciones && opcion_actual != NULL; i++) {
                if(opcion_actual->tecla == tecla_opcion) {
                        char *opcion_nuevo_texto = copiar_string(nuevo_texto);
                        if(opcion_nuevo_texto == NULL) {
                                lista_iterador_destruir(iterador);
                                return false;
                        }

                        free(opcion_actual->texto);
                        opcion_actual->texto = opcion_nuevo_texto;
                        lista_iterador_destruir(iterador);
                        return true;
                } else {
                        lista_iterador_siguiente(iterador);
                        opcion_actual = lista_iterador_obtener_actual(iterador);
                }
        }

        lista_iterador_destruir(iterador);
        return false;
}

size_t menu_cantidad_opciones(menu_t *menu)
{
        if(menu == NULL)
                return 0;

        return lista_cantidad(menu->opciones);
}

void destruir_menu(menu_t *menu)
{
        if(menu == NULL)
                return;

        if(menu->estilos != NULL)
                free(menu->estilos);

        if(menu->titulo != NULL)
                free(menu->titulo);

        lista_destruir_todo(menu->opciones, liberar_opcion);
        free(menu);
}