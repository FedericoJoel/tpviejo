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

	//GENERAMOS UN THREAD PARA LA CONSOLA
	pthread_t t_consola;
	pthread_create(&t_consola,NULL,(void*)&ejecutarConsola, NULL);

	//ESPERAMOS A QUE TERMINEN TODOS LOS THREAD
	pthread_join(t_consola,NULL);

	return EXIT_SUCCESS;

////	PRUEBA YAMALIB
//	saludar();
//	return EXIT_SUCCESS;

}



void ejecutarConsola(){

	  char * linea;
	  while(1) {
	    linea = readline(">");

//	    ABORTO CONSOLA
	    if (!linea || string_starts_with(linea, "exit")) {
	      break;
	    }
//	    RECONOZCO COMANDOS
	    if(string_starts_with(linea, "format")){
	    	fs_format();
	    }else if(string_starts_with(linea, "rm")){
	    	fs_rm(string_substring(linea, 3, strlen(linea)));
	    }else if(string_starts_with(linea,"rename")){
	    	fs_rename(string_substring(linea, 7, strlen(linea)));
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
	// recordar que C maneja los string como array de char, por lo tanto array de string es char **
	char ** argumentos;
	int i;
	argumentos = string_split(arg, " ");

	for(i=0;i<str_array_size(argumentos);i++){
		string_trim(&argumentos[i]);
	}
	str_array_print(argumentos);
}

void fs_rename(char * arg){
	printf("Renombrar el archivo '%s'\n",arg);

}

int str_array_size(char ** array){
	    size_t count = 0;
	    while (array[count] != NULL) count++;
	    return count;
}

void str_array_print(char ** array){
	int i;
		for (i=0;i<str_array_size(array);i++){
			printf("El argumento de la posicion %d es '%s'\n",i,array[i]);
		}
}
