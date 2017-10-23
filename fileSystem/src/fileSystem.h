#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/temporal.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <lib.h>
#include <stdint.h>
#include <pthread.h>
#include <sockets.h>
#include "directorio.h"
#include "archivo.h"
#include "nodo.h"

typedef struct{
	int tamanioTotalFs;
	int tamanioLibreFs;
	t_list* nodos;
}estructuraFs;

typedef struct{
	estructuraFs estructuraNodos;
	t_directory directorio[99];
	t_archivo tablaArchivo;
	t_bitarray* bitArray;
}t_fileSystem;

t_fileSystem fs;

int PUERTO_FS= 3490;
int s_servidor;
t_list *t_dataNodes;
t_list *s_dataNodes;
pthread_t* t_atiende_dn;
pthread_t t_espera_data_nodes;

void cargarNodos();
void posicion(t_bitarray* array, int pos);
void fs_format();
void fs_rm(char * arg);
void fs_rename(char * arg);
void fs_mv(char * arg);
void fs_cat(char * arg);
void fs_mkdir(char * arg);
void fs_cpfrom(char * arg);
void fs_cpto(char * arg);
void fs_cpblok(char * arg);
void fs_md5(char * arg);
void fs_ls(char * arg);
void fs_info(char * arg);
void ejecutarConsola();
void str_array_print(char ** array);
void esperar_data_nodes();
void iniciar_servidor();
void atender_dn();
void set_bloque(int *s_nodo,char* datos);
int existenArchivosDeConfiguracion();

#endif /* FILESYSTEM_H_ */
