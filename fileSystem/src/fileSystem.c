#include "fileSystem.h"

int PUERTO_FS= 3490;
int s_servidor;
int s_clientes[clientes_max];
t_list *t_dataNodes;
t_list *lista_dataNodes;
pthread_t* t_atiende_dn;
pthread_t t_espera_data_nodes;
int bloque_size=12;

int main(void) {
	printf("ESTO FUNCI\nONA E\nSPEC\nTAC\nULAR;;JAJA");

	//INICIAMOS UN SERVIDOR Y LE MANDAMOS UN MENSAJE PARA QUE GUARDE UN PAQUETE.}
	int cant_bloques = size_in_bloks("ESTO FUNCIONA ESPECTACULAR;;JAJAJA");

	t_list* lista = list_create();
//	char** palabras = string_split("ESTO FUNCI\nONA E\nSPEC\nTAC\nULAR;;JAJAJA","\n");
	t_list* resultado = cortar_texto("ESTO FUNCI\nONA E\nSPEC\nTAC\nULAR;;JAJA",lista);
	char* bloque0 = (char*)list_get(resultado,0);
	char* bloque1 = (char*)list_get(resultado,1);
	char* bloque2= (char*)list_get(resultado,2);
	printf("bloque 0: %s \n", bloque0);
	printf("bloque 1: %s \n", bloque1);
	printf("bloque 2: %s \n", bloque2);
//	t_list* resultado = cortar_datos(0,palabras,lista);
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
	int size = str_array_size(array);
		for (i=0;i<size;i++){
			printf("El argumento de la posicion %d es '%s'\n",i,array[i]);
		}
}

void esperar_conexiones(){
	//Creamos un servidor
	s_servidor = crearServidor(PUERTO_FS);

	//Creo lista de sockets
	lista_dataNodes = list_create();

	while(1){
		//BLOQUEANTE espero una conexion de un DN
		int autenticacion = 0;
		int socket = esperarConexionAuth(s_servidor,&autenticacion);
		//SI ES UN DATANODE
		if(autenticacion == DATANODE){
			enviarMensajeConProtocolo(socket, string_itoa(DN_OKCONN), DN_OKCONN); //CONFIRMO CONEXION Y RECIBO LA INFO
			t_nodo* nodo = malloc(sizeof(t_nodo));
			nodo->id = atoi(esperarMensaje(socket));
			nodo->ip = esperarMensaje(socket);
			nodo->bloque_cant = esperarMensaje(socket);
			nodo->socket = socket;
			//Lo agrego a la lista de datanodes's sockets
			int posicion = list_add(lista_dataNodes, (void *) nodo);

			printf("Se conecto un nuevo DATANODE,en el socket %d se asigno a la posicion %d de la lista \n",socket,posicion);
			if(!set_bloque(socket,"datosdatosdatosdatos",5)) printf("el nodo no responde");
		}
	}
}

void iniciar_servidor(){
	pthread_create(&t_espera_data_nodes,NULL,(void*)&esperar_conexiones, NULL);
}

int set_bloque(int s_nodo,char* datos,int bloque){
	if (keep_alive(s_nodo)==0){
		printf("grabo un bloque en el nodo sockeft %d \n",s_nodo);
		char* bloqueS = string_itoa(bloque);
		char* mensaje = generar_string_separador2(bloqueS, datos, ";");
		enviarMensajeConProtocolo(s_nodo, mensaje, DN_SETBLOQUE);
		return EXIT_SUCCESS;
	}else
	return EXIT_FAILURE;
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

int keep_alive(int socket){
	enviarProtocolo(socket,DN_KEEPALIVE);
	int answ = recibirProtocolo(socket);
	if (answ==DN_ALIVE){
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int size_in_bloks(char* mensaje){
	size_t mensaje_size = strlen(mensaje);
	int cantidad_bloques = mensaje_size / bloque_size;
	return cantidad_bloques;
}

//t_list* cortar_datos(char* mensaje){
// cortar_datos(0, palabras, bloques)

t_list* cortar_texto(char* mensaje, t_list* lista) {
	char** palabras = string_split(mensaje,"\n");
	int i = 0;
	int ocupado = 0;
	int cant_palabras = str_array_size(palabras);
	char* bloque = calloc(bloque_size, sizeof(char));
	for(i=0; i< cant_palabras; i++) {
		int tam_palabra = string_length((palabras[i]));
		char* palabra_actual = calloc(tam_palabra,sizeof(char));
		palabra_actual = palabras[i];
		if(tam_palabra <= bloque_size) {
			if((ocupado + tam_palabra ) <= bloque_size -1) {
				memcpy(bloque+ ocupado,palabra_actual,tam_palabra);
				ocupado += tam_palabra;
				free(palabra_actual);
			}else {
				list_add(lista,(void*) bloque);
				ocupado = 0;
				bloque = calloc(bloque_size, sizeof(char));
				memcpy(bloque,palabra_actual,tam_palabra);
				ocupado += tam_palabra;
				free(palabra_actual);
			}
		}
	}
	list_add(lista,(void*) bloque);
	return lista;

}

//t_list* cortar_texto(char* mensaje, t_list* lista) {
//char** palabras = string_split(mensaje,"\n");
//int i = 0;
//int ocupado = 0;
//int cant_palabras = str_array_size(palabras);
//char* bloque = calloc(bloque_size, sizeof(char));
//for(i=0; i< cant_palabras; i++) {
//	int tam_palabra = string_length((palabras[i]));
//	char* palabra_actual = malloc(tam_palabra+1);
//	palabra_actual = palabras[i];
//	palabra_actual[tam_palabra+1] = '\n';
//	if(tam_palabra <= bloque_size) {
//		if((ocupado + tam_palabra) <= bloque_size) {
//			memcpy(bloque+ ocupado,palabra_actual,tam_palabra);
//			ocupado += tam_palabra;
//			free(palabra_actual);
//		}else {
//			list_add(lista,(void*) bloque);
////			printf("contenido \n %s",bloque[i+1]);
//			ocupado = 0;
//			bloque = calloc(bloque_size, sizeof(char));
//			memcpy(bloque,palabra_actual,tam_palabra);
//			ocupado += tam_palabra;
//			free(palabra_actual);
//		}
//	}
//}
//list_add(lista,(void*) bloque);
//return lista;
//
//}
