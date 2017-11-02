#ifndef NODO_H_
#define NODO_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <pthread.h>

typedef struct{
	char* nombreNodo;
	int tamanioTotalNodo;
	int tamanioLibreNodo;
}estructuraNodo;

int ordenarNodo(estructuraNodo* nodo1, estructuraNodo* nodo2);
int obtenerId (char* nodo);
estructuraNodo* levantarNodo(int pos);
char* sacar (char* palabra, char* caracter);
t_bitarray* cargarBitmapAMemoria();
void cargarLinea(int largo, char* linea, t_bitarray *bitArray);
void imprimirEstado (t_bitarray *bitArray, off_t pos);
int tamanioTotalFs();
int tamanioLibreFs();
int cantidadNodos();
int existenNodos();
void borrarNodo(int idNodo, int idBloque);

#endif /* NODO_H_ */
