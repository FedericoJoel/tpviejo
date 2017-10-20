#include "nodo.h"

char* rutaNodo =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/fileSystem/src/nodos.bin";

char* rutaBitMap =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/fileSystem/src/nodo2.dat";

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
	config_destroy(config);
	char** listaNodos = string_split(nodos, ",");
	int cantidad = cantidadElementos(listaNodos);
	free(listaNodos);
	return cantidad;
}

int cantidadElementos(char ** array){
	    size_t count = 0;
	    while (array[count] != NULL) count++;
	    return count;
}

estructuraNodo levantarNodo(int posicion) {
	char* nodo = string_new();
	char* nodos = string_new();
	estructuraNodo nodoNuevo;

	t_config* config = config_create(rutaNodo);

	nodos = config_get_string_value(config, "NODOS");
	char** listaNodos = string_split(nodos, ",");
	if ((nodo = listaNodos[posicion]) != NULL) {
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

		nodoNuevo.nombreNodo = nodo;
		nodoNuevo.tamanioTotalNodo = tamanioTotalNodo;
		nodoNuevo.tamanioLibreNodo = tamanioLibreNodo;

		printf("%s = %d \n", nodoTotal, tamanioTotalNodo);
		printf("%s = %d \n", nodoLibre, tamanioLibreNodo);

		free(nodoLibre);
		free(nodoTotal);
		free(listaNodos);
		free(nodos);
		return nodoNuevo;
	}else{
		nodoNuevo.nombreNodo = "Vacio";
		return nodoNuevo;
	}
}

char* sacar(char* palabra, char* caracter) {
	char** palabraN;
	palabraN = string_split(palabra, caracter);
	return palabraN[0];
}

void cargarBitmap(t_bitarray* bitArray){
	size_t len = 0;
	int pos = 0;
	FILE * archivo = fopen(rutaBitMap, "r");
	char * linea = NULL;
	if ((getline(&linea, &len, archivo)) != EOF){
		size_t largo = string_length(linea);
		bitArray = bitarray_create_with_mode(linea, largo, LSB_FIRST);
		for (pos = 0; pos < largo; pos++){
			cargarLinea(largo, linea, bitArray);
			imprimirEstado(bitArray, pos);
		}
	}
}

void cargarLinea(int largo, char* linea, t_bitarray* bitArray){
	int pos = 0;
	for (pos = 0; pos < largo; pos++){
		if (linea[pos] == '1'){
			bitarray_set_bit(bitArray, pos);
		}else{
			bitarray_clean_bit(bitArray, pos);
		}
	}
}

void imprimirEstado (t_bitarray *bitArray, int pos){
	if (bitarray_test_bit(bitArray, pos))
		printf("Ocupado \n");
	else{
		printf("Libre \n");
	}
}
