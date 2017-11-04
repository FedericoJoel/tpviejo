#include "lib.h"  /* Include the header (not strictly necessary here) */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <commons/log.h>
#include <commons/string.h>

//---------------logger-------------------

t_log* abrir_logger(char* ruta_archivo, char* nombre_programa,
		t_log_level nivel) {
	return log_create(ruta_archivo, nombre_programa, 1, nivel);
}

char* bloque_archivo_to_string(t_bloque_archivo* bloque) {
	char* char_bloque = string_new();

	char* char_bloque_archivo;
	char_bloque_archivo = int_to_string(bloque->bloque_archivo);
	char *char_bytes = int_to_string(bloque->bytes);
	char* char_ruta_temporal = bloque->ruta_temporal;
	char* char_copia0 = copia_to_string(bloque->copia0);
	char* char_copia1 = copia_to_string(bloque->copia1);
	char* char_elegida = int_to_string(bloque->elegida);

	string_append(&char_bloque, char_bloque_archivo);
	string_append(&char_bloque, char_bytes);
	char* tamanio = int_to_string(strlen(char_ruta_temporal));
	string_append(&char_bloque, tamanio);
	free(tamanio);
	string_append(&char_bloque, char_ruta_temporal);
	tamanio = int_to_string(strlen(char_copia0));
	string_append(&char_bloque, tamanio);
	free(tamanio);
	string_append(&char_bloque, char_copia0);
	tamanio = int_to_string(strlen(char_copia1));
	string_append(&char_bloque, tamanio);
	free(tamanio);
	string_append(&char_bloque, char_copia1);
	string_append(&char_bloque, char_elegida);

	free(char_bloque_archivo);
	free(char_bytes);
	free(char_ruta_temporal);
	free(char_copia0);
	free(char_copia1);
	free(char_elegida);
	return char_bloque;
}

t_bloque_archivo* bloque_archivo_from_string(char* char_bloque) {
	t_bloque_archivo* bloque = malloc(sizeof(t_bloque_archivo));
	int puntero;
	puntero = 0;

	char* char_bloque_archivo = extraer_string(char_bloque, puntero, puntero + 3);
	bloque->bloque_archivo = atoi(char_bloque_archivo);
	puntero += 4;

	char* char_bytes = extraer_string(char_bloque, puntero, puntero + 3);
	bloque->bytes = atoi(char_bytes);
	puntero += 4;


	char* char_tamanio_temporal = extraer_string(char_bloque, puntero, puntero + 3);
	int tamanio_temporal = atoi(char_tamanio_temporal);
	puntero += 4;

	char* char_archivo_temporal = extraer_string(char_bloque, puntero, puntero + tamanio_temporal -1);
	bloque->ruta_temporal = string_from_format(char_archivo_temporal);
	puntero += tamanio_temporal;


	char* char_tamanio_copia0 = extraer_string(char_bloque, puntero, puntero + 3);
	int tamanio_copia0 = atoi(char_tamanio_copia0);
	puntero += 4;

	char* char_copia0 = extraer_string(char_bloque, puntero, puntero + tamanio_copia0 - 1);
	bloque->copia0 = copia_from_string(char_copia0);
	puntero += tamanio_copia0;


	char* char_tamanio_copia1 = extraer_string(char_bloque, puntero, puntero + 3);
	int tamanio_copia1 = atoi(char_tamanio_copia1);
	puntero += 4;

	char* char_copia1 = extraer_string(char_bloque, puntero, puntero + tamanio_copia1 - 1);
	bloque->copia1 = copia_from_string(char_copia1);
	puntero += tamanio_copia1;


	char* char_elegida = extraer_string(char_bloque, puntero, puntero + 3);
		bloque->elegida = atoi(char_elegida);

	return bloque;
}


t_copia* copia_from_string(char* char_copia) {
	t_copia* nueva_copia = malloc(sizeof(t_copia));
	int puntero;
	puntero = 0;

	char* char_nodo = extraer_string(char_copia, puntero, puntero + 3);
	nueva_copia->nodo = atoi(char_nodo);
	puntero += 4;

	char* char_bloque_nodo = extraer_string(char_copia, puntero, puntero + 3);
	nueva_copia->bloque_nodo = atoi(char_bloque_nodo);
	puntero += 4;

	char* char_tamanio_ip = extraer_string(char_copia, puntero, puntero + 3);
	puntero += 4;
	int tamanio_ip = atoi(char_tamanio_ip);
	char* char_ip = extraer_string(char_copia, puntero, puntero + tamanio_ip);
	nueva_copia->ip = string_from_format(char_ip);

	free(char_nodo);
	free(char_bloque_nodo);
	free(char_tamanio_ip);
	free(char_ip);

	return nueva_copia;
}


char* copia_to_string(t_copia* copia) {
	char* char_copia = string_new();
	char* char_nodo = int_to_string(copia->nodo);
	char* char_bloque = int_to_string(copia->bloque_nodo);
	char* ip = copia->ip;
	int ip_len = strlen(ip);

	string_append(&char_copia, char_nodo);
	string_append(&char_copia, char_bloque);
	string_append(&char_copia, int_to_string(ip_len));
	string_append(&char_copia, ip);

	free(char_nodo);
	free(char_bloque);
	free(ip);

	return char_copia;
}


char* int_to_string(int numero) {
	char* longitud = string_new();
	char* num_char = string_itoa(numero);
	char* rev = string_reverse(num_char);
	string_append(&longitud, rev);
	free(num_char);
	free(rev);
	string_append(&longitud, "0000");
	longitud = string_substring(longitud, 0, 4);
	longitud = string_reverse(longitud);
	return longitud;
}

char* extraer_string(char* string, int inicio, int fin) {
	if (fin < inicio) {
		char* r = string_new();
		string_append(&r, "\0");
		return r;
	}
	return (string_substring(string, inicio, 1 + fin - inicio));
}


char* generar_string_separador2(char* str1, char* str2, char* separador){
	char* string = string_new();
	string_append(&string,str1);
	string_append(&string,separador);
	string_append(&string,str2);
	return string;
}

void string_append_separador(char** original, char* string_to_add,char* separador) {
	string_append(original,separador);
	string_append(original,string_to_add);
}

char** array_string_new(int size){
	char** array = calloc(size,sizeof(char*));
	return array;
}

char* sacar(char* palabra, char* caracter) {
	char** palabraN = string_split(palabra, caracter);
	return palabraN[0];
}

int cantidadElementos(char ** array){
    size_t count = 0;
    while (array[count] != NULL) count++;
    return count;
}
