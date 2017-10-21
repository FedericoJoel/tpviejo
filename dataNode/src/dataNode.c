/*============================================================================

 ============================================================================
 Name        : dataNode.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================*/

#include "dataNode.h"

int PUERTO_FS=3490;
int s_filesystem;
int bloque_size=1;
char* node_id="1";
char* node_ip="127.0.0.1";

int bin;
char* data;

const char *filepath = "info.bin";

int main(void) {

	map_data();
	escribir_bloque("hola",2);
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
	int p_resp;
	s_filesystem = conectarAuth(PUERTO_FS,IP,DATANODE,&p_resp);
	if(p_resp == DN_OKCONN){
	enviarMensaje(s_filesystem, node_id);
	enviarMensaje(s_filesystem, node_ip);
	printf("Conectado a fs con socket %d \n",s_filesystem);
	} else
	{
		printf("error de coneccion \n");
	}
}

void set_bloque(){
	char* mensaje = esperarMensaje(s_filesystem);
	char** array = string_split(mensaje, ";");
	printf("escribo bloque '%s' con los datos '%s' \n",array[0],array[1]);
	int bloque = atoi(array[0]);
	escribir_bloque(bloque,array[1]);
}

void get_bloque(){
	char* bloque = esperarMensaje(s_filesystem);
	printf("leo datos del bloque '%s'\n",bloque);
	enviarMensajeConProtocolo(s_filesystem,"datosdatoslectura",DN_GETBLOQUEANSW);
}

void error_protocolo(){
	printf('Recibimos un protocolo incorrecto \n');
}

void escuchar_fs(){
	if(recibirProtocolo(s_filesystem) == DN_SETBLOQUE){
		set_bloque();
	}else if (recibirProtocolo(s_filesystem) == DN_GETBLOQUE){
		get_bloque();
	}else
		error_protocolo();
}

int map_data(){
//Abro el archivo
	int fd = open(filepath, O_RDWR, (mode_t)0600);
	if (fd == -1){
		perror("Error opening file for writing");
		exit(EXIT_FAILURE);
	}
	struct stat fileStat;
	if(fstat(fd,&fileStat) < 0){
		return 1;
	}

	//Tamaño de la memoria
	int data_size = fileStat.st_size;
	data = mmap(0, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (data == MAP_FAILED){
		close(fd);
		perror("Error mmapping the file");
	    exit(EXIT_FAILURE);
	}
}

void escribir(char* mensaje,int offset){
	size_t mensaje_size = strlen(mensaje); // no escribe el \0
	memcpy(data+offset, mensaje, mensaje_size);
}

void escribir_bloque(char* mensaje, int bloque){
	escribir(mensaje,bloque_size*bloque);
}
