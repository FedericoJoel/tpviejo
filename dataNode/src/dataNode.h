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


/////////////
//PROTOCOLO//
/////////////
///FS - DN///
#define DN_SETBLOQUE 0001
#define DN_SETBLOQUEANSW 2
#define DN_GETBLOQUE 3
#define DN_GETBLOQUEANSW 4
//FS - YAMA//
#define FS_READ 5
#define FS_STORE 6
#define YM_READANSW 7
#define YM_STOREANSW 8

void escuchar_fs();
void set_bloque();
void conectarse_fs();

#endif /* DATANODE_H_ */
