#include "archivo.h"

/*char* rutaArchivo =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/archivos/5/ejemplo.csv";
*/
void eliminarNodoDeArchivo(t_archivo* archivo, int id){
	char* nodoNumero = string_itoa(id);
	char* nodoNombre = string_new();
	string_append(&nodoNombre, "Nodo");
	string_append(&nodoNombre, nodoNumero);

	free(nodoNumero);

	int pos = 0;
	int cantidadBloques = list_size(archivo->bloques);

	for(pos = 0; pos < cantidadBloques; pos++){
		estructuraBloque* bloque = malloc(sizeof(estructuraBloque));
		int modifico = 0;
		bloque = list_get(archivo->bloques, pos);
		if (string_equals_ignore_case(dameNodo(bloque->nodoBloque), nodoNombre)){
			bloque->nodoBloque = string_duplicate("VACIO");
			modifico = 1;
		}
		if (string_equals_ignore_case(dameNodo(bloque->nodoBloque1), nodoNombre)){
			bloque->nodoBloque1 = string_duplicate("VACIO");
			modifico = 1;
		}
		if (modifico == 1){
			list_replace(archivo->bloques, pos,(void*) bloque);
		}
//		free(bloque);
	}
	free(nodoNombre);
}

char* dameNodo(char* nodoBloque){
	char* nodo = string_new();
	char** listaArg = string_split(nodoBloque, ",");
	string_append(&nodo, listaArg[0]);
	char* nodoObtenido = sacar(nodo, "[");
	return nodoObtenido;
}

void eliminarBloqueDeArchivo(t_archivo* archivo, char* numeroBloque, char* numeroCopia){
	int pos = 0;
	int cantidadBloques = list_size(archivo->bloques);
	char* nodoBloqueASacar = string_new();
	string_append(&nodoBloqueASacar, "BLOQUE");
	string_append(&nodoBloqueASacar, numeroBloque);
	string_append(&nodoBloqueASacar, "COPIA");
	string_append(&nodoBloqueASacar, numeroCopia);

	for(pos = 0; pos < cantidadBloques; pos++){
		estructuraBloque* bloque = malloc(sizeof(estructuraBloque));
		bloque = list_get(archivo->bloques, pos);
		if (string_equals_ignore_case(bloque->bloqueCopia, nodoBloqueASacar)){
			if (!(string_equals_ignore_case(bloque->nodoBloque1, "VACIO")) ){
				char** nodoBloque = string_split(bloque->nodoBloque, ",");
				char* nodo = string_new();
				nodo = sacar(nodoBloque[0], "[");
				int idNodo = obtenerId(nodo);
				char* bloque = string_new();
				bloque = sacar(nodoBloque[1], "]");
				bloque = sacar(bloque, " ");

				borrarNodo(idNodo, atoi(bloque));

				eliminarNodoDeArchivo(archivo, idNodo);
				free(nodo);
				free(bloque);
			}else{
				printf("No se puede borrrar, es la ultima copia");
			}
		}
		if (string_equals_ignore_case(bloque->bloqueCopia1, nodoBloqueASacar)){
			if (!(string_equals_ignore_case(bloque->nodoBloque, "VACIO")) ){
				char** nodoBloque1 = string_split(bloque->nodoBloque1, ",");
				char* nodo = string_new();
				nodo = sacar(nodoBloque1[0], "[");
				int idNodo = obtenerId(nodo);
				char* bloque = string_new();
				bloque = sacar(nodoBloque1[1], "]");
				bloque = sacar(bloque, " ");

				borrarNodo(idNodo, atoi(bloque));

				eliminarNodoDeArchivo(archivo, idNodo);
				free(nodo);
				free(bloque);
			}else{
				printf("No se puede borrrar, es la ultima copia");
			}
		}
	}
	free(nodoBloqueASacar);

}

void cargarTablaArchivo(t_archivo* nuevoArchivo, char* rutaArchivo) {
	FILE * archivo = fopen(rutaArchivo, "r");
	char * lineaElem = string_new();
	char * lineaDatos = string_new();
	size_t len = 0;
	int cantidad = 0;

	if (getline(&lineaElem, &len, archivo) != EOF){
		cantidad = contarCampos(lineaElem);
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
	nuevoBloque->bloqueCopia1 = string_duplicate(copiaBloque);
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
