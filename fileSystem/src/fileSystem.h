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
int str_array_size(char** array);
void str_array_print(char ** array);
void esperar_data_nodes();
void iniciar_servidor();
void atender_dn();
void set_bloque(int *s_nodo,char* datos);

#endif /* FILESYSTEM_H_ */
