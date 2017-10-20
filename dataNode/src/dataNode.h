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
int escribir(char* mensaje, int offset);
int map_data();

#endif /* DATANODE_H_ */
