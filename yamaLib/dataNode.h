#ifndef NODO_H_
#define NODO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <stdint.h>

//#include "util.h"

#define BLOQUE_CANT_COPIAS 2

#define TAMANIO_BLOQUE_MB 20
#define TAMANIO_BLOQUE_B  (1024 * 1024 * 20) //20mb


typedef struct { //estructura que tiene las dos copias del bloque
	int parte_numero; //numero de bloque
	int nodosbloque[1]; //tiene dos estructuras t_archivo_nodo_bloque
}structArchivoBloqueConCopias;


typedef struct {
	uint32_t posicion;
	bool libre;
	bool requerido_para_copia;//cuando pido el bloque para copiar pero todavia no esta confirmado el cambio, todavia sigue libre
}structBloque;

typedef struct {
	int cant_bloques;
	bool conectado;
	bool esNuevo;
//	lista de structBloque (descomposicion de archivo)
} structNodo;


structArchivoBloqueConCopias* bloqueDeDatosCrear();

bool bloqueEstaUsado(structBloque* bloque);

#endif /* NODO_H_ */
