/*
 ============================================================================
 Name        : fileSystem.c
 Author      : Bool Basaur
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "fileSystem.h"

int main(void) {
	ejecutarConsola();

//	PRUEBA YAMALIB
//	saludar();
//	return EXIT_SUCCESS;

//	PRUEBA DE COMMONS
//	char* tiempo =  temporal_get_string_time();
//	puts(tiempo);
//	free(tiempo);
//	return EXIT_SUCCESS;
	return EXIT_SUCCESS;
}



void ejecutarConsola(){

	  char * linea;
	  while(1) {
	    linea = readline(">");

	    if (!linea || !strncmp(linea, "exit", 4)) {
	      break;
	    }

	    if(!strncmp(linea, "format", 6)){
	    	fs_format();
	    }else if(!strncmp(linea, "rm", 2)){
	    	fs_rm(string_substring(linea, 3, strlen(linea)));
	    }else if(!strncmp(linea, "rm -d", 5)){
	    	fs_rmd(string_substring(linea, 6, strlen(linea)));
	    	//ACA TENGO UN PROBLEMA POR QUE RECONOCE PRIMERO EL RM Y SE METE EN EL IF DE ARRIBA
	    	//QUIZA LA SOLUCION EN SPLITEAR LOS ARGUMENTOS QUE VIENEN Y RECONOCERLOS DENTRO DE LA FUNCION RM
	    }

	    free(linea);
	  }

//ESTE SERIA EL SWICH QUE PUEDO USAR CON LA HASH FUNCTION
//	while(1) {
//	char * linea = readline(">");
//	printf("%s\n", linea);
//	int hash_key = hash(linea);
//		switch(hash_key) {
//			case LS:
//				printf("Running ls...\n");
//				break;
//			case CD:
//				printf("Running cd...\n");
//				break;
//			case MKDIR:
//				printf("Running mkdir...\n");
//				break;
//			case PWD:
//				printf("Running pwd...\n");
//				break;
//			default:
//				printf("[ERROR] '%d' is not a valid command.\n", hash_key);
//	    }
//	}
}

void fs_format(){
	printf("Formateo de disco \n");
}

void fs_rm(char * arg){
	printf("Remover Archivo con el siguiente argumento '%s' \n",arg);
}

void fs_rmd(char * arg){
	printf("Remover Archivo con parametro -d y con el siguiente argumento '%s' \n",arg);
}

//FUNCION DE HASH
int hash(const char *str)
{
    int h = 0;
    while (*str)
       h = h << 1 ^ *str++;
    return h;
}
