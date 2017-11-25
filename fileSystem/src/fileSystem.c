#include "fileSystem.h"

int s_servidor;
int s_yama;
int s_clientes[clientes_max];
t_list *lista_dataNodes;
pthread_t* t_atiende_dn;
pthread_t t_espera_data_nodes;
int bloque_size=12;

int main(void) {

	lista_dataNodes = list_create();
	t_nodo* nodo = malloc(sizeof(t_nodo));
	nodo->id = 21;
	nodo->ip = "221.21.21.21";
	nodo->bloque_cant = 10;
	nodo->socket = 5;
	//Lo agrego a la lista de datanodes's sockets
	int posicion = list_add(lista_dataNodes, (void *) nodo);
	printf("guardado, posicicon %d\n", posicion);
	nodo = malloc(sizeof(t_nodo));
	nodo->id = 3;
	nodo->ip = "331.3.3.3";
	nodo->bloque_cant = 30;
	nodo->socket = 21;
	//Lo agrego a la lista de datanodes's sockets
	posicion = list_add(lista_dataNodes, (void *) nodo);
	printf("guardado, posicicon %d\n", posicion);
	t_nodo* encontrado = malloc(sizeof(t_nodo));
	encontrado = find_nodo_by_id(21);
	printf("id %d, ip %s, bloque cant %d, socket %d \n",encontrado->id,encontrado->ip,encontrado->bloque_cant,encontrado->socket);

	printf("ESTO FUNCI\nONA E\nSPEC\nTAC\nULAR;;JAJA");

	//INICIAMOS UN SERVIDOR Y LE MANDAMOS UN MENSAJE PARA QUE GUARDE UN PAQUETE.}
	int cant_bloques = size_in_bloks("ESTO FUNCIONA ESPECTACULAR;;JAJAJA");

//	char** palabras = string_split("ESTO FUNCI\nONA E\nSPEC\nTAC\nULAR;;JAJAJA","\n");
//	t_list* resultado = cortar_texto("ESTO FUNCI\nONA E\nSPEC\nTAC\nULAR;;JAJA",lista);
	t_list* resultado = cortar_binario("hola1hola2hola3hola4hola5hola");
	char* bloque0 = (char*)list_get(resultado,0);
	char* bloque1 = (char*)list_get(resultado,1);
	char* bloque2= (char*)list_get(resultado,2);
	char* bloque3= (char*)list_get(resultado,3);
	printf("bloque 0: %s \n", bloque0);
	printf("bloque 1: %s \n", bloque1);
	printf("bloque 2: %s \n", bloque2);
	printf("bloque 3: %s \n", bloque3);
//	t_list* resultado = cortar_datos(0,palabras,lista);
	iniciar_servidor();
	//TODO puedo hacer 2 servidores, uno para yama y uno para los dn? o tengo que armar una especie de handshake para saber quien se me conecta?

	//GENERAMOS UN THREAD PARA LA CONSOLA
	pthread_t t_consola;
	pthread_create(&t_consola,NULL,(void*)&ejecutarConsola, NULL);

	cargarNodosAFs(&fs.estructuraNodos);
	cargarDirectorio(fs.directorio);

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


void fs_format(){
	printf("Formateo de disco \n");
	eliminarArchivosDeDirectorio(fs.directorio);
	eliminarNodos(&fs.estructuraNodos);
}

void guardarFsEnArchivo(){
	fs.estructuraNodos.tamanioLibreFs = fs.estructuraNodos.tamanioTotalFs;
	int tamanio = list_size(fs.estructuraNodos.nodos);
	int pos = 0;
	FILE* archivo = fopen("/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/nodos.bin", "w");
	char* tamanioFs = string_duplicate("TAMANIO=");
	string_append(&tamanioFs, string_itoa(fs.estructuraNodos.tamanioTotalFs));
	fputs(tamanioFs, archivo);
	fputc('\n', archivo);
	char* tamanioLibreFs = string_duplicate("LIBRE=");
	string_append(&tamanioLibreFs, string_itoa(fs.estructuraNodos.tamanioLibreFs));
	fputs(tamanioLibreFs, archivo);
	fputc('\n', archivo);
	char* nodos = string_duplicate("NODOS=[");
	estructuraNodo* nodo;
	for (; pos < tamanio; pos++){
		nodo = list_get(fs.estructuraNodos.nodos, pos);
		string_append(&nodos, nodo->nombreNodo);
		string_append(&nodos, ", ");
	}
	fputs(nodos, archivo);
	fputc('\n', archivo);
	string_append(&nodos, "]");
	char* nodoLibre;
	char* nodoTotal;
	for (pos = 0; pos < tamanio; pos++){
		nodoLibre = string_new();
		nodoTotal = string_new();
		nodo = list_get(fs.estructuraNodos.nodos, pos);
		nodo->tamanioLibreNodo = nodo->tamanioTotalNodo;
		string_append(&nodoLibre, nodo->nombreNodo);
		string_append(&nodoLibre, "Libre=");
		string_append(&nodoLibre, string_itoa(nodo->tamanioLibreNodo));
		string_append(&nodoTotal, nodo->nombreNodo);
		string_append(&nodoTotal, "Total=");
		string_append(&nodoTotal, string_itoa(nodo->tamanioTotalNodo));
		fputs(nodoLibre, archivo);
		fputc('\n', archivo);
		fputs(nodoTotal, archivo);
		fputc('\n', archivo);
	}
	fclose(archivo);
	free(nodoLibre);
	free(nodoTotal);
	free(nodos);
	free(tamanioLibreFs);
	free(tamanioFs);
}

void eliminarNodos(estructuraFs* fs){
	int pos = 0;
	int tamanio = list_size(fs->nodos);
	for (; pos < tamanio; pos++){
		estructuraNodo* nodo = list_get(fs->nodos, pos);
		char* nombreArchivo = string_new();
		string_append(&nombreArchivo, "/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/bitmaps/");
		char* Nodo = string_new();
		string_append(&Nodo, nodo->nombreNodo);
		string_to_lower(Nodo);
		string_append(&nombreArchivo, Nodo);
		string_append(&nombreArchivo,".dat");
		vaciarBitMap(nombreArchivo);
		free(nombreArchivo);
		free(Nodo);
	}
	guardarFsEnArchivo();
	char* touch = string_new();
	char* rm = string_new();
	string_append(&rm, "rm -r /home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/directorio.dat");
	string_append(&touch, "touch /home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/directorio.dat");
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
			int finalizo = system(rm);
			if (finalizo == 0){
				path = string_split(argumentos[cantidadDeElementos-1], "/");
				int cantidadDeElementosPath = cantidadElementos(path);
				eliminarUnDirectorio(fs.directorio, path);
				printf("Se eliminó el directorio %s \n",path[cantidadDeElementosPath - 1]);
			}else if (finalizo == 256){

			}
			guardarDirectorioEnMemoria(fs.directorio);
			free(rm);
			free(path);
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
		guardarArchivoEnArchivo(estructuraArchivo, pathArchivo);
		printf("%s \n %s \n %s \n",pathArchivo, numeroBloque, numeroCopia);
		printf("Se elimino el nodo '%s'\n",*argumentos);
		free(pathArchivo);
		free(numeroBloque);
		free(numeroCopia);
		free(estructuraArchivo);
	}
	else
	{
	if(string_starts_with(*argumentos, "/"))
		{
		path = string_split(arg, "/");
		char* rm = string_new();
		string_append(&rm, "rm -d ");
		string_append(&rm, argumentos[cantidadDeElementos - 1]);
		vaciarArchivo(argumentos[0]);
		int finalizo = system(rm);
		if (finalizo == 0){
			path = string_split(arg, "/");
			int cantidadDeElementosPath = cantidadElementos(path);
			printf("Archivo Eliminado %s\n",path[cantidadDeElementosPath - 1]);
		}else if (finalizo == 256){

		}
		free(rm);
		free(path);
		}
	else{
		printf("Archivo Eliminado '%s'\n", *argumentos);
		}
	}
	free(argumentos);
}

