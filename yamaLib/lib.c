#include "lib.h"  /* Include the header (not strictly necessary here) */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <commons/log.h>
#include <commons/string.h>

void saludar()    /* Function definition */
{
    puts("hola");
}

int ejecutarRutina()
{
	puts("Se ejecuto la rutina \n");
	return 0;
}

char* sacar(char* palabra, char* caracter) {
	char** palabraN = string_split(palabra, caracter);
	return palabraN[0];
}

int cantidadElementos(char ** array){
    size_t count = 0;
    while (array[count] != NULL) count++;
    return count;
}
