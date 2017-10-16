#include "master.h"


int puerto_yama = 2323;
int INICIAR_TRANSFORMACION = 0010;
int s_yama;


int main(int argc, char **argv) {
	int i;


	levantar_logger();

	levantar_config();

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

void transformacion() {
	int i;
	int proto_recibido;
	char* char_bloque_recibido;
	t_bloque_archivo* bloque_recibido;
	proto_recibido = recibirProtocolo(socket_yama);

	//me fijo si es el de transformacion que esperaba
	switch(proto_recibido) {
	case YM_MS_TRANSFORMACION:
		cantidad_bloques = atoi(esperarMensaje(socket_yama));
		for(i=0; i < cantidad_bloques; i++) {
			char_bloque_recibido = esperarMensaje(socket_yama);
			printf("bloque: %s \n", char_bloque_recibido);
			bloque_recibido = bloque_archivo_from_string(char_bloque_recibido);
			printf("bloque ip: %s\n",bloque_recibido->copia0->ip);
			list_add(&list_bloques, (void*) bloque_recibido);
		}

		//TODO enviar lista a master
		break;
	}
}

void avisar_fin_tranformacion() {
	int i;

	for(i=0;i < cantidad_bloques; i++) {
		//TODO esperar que terminen todas las transformaciones en workers, por ahora se mockea los fin de transformacion
	}

	void _iterate_bloques(t_bloque_archivo* bloque){
		char* char_bloque =int_to_string(bloque->bloque_archivo);
		enviarMensajeConProtocolo(socket_yama,char_bloque,FIN_TRANSF_NODO);
	}

	list_iterate(&list_bloques,(void*) _iterate_bloques);

	//aviso que terminaro todos
	enviarProtocolo(socket_yama,FIN_TRANSFORMACION);
}


void desconectarse_de_yama(){
	log_info(logger,"desconectandose de proceso YAMA");

	enviarProtocolo(socket_yama,MS_YM_DESCONECTAR);
}






