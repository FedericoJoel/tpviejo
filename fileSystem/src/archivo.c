#include "archivo.h"

char* rutaArchivo =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/fileSystem/src/ejemplo.txt";

void cargarTablaArchivo() {
	FILE * archivo = fopen(rutaArchivo, "r");
	char * lineaElem = string_new();
	char * lineaDatos = string_new();
	size_t len = 0;
	t_archivo nuevoArchivo;
	nuevoArchivo.bloques = list_create();
	int cantidad = 0;

	if (getline(&lineaElem, &len, archivo) != EOF){
		cantidad = contarCampos(lineaElem);
		if (getline(&lineaDatos, &len, archivo) != EOF){
			cargarDatos(lineaDatos, lineaElem, &nuevoArchivo, cantidad);
		}
	}
	fclose(archivo);
}

void cargarDatos(char* lineaDatos, char* lineaElem, t_archivo* nuevoArchivo, int cantidadCampos){
	int pos = 0;
	int elemento = 0;
	char* palabra;

	while(cantidadCampos > 0){
		palabra = tomarPalabra(lineaElem, &pos);
		if (string_equals_ignore_case(palabra, "TAMANIO")){
			int tamanio = dameTamanio(lineaDatos, elemento);
			nuevoArchivo->tamanio = tamanio;
			cantidadCampos--;
		}else{
			if (string_equals_ignore_case(palabra, "TIPO")){
				char* tipo = dameTipo(lineaDatos, elemento);
				nuevoArchivo->tipo = tipo;
				cantidadCampos--;
				free(tipo);
			}else{
				char* nodoBloque;
				estructuraBloque* nuevoBloque = malloc(sizeof nuevoBloque);
				nodoBloque = dameBloque(lineaDatos, elemento);
				(*nuevoBloque).bloqueCopia = palabra;
				(*nuevoBloque).nodoBloque = nodoBloque;
				cantidadCampos--;

				elemento++; //avanza a la copia
				pos++; //esquiva la coma que separa la copia de bloques

				palabra = tomarPalabra(lineaElem, &pos);
				nodoBloque = dameBloque(lineaDatos, elemento);
				(*nuevoBloque).bloqueCopia1 = palabra;
				(*nuevoBloque).nodoBloque1 = nodoBloque;
				cantidadCampos--;

				elemento++; //avanza a el tamaño del bloque
				pos++; //esquiva la coma que separa la copia de bloques

				palabra = tomarPalabra(lineaElem, &pos);
				int tamanioBloque = dameTamanio(lineaDatos, elemento);
				(*nuevoBloque).nombreTamanioBloque = palabra;
				(*nuevoBloque).tamanioBloque = tamanioBloque;
				cantidadCampos--;

				list_add((*nuevoArchivo).bloques, (void*) nuevoBloque);

				free(nodoBloque);
			}
		}
		elemento++;
		pos++; //esquiva la ,
	}
//	free(palabra);
}

char* dameBloque (char* linea, int elemento){
	int pos = dameElemento(linea, elemento);
	char* palabra = tomarPalabra(linea, &pos);
	return palabra;
}

int contarCampos(char* linea){
	int largo = 0;
	int cantidad = 0;
	int a = string_length(linea);
	for(;a - largo > 0;){
		largo = largo + contarCaracteres(linea, largo);
		largo++; //pasa la posicion de la ,
		cantidad++;
	}

	return cantidad;
}

int dameTamanio(char* linea, int elemento) {

	int tamanio;
	int pos = dameElemento(linea, elemento);
	char* palabra = tomarPalabra(linea, &pos);
	tamanio = atoi(palabra);
//	free(palabra);
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
	char* palabra = string_new();

	if ((caracteresTamanio = contarCaracteres(linea, numero)) != -1) {
		for (; caracteresTamanio != 0 && linea[numero] != '='; numero++, posAux++, caracteresTamanio--) {
			palabra[posAux] = linea[numero];
		}
		palabra[posAux] = '\0';
		if (linea[numero] == ','){
			*pos = numero;
			return palabra;
		}else{
	return "Error";
		}
	}else{return "Algo";}}
