#ifndef NODO_H_
#define NODO_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <pthread.h>
#include <lib.h>

typedef struct{
	char* nombreNodo;
	int tamanioTotalNodo;
	int tamanioLibreNodo;
}estructuraNodo;

void vaciarBitMap(char* path);
int buscarPosicionLibre(t_bitarray* bitArray);
void ocuparPosicionDeBitArray(t_bitarray* bitArray, int posLibre);
void escribirBitArrayEnArchivo(t_bitarray* bitArray, char* nodo);
int ordenarNodo(estructuraNodo* nodo1, estructuraNodo* nodo2);
int ordenarNodoPorEspacioLibre(estructuraNodo* nodo1, estructuraNodo* nodo2);
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

//DEVUELVE EL VALOR DE UNA POSICION DETERMINADA
void posicion(t_bitarray* array, int pos);


#endif /* NODO_H_ */
