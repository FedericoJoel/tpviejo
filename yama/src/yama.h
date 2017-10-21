/*
 * yama.h
 *
 *  Created on: 10/9/2017
 *      Author: utnso
 */

#ifndef YAMA_H_
#define YAMA_H_

#include <lib.h>
#include <sockets.h>
#include <commons/collections/list.h>
#include <commons/string.h>


#define nombre_programa "YAMA"
#define clientes_max 10
#define ETAPA_TRANSFORMACION 0
#define ETAPA_REDUCCION_LOCAL 1
#define ETAPA_REDUCCION_GLOBAL 2

#define ESTADO_EN_PROCESO 0
#define ESTADO_FINALIZADO_OK 1
#define ESTADO_ERROR 2



t_list * archivoPrueba2();





//-------config------------

char* ruta_config = "/home/utnso/workspace/tp-2017-2c-LaYamaQueLlama/yama/resources/config.cfg";
t_config* config;

//-------logger------------

char* ruta_log = "/home/utnso/workspace/tp-2017-2c-LaYamaQueLlama/yama/resources/logger.log";
t_log_level level=LOG_LEVEL_INFO;
t_log* logger;


//--------server------------
int socket_server;
int socket_clientes[clientes_max];
int cantidad_masters_conectados = 0;
fd_set fds_masters;
t_proto protocolo;

int levantar_servidor(void);
void construir_fds(int* max_actual,int conectados[]);
void leer_cambios_select();
void recibir_nuevo_master();
void recibir_data_de_master(int posicion);

void atender_inicio_job(int posicion);
void atender_fin_transf_nodo(int posicion);
void atender_fin_transformacion(int posicion);

void enviar_transformacion(int master, t_list* lista_bloques);
void enviar_reduccion_local(int posicion);

t_list * tablaPlanificacionCompleta();
t_list * tablaPlanif();
void transformarBloques(t_list * tabla);
void modificarBloqueTablaEstados(int bloque, int etapa, int estado, int worker, int job);
int cargaTrabajoMaxima(int job);
int cargaTrabajoWorker(int worker);
void enviarAMaster(t_list * tabla);

//----------otros---------------------
char* AUTH;

void levantar_config(void);
void levantar_logger(void);
void esperar_master(void);
void esperar_masters(void);

#endif /* YAMA_H_ */
