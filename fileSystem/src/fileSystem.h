#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/temporal.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "directorio.h"
#include "archivo.h"
#include "nodo.h"
#include <lib.h>
#include <stdint.h>
#include <pthread.h>
#include <sockets.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <protocolos.h>
#include <string.h>
//CANTIDAD MAXIMA DE CLIENTES CONCURRENTES
#define clientes_max 10


typedef struct nodo{
	int id;
	char* ip;
	int socket;
	int bloque_cant;
}t_nodo;



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


//DEVUELVE LA POSICION DE UN NODO SEGUN EL NOMBRE
int encontrarPosicion(t_list* fs, char* nombre);

//AGREGA UN NODO A LA ESTRUCTURA FS
void agregarNodoAFs(estructuraFs* fs, int id, int bloquesLibres, int bloquesTotales);

//ELIMINA UN NODO DE LA ESTRUCTURA FS
void eliminarNodoDeFs(estructuraFs* fs, int id);

//MODIFICA LOS BLOQUES LIBRE DE UN NODO QUE YA FORMA PARTE DEL FS
void modificarNodoDeFs(estructuraFs* fs, int id,int bloquesOcupados);

//CARGA LOS NODOS EXISTENTES EN EL ARCHIO AL FS
void cargarNodosAFs(estructuraFs* fs);

t_list* cortar_texto(char* mensaje, t_list* lista);
int size_in_bloks(char* mensaje);

void ejecutarConsola();
void str_array_print(char ** array);
void esperar_conexiones();
void iniciar_servidor();
void atender_dn();

int set_bloque(int s_nodo,char* datos,int bloque);
char* get_bloque(int s_nodo, int bloque);
int keep_alive(int socket);
t_list * cortar_datos(int numeroDePalabra,char** palabras, t_list * bloques);

int leer_cliente(int s_nodo, char * buffer);
int existenArchivosDeConfiguracion();
void fs_format();

//GUARDA LA ESTRUCTURA DEL FS DE MEMORIA EN UN ARCHIVO
void guardarFsEnArchivo();

//ELIMINA TODOS LOS NODOS DE LA ESTRUCTURA FS
void eliminarNodos(estructuraFs* fs);

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
char* obtener_ruta_metadata(char* ruta);
#endif /* FILESYSTEM_H_ */
