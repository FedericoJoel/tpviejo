#include "master.h"

int main(int argc, char **argv) {

	//arrancar logger
	logger = start_logger();
	log_info(logger, "starting...");

	abrir_file_args(argc, argv);

	return EXIT_SUCCESS;
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
