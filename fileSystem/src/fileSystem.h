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
#include <commons/collections/list.h>
#include <lib.h>
#include <stdint.h>
#include <pthread.h>
#include <sockets.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <protocolos.h>
#include <string.h>

typedef struct nodo{
	int id;
	char* ip;
	int socket;
	int bloque_cant;
}t_nodo;

//CANTIDAD MAXIMA DE CLIENTES CONCURRENTES
#define clientes_max 10


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

t_list* cortar_texto(char* mensaje, t_list* lista);
int size_in_bloks(char* mensaje);

void ejecutarConsola();
int str_array_size(char** array);
void str_array_print(char ** array);
void esperar_conexiones();
void iniciar_servidor();
void atender_dn();

int set_bloque(int s_nodo,char* datos,int bloque);
char* get_bloque(int s_nodo, int bloque);
int keep_alive(int socket);
t_list * cortar_datos(int numeroDePalabra,char** palabras, t_list * bloques);

int leer_cliente(int s_nodo, char * buffer);

#endif /* FILESYSTEM_H_ */
