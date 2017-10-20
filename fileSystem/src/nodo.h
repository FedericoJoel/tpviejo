#ifndef NODO_H_
#define NODO_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>


typedef struct{
	char* nombreNodo;
	int tamanioTotalNodo;
	int tamanioLibreNodo;
}estructuraNodo;

estructuraNodo levantarNodo(int pos);
char* sacar (char* palabra, char* caracter);
void cargarBitmap(t_bitarray* bitArray);
void cargarLinea(int largo, char* linea, t_bitarray *bitArray);
void imprimirEstado (t_bitarray *bitArray, int pos);
int tamanioTotalFs();
int tamanioLibreFs();
int cantidadNodos();
int cantidadElementos(char ** array);
int existenNodos();

#endif /* NODO_H_ */
