#include "fileSystem.h"

int s_servidor;
int s_yama;
int s_clientes[clientes_max];
t_list *lista_dataNodes;
pthread_t* t_atiende_dn;
pthread_t t_espera_data_nodes;
int bloque_size=1048576;

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
//	int cant_bloques = size_in_bloks("ESTO FUNCIONA ESPECTACULAR;;JAJAJA");


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
//	iniciar_servidor();
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
			break;
		}
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

//TERMINADO
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

//TERMINADO
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
			int finalizo = comprobarDirectorio(fs.directorio, argumentos[cantidadDeElementos - 1]);
			if (finalizo == 0){
				int vacio = carpetaVacia(fs.directorio, argumentos[cantidadDeElementos - 1]);
				if (vacio == 0){
					path = string_split(argumentos[cantidadDeElementos-1], "/");
					int cantidadDeElementosPath = cantidadElementos(path);
					eliminarUnDirectorio(fs.directorio, path);
					printf("Se eliminó el directorio %s \n",path[cantidadDeElementosPath - 1]);
				}else{
					path = string_split(argumentos[cantidadDeElementos-1], "/");
					int cantidadDeElementosPath = cantidadElementos(path);
					printf("El directorio no se encuentra vacio %s \n", path[cantidadDeElementosPath - 1]);
				}
			}else{
				printf("El directorio no existe");
			}
			guardarDirectorioEnMemoria(fs.directorio);
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
		vaciarArchivo(argumentos[0]);
			path = string_split(arg, "/");
			int cantidadDeElementosPath = cantidadElementos(path);
			printf("Archivo Eliminado %s\n",path[cantidadDeElementosPath - 1]);
		free(path);
		}
	else{
		printf("Archivo Eliminado '%s'\n", *argumentos);
		}
	}
	free(argumentos);
}

