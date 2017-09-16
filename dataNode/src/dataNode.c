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

int PUERTO_FS=3490;
int s_filesystem;

int main(void) {

	conectarse_fs();
	while(1){
		escuchar_fs();
	}

	return EXIT_SUCCESS;
}

void conectarse_fs(){
	char * IP = string_new();
	string_append(&IP, "127.0.0.1");
	printf("Intento conectarme a Filesystem\n");
	s_filesystem = conectar(PUERTO_FS,IP);
	printf("Conectado a fs con socket %d \n",s_filesystem);
}

void set_bloque(){
	char* mensaje = esperarMensaje(s_filesystem);
	printf("escribo bloque con los datos %s \n",mensaje);
}

void escuchar_fs(){
	if(recibirProtocolo(s_filesystem) == DN_SETBLOQUE){
		set_bloque();
	}
	//TODO hace falta que cree un thread por cada solicitud de escritura? o mientras esta escribiendo algo no puede recivir otra solicitud de escritura?
}
