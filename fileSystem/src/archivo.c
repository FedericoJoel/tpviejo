#include "archivo.h"

char* rutaArchivo =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/fileSystem/src/ejemplo.txt";

void cargarTablaArchivo(t_archivo* nuevoArchivo) {
	FILE * archivo = fopen(rutaArchivo, "r");
	char * lineaElem = string_new();
	char * lineaDatos = string_new();
	size_t len = 0;
	nuevoArchivo->bloques = list_create();
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
	estructuraBloque* nuevoBloque = malloc(sizeof nuevoBloque);
	(*nuevoArchivo).bloques = list_create();

	while(cantidadCampos > 0){
		char* palabra = string_new();
		palabra = tomarPalabra(lineaElem, &pos);
		if (string_equals_ignore_case(palabra, "TAMANIO")){
			int tamanio = dameTamanio(lineaDatos, elemento);
			nuevoArchivo->tamanio = tamanio;
			cantidadCampos--;
			free(palabra);
		}else{
			if (string_equals_ignore_case(palabra, "TIPO")){
				char* tipo = string_new();
				tipo = dameTipo(lineaDatos, elemento);
				nuevoArchivo->tipo = tipo;
				cantidadCampos--;
				free(tipo);
				free(palabra);
			}else{
				char* nodoBloque = string_new();

				nuevoBloque->bloqueCopia = string_new();
				nuevoBloque->bloqueCopia = string_duplicate(palabra);
				nodoBloque = dameBloque(lineaDatos, elemento);
				nuevoBloque->nodoBloque = string_new();
				nuevoBloque->nodoBloque = string_duplicate(nodoBloque);
				cantidadCampos--;

				elemento++; //avanza a la copia
				pos++; //esquiva la coma que separa la copia de bloques

				palabra = tomarPalabra(lineaElem, &pos);
				nodoBloque = dameBloque(lineaDatos, elemento);
				nuevoBloque->bloqueCopia1 = string_new();
				nuevoBloque->bloqueCopia1 = string_duplicate(palabra);
				nodoBloque = dameBloque(lineaDatos, elemento);
				nuevoBloque->nodoBloque1 = string_new();
				nuevoBloque->nodoBloque1 = string_duplicate(nodoBloque);
				cantidadCampos--;

				elemento++; //avanza a el tamaño del bloque
				pos++; //esquiva la coma que separa la copia de bloques

				palabra = tomarPalabra(lineaElem, &pos);
				int tamanioBloque = dameTamanio(lineaDatos, elemento);
				nuevoBloque->nombreTamanioBloque = string_new();
				nuevoBloque->nombreTamanioBloque = string_duplicate(palabra);
				nuevoBloque->tamanioBloque = tamanioBloque;
				cantidadCampos--;

				list_add(nuevoArchivo->bloques, (void*) nuevoBloque);
				free(palabra);
				free(nodoBloque);
			}
		}
		elemento++;
		pos++; //esquiva la ,
	}

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
