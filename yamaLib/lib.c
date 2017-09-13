#include "lib.h"  /* Include the header (not strictly necessary here) */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <commons/log.h>

void saludar()    /* Function definition */
{
    puts("hola");
}

int ejecutarRutina()
{
	puts("Se ejecuto la rutina \n");
	return 0;
}

int crearFork(){
	return fork();
}
