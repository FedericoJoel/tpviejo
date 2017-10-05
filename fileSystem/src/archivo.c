#include "archivo.h"

char* rutaArchivo =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/fileSystem/src/ejemplo.txt";

void mostrarTablaArchivo() {
	FILE * archivo = fopen(rutaArchivo, "r");
	char * linea = string_new();
	size_t len = 0;
	int bloqueNumero = 0;
	int copiaNumero;
	int tamanio = 0;
	int pos = 0;
	char* tipo = string_new();
	char* bloqueClave = string_new();
	string_append(&bloqueClave, "BLOQUE");
	char* copiaClave = string_new();
	string_append(&copiaClave, "COPIA");
	char* bloqueCopia = string_new();
	char* bloqueCopia1 = string_new();
	char* bloqueBytes = string_new();

	if (getline(&linea, &len, archivo) != EOF){
		dameTamanio(linea, &tamanio, &pos);
		dameTipo(linea, tipo, &pos);

		for(bloqueNumero = 0; copiaNumero < 100; bloqueNumero++ ){
			copiaNumero = 0;
			bloqueCopia = generarBloqueCopia(bloqueNumero, copiaNumero, bloqueClave,
					copiaClave);
			copiaNumero++;
			bloqueCopia1 = generarBloqueCopia(bloqueNumero, copiaNumero,
				bloqueClave, copiaClave);
			bloqueBytes = generarBloqueBytes(bloqueNumero, bloqueClave, "BYTES");
			sacarDatos(linea, bloqueCopia, bloqueCopia1, bloqueBytes, &pos);
		}
	}

	free(bloqueClave);
	free(copiaClave);
	free(bloqueCopia);
	free(bloqueCopia1);
	free(bloqueBytes);
}

char* generarBloqueBytes(int bloqueNumero, char* bloqueClave, char* bytes) {
	char* bloqueBytes = string_new();
	char* numero = string_new();
	string_append(&bloqueBytes, bloqueClave);
	sprintf(numero, "%d", bloqueNumero);
	string_append(&bloqueBytes, numero);
	string_append(&bloqueBytes, bytes);
	free(numero);
	return bloqueBytes;
}

char* generarBloqueCopia(int bloqueNumero, int copiaNumero, char* bloqueClave,
		char* copiaClave) {
	char* bloqueCopia = string_new();
	char* numero = string_new();
	char* numero1 = string_new();
	string_append(&bloqueCopia, bloqueClave);
	sprintf(numero, "%d", bloqueNumero);
	string_append(&bloqueCopia, numero);
	string_append(&bloqueCopia, copiaClave);
	sprintf(numero1, "%d", copiaNumero);
	string_append(&bloqueCopia, numero1);
	free(numero);
	free(numero1);
	return bloqueCopia;
}

void sacarDatos(char* linea, char* bloqueCopia, char* bloqueCopia1,
		char* bloqueBytes, int* posActual) {

	int pos = *posActual;
	int bytes = 0;
	char* nodoCopia = string_new();
	char* nodoCopia1 = string_new();

	dameInfoBloqueNodo(linea, bloqueCopia, bloqueCopia1, &pos, nodoCopia,
			nodoCopia1);
	dameInfoBloqueBytes(linea, bloqueBytes, &bytes, &pos);

	*posActual = pos;

	free(nodoCopia);
	free(nodoCopia1);
}

void dameTamanio(char* linea, int* tamanio, int* pos) {
	int numero = *pos + 8;
	int posArray = 0;
	int caracteresTamanio;

	if ((caracteresTamanio = contarCaracteres(linea, numero)) != -1) {
		char* charTamanio = string_new();

		for (; caracteresTamanio != 0; numero++) {
			if ((linea[numero] != ';') && (linea[numero] != '\0')) {
				charTamanio[posArray] = linea[numero];
				posArray++;
				caracteresTamanio--;
			} else {
				break;
			}
		}
		printf("TAMANIO: %s \n", charTamanio);
		*tamanio = atoi(charTamanio);
		free(charTamanio);
		*pos = numero;
	} else {
		printf("Fallo");
	}
}

int contarCaracteres(char* linea, int pos) {
	int cantidad = 0;
	while (linea[pos] != ';') { // && (linea[pos] != NULL)){
		cantidad++;
		pos++;
	}
	if (linea[pos] == ';')
		return cantidad;
	else {
		return -1;
	}
}

