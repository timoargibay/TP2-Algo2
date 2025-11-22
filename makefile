VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -Wtrampolines -O2 -g
CC = gcc

compilar_todo: src/*.c tp2.c
	gcc $(CFLAGS) -c src/menu.c -o archivos_objeto/menu.o
	gcc $(CFLAGS) -c src/extras_tp1.c -o archivos_objeto/extras_tp1.o
	gcc $(CFLAGS) -c src/tp1.c -o archivos_objeto/tp1.o
	gcc $(CFLAGS) src/*.c pruebas_alumno.c -o pruebas_alumno
	gcc $(CFLAGS) src/*.c tp2.c -o tp2

pruebas_menu: src/*.c prueba_menu.c
	$(CC) $(CFLAGS) src/*.c prueba_menu.c -o prueba_menu
	valgrind $(VALGRIND_FLAGS) ./prueba_menu

pruebas_alumno: src/*.c pruebas_alumno.c
	$(CC) $(CFLAGS) src/*.c pruebas_alumno.c -o pruebas_alumno

valgrind-pruebas: compilar_todo
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

format:
	clang-format -i -style=file src/*.c
	clang-format -i -style=file src/*.h
	clang-format -i -style=file pruebas_alumno.c
	clang-format -i -style=file tp2.c
	clang-format -i -style=file prueba_menu.c

preparar_entrega: compilar_todo format
	rm -f tp2
	rm -f pruebas_alumno
	rm -f prueba_menu
	rm -f archivos_objeto/*.o
