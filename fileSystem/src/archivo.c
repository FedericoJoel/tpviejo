#include "archivo.h"

/*char* rutaArchivo =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/archivos/5/ejemplo.csv";
*/

void vaciarArchivo(char* path){
	t_archivo* estructuraArchivo = malloc(sizeof(t_archivo));
	cargarTablaArchivo(estructuraArchivo, path);
	int largo = list_size(estructuraArchivo->bloques);
	int pos = 0;
	for (; pos < largo; pos++){
		estructuraBloque* nuevoBloque = list_get(estructuraArchivo->bloques, pos);
		if (!(string_equals_ignore_case(nuevoBloque->nodoBloque, "VACIO"))){
			char* nodo = string_new();
			char* Nodo = string_new();
			string_append(&nodo, "/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/bitmaps/");
			char* bloque = string_new();
			char** argumentos = string_split(nuevoBloque->nodoBloque, ",");
			string_append(&Nodo, sacar(argumentos[0], "["));
			string_to_lower(Nodo);
			string_append(&nodo, Nodo);
			string_append(&nodo, ".dat");
			t_bitarray* bitArray = cargarBitmapAMemoria(nodo);
			bloque = sacar(argumentos[1], "]");
			int posicion = atoi(bloque);
			free(bloque);
			posicion--;
			bitarray_clean_bit(bitArray, posicion);
			escribirBitArrayEnArchivo(bitArray, nodo);
			free(nodo);
			free(Nodo);
			free(argumentos);
		}
		if (!(string_equals_ignore_case(nuevoBloque->nodoBloque1, "VACIO"))){
			char* nodo = string_new();
			char* Nodo = string_new();
			string_append(&nodo, "/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/bitmaps/");
			char* bloque = string_new();
			char** argumentos = string_split(nuevoBloque->nodoBloque1, ",");
			string_append(&Nodo, sacar(argumentos[0], "["));
			string_to_lower(Nodo);
			string_append(&nodo, Nodo);
			string_append(&nodo, ".dat");
			t_bitarray* bitArray = cargarBitmapAMemoria(nodo);
			bloque = sacar(argumentos[1], "]");
			int posicion = atoi(bloque);
			free(bloque);
			posicion--;
			bitarray_clean_bit(bitArray, posicion);
			escribirBitArrayEnArchivo(bitArray, nodo);
			free(nodo);
			free(Nodo);
			free(argumentos);
		}
	}
}

void vaciarListaNodos(t_list* lista){
	int tamanio = list_size(lista);
	int pos = 0;
	for (;pos < tamanio; pos++){
		estructuraNodo* nodo = list_get(lista,pos);
		free(nodo);
	}
}

char* informacionNodoBloque(t_archivo* archivo){
	int pos = 0;
	int tamanio = list_size(archivo->bloques);
	char* informacion = string_new();
	for(; pos < tamanio; pos++){
		estructuraBloque* estructuraBloqueDeLista = list_get(archivo->bloques, pos);
		if(!(string_equals_ignore_case(estructuraBloqueDeLista->nodoBloque, "VACIO")) ||
				!(string_equals_ignore_case(estructuraBloqueDeLista->nodoBloque1, "VACIO"))){
			char* nodoALeer;
			if(!(string_equals_ignore_case(estructuraBloqueDeLista->nodoBloque, "VACIO"))){
				nodoALeer = string_duplicate(estructuraBloqueDeLista->nodoBloque);
			}else{
				nodoALeer = string_duplicate(estructuraBloqueDeLista->nodoBloque1);
			}
			char** palabras = string_split(nodoALeer, ",");
			char* nodo = sacar(palabras[0], "[");
			int idNodo = obtenerId(nodo);
			char* bloque = sacar(palabras[1], "]");
			int idBloque = atoi(bloque);
			int tamanio = estructuraBloqueDeLista->tamanioBloque;

			string_append(&informacion, get_bloque(idNodo, idBloque));

			free(nodoALeer);
			free(palabras);
			free(nodo);
			free(bloque);
		}
	}
	return informacion;
}

