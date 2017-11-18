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
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <sockets.h>

#define nombre_programa "MASTER"

int puerto_yama,
	socket_yama,
	cantidad_bloques;

char* ip_yama;
char* ruta_transformador;
char* ruta_reductor;
char* ruta_archivo_job_inicial;
char* ruta_archivo_job_resultado;
t_list list_bloques;




//-------config------------

char* ruta_config = "/home/utnso/workspace/tp-2017-2c-LaYamaQueLlama/master/resources/config.cfg";
t_config* config;

//-------logger------------

char* ruta_log = "/home/utnso/workspace/tp-2017-2c-LaYamaQueLlama/master/resources/logger.log";
t_log_level level=LOG_LEVEL_INFO;
t_log* logger;


//----------otros---------------------
char* AUTH;
int socket_server;


t_dictionary* sockets_workers;
void levantar_config(void);
void levantar_logger(void);
int levantar_servidor_masters(void);
void leer_variables_args(char** argv);
t_log* start_logger();
int abrir_file_args(int argc, char** argv);
int conectar_con_yama();
void comenzar_job();
void iniciar_transformacion();
void avisar_fin_tranformacion();
void reduccion_local();
void desconectarse_de_yama();



#endif /* MASTER_H_ */
