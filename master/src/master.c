#include "master.h"

int PUERTO_YAMA=3600;
//IP INVENTADA
char* IP_YAMA="172.0.0.3";
int s_yama;


int main(int argc, char **argv) {

	//arrancar logger
	logger = start_logger();
	log_info(logger, "starting...");
	abrir_file_args(argc, argv);

	//conectar con YAMA
	conectarse_yama();

	return EXIT_SUCCESS;
}

void conectarse_yama(){
	char * IP = string_new();
	string_append(&IP, "127.0.0.2");
	log_trace(logger, "Intento conectarme a YAMA\n");
	s_yama = conectar(PUERTO_YAMA,IP);

	if(s_yama){
			log_trace(logger, "No se pudo conectar con yama. YAMA: %s:%d\n", IP_YAMA, PUERTO_YAMA);
		}
		log_trace(logger, "Conectado con YAMA OK");

		char* mensaje = esperarMensaje(s_yama);

		//envio el mensaje
		enviarMensaje(s_yama, "Hola");
		//prueba para comprobar el comunicacion
		if(mensaje == '\0'){
			log_trace(logger, "no se pudo enviar mensaje a YAMA");
		}
		printf("Conectado a fs con socket %d \n",s_yama);
}

t_log* start_logger() {
	logger = log_create("master_log","MASTER",1,level);
	return logger;
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
		log_info(logger, "file %s no existe", ruta_file);
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
