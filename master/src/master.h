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
#include <commons/log.h>
#include <commons/string.h>
#include <sockets.h>

t_log_level level=LOG_LEVEL_INFO;
t_log* logger;

t_log* start_logger();
int abrir_file_args(int argc, char** argv);
void conectarse_yama();

#endif /* MASTER_H_ */
