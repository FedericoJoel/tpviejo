#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>

//ARCHIVO//

typedef struct{
	char* nombre;
	int bloque;
	char* nombreCopia;
	int bloqueCopia;
	int tamanioBloque;
}estructuraBloque;

typedef struct{
	estructuraBloque bloqueCompuesto;
	struct listaBloqueArchivo * sig;
}listaBloqueArchivo;

typedef struct{
	int tamanio;
	char* tipo;
	listaBloqueArchivo bloques;
}t_archivo;



t_archivo leerArchivo();

#endif /* ARCHIVO_H_ */
