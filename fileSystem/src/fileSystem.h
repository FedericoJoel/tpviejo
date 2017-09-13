#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/temporal.h>
#include <commons/string.h>
#include <commons/config.h>
#include <lib.h>
#include <stdint.h>
#include <pthread.h>
#include "sockets.h"

char ruta_config="./config.txt";

typedef struct{
	int id;
	char* ip;
	int puerto;
}t_nodo;

void fs_format();

void fs_rm(char * arg);

void fs_rename(char * arg);

void ejecutarConsola();

int str_array_size(char** array);

void str_array_print(char ** array);

#endif /* FILESYSTEM_H_ */