//TERMINADO
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
				char** archivo = string_split(path[cantidadParametrosPath], ".");
				char** archivo1 = string_split(path1[cantidadParametrosPath1], ".");
				if(cantidadElementos(archivo) == 1 && cantidadElementos(archivo1) == 1){
					if (comprobarDirectorio(fs.directorio, argumentos[0]) == 0 &&
							comprobarDirectorio(fs.directorio, argumentos[1]) == 0){
						modificarDirectorio(fs.directorio, path[cantidadParametrosPath], path1[cantidadParametrosPath1]);
						guardarDirectorioEnMemoria(fs.directorio);
						printf("Se renombro de %s a %s \n", argumentos[0], argumentos[1]);
					}else{
						printf("Uno de los directorios no existe");
				}
				}else if(cantidadElementos(archivo) == 2 && cantidadElementos(archivo1) == 2){
					char* ruta = obtener_ruta_metadata(argumentos[0]);
					char** listaNueva = string_split(ruta, "/");
					int cantidad = cantidadElementos(listaNueva);
					cantidad--;
					int largoTotal = string_length(ruta);
					int largoArchivo = string_length(listaNueva[cantidad]);
					int largoRuta = largoTotal - largoArchivo;
					char* nuevoNombre = string_new();
					string_append(&nuevoNombre, path1[cantidadParametrosPath1]);
					int largoNuevoNombre = string_length(path1[cantidadParametrosPath1]) - 3; //SACA LA EXTENCION .TXT INGRESADA POR CONSOLA
					char* nuevaRuta = calloc(largoRuta+largoNuevoNombre+1, sizeof(char));
					memcpy(nuevaRuta, ruta, largoRuta);
					memcpy(nuevaRuta+largoRuta, nuevoNombre, largoNuevoNombre);
					string_append(&rename, ruta);
					string_append(&rename, " ");
					string_append(&nuevaRuta, "csv");
					string_append(&rename, nuevaRuta);
					system(rename);
					printf("Se cambio el archivo de %s a %s", path[cantidadParametrosPath], path1[cantidadParametrosPath1]);
					free(nuevaRuta);
					free(nuevoNombre);
					free(ruta);
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

char* obtener_ruta_metadata(char* ruta){
	char** lista = string_split(ruta, "/");
	int cantidadDeElementos = cantidadElementos(lista);
	cantidadDeElementos--;
	char* nombre = string_duplicate("/");
	string_append(&nombre, lista[cantidadDeElementos-1]);
	int posicion = damePosicionDeElemento(nombre, fs.directorio);
	char* index = string_itoa(posicion);
	char* rutaNueva = string_duplicate("/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/archivos/");
	string_append(&rutaNueva, index);
	string_append(&rutaNueva, "/");
	char** listaNueva = string_split(lista[cantidadDeElementos], ".");
	string_append(&rutaNueva, listaNueva[0]);
	string_append(&rutaNueva, ".csv");
	free(index);
	free(nombre);
	return rutaNueva;
}

//TERMINADO
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

		if (cantidadElementos(string_split(path[cantidadParametrosPath], ".")) == 1){
		//AFECTA A DIRECTORIO
			char* rutaVieja = string_duplicate("/");
			char* rutaNueva = string_duplicate("/");
			string_append(&rutaVieja, path[cantidadParametrosPath]);
			string_append(&rutaNueva, path1[cantidadParametrosPath1]);
			if (comprobarDirectorio(fs.directorio, argumentos[0]) == 0 &&
					comprobarDirectorio(fs.directorio, argumentos[1]) == 0){
				char* directorioNuevo = string_duplicate("/");
				string_append(&directorioNuevo, path[cantidadParametrosPath]);
				int posicionNueva = damePosicionDeElemento(directorioNuevo, fs.directorio);
				char* directorioPadre = string_duplicate("/");
				string_append(&directorioPadre, path1[cantidadParametrosPath1]);
				int posicionPadre = damePosicionDeElemento(directorioPadre, fs.directorio);
				fs.directorio[posicionNueva].padre = posicionPadre;
				guardarDirectorioEnMemoria(fs.directorio);
				printf("Se movio de %s a %s", argumentos[0], argumentos[1]);
				free(directorioPadre);
				free(directorioNuevo);
				free(rutaNueva);
				free(rutaVieja);
			}else{
				free(rutaNueva);
				free(rutaVieja);
				printf("Algun directorio no existe");
			}
		}else if (cantidadElementos(string_split(path[cantidadParametrosPath], ".")) == 2){
			//AFECTA A ARCHIVO
			int largoTotal = string_length(argumentos[0]);
			int largoArchivo = string_length(path[cantidadParametrosPath]);
			int largoRuta = largoTotal - largoArchivo - 1; //1 x la ultima barra
			char* ruta = calloc(largoRuta, sizeof(char));
			memcpy(ruta, argumentos[0],largoRuta);
			if (comprobarDirectorio(fs.directorio, argumentos[1]) == 0 &&
					comprobarDirectorio(fs.directorio, ruta) == 0){
				char* mv = string_duplicate("mv ");
				char* rutaVieja = obtener_ruta_metadata(argumentos[0]);
				string_append(&mv, rutaVieja);
				char* nuevoDirectorio = string_duplicate("/");
				string_append(&nuevoDirectorio, path1[cantidadParametrosPath1]);
				int posicionNueva = damePosicionDeElemento(nuevoDirectorio, fs.directorio);
				char* rutaNueva = string_duplicate("/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/archivos/");
				string_append(&rutaNueva, string_itoa(fs.directorio[posicionNueva].index));
				string_append(&mv, " ");
				string_append(&mv, rutaNueva);
				system(mv);
				free(mv);
				free(nuevoDirectorio);
				free(rutaNueva);
				free(rutaVieja);
			}else{
				printf("Uno de los directorios no existe");
			}
			free(ruta);
		}

	}else{
		printf("Uno de los argumentos no comienza con /");
	}
}

