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
#include <sys/types.h>
#include <commons/log.h>
#include <commons/config.h>


//---------logger----------
t_log* abrir_logger(char* ruta_archivo, char* nombre_programa,t_log_level nivel);


#endif /* LIB_H_ */
