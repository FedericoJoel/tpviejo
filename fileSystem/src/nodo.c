#include "nodo.h"

char* rutaNodo =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/nodos.bin";

char* rutaBitMap =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/bitmaps/bitArrayNodo.dat";

int buscarPosicionLibre(t_bitarray* bitArray){
	int pos = 0;
	int tamanio = bitarray_get_max_bit(bitArray) / 8;
	for (; pos < tamanio; pos++){
		if (bitarray_test_bit(bitArray, pos) == 0){
			return pos;
		}
	}
	return -1;
}

void ocuparPosicionDeBitArray(t_bitarray* bitArray, int posLibre){
	bitarray_set_bit(bitArray, posLibre);
}

void escribirBitArrayEnArchivo(t_bitarray* bitArray, char* nodo){
	int tamanio = bitarray_get_max_bit(bitArray) / 8;
	int pos = 0;
	char* lectura = string_new();
	for (; pos < tamanio; pos++){
		string_append(&lectura, string_itoa(bitarray_test_bit(bitArray, pos)));
	}
	FILE* archivo = fopen(nodo, "w");
	fputs(lectura, archivo);
	fclose(archivo);
	free(lectura);
	bitarray_destroy(bitArray);
}

void vaciarBitMap(char* path){
	char* linea = NULL;
	size_t len = 0;
	FILE * archivo = fopen(path, "r");
	getline(&linea, &len, archivo);
	int largo = string_length(linea);
	int pos = 0;
	fclose(archivo);
	archivo = fopen(path, "w");
	for(; pos < largo; pos++){
		fputc('0', archivo);
	}
	free(linea);
	fclose(archivo);
}

FILE* dameArchivo(){
	FILE * archivo = fopen(rutaBitMap, "r");
	return archivo;
}

int ordenarNodo(estructuraNodo* nodo1, estructuraNodo* nodo2){
	int id1 = obtenerId(nodo1->nombreNodo);
	int id2 = obtenerId(nodo2->nombreNodo);
	return(id1<id2);
}

int ordenarNodoPorEspacioLibre(estructuraNodo* nodo1, estructuraNodo* nodo2){
	int porcentageNodo1 = nodo1->tamanioLibreNodo*100/nodo1->tamanioTotalNodo;
	int porcentageNodo2 = nodo2->tamanioLibreNodo*100/nodo2->tamanioTotalNodo;
	return(porcentageNodo1>porcentageNodo2);
}

int obtenerId (char* nodo){
	int id = 0;
	int posicion = 1;
	int nodoLargo = 3;	//Nodo = 3
	char* palabra = string_new();
	string_append(&palabra, string_substring_from(nodo, nodoLargo+posicion));
	id = atoi(palabra);
	free(palabra);
	return id;
}

int tamanioTotalFs(){
	t_config* config = config_create(rutaNodo);
	int tamanioFs = config_get_int_value(config, "TAMANIO");
	config_destroy(config);
	return tamanioFs;
}

int existenNodos(){
	if(tamanioTotalFs()){
		return EXIT_SUCCESS;
	}else{
		return EXIT_FAILURE;
	}
}

int tamanioLibreFs(){
	t_config* config = config_create(rutaNodo);
	int espacioLibreFs = config_get_int_value(config, "LIBRE");
	config_destroy(config);
	return espacioLibreFs;
}

int cantidadNodos(){
	t_config* config = config_create(rutaNodo);
	char* nodos = config_get_string_value(config, "NODOS");
//	config_destroy(config);
	char** listaNodos = string_split(nodos, ",");

	int cantidad = cantidadElementos(listaNodos);
//	free(listaNodos);
	return cantidad;
}

estructuraNodo* levantarNodo(int posicion) {
	char* nodo = string_new();
	char* nodos = string_new();
	estructuraNodo* nodoNuevo = malloc(sizeof (estructuraNodo));

	t_config* config = config_create(rutaNodo);

	nodos = config_get_string_value(config, "NODOS");
	char** listaNodos = string_split(nodos, ",");
	string_append(&nodo, listaNodos[posicion]);
	if ( nodo != NULL) {
		char *nodoTotal = string_new();
		char *nodoLibre = string_new();

		if (string_starts_with(nodo, " ")) {
			nodo = sacar(nodo, " ");
			if (string_ends_with(nodo, "]")) {
				nodo = sacar(nodo, "]");
			}
		} else {
			nodo = sacar(nodo, "[");
		}

		string_append(&nodoTotal, nodo);
		string_append(&nodoTotal, "Total");
		string_append(&nodoLibre, nodo);
		string_append(&nodoLibre, "Libre");
		int tamanioTotalNodo = config_get_int_value(config, nodoTotal);
		int tamanioLibreNodo = config_get_int_value(config, nodoLibre);

		nodoNuevo->nombreNodo = string_duplicate(nodo);
		nodoNuevo->tamanioTotalNodo = tamanioTotalNodo;
		nodoNuevo->tamanioLibreNodo = tamanioLibreNodo;

		printf("%s = %d \n", nodoTotal, tamanioTotalNodo);
		printf("%s = %d \n", nodoLibre, tamanioLibreNodo);

		free(nodoLibre);
		free(nodoTotal);
		free(listaNodos);
		free(nodos);
		free(nodo);
		return nodoNuevo;
	}else{
		nodoNuevo->nombreNodo = string_duplicate("Vacio");
		return nodoNuevo;
	}
}


t_bitarray* cargarBitmapAMemoria(char* path){
	size_t len = 0;
	off_t pos = 0;
	FILE * archivo = fopen(path, "r");
	char * linea = NULL;
		if ((getline(&linea, &len, archivo)) != EOF){
			size_t largo = string_length(linea);
			t_bitarray* bitArray = bitarray_create_with_mode(linea, largo, LSB_FIRST);
			for (pos = 0; pos < largo; pos++){
				cargarLinea(pos, linea, bitArray);
			}
			return bitArray;
		}
		return NULL;
}

void cargarLinea(int pos, char* linea, t_bitarray* bitArray){
	char* copia = string_duplicate(linea);
	if (copia[pos] == '1'){
		bitarray_set_bit(bitArray, pos);
	}else{
		bitarray_clean_bit(bitArray, pos);
	}
	free(copia);
}

void imprimirEstado (t_bitarray *bitArray, off_t pos){
	if (bitarray_test_bit(bitArray, pos))
		printf("Ocupado \n");
	else{
		printf("Libre \n");
	}
}

void borrarNodo(int idNodo, int idBloque){
	printf("Bloque %d borrado, del nodo %d", idBloque, idNodo);
}

void posicion(t_bitarray* array, int pos){
	bitarray_test_bit(array, pos);
}