//DESARROLLADO
void fs_cat(char * arg){
	if (string_starts_with(arg, "/")){
		char* directorio = string_new();
		char** argumentosSinBarra = string_split(arg, "/");
		int cantidadDeElementos = cantidadElementos(argumentosSinBarra);
		int pos = 0;
		for(; pos < cantidadDeElementos-1; pos++){
			string_append(&directorio, "/");
			string_append(&directorio, argumentosSinBarra[pos]);
		}
		if (comprobarDirectorio(fs.directorio, directorio) == 0){
			char* informacion = obtenerInformacionDeBloquesDeUnArchivo(arg);
			printf("Archivo a mostrar '%s'\n",argumentosSinBarra[cantidadDeElementos - 1]);
			printf("%s \n", informacion);
			free(informacion);
		}else{ printf("No existe el directorio");}
	free(directorio);
	}else{
		printf("Path ingresado, no valido %s \n",arg);
	}
}

char* obtenerInformacionDeBloquesDeUnArchivo(char* rutaArchivo){
	t_archivo* archivo = malloc(sizeof(t_archivo));
	char* rutaMetadata = obtener_ruta_metadata(rutaArchivo);
	cargarTablaArchivo(archivo,rutaMetadata);
	char* informacion = informacionNodoBloque(archivo);
	vaciarListaNodo(archivo->bloques);
	free(archivo);
	return informacion;
}

void vaciarListaNodo(t_list* nodos){
	int pos = 0;
	int tamanio = list_size(nodos);
	for(; pos < tamanio; pos++){
		estructuraNodo* nodo = list_get(nodos, pos);
		free(nodo);
	}
}

void fs_mkdir(char * arg){
	if (string_starts_with(arg, "/")){
		char** path = string_split(arg, "/");
		int cantidadDeElementos = cantidadElementos(path);
		agregarPath(fs.directorio, arg);
		guardarDirectorioEnMemoria(fs.directorio);
		printf("Se creo el directorio %s \n", path[cantidadDeElementos -1]);
	}
	else
	{
		printf("Permiso denegado, no se puede crear el directorio '%s'\n",arg);
	}
}

