#include "master.h"

int puerto_yama = 2323;
int INICIAR_TRANSFORMACION = 0010;
int s_yama;


int main(int argc, char **argv) {
	int i;
	sockets_workers = dictionary_create();


	levantar_logger();

	levantar_config();

	if(levantar_servidor_workers() == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	leer_variables_args(argv);

	//usar cuando haya que implementar el envio de scripts a workers
	//abrir_file_args(argc, argv);

	if(conectar_con_yama() == EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	comenzar_job();

	transformacion();

	//TODO se envia informacion de transformacion a master

	//avisa cada vez que termina una transformacion a YAMA
	avisar_fin_tranformacion();


	desconectarse_de_yama();

	log_info(logger,"Salir del programa");

	//conectar con YAMA
	conectarse_yama();

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


int abrir_file_args(int argc, char** argv) {
	char* ruta_file;
	char* linea = NULL;
	size_t len= 0;
	ssize_t leido;
	FILE* archivo;

	//abrir file y loggear lineas
	ruta_file = string_from_format(ruta_archivo_job_inicial);
	log_info(logger, "ruta file: %s", ruta_file);

	archivo = fopen(ruta_file, "r");
	if(archivo == NULL) {
		log_error(logger, "file %s no existe", ruta_file);
		return EXIT_FAILURE;
	}

	while((leido = getline(&linea, &len, archivo)) != -1) {
		printf("largo de linea %d \n", leido);
		printf("linea: %s \n", linea);
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

void iniciar_transformacion() {
	int i;
	int proto_recibido;
	int socket_worker;
	char* char_bloque_recibido;
	t_reg_planificacion* worker_recibido;
	proto_recibido = recibirProtocolo(socket_yama);


	//me fijo si es el de transformacion que esperaba
	switch(proto_recibido) {
	case YM_MS_TRANSFORMACION:
		cantidad_bloques = atoi(esperarMensaje(socket_yama));
		for(i=0; i < cantidad_bloques; i++) {
			char_bloque_recibido = esperarMensaje(socket_yama);
			printf("worker: %s \n", char_bloque_recibido);
			worker_recibido = reg_planificacion_from_string(char_bloque_recibido);
			//todo aca se manda la info de transformacion al worker indicado
			socket_worker = conectar(config_get_string_value(config,"PUERTO_WORKER"),worker_recibido->ip);

			if(socket_worker){
				dictionary_put(sockets_workers,int_to_string(worker_recibido->worker),(void*)socket_worker);
				enviarMensajeConProtocolo(socket_worker,char_bloque_recibido,INICIO_TRANF_WORKER);
				//cargar nuevo thread para esperar la respuesta del worker
			}

			int i;
			log_info(logger,"para el worker: %d",worker_recibido->worker);
			for (i=0; i< list_size(worker_recibido->bloquesAsignados); i++) {
				int bloque_archivo = (int) list_get(worker_recibido->bloquesAsignados,i);
				log_info(logger,"bloque de archivo: %d",bloque_archivo);
			}
			//vale la pena guardar en una lista todos los t_reg_planificacion??
			list_add(&list_bloques, (void*) worker_recibido);
		}
		break;
	}
}


//esta seria la funcion del thread que se abre con el worker en la transformacion
void tranformacion(int worker) {
	int socket_esperado = dictionary_get(sockets_workers,int_to_string(worker));
	t_resp_master* respuesta;
	respuesta->etapa = TRANSFORMACION;
	respuesta->worker = worker;

	char* protocolo = esperarMensaje(socket_esperado);
	switch(protocolo){
		case TRANSFORMACION_OK:
			//enviar respuesta serializada en un t_respuesta_master
			respuesta->estado = 1;
			enviarMensaje(socket_yama,respuesta_master_to_string(respuesta));
		break;
		case TRANSFORMACION_ERROR:
			respuesta->estado = 0;
			enviarMensaje(socket_yama,respuesta_master_to_string(respuesta));
	}

}


void desconectarse_de_yama(){
	log_info(logger,"desconectandose de proceso YAMA");

	enviarProtocolo(socket_yama,MS_YM_DESCONECTAR);
}






