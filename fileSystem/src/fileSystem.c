#include "fileSystem.h"


int main(void) {
	//INICIAMOS UN SERVIDOR Y LE MANDAMOS UN MENSAJE PARA QUE GUARDE UN PAQUETE.
//	iniciar_servidor();

	//GENERAMOS UN THREAD PARA LA CONSOLA
	pthread_t t_consola;
	pthread_create(&t_consola,NULL,(void*)&ejecutarConsola, NULL);

	//ESPERAMOS A QUE TERMINEN TODOS LOS THREAD
	pthread_join(t_espera_data_nodes,NULL);
	pthread_join(t_consola,NULL);

	return EXIT_SUCCESS;

}

void modificarNodoDeFs(estructuraFs* fs, int id,int bloquesOcupados){
	char* nodoNumero = string_itoa(id);
	char* nodoNombre = string_new();
	string_append(&nodoNombre, "Nodo");
	string_append(&nodoNombre, nodoNumero);
	estructuraNodo* nodo = malloc(sizeof(estructuraNodo));

	int posicion = encontrarPosicion(fs->nodos, nodoNombre);

	nodo = list_get(fs->nodos, posicion);

	nodo->tamanioLibreNodo = nodo->tamanioLibreNodo - bloquesOcupados;

	list_replace(fs->nodos, posicion, (void*) nodo);

}

void eliminarNodoDeFs(estructuraFs* fs, int id){

	char* nodoNumero = string_new();
	nodoNumero = string_itoa(id);
	char* nodoNombre = string_new();
	string_append(&nodoNombre, "Nodo");
	string_append(&nodoNombre, nodoNumero);

	free(nodoNumero);

	int posicion = encontrarPosicion(fs->nodos,nodoNombre);

	list_remove(fs->nodos, posicion);
}

int encontrarPosicion(t_list* nodos, char* nombre){
	int posicion = 0;
	int largo = list_size(nodos);

	while (posicion < largo){
		estructuraNodo* nodo = malloc(sizeof (estructuraNodo));
		nodo = list_get(nodos, posicion);
		if(string_equals_ignore_case(nodo->nombreNodo, nombre)){
//			free(nodo);
			break;
		}
//		free(nodo);
		posicion++;
	}
	return posicion;
}

int existenArchivosDeConfiguracion(){
	if((existeDirectorio() == 0) && (existenNodos() == 0)){
		return EXIT_SUCCESS;
	}else{
		return EXIT_FAILURE;
	}
}

void agregarNodoAFs(estructuraFs* fs, int id, int bloquesLibres, int bloquesTotales){
	fs->tamanioLibreFs = fs->tamanioLibreFs + bloquesLibres;
	fs->tamanioTotalFs = fs->tamanioTotalFs + bloquesTotales;
	estructuraNodo* nuevoNodo = malloc(sizeof (estructuraNodo));
	char* nodoNumero = string_new();
	nodoNumero = string_itoa(id);
	char* nodoNombre = string_new();
	string_append(&nodoNombre, "Nodo");
	string_append_with_format(&nodoNombre, "%s", nodoNumero);

	nuevoNodo->nombreNodo = string_duplicate(nodoNombre);

	nuevoNodo->tamanioLibreNodo = bloquesLibres;
	nuevoNodo->tamanioTotalNodo = bloquesTotales;

	list_add(fs->nodos, (void*) nuevoNodo);

	list_sort(fs->nodos, (void*) ordenarNodo);

	free(nodoNombre);
	free(nodoNumero);
//	free(nuevoNodo);
}

