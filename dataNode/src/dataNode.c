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

int bin;
char *memoria;
int size_memory=100;

const char *filepath = "info.bin";

int main(void) {

	escribir("PRUEBA",2);
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

int escribir(char* mensaje,int offset){

	//Abro el archivo
	 int fd = open(filepath, O_RDWR | O_CREAT, (mode_t)0600);

	 if (fd == -1)
	 {
	    perror("Error opening file for writing");
	    exit(EXIT_FAILURE);
	 }

	 size_t mensaje_size = strlen(mensaje) + 1; // + \0 null character

	 //Me muevo a la ultima posicion
	 if (lseek(fd, size_memory, SEEK_SET) == -1)
	 {
		 close(fd);
	     perror("Error calling lseek() to 'stretch' the file");
	     exit(EXIT_FAILURE);
	 }

	 //Escribo un caracter cualquiera
	  if (write(fd, "", 1) == -1)
	    {
	        close(fd);
	        perror("Error writing last byte of the file");
	        exit(EXIT_FAILURE);
	    }

	  //Esta listo para Mapear
	  char *map = mmap(0, size_memory, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	  if (map == MAP_FAILED)
	  {
	     close(fd);
	     perror("Error mmapping the file");
	     exit(EXIT_FAILURE);
	  }

	  size_t i;
//	  for (i = offset; i < mensaje_size; i++)
//	  {
//	      printf("Writing character %c at %zu\n", mensaje[i], i);
//	      map[i] = mensaje[i];
//	  }
	  memcpy(map+offset, mensaje, mensaje_size);


	  // Write it now to disk
//	  if (msync(map, size_memory, MS_SYNC) == -1)
//	  {
//	      perror("Could not sync the file to disk");
//	  }
//		 printf('%s \n',map );
	  // Don't forget to free the mmapped memory
	  if (munmap(map, size_memory) == -1)
	  {
	      close(fd);
	      perror("Error un-mmapping the file");
	      exit(EXIT_FAILURE);
	  }

	  // Un-mmaping doesn't close the file, so we still need to do that.
	  close(fd);

	  return 0;

}