void eliminarNodoDeArchivo(t_archivo* archivo, int idNodo, int idBloque){
	char* nodoNumero = string_itoa(idNodo);
	char* nodoNombre = string_new();
	string_append(&nodoNombre, "nodo");
	string_append(&nodoNombre, nodoNumero);
	free(nodoNumero);

	char* path = string_duplicate("/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/bitmaps/");
	string_append(&path, nodoNombre);
	string_append(&path, ".dat");
	t_bitarray* bitArray = cargarBitmapAMemoria(path);
	bitarray_clean_bit(bitArray, idBloque-1);
	escribirBitArrayEnArchivo(bitArray, path);
	free(path);
	free(nodoNombre);
}

void guardarArchivoEnArchivo(t_archivo* archivo, char* path){
	char* elementos = string_duplicate("TAMANIO,TIPO");
	char* datos = string_duplicate(string_itoa(archivo->tamanio));
	int pos = 0;
	int cantidadBloques = list_size(archivo->bloques);
	string_append(&datos, ",");
	string_append(&datos, archivo->tipo);
	estructuraBloque* bloque;
	for(; pos < cantidadBloques; pos++){
		bloque = list_get(archivo->bloques, pos);
		string_append(&datos, ",");
		string_append(&elementos, ",");
		string_append(&elementos, bloque->bloqueCopia);
		string_append(&elementos, ",");
		string_append(&datos, bloque->nodoBloque);
		string_append(&datos, ",");
		string_append(&elementos, bloque->bloqueCopia1);
		string_append(&elementos, ",");
		string_append(&datos, bloque->nodoBloque1);
		string_append(&datos, ",");
		string_append(&elementos, bloque->nombreTamanioBloque);
		string_append(&datos, string_itoa(bloque->tamanioBloque));
	}
	FILE* archivoModificado = fopen(path, "w");
	fputs(elementos, archivoModificado);
	fputc('\n', archivoModificado);
	fputs(datos, archivoModificado);
	fputc('\n', archivoModificado);
	fclose(archivoModificado);
}

char* dameNodo(char* nodoBloque){
	char* nodo = string_new();
	char** listaArg = string_split(nodoBloque, ",");
	string_append(&nodo, listaArg[0]);
	char* nodoObtenido = sacar(nodo, "[");
	return nodoObtenido;
}

void eliminarBloqueDeArchivo(t_archivo* archivo, char* numeroBloque, char* numeroCopia) {
	int pos = 0;
	int cantidadBloques = list_size(archivo->bloques);
	char* nodoBloqueASacar = string_new();
	string_append(&nodoBloqueASacar, "BLOQUE");
	string_append(&nodoBloqueASacar, numeroBloque);
	string_append(&nodoBloqueASacar, "COPIA");
	string_append(&nodoBloqueASacar, numeroCopia);

	for(pos = 0; pos < cantidadBloques; pos++){
		estructuraBloque* bloque = list_get(archivo->bloques, pos);
		if (string_equals_ignore_case(bloque->bloqueCopia, nodoBloqueASacar) ){
			if(!(string_equals_ignore_case(bloque->nodoBloque1, "VACIO"))){
				char** nodoBloque = string_split(bloque->nodoBloque, ",");
				char* nodo = string_new();
				nodo = sacar(nodoBloque[0], "[");
				int idNodo = obtenerId(nodo);
				char* bloqueChar = string_new();
				bloqueChar = sacar(nodoBloque[1], "]");
				bloqueChar = sacar(bloqueChar, " ");
				int bloqueNumero = atoi(bloqueChar);

				borrarNodo(idNodo, bloqueNumero);

				eliminarNodoDeArchivo(archivo, idNodo, bloqueNumero);
				bloque->nodoBloque = string_duplicate("VACIO");
				list_replace(archivo->bloques, pos,(void*) bloque);
				free(nodo);
				free(bloqueChar);
				break;
			}else{
				printf("No se puede borrrar, es la ultima copia \n");
				break;
			}
		}else{
			if (!(string_equals_ignore_case(bloque->nodoBloque, "VACIO"))){
				char** nodoBloque1 = string_split(bloque->nodoBloque1, ",");
				char* nodo = string_new();
				nodo = sacar(nodoBloque1[0], "[");
				int idNodo = obtenerId(nodo);
				char* bloqueChar = string_new();
				bloqueChar = sacar(nodoBloque1[1], "]");
				bloqueChar = sacar(bloqueChar, " ");

				borrarNodo(idNodo, atoi(bloqueChar));

				eliminarNodoDeArchivo(archivo, idNodo, atoi(bloqueChar));
				bloque->nodoBloque1 = string_duplicate("VACIO");
				list_replace(archivo->bloques, pos,(void*) bloque);
				free(nodo);
				free(bloqueChar);
				break;
			}else{
				printf("No se puede borrrar, es la ultima copia \n");
				break;
			}
		}
	}
	free(nodoBloqueASacar);
}

