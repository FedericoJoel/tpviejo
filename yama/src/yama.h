/*
 * yama.h
 *
 *  Created on: 10/9/2017
 *      Author: utnso
 */

#ifndef YAMA_H_
#define YAMA_H_

#include <lib.h>
#include <sockets.h>

#define nombre_programa "YAMA"



char* ruta_config = "/home/utnso/workspace/tp-2017-2c-LaYamaQueLlama/yama/resources/config.cfg";
t_config* config;

char* ruta_log = "/home/utnso/workspace/tp-2017-2c-LaYamaQueLlama/yama/resources/logger.log";
t_log_level level=LOG_LEVEL_INFO;
t_log* logger;


void levantar_config(void);
void levantar_logger(void);

#endif /* YAMA_H_ */