//FALTA DESARROLLAR BLOQUE0BYTES Y SETEO EN NODO
void fs_cpfrom(char * arg){
	char** argumentos;
	argumentos = string_split(arg, " ");

	if (string_starts_with(argumentos[0], "/") && string_starts_with(argumentos[1], "/")){
		if (comprobarDirectorio(fs.directorio, argumentos[1]) == 0){
			char** elementosPath = string_split(argumentos[0], ".");
			if (cantidadElementos(elementosPath) == 2){
				if (string_equals_ignore_case(elementosPath[cantidadElementos(elementosPath) - 1], "bin") || string_equals_ignore_case(elementosPath[cantidadElementos(elementosPath) - 1],"txt")){
					char* elementos = string_new();
					char* informacion = string_new();
					double tamanioArchivo = tomarTamanioDeArchivo(argumentos[0]);
					int tamanioArchivoEntero = tamanioArchivo;
					string_append(&elementos, "TAMANIO");
					string_append(&informacion, string_itoa(tamanioArchivoEntero));
					string_append(&informacion, ",");
					string_append(&elementos, ",TIPO,");
					char * linea = NULL;
					size_t len = 0;
					FILE* archivoLeido = fopen(argumentos[0], "r");
					int leido = 0;
					char* informacionArchivo = string_new();
					while((leido = getline(&linea, &len, archivoLeido)) != -1){
						string_append(&informacionArchivo, linea);
						linea = NULL;
						len = 0;
					}
					t_list* listaBloques;
					if (string_equals_ignore_case(elementosPath[cantidadElementos(elementosPath) - 1], "bin")){
						string_append(&informacion, "BINARIO,");
//						listaBloques = cortar_datos()
					}else{
						string_append(&informacion, "TEXTO,");
						listaBloques = cortar_texto(informacionArchivo);
					}

					int bloquesAUsar = list_size(listaBloques);
					int cantidad = 0;
					generarBloqueCopiaBytes(&elementos, cantidad);
					int posicionLista = 0;
					char* nodosOcupados = ocuparBloques(argumentos[0], list_get(listaBloques, posicionLista));
					int tamanioUsadoBloque = string_length(list_get(listaBloques,posicionLista));
					string_append(&informacion, nodosOcupados);
					string_append(&informacion, ",");
					string_append(&informacion, string_itoa(tamanioUsadoBloque));
					string_append(&informacion, ",");
					cantidad++;
					posicionLista++;
					while(cantidad < bloquesAUsar){
						string_append(&elementos, ",");
						generarBloqueCopiaBytes(&elementos, cantidad);
						nodosOcupados = ocuparBloques(argumentos[0], list_get(listaBloques,posicionLista));
						string_append(&informacion, nodosOcupados);
						string_append(&informacion, ", ");
						tamanioUsadoBloque = string_length(list_get(listaBloques,posicionLista));
						string_append(&informacion, nodosOcupados);
						string_append(&informacion, ",");
						string_append(&informacion, string_itoa(tamanioUsadoBloque));
						string_append(&informacion, ",");
						cantidad++;
						posicionLista++;
//						nodosOcupados = ocuparBloques(argumentos[0]);
//(						string_append(&informacion, nodosOcupados);
						}
						printf("%s \n", elementos);
						printf("%s \n", informacion);
						free(nodosOcupados);
					elementosPath = string_split(argumentos[0], "/");
					char* directorio = string_duplicate("/");
					char** rutaYamaFS = string_split(argumentos[1], "/");
					string_append(&directorio, rutaYamaFS[cantidadElementos(rutaYamaFS)-1]);
					int posicionDirectorio = damePosicionDeElemento(directorio, fs.directorio);
					char* rutaCsv = string_duplicate("/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/archivos/");
					string_append(&rutaCsv, string_itoa(posicionDirectorio));
					string_append(&rutaCsv,"/");
					char** nombreDeArchivo = string_split(elementosPath[cantidadElementos(elementosPath)-1], ".");
					string_append(&rutaCsv,nombreDeArchivo[cantidadElementos(nombreDeArchivo)-2]);
					string_append(&rutaCsv, ".csv");
					free(nombreDeArchivo);
					free(rutaYamaFS);
					string_append(&elementos, "\n");
					string_append(&informacion, "\n");
					FILE* archivoNuevo = fopen(rutaCsv, "w");
					fputs(elementos, archivoNuevo);
					fputs(informacion, archivoNuevo);
					printf("Archivo %s copiado a YAMA: '%s'\n", elementosPath[cantidadElementos(elementosPath) - 1], argumentos[1]);
					fclose(archivoNuevo);
					free(directorio);
					free(elementos);
					free(informacion);
				}else{
					printf("No se especifico tipo de archivo");
					}
			}else{
				printf("El path del fs no es un archivo valido");
			}
			free(elementosPath);
		}else{
			printf("El path de YAMA no existe");
		}
	}else{
		printf("Alguno de los path no empieza con /: %s \n %s \n", argumentos[0], argumentos[1]);
	}
	free(argumentos);
}

void generarBloqueCopiaBytes(char** elementos, int cantidad){
	int copia = 0;
	string_append(elementos, "BLOQUE");
	string_append(elementos, string_itoa(cantidad));
	string_append(elementos,"COPIA");
	string_append(elementos, string_itoa(copia));
	string_append(elementos, ",");
	string_append(elementos,"BLOQUE");
	string_append(elementos, string_itoa(cantidad));
	string_append(elementos,"COPIA");
	copia++;
	string_append(elementos, string_itoa(copia));
	string_append(elementos, ",");
	string_append(elementos,"BLOQUE");
	string_append(elementos, string_itoa(cantidad));
	string_append(elementos,"BYTES");
}