void mostrarArchivo(t_archivo* nuevoArchivo, char* rutaArchivo){
	FILE * archivo = fopen(rutaArchivo, "r");
	char * lineaElem = NULL;
	char * lineaDatos = NULL;
	size_t len = 0;
	int cantidad = 0;

	if (getline(&lineaElem, &len, archivo) != EOF){
		printf("%s \n", lineaElem);
		len=0;
		if (getline(&lineaDatos, &len, archivo) != EOF){
			printf("%s \n", lineaDatos);
		}
	}
	fclose(archivo);
	free(lineaDatos);
	free(lineaElem);
}

void cargarTablaArchivo(t_archivo* nuevoArchivo, char* rutaArchivo) {
	FILE * archivo = fopen(rutaArchivo, "r");
	char * lineaElem = NULL;
	char * lineaDatos = NULL;
	size_t len = 0;
	int cantidad = 0;

	if (getline(&lineaElem, &len, archivo) != EOF){
		cantidad = contarCampos(lineaElem);
		len=0;
		if (getline(&lineaDatos, &len, archivo) != EOF){
			cargarDatos(lineaDatos, lineaElem, nuevoArchivo, cantidad);
		}
	}
	fclose(archivo);
	free(lineaDatos);
	free(lineaElem);
}

void cargarDatos(char* lineaDatos, char* lineaElem, t_archivo* nuevoArchivo, int cantidadCampos){
	int pos = 0;
	int elemento = 0;
	nuevoArchivo->bloques = list_create();

	while(cantidadCampos > 0){
		char* palabra = string_new();
		string_append(&palabra, tomarPalabra(lineaElem, &pos));
		if (string_equals_ignore_case(palabra, "TAMANIO")){
			int tamanio = dameTamanio(lineaDatos, elemento);
			nuevoArchivo->tamanio = tamanio;
			cantidadCampos--;
			free(palabra);
		}else{
			if (string_equals_ignore_case(palabra, "TIPO")){
				char* tipo = string_new();
				tipo = dameTipo(lineaDatos, elemento);
				nuevoArchivo->tipo = string_duplicate(tipo);
				cantidadCampos--;
				free(tipo);
				free(palabra);
			}else{
//				estructuraBloque* nuevoBloque = malloc(sizeof (estructuraBloque));
				char* nodoBloque = string_new();

				nodoBloque = dameBloque(lineaDatos, elemento);
				cantidadCampos--;
				elemento++; //avanza a la copia
				pos++; //esquiva la coma que separa la copia de bloques

				char* nodoBloque1 = string_new();
				char* copiaBloque1 = string_new();
				string_append(&copiaBloque1, tomarPalabra(lineaElem, &pos));
				nodoBloque1 = dameBloque(lineaDatos, elemento);

				elemento++; //avanza a el tamaño del bloque
				pos++; //esquiva la coma que separa la copia de bloques
				cantidadCampos--;

				char* tamanioBytes = string_new();
				string_append(&tamanioBytes, tomarPalabra(lineaElem, &pos));
				int tamanioBloque = dameTamanio(lineaDatos, elemento);
				cantidadCampos--;

				list_add(nuevoArchivo->bloques, (void*) crearBloque(palabra, copiaBloque1, nodoBloque, nodoBloque1,
						tamanioBytes, tamanioBloque));

				free(nodoBloque);
				free(palabra);
				free(nodoBloque1);
				free(copiaBloque1);
				free(tamanioBytes);

//				free(nuevoBloque);
			}
		}
		elemento++;
		pos++; //esquiva la ,
	}
}