void dameTipo(char* linea, char* tipo, int* pos) {
	int caracteresTamanio;
	int caracter = 0;
	int numero = *pos + 6;
	if ((caracteresTamanio = contarCaracteres(linea, numero)) != -1) {
		tipo[caracteresTamanio] = '\0';
		for (; caracteresTamanio != 0; numero++) {
			if ((linea[numero] != ';') && (linea[numero] != '\0')) {
				tipo[caracter] = linea[numero];
				caracter++;
				caracteresTamanio--;
			} else {
				break;
			}
		}
		printf("TIPO: %s \n", tipo);
		*pos = numero;
	} else {
		printf("Fallo");
	}
}

void dameInfoBloqueNodo(char* linea, char* bloqueCopia, char* bloqueCopia1,
		int* pos, char* nodoCopia, char* nodoCopia1) {

	int nuevaPosicion = *pos;
	char* nuevoNodoCopia = string_new();
	char* palabra = string_new();
	string_append(&nuevoNodoCopia, nodoCopia);
	char* nuevoNodoCopia1 = string_new();
	string_append(&nuevoNodoCopia, nodoCopia1);

	palabra = tomarPalabra(linea, &nuevaPosicion);

	if (string_equals_ignore_case(palabra, bloqueCopia)){
	separarNodoCopia(linea, bloqueCopia, &nuevaPosicion, nuevoNodoCopia);
	string_append(&nodoCopia, nuevoNodoCopia);
	*pos = nuevaPosicion;
	palabra = tomarPalabra(linea, &nuevaPosicion);
	}
	if (string_equals_ignore_case(palabra, bloqueCopia1)){
	separarNodoCopia(linea, bloqueCopia1, &nuevaPosicion, nuevoNodoCopia1);
	string_append(&nodoCopia1, nuevoNodoCopia1);
	*pos = nuevaPosicion;
	}

	free(nuevoNodoCopia);
	free(nuevoNodoCopia1);
}

void separarNodoCopia(char* linea, char* bloqueCopia, int* pos, char* nodoCopia) {
	int numero = *pos + 1;
	int caracteresTamanio;
	int posAux = 0;
	char* nodoBloque = string_new();
	caracteresTamanio = contarCaracteres(linea, numero);
	if (linea[numero] != ';') {
		posAux = 0;
		nodoBloque[caracteresTamanio] = '\0';
		for (; caracteresTamanio != 0; numero++) {
			if ((linea[numero] != ';') && (linea[numero] != '\0')) {
				nodoBloque[posAux] = linea[numero];
				posAux++;
				caracteresTamanio--;
			} else {
				break;
			}
		}
		printf("%s: %s \n", bloqueCopia, nodoBloque);
		string_append(&nodoCopia, nodoBloque);
		*pos = numero;
	}
}

void dameInfoBloqueBytes(char* linea, char* bloqueBytes, int* bytes, int* pos) {
	int numero = *pos + 1;
	int caracteresTamanio;
	int posAux = 0;
	char* nodoBloque = string_new();

	if ((caracteresTamanio = contarCaracteres(linea, numero)) != -1) {
		for (; caracteresTamanio != 0 && linea[numero] != '='; numero++) {
			if ((linea[numero] == bloqueBytes[posAux])) {
				posAux++;
				caracteresTamanio--;
			} else {
				break;
			}
		}
		if (linea[numero] == '=') {
			posAux = 0;
			nodoBloque[caracteresTamanio] = '\0';
			numero++;
			for (; caracteresTamanio != 0; numero++) {
				if ((linea[numero] != ';') && (linea[numero] != '\0')) {
					nodoBloque[posAux] = linea[numero];
					posAux++;
					caracteresTamanio--;
				} else {
					break;
				}
			}
			printf("%s: %s \n", bloqueBytes, nodoBloque);
			*bytes = atoi(nodoBloque);
			*pos = numero;
		}
	}
}

char* tomarPalabra(char* linea, int* pos) {
	int numero = *pos + 1;
	int caracteresTamanio;
	int posAux = 0;
	char* palabra = string_new();

	if ((caracteresTamanio = contarCaracteres(linea, numero)) != -1) {
		for (; caracteresTamanio != 0 && linea[numero] != '='; numero++) {
			palabra[posAux] = linea[numero];
			posAux++;
			caracteresTamanio--;
		}
		palabra[caracteresTamanio+1] = '\0';
		if (linea[numero] == '=')
			*pos = numero;
			return palabra;
	}
	return "Error";
}
