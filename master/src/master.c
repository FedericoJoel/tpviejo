#include "master.h"

int main(int argc, char **argv) {

	levantar_logger();

	levantar_config();

	//usar cuando haya que implementar el envio de scripts a workers
	//abrir_file_args(argc, argv);

	conectar_con_yama();

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



int abrir_file_args(int argc, char** argv) {
	int i;
	char* ruta_file;
	char* linea = NULL;
	size_t len= 0;
	ssize_t leido;
	FILE* archivo;

	//loggear argumentos
	for(i = 0; i < argc; ++i) {
		log_info(logger, "argumento %d: %s \n", i, argv[i]);
	}

	//abrir file y loggear lineas
	ruta_file = string_from_format(argv[1]);
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

void conectar_con_yama(){
	char* msg;
	int proto_recibido;

	log_info(logger,"conectando con proceso YAMA");
	socket_yama = conectar(puerto_yama,ip_yama);

	proto_recibido = recibirProtocolo(socket_yama);
	log_info(logger,"protocolo recibido: %d",proto_recibido);

	msg = esperarMensaje(socket_yama);
	log_info(logger,"mensaje recibido: %s",msg);

}

void desconectarse_de_yama(){
	log_info(logger,"desconectandose de proceso YAMA");

	enviarProtocolo(socket_yama,MS_YM_DESCONECTAR);
}
