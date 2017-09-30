#include "nodo.h"

char* rutaNodo =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/fileSystem/src/nodos.bin";

void mostrarNodos() {
	/*	FILE * archivo;
	 char * linea = NULL;
	 size_t len = 0;
	 archivo = fopen(rutaNodo, "r");
	 getline(&linea, &len, archivo);
	 */
	char* nodo;
	int pos = 0;
	t_config* config = config_create(rutaNodo);
	int tamanioFs = config_get_int_value(config, "TAMANIO");
	int tamanioTotalNodo;
	int tamanioLibreNodo;
	printf("TAMANIO = %d \n", tamanioFs);
	int espacioLibreFs = config_get_int_value(config, "LIBRE");
	printf("LIBRE = %d \n", espacioLibreFs);
	char* nodos = config_get_string_value(config, "NODOS");
	char** listaNodos = string_split(nodos, ",");
	printf("NODOS = %s \n", nodos);
	while ((nodo = listaNodos[pos]) != NULL) {
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
		tamanioTotalNodo = config_get_int_value(config, nodoTotal);
		tamanioLibreNodo = config_get_int_value(config, nodoLibre);
		printf("%s = %d \n", nodoTotal, tamanioTotalNodo);
		printf("%s = %d \n", nodoLibre, tamanioLibreNodo);
		free(nodoLibre);
		free(nodoTotal);
		pos++;

	}
}
char* sacar(char* palabra, char* caracter) {
	char** palabraN;
	palabraN = string_split(palabra, caracter);
	return palabraN[0];
}