void fs_rename(char * arg){
	char** argumentos;
	argumentos = string_split(arg, " ");
	char* rename = string_new();
	string_append(&rename, "mv ");

	if (string_starts_with(argumentos[0], "/") && string_starts_with(argumentos[1], "/")){
		int pos = 0;
		char** path = string_split(argumentos[0], "/");
		int cantidadParametrosPath = cantidadElementos(path);
		char** path1 = string_split(argumentos[1], "/");
		int cantidadParametrosPath1 = cantidadElementos(path1);
		cantidadParametrosPath--;
		cantidadParametrosPath1--;
		if (cantidadParametrosPath == cantidadParametrosPath1){
			while (string_equals_ignore_case(path[pos], path1[pos]) && pos < cantidadParametrosPath){
				pos++;
			}
			if (pos == cantidadParametrosPath){
				string_append(&rename, argumentos[0]);
				string_append(&rename, " ");
				string_append(&rename, argumentos[1]);
				int finalizo = system(rename);
				if(finalizo == 0){
					char** nombre = string_split(argumentos[0], "/");
					char** nombre1 = string_split(argumentos[1], "/");
					int cantidad = cantidadElementos(nombre);
					int cantidad1 = cantidadElementos(nombre1);
					modificarDirectorio(fs.directorio, nombre[cantidad-1], nombre1[cantidad1-1]);
					guardarDirectorioEnMemoria(fs.directorio);
					free(nombre);
					free(nombre1);
					printf("Se renombro de %s a %s \n", argumentos[0], argumentos[1]);
				}else if(finalizo == 256){

				}else{

				}
			}else{
				printf("No se puede renombrar, ya que los path no coinciden \n");
			}
		}else{
			printf("No se puede renombrar, ya que los path no coinciden \n");
		}
	}else{
		printf("Uno de los path no comienza con /");
	}
	free(rename);
}

