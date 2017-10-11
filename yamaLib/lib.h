/*
 * lib.h
 *
 *  Created on: 2/9/2017
 *      Author: utnso
 */

#ifndef LIB_H_
#define LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>


//---------------structs filesystem-----------------

typedef struct copia{
	int nodo;
	int bloque_nodo;
	char* ip;
}t_copia;

typedef struct bloque_archivo{
	int bloque_archivo;
	int bytes;
	char* ruta_temporal;
	t_copia* copia0;
	t_copia* copia1;
	t_copia* elegida;
}t_bloque_archivo;


//---------logger----------
t_log* abrir_logger(char* ruta_archivo, char* nombre_programa,t_log_level nivel);


//------parseos-------------
char* int_to_string(int numero);
char* extraer_string(char* string, int inicio, int fin);
char* copia_to_string(t_copia* copia);
char* bloque_archivo_to_string(t_bloque_archivo* bloque);
t_bloque_archivo* bloque_archivo_from_string(char* char_bloque);


#endif /* LIB_H_ */