void cargarNodosAFs(estructuraFs* nuevoFs){

	int tamanioFs = tamanioTotalFs();
	int espacioLibreFs = tamanioLibreFs();
	int contador = 0;

	nuevoFs->nodos = list_create();

	nuevoFs->tamanioTotalFs = tamanioFs;
	printf("TAMANIO = %d \n", tamanioFs);
	nuevoFs->tamanioLibreFs = espacioLibreFs;
	printf("LIBRE = %d \n", espacioLibreFs);

	int cantidadDeNodos = cantidadNodos();
	for(contador = 0; contador < cantidadDeNodos; contador++){
		estructuraNodo* nuevoNodo = malloc(sizeof (estructuraNodo));
		nuevoNodo = levantarNodo(contador);
		list_add(nuevoFs->nodos, (void*) nuevoNodo);
//		free(nuevoNodo);
	}
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

void posicion(t_bitarray* array, int pos){
	bitarray_test_bit(array, pos);
}

void fs_format(){
	printf("Formateo de disco \n");
	eliminarArchivosDeDirectorio(fs.directorio);
	eliminarNodos(&fs.estructuraNodos);
}

void eliminarNodos(estructuraFs* fs){
	list_clean(fs->nodos);
	char* touch = string_new();
	char* rm = string_new();
	string_append(&rm, "rm -r /home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/algo.dat");
	string_append(&touch, "touch /home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/directorio1.dat");
	system(rm);
	system(touch);
	free(rm);
	free(touch);

}

void fs_rm(char * arg){
	// recordar que C maneja los string como array de char, por lo tanto array de string es char **
	char ** argumentos;
	char ** path;
	int i;

	argumentos = string_split(arg, " ");
	int cantidadDeElementos = cantidadElementos(argumentos);

	for(i=0;i<cantidadDeElementos;i++){
		string_trim(&argumentos[i]);
	}

	if (string_equals_ignore_case(*argumentos, "-d")){
		if (string_starts_with(argumentos[1], "/")){
			char* rm = string_new();
			string_append(&rm, "rm -d ");
			string_append(&rm, argumentos[cantidadDeElementos - 1]);
			printf("comando: %s \n", rm);
			int finalizo = system(rm);
			if (finalizo == 0){
				path = string_split(arg, "/");
				int cantidadDeElementosPath = cantidadElementos(path);
				printf("Se eliminó el directorio %s \n",path[cantidadDeElementosPath - 1]);
			}else if (finalizo == 256){

			}
			free(rm);

		}
		else
		{
			printf("No se puede eliminar el directorio, ya que no se encuentra \n");
		}
	}
	else if(string_equals_ignore_case(*argumentos, "-b"))
	{
		t_archivo* estructuraArchivo = malloc(sizeof(t_archivo));
		char* pathArchivo = string_duplicate(argumentos[1]);
		char* numeroBloque = string_duplicate(argumentos[2]);
		char* numeroCopia = string_duplicate(argumentos[3]);
		cargarTablaArchivo(estructuraArchivo, pathArchivo);
		eliminarBloqueDeArchivo(estructuraArchivo, numeroBloque, numeroCopia);
		printf("%s \n %s \n %s \n",pathArchivo, numeroBloque, numeroCopia);
		printf("Se elimino el nodo '%s'\n",*argumentos);
	}
	else
	{
	if(string_starts_with(*argumentos, "/"))
		{
		path = string_split(arg, "/");
		int cantidadDeElementosPath = cantidadElementos(path);
		char* rm = string_new();
		string_append(&rm, "rm -d ");
		string_append(&rm, argumentos[cantidadDeElementos - 1]);
		printf("comando: %s \n", rm);
		int finalizo = system(rm);
		if (finalizo == 0){
			path = string_split(arg, "/");
			int cantidadDeElementosPath = cantidadElementos(path);
			printf("Archivo Eliminado %s\n",path[cantidadDeElementosPath - 1]);
		}else if (finalizo == 256){

		}
		free(rm);
		}
	else{
		printf("Archivo Eliminado '%s'\n", *argumentos);
		}
	}
}

void fs_rename(char * arg){
	char** argumentos;
	char** path;
	int cantidadElementosPath = sizeof(path)/sizeof(path[0]);
	argumentos = string_split(arg, " ");
	if (string_starts_with(argumentos[0], "/")){
	path = string_split(arg, "/");
	printf("Renombrar el archivo '%s' a '%s' \n",path[cantidadElementosPath - 1],argumentos[1]);
	}
	else
	{
	printf("Renombrar el archivo '%s' a '%s' \n",argumentos[0],argumentos[1]);
	}

}

void fs_mv(char * arg){
	char** argumentos;
	char** path;
	int cantidadElementosPath = sizeof(path)/sizeof(path[0]);
	argumentos = string_split(arg, " ");
	if (string_starts_with(argumentos[0], "/")){
	path = string_split(arg, "/");
	printf("Se movio de '%s' a '%s' \n",path[cantidadElementosPath - 1], argumentos[1]);
	}
	else
	{
	printf("Se movio de '%s' a '%s' \n",argumentos[0], argumentos[1]);
	}
}

void fs_cat(char * arg){
	char** path;
	int cantidadElementos = sizeof(path)/sizeof(path[0]);
	if (string_starts_with(arg, "/")){
	path = string_split(arg, "/");
	printf("Archivo a mostrar '%s'\n",path[cantidadElementos - 1]);
	}
	else
	{
	printf("Archivo a mostrar '%s'\n",arg);
	}
}

void fs_mkdir(char * arg){
	char** path;
	int cantidadElementos = sizeof(path)/sizeof(path[0]);
	if (!string_starts_with(arg, "/")){
	path = string_split(arg, "/");
	printf("Directorio creado '%s'\n",path[cantidadElementos - 1]);
	}
	else
	{
	printf("Permiso denegado, no se puede crear el directorio '%s'\n",arg);
	}
}

void fs_cpfrom(char * arg){
	char** path;
	char** argumentos;
	int cantidadElementos = sizeof(path)/sizeof(path[0]);

	if (string_starts_with(arg, "/")){
	argumentos = string_split(arg, " ");
	path = string_split(argumentos[0], "/");
	printf("Archivo copiado a YAMA: '%s'\n", strcat(argumentos[1], path[cantidadElementos - 1]));
	}
	else
	{
	printf("Archivo copiado a YAMA: '%s'\n", arg);
	}
}

void fs_cpto(char * arg){
	char** path;
	int cantidadElementos = sizeof(path)/sizeof(path[0]);
	if (string_starts_with(arg, "/")){
	path = string_split(arg, "/");
	printf("Archivo copiado a YAMAFs:'%s'\n", path[cantidadElementos - 1]);
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
	int cantidadElementos = sizeof(path)/sizeof(path[0]);
	if (string_starts_with(arg, "/")){
	path = string_split(arg, "/");
	printf("Archivo con MD5: '%s' creado \n", path[cantidadElementos - 1]);
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
	int cantidadElementos = sizeof(path)/sizeof(path[0]);
	if (string_starts_with(arg, "/")){
	path = string_split(arg, "/");
	printf("Información del archivo: '%s'\n", path[cantidadElementos - 1]);
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


	//Creo lista de sockets
	s_dataNodes = list_create();


	while(1){

		char * AUTH = string_new();
		//BLOQUEANTE espero una conexion de un DN
		int socket = esperarConexion(s_servidor, AUTH);
		printf("la autorizacion es %s \n",AUTH);

		//Lo agrego a la lista de datanodes's sockets
		int posicion = list_add(s_dataNodes, (void *) &socket);
		printf("Escuchamos una nueva coneccion,en el socket %d se asigno a la posicion %d de la lista \n",socket,posicion);

		//LE MANDO UN MENSAJE PARA QUE ESCRIBA UN BLOQUE
		void* respuesta = list_get(s_dataNodes, 0);
		int * s_nodo = (int*) respuesta;
		set_bloque(s_nodo,"datosdatos");
//		int tam = sizeof(t_atiende_dn)/sizeof(*t_atiende_dn);
//		pthread_create(&t_atiende_dn + tam,NULL,(void*)&atender_dn, &socket);
//		printf("el tamaño de t_espera_data_nodes ahora es de %d \n",tam);
	}
}

void iniciar_servidor(){
	pthread_create(&t_espera_data_nodes,NULL,(void*)&esperar_data_nodes, NULL);
}

//void atender_dn(void* soc){
//	int *socket = (int *) soc;
//	printf("voy a ejecutar una escritura \n",*socket);
//	enviarMensajeConProtocolo(*socket, "DATOSDATOSDATOS", DN_SETBLOQUE);
//}

void set_bloque(int *s_nodo,char* datos){
	printf("grabo un bloque en el nodo sockeft %d \n",*s_nodo);
	enviarMensajeConProtocolo(*s_nodo, "DATOSDATOSDATOS", DN_SETBLOQUE);
	// TODO VERIFICAR SI SALE OK
	// TODO que me conviene hacer con en que bloque lo guarda? lo meto en el protocolo? mando 2 mensajes juntos?
	//	pthread_t t_atiende_escritura;
	//	pthread_create(&t_atiende_escritura,NULL,(void*)&atender_escritura, &s_nodo);
	//	TODO COMO HAGO PARA RETORNAR SI LA ESCRITURA FUE BUENA O MALA CON EL THREAD y mandarle varios parametros
	//	pthread_join(t_atiende_escritura);
}
