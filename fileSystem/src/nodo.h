#ifndef NODO_H_
#define NODO_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/bitarray.h>

void mostrarNodos();
char* sacar (char* palabra, char* caracter);
void cargarBitmap();
void cargarLinea(int largo, char* linea, t_bitarray *bitArray);
void imprimirEstado (t_bitarray *bitArray, int pos);

#endif /* NODO_H_ */
