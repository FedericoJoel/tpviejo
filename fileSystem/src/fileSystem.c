/*
 ============================================================================
 Name        : fileSystem.c
 Author      : Bool Basaur
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <commons/temporal.h>
#include <lib.h>

int main(void) {


//  char * linea;
//  while(1) {
//    linea = readline(">");
//    if (!linea) {
//      break;
//    }
//    if(!strncmp(linea, "joaquin", 7)){
//    	printf("sacaplin\n");
//    }
////    printf("%s\n", linea);
//    free(linea);
//  }


//	PRUEBA YAMALIB
	saludar();
	return EXIT_SUCCESS;

//	PRUEBA DE COMMONS
//	char* tiempo =  temporal_get_string_time();
//	puts(tiempo);
//	free(tiempo);
//	return EXIT_SUCCESS;
}