void fs_mv(char * arg){
	char** argumentos;
	argumentos = string_split(arg, " ");

	if (string_starts_with(argumentos[0], "/") && string_starts_with(argumentos[1], "/")){
		char** path = string_split(argumentos[0], "/");
		int cantidadParametrosPath = cantidadElementos(path);
		cantidadParametrosPath--;
		char** path1 = string_split(argumentos[1], "/");
		int cantidadParametrosPath1 = cantidadElementos(path1);
		cantidadParametrosPath1--;

		if (string_equals_ignore_case(path[cantidadParametrosPath], path1[cantidadParametrosPath1]) ){
			char* rename = string_new();
			string_append(&rename, "mv ");
			string_append(&rename, argumentos[0]);
			string_append(&rename, " ");
			string_append(&rename, argumentos[1]);
			system(rename);
			printf("Se movio de %s a %s", argumentos[0], argumentos[1]);
			free(rename);
		}else{
			printf("El nombre no coincide");
		}
	}else{
		printf("Uno de los argumentos no comienza con /");
	}
}

//DESARROLLANDO
void fs_cat(char * arg){
	char** path;
	int cantidadDeElementos = cantidadElementos(arg);
	if (string_starts_with(arg, "/")){
//		t_archivo* archivo = malloc(sizeof(t_archivo));
//		cargarTablaArchivo(archivo,arg);
//		informacionNodoBloque(archivo);
//		vaciarListaNodo(archivo->bloques);
//		free(archivo);
		path = string_split(arg, "/");
		printf("Archivo a mostrar '%s'\n",path[cantidadDeElementos - 1]);
	}
	else
	{
		printf("Path ingresado, no valido %s \n",arg);
	}
	free(path);
}

void fs_mkdir(char * arg){
	if (string_starts_with(arg, "/")){
		char* mkdir = string_new();
		string_append(&mkdir, "mkdir ");
		string_append(&mkdir, arg);
		int finalizo = system(mkdir);
		char** path = string_split(arg, "/");
		int cantidadDeElementos = cantidadElementos(path);

		if(finalizo == 0){
			agregarPath(fs.directorio, arg);
			guardarDirectorioEnMemoria(fs.directorio);
			printf("Directorio creado '%s'\n",path[cantidadDeElementos - 1]);
		}else if (finalizo == 256){
				printf("Existe el directorio %s \n", path[cantidadDeElementos - 1]);
			}else{
				printf("Error");
			}
	}
	else
	{
		printf("Permiso denegado, no se puede crear el directorio '%s'\n",arg);
	}
}

//FALTA DESARROLLAR
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

//FALTA DESARROLLAR
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

//FALTA DESARROLLAR
void fs_cpblok(char * arg){
	char** argumentos;
	argumentos = string_split(arg, " ");
	printf("Archivo a copiar '%s'\n", argumentos[0]);
	printf("Del bloque '%s' .\n", argumentos[1]);
	printf("Copia creada en el nodo '%s'\n", argumentos[2]);
}

