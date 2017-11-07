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
	int elegida;
}t_bloque_archivo;

typedef struct estado {
	int job;
	int master;
	int nodo;
	int bloque;
	int etapa;
	char* temporal;
	int estado;
}t_estado;

typedef struct reg_planificacion{
	int availability;
	int worker;
	t_list *bloques;
	t_list *bloquesAsignados;
	int job;
	char *temporalTransformacion;
	char *tempReducLocal;
	char *tempReudcGlobal;
	char* ip;
}t_reg_planificacion;

typedef struct respuesta_master{
	int job;
	int worker;
	int estado;
}t_resp_master;

//---------logger----------
t_log* abrir_logger(char* ruta_archivo, char* nombre_programa,t_log_level nivel);

//--------Manejo de strings----------
void string_append_separador(char** original, char* string_to_add,char* separador);
char* generar_string_separador2(char* str1, char* str2, char* separador);

//------parseos-------------
char* int_to_string(int numero);
char* extraer_string(char* string, int inicio, int fin);

t_copia* copia_from_string(char* copia);
t_bloque_archivo* bloque_archivo_from_string(char* char_bloque);
t_reg_planificacion* reg_planificacion_from_string(char* char_reg_planificacion);
t_list* iterate_bloques_from_string(char* bloques, int tamanio);

char* reg_planificacion_to_string( t_reg_planificacion* data_worker);
char* copia_to_string(t_copia* copia);
char* bloque_archivo_to_string(t_bloque_archivo* bloque);
char* iterate_bloques_to_string(t_list* bloques);

#endif /* LIB_H_ */
