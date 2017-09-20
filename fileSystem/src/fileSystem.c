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



int PUERTO_FS= 3490;
int s_servidor;
t_list *t_dataNodes;
t_list *s_dataNodes;
pthread_t* t_atiende_dn;
pthread_t t_espera_data_nodes;

int main(void) {
	iniciar_servidor();

	//GENERAMOS UN THREAD PARA LA CONSOLA
	pthread_t t_consola;
	pthread_create(&t_consola,NULL,(void*)&ejecutarConsola, NULL);

	//ESPERAMOS A QUE TERMINEN TODOS LOS THREAD
	pthread_join(t_espera_data_nodes,NULL);
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
	    if(string_equals_ignore_case(linea, "format")){
	    	fs_format();
	    }else if(string_starts_with(linea, "rm ")){
	    	fs_rm(string_substring(linea, 3, strlen(linea)));
	    }else if(string_starts_with(linea,"rename ")){
	    	fs_rename(string_substring(linea, 7, strlen(linea)));
	    }else if(string_starts_with(linea,"mv ")){
	    	fs_mv(string_substring(linea, 3, strlen(linea)));
	    }else if(string_starts_with(linea,"cat ")){
	    	fs_cat(string_substring(linea, 4, strlen(linea)));
	    }else if(string_starts_with(linea,"mkdir ")){
	    	fs_mkdir(string_substring(linea, 6, strlen(linea)));
	    }else if(string_starts_with(linea,"cpfrom ")){
	    	fs_cpfrom(string_substring(linea, 7, strlen(linea)));
	    }else if(string_starts_with(linea,"cpto ")){
	    	fs_cpto(string_substring(linea, 5, strlen(linea)));
	    }else if(string_starts_with(linea,"cpblock ")){
	    	fs_cpblok(string_substring(linea, 8, strlen(linea)));
	    }else if(string_starts_with(linea,"md5 ")){
	    	fs_md5(string_substring(linea, 4, strlen(linea)));
	    }else if(string_starts_with(linea,"ls ")){
	    	fs_ls(string_substring(linea, 3, strlen(linea)));
	    }else if(string_starts_with(linea,"info ")){
	    	fs_info(string_substring(linea, 5, strlen(linea)));
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
	char ** path;
	int i;
	argumentos = string_split(arg, " ");

	for(i=0;i<str_array_size(argumentos);i++){
		string_trim(&argumentos[i]);
	}

	if (string_equals_ignore_case(*argumentos, "-d")){
		if (string_starts_with(argumentos[1], "/")){
		printf("Se eliminó el directorio '%s'\n",argumentos[str_array_size(*argumentos) - 1]);
		}
		else
		{
		printf("No se puede eliminar el directorio \n");
		}
	}
	else if(string_equals_ignore_case(*argumentos, "-b"))
	{
		printf("Se elimino el nodo '%s'\n",*argumentos);
	}
	else
	{
	if(string_starts_with(*argumentos, "/"))
		{
		path = string_split(arg, "/");
		printf("Archivo Eliminado '%s'\n",path[str_array_size(*path) - 1]);
		}
		else
		{
		printf("Archivo Eliminado '%s'\n", *argumentos);
		}
	}
}

void fs_rename(char * arg){
	char** argumentos;
	char** path;
	argumentos = string_split(arg, " ");
	if (string_starts_with(argumentos[0], "/")){
	path = string_split(arg, "/");
	printf("Renombrar el archivo '%s' a '%s' \n",path[str_array_size(*path) - 1],argumentos[1]);
	}
	else
	{
	printf("Renombrar el archivo '%s' a '%s' \n",argumentos[0],argumentos[1]);
	}

}

void fs_mv(char * arg){
	char** argumentos;
	char** path;
	argumentos = string_split(arg, " ");
	if (string_starts_with(argumentos[0], "/")){
	path = string_split(arg, "/");
	printf("Se movio de '%s' a '%s' \n",path[str_array_size(*path) - 1], argumentos[1]);
	}
	else
	{
	printf("Se movio de '%s' a '%s' \n",argumentos[0], argumentos[1]);
	}
}

void fs_cat(char * arg){
	char** path;
	if (string_starts_with(arg, "/")){
	path = string_split(arg, "/");
	printf("Archivo a mostrar '%s'\n",path[str_array_size(*path) - 1]);
	}
	else
	{
	printf("Archivo a mostrar '%s'\n",arg);
	}
}

void fs_mkdir(char * arg){
	char** path;
	if (string_starts_with(arg, "/")){
	path = string_split(arg, "/");
	printf("Directorio creado '%s'\n",path[str_array_size(*path) - 1]);
	}
	else
	{
	printf("Directorio creado '%s'\n",arg);
	}
}

void fs_cpfrom(char * arg){
	char** path;
	char** argumentos;

	if (string_starts_with(arg, "/")){
	argumentos = string_split(arg, " ");
	path = string_split(argumentos[0], "/");
	printf("Archivo copiado a YAMA: '%s'\n", strcat(argumentos[1], path[str_array_size(*path) - 1]));
	}
	else
	{
	printf("Archivo copiado a YAMA: '%s'\n", arg);
	}
}

void fs_cpto(char * arg){
	char** path;
	if (string_starts_with(arg, "/")){
	path = string_split(arg, "/");
	printf("Archivo copiado a YAMAFs:'%s'\n", path[str_array_size(*path) - 1]);
	}
	else
	{
	printf("Archivo copiado a YAMAFs:'%s'\n", arg);
	}
}

void fs_cpblok(char * arg){
	char** argumentos;
	argumentos = string_split(arg, " ");
	printf("Archivo a copiar '%s'\n", argumentos[0]);
	printf("Del bloque '%s' .\n", argumentos[1]);
	printf("Copia creada en el nodo '%s'\n", argumentos[2]);
}

void fs_md5(char * arg){
	char** path;
	if (string_starts_with(arg, "/")){
	path = string_split(arg, "/");
	printf("Archivo con MD5: '%s' creado \n", path[str_array_size(*path) - 1]);
	}
	else
	{
	printf("Archivo con MD5: '%s' creado \n", arg);
	}
}

void fs_ls(char * arg){
	printf("Archivos en la dirección: '%s'\n", arg);
}

void fs_info(char * arg){
	char** path;
	if (string_starts_with(arg, "/")){
	path = string_split(arg, "/");
	printf("Información del archivo: '%s'\n", path[str_array_size(*path) - 1]);
	}
	else
	{
	printf("Información del archivo: '%s'\n", arg);
	}
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

void esperar_data_nodes(){
	//Creamos un servidor
	s_servidor = crearServidor(PUERTO_FS);
	char * AUTH;

	//Creo lista de sockets
	s_dataNodes = list_create();


	while(1){
		//BLOQUEANTE espero una conexion de un DN
		int socket = esperarConexion(s_servidor, AUTH);

		//Lo agrego a la lista de datanodes's sockets
		int posicion = list_add(s_dataNodes, (void *) socket);
		printf("Escuchamos una nueva coneccion,en el socket %d se asigno a la posicion %d de la lista \n",socket,posicion);

		//Creo un thread que atienda el socket

		int tam = sizeof(t_atiende_dn)/sizeof(*t_atiende_dn);
		pthread_create(&t_atiende_dn + tam,NULL,(void*)&atender_dn, &socket);
		printf("el tamaño de t_espera_data_nodes ahora es de %d \n",tam);
	}
}

void iniciar_servidor(){
	pthread_create(&t_espera_data_nodes,NULL,(void*)&esperar_data_nodes, NULL);
}

void atender_dn(void* argv){
	int *socket = (int *) argv;
	printf("a atender le llego el socket %d",*socket);

}