//FALTA DESARROLLAR
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
	if( string_starts_with(arg, "/")){
		char* ls = string_duplicate("ls ");
		string_append(&ls, arg);
		system(ls);
		free(ls);
	}else{
		printf("El path no comienza con / ");
	}
}

//FALTA DESARROLLAR
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
		}else if(autenticacion == YAMA){
			enviarMensajeConProtocolo(socket, string_itoa(YAMA_OKCONN), YAMA_OKCONN); //CONFIRMO CONEXION Y RECIBO LA INFO
			s_yama = socket;
			//GENERAMOS UN THREAD PARA ATENDER A YAMA
			pthread_t t_yama;
			pthread_create(&t_yama,NULL,(void*)&atender_yama, NULL);
		}
	}
}

void iniciar_servidor(){
	pthread_create(&t_espera_data_nodes,NULL,(void*)&esperar_conexiones, NULL);
}

int set_bloque(int id_nodo,char* datos,int bloque){
	t_nodo* encontrado = malloc(sizeof(t_nodo));
	encontrado = find_nodo_by_id(id_nodo);
	if (keep_alive(encontrado->socket)==0){
		printf("Grabo un bloque en el nodo sockeft %d \n",encontrado->socket);
		char* bloqueS = string_itoa(bloque);
		char* mensaje = generar_string_separador2(bloqueS, datos, ";");
		enviarMensajeConProtocolo(encontrado->socket, mensaje, DN_SETBLOQUE);
		return EXIT_SUCCESS;
	}else{//TODO aca puedo manejar la desconeccion de nodos.
		return EXIT_FAILURE;
	}
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

t_nodo *find_nodo_by_id(int id) {
	int i;
	//t_link_element *puntTabla = estados->head;

	for(i=0; i<list_size(lista_dataNodes); i++){
		//t_estado *estado = puntTabla->data;
		t_nodo *nodo = list_get(lista_dataNodes, i);
		if(nodo->id == id)
		{
			return nodo;
		}
	}
	return EXIT_FAILURE;
}

char* get_bloque(int id_nodo, int bloque){
	t_nodo* encontrado = malloc(sizeof(t_nodo));
	encontrado = find_nodo_by_id(id_nodo);
	if (keep_alive(encontrado->socket)==0){
		char* bloqueS = string_itoa(bloque);
		enviarMensajeConProtocolo(encontrado->socket,bloqueS,DN_GETBLOQUE);
		char* respuesta = esperarMensaje(encontrado->socket);
		return respuesta;
	}else{//TODO aca puedo manejar la desconeccion de nodos.
		return "ERROR";
	}
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
	int sobrante = mensaje_size - (cantidad_bloques*bloque_size);
	if (sobrante >0){
		return (cantidad_bloques +1);
	}
	return cantidad_bloques;
}

t_list* cortar_texto(char* mensaje) {
	t_list* lista = list_create();
	char** palabras = string_split(mensaje,"\n");
	int i = 0;
	int ocupado = 0;
	int cant_palabras = str_array_size(palabras);
	for (i=0;i<cant_palabras;i++){
		string_append(&palabras[i],"\n");
	}
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
		}else{
			printf("la palabra es muy grande \n");
		}
	}
	list_add(lista,(void*) bloque);
	return lista;
}

t_list* cortar_binario(char* mensaje) {
	t_list* lista = list_create();
	int i = 0;
	int j=0;
	int c_bloques = size_in_bloks(mensaje);
	for (i=0; i<c_bloques;i++){
		char* bloque = calloc(bloque_size, sizeof(char));
		for (j=0;j<bloque_size-1;j++){
			memcpy(bloque + j,mensaje+(i*bloque_size) + j,1);
		}
		list_add(lista,(void*) bloque);
	}
	return lista;
}

void atender_yama(){
//	while (1){
//		int answ = recibirProtocolo(s_yama);
//		if(answ == YM_FS_RUTA){
//			char* ruta_archivo= esperarMensaje(s_yama);
//			t_archivo* archivo = malloc(sizeof(t_archivo));
//			char* ruta_metadata = obtener_ruta_metadata(ruta);
//			cargarTablaArchivo(archivo, ruta);
//			archivo->bloques->
//		}
//	}
//}
//int guardar_archivo_yamafs(char* archivo, ){
//
}
