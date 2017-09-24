#include "yama.h"

int main(void) {

	levantar_logger();

	levantar_config();

	log_info(logger,"Salir del programa");
	return EXIT_SUCCESS;
}


void levantar_config(void) {
	log_info(logger,"Levantando configuracion");
	config = config_create(ruta_config);
	log_info(logger,"Configuracion levantada");
}


void levantar_logger(void) {
	logger = abrir_logger(ruta_log,nombre_programa,LOG_LEVEL_INFO);
	log_info(logger,"Logger iniciado");
}
