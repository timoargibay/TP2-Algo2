#include "src/menu.h"
#include "src/tp1.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char *argumento[])
{
        tp1_t *pokemons_1 = NULL;
        if (argc >= 1)
                pokemons_1 = tp1_leer_archivo(argumento[1]);
	else
                pokemons_1 = tp1_leer_archivo("pokedex.csv"); 

        if (pokemons_1 == NULL)
		        return 1;

        
}
