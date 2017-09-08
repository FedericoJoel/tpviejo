/*
 ============================================================================
 Name        : master.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>

int main(void) {
	//lista de nodos que posee
	t_log* logger;
	t_log_level level=LOG_LEVEL_INFO;

	logger = log_create("master_log","MASTER",1,level);

	log_info(logger, "starting...");

	return EXIT_SUCCESS;
}