char* ocuparBloques(char* ruta, char* contenidoBloque){
	int pos = 0;
	list_sort(fs.estructuraNodos.nodos, (void*) ordenarNodoPorEspacioLibre);
	estructuraNodo* nodo1 = list_get(fs.estructuraNodos.nodos, pos);
	nodo1->tamanioLibreNodo--;
	pos++;
	estructuraNodo* nodo2 = list_get(fs.estructuraNodos.nodos, pos);
	nodo2->tamanioLibreNodo--;
	char* nodosUsados = string_duplicate("[");
	string_append(&nodosUsados, nodo1->nombreNodo);
	string_append(&nodosUsados, ", ");
	char* rutaBitMap1 = string_duplicate("/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/bitmaps/");
	char* nodoNombreMinisculo1 = string_duplicate(nodo1->nombreNodo);
	string_to_lower(nodoNombreMinisculo1);
	string_append(&rutaBitMap1, nodoNombreMinisculo1);
	string_append(&rutaBitMap1,".dat");
	t_bitarray* array1 = cargarBitmapAMemoria(rutaBitMap1);
	int posLibre1 = buscarPosicionLibre(array1);
	ocuparPosicionDeBitArray(array1, posLibre1);
	escribirBitArrayEnArchivo(array1, rutaBitMap1);
	int idNodo1 = obtenerId(nodo1->nombreNodo);
	set_bloque(idNodo1, contenidoBloque, posLibre1);
	posLibre1++;
	string_append(&nodosUsados, string_itoa(posLibre1));
	string_append(&nodosUsados, "],");
	string_append(&nodosUsados, "[");
	string_append(&nodosUsados, nodo2->nombreNodo);
	string_append(&nodosUsados, ", ");
	char* rutaBitMap2 = string_duplicate("/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/bitmaps/");
	char* nodoNombreMinisculo2 = string_duplicate(nodo2->nombreNodo);
	string_to_lower(nodoNombreMinisculo2);
	string_append(&rutaBitMap2, nodoNombreMinisculo2);
	string_append(&rutaBitMap2, ".dat");
	t_bitarray* array2 = cargarBitmapAMemoria(rutaBitMap2);
	int posLibre2 = buscarPosicionLibre(array2);
	ocuparPosicionDeBitArray(array2, posLibre2);
	escribirBitArrayEnArchivo(array2, rutaBitMap2);
	int idNodo2 = obtenerId(nodo2->nombreNodo);
	set_bloque(idNodo2, contenidoBloque, posLibre2);
	posLibre2++;
	string_append(&nodosUsados, string_itoa(posLibre2));
	string_append(&nodosUsados, "]");
	free(rutaBitMap1);
	free(rutaBitMap2);
	free(nodoNombreMinisculo1);
	free(nodoNombreMinisculo2);
	return nodosUsados;
}

int tomarTamanioDeArchivo(char* ruta){
	struct stat st;
	if (stat(ruta, &st) == 0){
	   	return st.st_size;
	}else{
		return -1;
	}
}

