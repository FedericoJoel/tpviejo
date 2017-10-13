#include "fileSystem.h"

int PUERTO_FS= 3490;
int s_servidor;
int s_clientes[clientes_max];
t_list *t_dataNodes;
t_list *s_dataNodes;
pthread_t* t_atiende_dn;
pthread_t t_espera_data_nodes;

int main(void) {
	//INICIAMOS UN SERVIDOR Y LE MANDAMOS UN MENSAJE PARA QUE GUARDE UN PAQUETE.
	iniciar_servidor();
	//TODO puedo hacer 2 servidores, uno para yama y uno para los dn? o tengo que armar una especie de handshake para saber quien se me conecta?

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

void esperar_conexiones(){
	//Creamos un servidor
	s_servidor = crearServidor(PUERTO_FS);

	//Creo lista de sockets
	s_dataNodes = list_create();

	while(1){
		//BLOQUEANTE espero una conexion de un DN
		int autenticacion = 0;
		int socket = esperarConexionAuth(s_servidor,&autenticacion);
		if(autenticacion == DATANODE){
			//Lo agrego a la lista de datanodes's sockets
			int posicion = list_add(s_dataNodes, (void *) &socket);
			enviarMensajeConProtocolo(socket, string_itoa(posicion), DN_OKCONN);
			printf("Se conecto un nuevo DATANODE,en el socket %d se asigno a la posicion %d de la lista \n",socket,posicion);
			set_bloque(socket,"datosdatosdatosdatos",5);
		}
	}

	//Creo FDSET SERVIDOR (puntero a descriptor server)
//	fd_set fds;
//
//	//Limpio el set
//	FD_ZERO (&fds);
//
//	//Agregamos el servidor
//	FD_SET (s_servidor, &fds);
//
//	//Declaramos respuesta y tiramos select
//	int rc;
//	int n_clientes = 0;
//	int i;
//
//	while(1){
//		rc = select(sizeof(fds)*8, &fds, NULL, NULL, NULL);
//		if (rc==-1) {
//			 perror("select failed");
//			 break;
//		}
//
//		//CHECK DE CLIENTES
//		for (i=0; i<n_clientes; i++)
//		{
//			if (FD_ISSET(s_clientes[i], &fds))
//			{
//				char * buffer;
//				if ( (leer_cliente(s_clientes[i],(char *)&buffer) ) > 0 )
//				{
//					printf("leimos el mensaje '%s' del socket %d \n",buffer,s_clientes[i]);
//				}
//				else
//				{
//					printf("puede que el socket se halla desconectado \n");
//					/* Hay un error en la lectura. Posiblemente el cliente ha cerrado la conexión. Hacer aquí el tratamiento. En el ejemplo, se cierra el socket y se elimina del array de socketCliente[] */
//				}
//			}
//		}
//
//		//CHECK DEL SERVER
//		if (FD_ISSET(s_servidor, &fds))
//		{
//			char* AUTH ="";
//			// Si hay una nueva coneccion en s_servidor la acepto
//			int socket = esperarConexion(s_servidor, AUTH);
//			printf("la autorizacion es %s \n",AUTH);
//			// Agrego el nuevo cliente al fds de clientes
//			FD_SET(socket, &fds);
//			// Lo agregamos al array de clientes
//			//TODO me conviene usar un array? o es mejor una lista?
//			s_clientes[n_clientes]= socket;
//			// Enviamos un mensaje con su lugar en la lista
//			enviarMensajeConProtocolo(socket, string_itoa(n_clientes), DN_OKCONN);
//			// Incrementamos la cantidad de clientes conectados.
//			n_clientes = n_clientes +1;
//		}
//	}

	//HASTA ACA VA EL SELECT

	//ESTO ES DE MULTITHREADING

		//LE MANDO UN MENSAJE PARA QUE ESCRIBA UN BLOQUE
//		void* respuesta = list_get(s_dataNodes, 0);
//		int * s_nodo = (int*) respuesta;
//		set_bloque(s_nodo,"datosdatos");
//		int tam = sizeof(t_atiende_dn)/sizeof(*t_atiende_dn);
//		pthread_create(&t_atiende_dn + tam,NULL,(void*)&atender_dn, &socket);
//		printf("el tamaño de t_espera_data_nodes ahora es de %d \n",tam);
//	}
}

void iniciar_servidor(){
	pthread_create(&t_espera_data_nodes,NULL,(void*)&esperar_conexiones, NULL);
}

//void atender_dn(void* soc){
//	int *socket = (int *) soc;
//	printf("voy a ejecutar una escritura \n",*socket);
//	enviarMensajeConProtocolo(*socket, "DATOSDATOSDATOS", DN_SETBLOQUE);
//}

void set_bloque(int s_nodo,char* datos,int bloque){
	printf("grabo un bloque en el nodo sockeft %d \n",s_nodo);
	char* bloqueS = string_itoa(bloque);
	char* mensaje = generar_string_separador2(bloqueS, datos, ";");
	enviarMensajeConProtocolo(s_nodo, mensaje, DN_SETBLOQUE);
}

int leer_cliente(int s_nodo, char* buffer){
	int protocolo = recibirProtocolo(s_nodo);
	if(protocolo == DN_GETBLOQUEANSW){
		char* mensaje = esperarMensaje(s_nodo);
		printf("escribo bloque con los datos %s \n",mensaje);
	}else if (protocolo == -1){
		return protocolo;
	}
	return 1;
}

char* get_bloque(int s_nodo, int bloque){
	char* bloqueS = string_itoa(bloque);
	enviarMensajeConProtocolo(s_nodo,bloqueS,DN_GETBLOQUE);
	char* respuesta = esperarMensaje(s_nodo);
	return respuesta;
}
