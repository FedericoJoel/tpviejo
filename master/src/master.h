/*
 * master.h
 *
 *  Created on: 13/9/2017
 *      Author: utnso
 */

#ifndef MASTER_H_
#define MASTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <lib.h>
#include <sockets.h>
#include <protocolos.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <sockets.h>

#define nombre_programa "MASTER"

int puerto_yama,
	socket_yama,
	cantidad_bloques,
	socket_server;
int finalizado = 0;

char* ip_yama;
char* ruta_transformador;
char* ruta_reductor;
char* ruta_archivo_job_inicial;
char* ruta_archivo_job_resultado;
char* AUTH;
t_dictionary* sockets_workers;
t_list list_bloques;
t_list list_bloques_global;

//threads
t_list threads_transformacion;
t_list threads_reduc_local;
pthread_t thread_replanificacion;
pthread_t thread_reduc_global;


//-------config------------

char* ruta_config = "/home/utnso/workspace/tp-2017-2c-LaYamaQueLlama/master/resources/config.cfg";
t_config* config;

//-------logger------------

char* ruta_log = "/home/utnso/workspace/tp-2017-2c-LaYamaQueLlama/master/resources/logger.log";
t_log_level level=LOG_LEVEL_INFO;
t_log* logger;


//----------otros---------------------
void levantar_config(void);
void levantar_logger(void);
int levantar_servidor_masters(void);
t_log* start_logger();
int abrir_file_args(int argc, char** argv);
int conectar_con_yama();
void comenzar_job();
void esperar_indicaciones();
void transformacion();
void replanificacion();
void reduccion_local();
void reduccion_global();

//funciones que corren en threads
void transformacion_nodo(void* planificacion);
void replanificacion_nodo(void* planificacion_nodo);
void reduccion_global_nodo(void* lista_nodos);
void reduccion_local_nodo(void* planificacion);

void desconectarse_de_yama();



#endif /* MASTER_H_ */
