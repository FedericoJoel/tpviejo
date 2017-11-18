/*
 * dataNode.h
 *
 *  Created on: 13/9/2017
 *      Author: utnso
 */

#ifndef DATANODE_H_
#define DATANODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sockets.h>
#include <pthread.h>
#include <commons/string.h>
#include <string.h>
#include <protocolos.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>


void escuchar_fs();
void set_bloque();
void get_bloque();
void error_protocolo();
void conectarse_fs();
void escribir(char* mensaje, int offset);
int map_data();
void escribir_bloque(char* mensaje, int bloque);
void keep_alive();
char* leer_bloque(int bloque);

#endif /* DATANODE_H_ */
