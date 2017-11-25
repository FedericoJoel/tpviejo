#include "master.h"

int puerto_yama = 2323;
int INICIAR_TRANSFORMACION = 0010;
int s_yama;


int main(int argc, char **argv) {
	sockets_workers = dictionary_create();

	levantar_logger();

	levantar_config();

	if(conectar_con_yama() == EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	comenzar_job();

	esperar_indicaciones();

	desconectarse_de_yama();

	log_info(logger,"Salir del programa");

	return EXIT_SUCCESS;
}

void levantar_config(void) {
	log_info(logger,"Levantando configuracion");
	config = config_create(ruta_config);
	log_info(logger,"Configuracion levantada");

	puerto_yama = config_get_int_value(config,"PUERTO_YAMA");
	ip_yama = config_get_string_value(config,"IP_YAMA");
}


void levantar_logger(void) {
	logger = abrir_logger(ruta_log,nombre_programa,LOG_LEVEL_INFO);
	log_info(logger,"Logger iniciado");


}

void leer_variables_args(char** argv) {
	ruta_transformador = argv[1];
	log_info(logger,"ruta_transformador = %s",ruta_transformador);
	ruta_reductor = argv[2];
	log_info(logger,"ruta_reductor = %s",ruta_reductor);
	ruta_archivo_job_inicial = argv[3];
	log_info(logger,"ruta_archivo_job_inicial= %s",ruta_archivo_job_inicial);
	ruta_archivo_job_resultado = argv[4];
	log_info(logger,"ruta_archivo_job_resultado= %s",ruta_archivo_job_resultado);

}

int levantar_servidor_masters() {
	log_info(logger, "levantando servidor MASTER");
	socket_server = crearServidor(config_get_int_value(config, "PUERTO_MASTER"));
	if (socket_server < 0) {
		log_error(logger, "No se pudo levantar el servidor");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


int get_char_archivo(char* ruta, char** archivo_serializado) {
	char* linea = NULL;
	size_t len= 0;
	ssize_t leido;
	FILE* archivo;

	//abrir file y loggear lineas
	log_info(logger, "abriendo file: %s", ruta);

	archivo = fopen(ruta, "r");
	if(archivo == NULL) {
		log_error(logger, "file %s no existe", ruta);
		return EXIT_FAILURE;
	}

	while((leido = getline(&linea, &len, archivo)) != -1) {
		string_append(archivo_serializado,linea);
	}

	fclose(archivo);
	if(linea){
		free(linea);
	}
	return EXIT_SUCCESS;
}

int conectar_con_yama(){
	int proto_recibido;

	log_info(logger,"conectando con proceso YAMA");
	socket_yama = conectar(puerto_yama,ip_yama);

	proto_recibido = recibirProtocolo(socket_yama);

	if(proto_recibido == YM_MS_ERRORCONN) {
		log_error(logger,"no se pudo conectar a yama. Reintentar mas tarde");
		return EXIT_FAILURE;
	}
	else log_info(logger,"conexion establecida con YAMA");
	return EXIT_SUCCESS;
}

void comenzar_job() {
	log_info(logger,"enviando job a YAMA");
	//envio protocolo y mensaje con la ruta del archivo en el yamafs
	enviarMensajeConProtocolo(socket_yama,ruta_archivo_job_inicial,MS_YM_INICIO_JOB);
}

void esperar_indicaciones() {
	int proto_recibido;

	while(finalizado == 0){
		//recibo algun protocolo de yama
		proto_recibido = recibirProtocolo(socket_yama);

		switch(proto_recibido) {
			case YM_MS_TRANSFORMACION://inicio transformacion
				transformacion();
			break;
			case REPLANIFICACION: //se replanifico transformacion de nodo
				replanificacion();
			break;
			case REDUC_LOCAL:
				reduccion_local();
			break;
			case REDUC_GLOBAL:
				reduccion_global();
			break;
		}
	}

}

void transformacion() {
	int i;
	char* char_bloque_recibido;
	pthread_t* thread_transformacion;

	//recibo cantidad de bloques que tengo que usar
	cantidad_bloques = atoi(esperarMensaje(socket_yama));

	for(i=0; i < cantidad_bloques; i++) {
		//recibo bloques, los guardo en lista y creo thread
		char_bloque_recibido = esperarMensaje(socket_yama);

		list_add(&list_bloques, (void*) char_bloque_recibido);
		thread_transformacion = (pthread_t *) malloc(sizeof(pthread_t));
		list_add(&threads_transformacion,(void*) thread_transformacion);

		//aca se manda la info de transformacion al worker indicado en un nuevo hilo
		pthread_create(thread_transformacion,NULL,(void*)&transformacion_nodo, (void*) char_bloque_recibido);
	}
}

void replanificacion() {
	//si hay replanificacion hay que matar a todos los threads de transformacion que podrian estar corriendo
	char* char_bloque_recibido;

	//espero data del nodo replanificado
	char_bloque_recibido = esperarMensaje(socket_yama);
	pthread_create(&thread_replanificacion,NULL,(void*)&replanificacion_nodo, (void*) char_bloque_recibido);
}

void reduccion_local() {
	char* char_bloque_recibido;
	pthread_t* thread_reduc_local;

	//espero data del nodo a reducir
	char_bloque_recibido = esperarMensaje(socket_yama);
	thread_reduc_local = (pthread_t *) malloc(sizeof(pthread_t));
	list_add(&threads_reduc_local,(void*) thread_reduc_local);

	pthread_create(thread_reduc_local,NULL,(void*)&reduccion_local_nodo, (void*) char_bloque_recibido);
}

void reduccion_global() {
	char* char_bloque_recibido;
	int i;

	pthread_t* thread_transformacion;

		//recibo cantidad de bloques que tengo que usar
		cantidad_bloques = atoi(esperarMensaje(socket_yama));

		for(i=0; i < cantidad_bloques; i++) {
			char_bloque_recibido = esperarMensaje(socket_yama);
			list_add(&list_bloques_global, (void*) char_bloque_recibido);
		}
		thread_reduc_global = (pthread_t *) malloc(sizeof(pthread_t));
		pthread_create(thread_reduc_global,NULL,(void*)&reduccion_global_nodo, (void*) &list_bloques_global);
}

//esta seria la funcion del thread que se abre con el worker en la transformacion
void transformacion_nodo(void* void_worker) {
	int i;
	int bloque_archivo;
	t_resp_master* respuesta;
	t_reg_planificacion* worker = reg_planificacion_from_string((char*) void_worker);
	int socket_worker = conectar(config_get_int_value(config,"PUERTO_WORKER"),worker->ip);

	if(socket_worker){
		//guardo en diccionario worker -> socket
		dictionary_put(sockets_workers,int_to_string(worker->worker),(void*)socket_worker);

		//envio la info al worker
		t_planificacion_worker* planificacion_worker = malloc(sizeof(t_planificacion_worker));
		planificacion_worker->planificacion = worker;

		planificacion_worker->programa_planificacion = string_new();
		get_char_archivo(ruta_transformador,&planificacion_worker->programa_planificacion);

		char* char_planificacion_worker = planificacion_worker_to_string(planificacion_worker);
		enviarMensajeConProtocolo(socket_worker,char_planificacion_worker,INICIO_TRANF_WORKER);

		//espero informacion de la finalizacion de las transformaciones
		respuesta = malloc(sizeof(t_resp_master));
		respuesta->etapa = TRANSFORMACION;
		respuesta->worker = worker->worker;

		for(i=0; i < list_size(worker->bloquesAsignados); i++) {

			int protocolo = recibirProtocolo(socket_worker);

			switch(protocolo){ //dependiendo del protocolo termino bien o mal la transformacion en el worker
				case TRANSFORMACION_OK:
					respuesta->estado = 1;
					bloque_archivo = (int) esperarMensaje(socket_worker);
					respuesta->bloque_archivo = bloque_archivo;
					enviarMensajeConProtocolo(socket_yama,respuesta_master_to_string(respuesta),FIN_TRANSF_WORKER);
				break;
				case TRANSFORMACION_ERROR:
					respuesta->estado = 0;
					bloque_archivo = (int) esperarMensaje(socket_worker);
					respuesta->bloque_archivo = bloque_archivo;
					enviarMensajeConProtocolo(socket_yama,respuesta_master_to_string(respuesta),FIN_TRANSF_WORKER);
				break;
			}
		}
	}
}


void reduccion_local_nodo(void* void_worker) {
	t_resp_master* respuesta;
	t_reg_planificacion* worker = reg_planificacion_from_string((char*) void_worker);

	int socket_worker = conectar(config_get_int_value(config,"PUERTO_WORKER"),worker->ip);

	if(socket_worker){
		t_planificacion_worker* planificacion_worker = malloc(sizeof(t_planificacion_worker));
		planificacion_worker->planificacion = worker;

		planificacion_worker->programa_planificacion = string_new();
		get_char_archivo(ruta_reductor,&planificacion_worker->programa_planificacion);

		char* char_planificacion_worker = planificacion_worker_to_string(planificacion_worker);
		enviarMensajeConProtocolo(socket_worker,char_planificacion_worker,INICIO_REDUC_LOCAL_WORKER);

		//espero informacion de la finalizacion de la reduccion
		respuesta = malloc(sizeof(t_resp_master));
		respuesta->etapa = REDUC_LOCAL;
		respuesta->worker = worker->worker;

		int protocolo = recibirProtocolo(socket_worker);

		switch(protocolo){ //dependiendo del protocolo termino bien o mal la reduccion en el worker
			case REDUC_LOCAL_OK:
				respuesta->estado = 1;
				respuesta->bloque_archivo = -1; //TODO no necesito el bloque en esta instancia
				enviarMensajeConProtocolo(socket_yama,respuesta_master_to_string(respuesta),FIN_REDUC_LOCAL_WORKER);
			break;
			case REDUC_LOCAL_ERROR:
				respuesta->estado = 0;
				respuesta->bloque_archivo = -1;
				enviarMensajeConProtocolo(socket_yama,respuesta_master_to_string(respuesta),FIN_REDUC_LOCAL_WORKER);
			break;
		}

	}

}

void reduccion_global_nodo(void* void_workers) {
	t_resp_master* respuesta;
	t_list* list_workers= (t_list*) void_workers;

}

void replanificacion_nodo() {

}


void desconectarse_de_yama(){
	log_info(logger,"desconectandose de proceso YAMA");

	enviarProtocolo(socket_yama,MS_YM_DESCONECTAR);
}