//DESARROLLADO
void fs_cpto(char * arg){
	char** argumentos;
	if (string_starts_with(arg, "/")){
		argumentos = string_split(arg, " ");
		char** argumentosSinBarra = string_split(argumentos[0], "/");
		int cantidadDeElementos = cantidadElementos(argumentosSinBarra);
		int pos = 0;
		char* directorio = string_new();
		for(; pos < cantidadDeElementos-1; pos++){
			string_append(&directorio, "/");
			string_append(&directorio, argumentosSinBarra[pos]);
		}
		char** esArchivo = string_split(argumentosSinBarra[cantidadDeElementos-1], ".");
		if (comprobarDirectorio(fs.directorio, directorio) == 0 && cantidadElementos(esArchivo) == 2){
			char* nuevaRuta = string_duplicate(argumentos[1]);
			string_append(&nuevaRuta, "/");
			string_append(&nuevaRuta, argumentosSinBarra[cantidadDeElementos - 1]);
			FILE* archivo = fopen(nuevaRuta, "w");
			char* informacion = obtenerInformacionDeBloquesDeUnArchivo(argumentos[0]);
			fputs(informacion, archivo);
			fclose(archivo);
			free(informacion);
			free(nuevaRuta);
			printf("Archivo copiado a: %s \n", argumentos[1]);
		}else if (comprobarDirectorio(fs.directorio, directorio) != 0){
			printf("No existe el directorio");
		}else{
			printf("El path no es un archivo");
		}
		free(argumentosSinBarra);
		free(directorio);
		free(esArchivo);
	}
	else
	{
		printf("La ruta no es valida \n");
	}
	free(argumentos);
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

//TERMINADO
//TODO actualmente se muestran los archivos .csv, habria que ver si se puede mostrar como el tipo de archivo que es
void fs_ls(char * arg){
	if( string_starts_with(arg, "/")){
		if (comprobarDirectorio(fs.directorio, arg) == 0){
			char** argumentos = string_split(arg, "/");
			int cantidadArgumentos = cantidadElementos(argumentos);
			char* ruta = string_duplicate("/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/archivos");
			char* directorio = string_duplicate("/");
			string_append(&directorio, argumentos[cantidadArgumentos-1]);
			int posicion = damePosicionDeElemento(directorio, fs.directorio);
			string_append(&ruta,"/");
			string_append(&ruta, string_itoa(fs.directorio[posicion].index));
			char* ls = string_duplicate("ls ");
			string_append(&ls, ruta);
			system(ls);
			free(ls);
			free(ruta);
			free(directorio);
			free(argumentos);
		}else{printf("No existe el directorio");}
	}else{
		printf("El path no comienza con / ");
	}
}

//TERMINADO
void fs_info(char * arg){
	if (string_starts_with(arg, "/")){
		char** argumentosSinBarra = string_split(arg, "/");
		int cantidadDeElementos = cantidadElementos(argumentosSinBarra);
		int pos = 0;
		char* directorio = string_new();
		for(; pos < cantidadDeElementos-1; pos++){
			string_append(&directorio, "/");
			string_append(&directorio, argumentosSinBarra[pos]);
		}
		char** esArchivo = string_split(argumentosSinBarra[cantidadDeElementos-1], ".");
		if (comprobarDirectorio(fs.directorio, directorio) == 0 && cantidadElementos(esArchivo) == 2){
			char* ruta = obtener_ruta_metadata(arg);
			t_archivo* archivo = malloc(sizeof(t_archivo));
			cargarTablaArchivo(archivo, ruta);
			printf("Información del archivo: '%s'\n", argumentosSinBarra[cantidadDeElementos - 1]);
			mostrarArchivo(archivo, ruta);
			free(archivo);
			free(ruta);
		}else if(comprobarDirectorio(fs.directorio, directorio) != 0){
				printf("No exise el path");
			}else{
				printf("El path no es de un archivo");
			}
		free(esArchivo);
		free(directorio);
		free(argumentosSinBarra);
	}
	else
	{
	printf("No es un path valido \n");
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

void actualizarArchivo(char* ruta){
	t_archivo* archivo = malloc(sizeof(t_archivo));
	cargarTablaArchivo(archivo, ruta);
	int tamanio = list_size(archivo->bloques);
	int pos = 0;
	for(;pos < tamanio; pos++){
		estructuraBloque* bloque = list_get(archivo->bloques, pos);
		if (nodoVive(bloque->nodoBloque) == EXIT_FAILURE){
			char* nuevaEscritura = string_duplicate(bloque->nodoBloque);
			string_append(&nuevaEscritura, "CAIDO");
			bloque->nodoBloque = nuevaEscritura;
		}
		if (nodoVive(bloque->nodoBloque1) == EXIT_FAILURE){
			char* nuevaEscritura = string_duplicate(bloque->nodoBloque1);
			string_append(&nuevaEscritura, "CAIDO");
			bloque->nodoBloque1 = nuevaEscritura;
		}
	}
	guardarArchivoEnArchivo(archivo, ruta);
}

int nodoVive(char* nodoBloque){
	char* nodo = dameNodo(nodoBloque);
	int idNodo = obtenerId(nodo);
	if(keep_alive(idNodo) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}else{
		return EXIT_SUCCESS;
	}
}