estructuraBloque* crearBloque(char* copiaBloque, char* copiaBloque1, char* nodoBloque,
		char* nodoBloque1, char* tamanioBytes,
		int tamanioBloque){
	estructuraBloque* nuevoBloque = malloc(sizeof(estructuraBloque));
	nuevoBloque->bloqueCopia = string_duplicate(copiaBloque);
	nuevoBloque->nodoBloque = string_duplicate(nodoBloque);
	nuevoBloque->bloqueCopia1 = string_duplicate(copiaBloque1);
	nuevoBloque->nodoBloque1 = string_duplicate(nodoBloque1);
	nuevoBloque->nombreTamanioBloque = string_duplicate(tamanioBytes);
	nuevoBloque->tamanioBloque = tamanioBloque;
	return nuevoBloque;
}

char* dameBloque (char* linea, int elemento){
	int pos = dameElemento(linea, elemento);
	char* palabra = tomarPalabra(linea, &pos);
	return palabra;
}

int contarCampos(char* linea){
	int largo = 0;
	int cantidad = 0;
	int caracteres = string_length(linea);
	for(;caracteres - largo > 0;){
		largo = largo + contarCaracteres(linea, largo);
		largo++; //pasa la posicion de la ,
		cantidad++;
	}

	return cantidad;
}

int dameTamanio(char* linea, int elemento) {

	int tamanio = -1;
	int pos = dameElemento(linea, elemento);
	char* palabra = tomarPalabra(linea, &pos);
	tamanio = atoi(palabra);
	free(palabra);
	return tamanio;
}

int dameElemento(char* linea, int elemento){
	int contador = 0;
	if(elemento == 0)
		return contador;
	while(elemento > 0){
		contador = contador + contarCaracteres(linea, contador);
		contador++; //esquiva la ,
		elemento--;
	}
	return contador;
}

int contarCaracteres(char* linea, int pos) {
	int cantidad = 0;

	if (linea[pos] == '['){
		int contadorComa = 0;
		while ((contadorComa < 2) && (linea[pos] != '\0') && (linea[pos] != '\n')){
			if (linea[pos] == ','){
				if (contadorComa < 1){
					contadorComa++;
					cantidad++;
					pos++;
				}else{break;}
			}else{
				cantidad++;
				pos++;
			}
		}
	}else {
		while ((linea[pos] != ',') && (linea[pos] != '\0') && (linea[pos] != '\n')){
			cantidad++;
			pos++;
		}
	}
	if (linea[pos] == ',' || (linea[pos] != '\0') || (linea[pos] != '\n'))
		return cantidad;
	else {
		return -1;
		}
}

char* dameTipo(char* linea, int elemento) {
	int pos = dameElemento(linea, elemento);
	char* tipo = tomarPalabra(linea, &pos);
	return tipo;
}

char* tomarPalabra(char* linea, int* pos) {
	int numero = *pos;
	int caracteresTamanio;
	int posAux = 0;


	if ((caracteresTamanio = contarCaracteres(linea, numero)) != -1) {
		char* palabra = string_repeat('\0', caracteresTamanio);
		for (; caracteresTamanio != 0 && linea[numero] != '='; numero++, posAux++, caracteresTamanio--) {
			palabra[posAux] = linea[numero];
		}
		if (linea[numero] == ',' || linea[numero] == '\n' || linea[numero] == '\0'){
			*pos = numero;
			return palabra;
		}else{
	return "Error";
		}
	}else{return "Algo";}}
