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

#define nombre_programa "MASTER"

int puerto_yama,
	socket_yama;

char* ip_yama;
char* ruta_transformador;
char* ruta_reductor;
char* ruta_archivo_job_inicial;
char* ruta_archivo_job_resultado;




//-------config------------

char* ruta_config = "/home/utnso/workspace/tp-2017-2c-LaYamaQueLlama/master/resources/config.cfg";
t_config* config;

//-------logger------------

char* ruta_log = "/home/utnso/workspace/tp-2017-2c-LaYamaQueLlama/master/resources/logger.log";
t_log_level level=LOG_LEVEL_INFO;
t_log* logger;


//----------otros---------------------
char* AUTH;

void levantar_config(void);
void levantar_logger(void);
void leer_variables_args(char** argv);
t_log* start_logger();
int abrir_file_args(int argc, char** argv);
int conectar_con_yama();
void comenzar_job();
void desconectarse_de_yama();


#endif /* MASTER_H_ */
