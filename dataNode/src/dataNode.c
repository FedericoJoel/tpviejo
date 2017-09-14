/*
 ============================================================================
 Name        : dataNode.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "dataNode.h"

int PUERTO=3491;


int main(void) {
	char * IP = string_new();
	string_append(&IP, "127.0.0.1");
	printf("Intento conectarme a Filesystem\n");
	int s_filesystem = conectar(PUERTO,IP);
	printf("Conectado a fs con socket %d \n",s_filesystem);
	return EXIT_SUCCESS;
}
