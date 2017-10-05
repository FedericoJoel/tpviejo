#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>

//ARCHIVO//

typedef struct{
	char* nombre;
	int bloque;
	char* nombreCopia;
	int bloqueCopia;
	int tamanioBloque;
}estructuraBloque;

typedef struct{
	estructuraBloque bloqueCompuesto;
	struct listaBloqueArchivo * sig;
}listaBloqueArchivo;

typedef struct{
	int tamanio;
	char* tipo;
	listaBloqueArchivo bloques;
}t_archivo;

void mostrarTablaArchivo();
char* generarBloqueCopia(int bloqueNumero, int copiaNumero, char* bloqueClave, char* copiaClave);
char* generarBloqueBytes(int bloqueNumero, char* bloqueClave, char* bytes);
void sacarDatos(char* linea, char* bloqueCopia, char* bloqueCopia1, char* bloqueBytes, int* posActual);
void dameTamanio(char* linea, int* tamanio, int* pos);
void dameTipo(char* linea, char* tipo, int* pos);
int contarCaracteres(char* linea, int pos);
void dameInfoBloqueNodo(char* linea, char* bloqueCopia, char* bloqueCopia1, int* pos, char* nodoCopia, char* nodoCopia1);
void separarNodoCopia(char* linea, char* bloqueCopia, int* pos, char* nodoCopia);
void dameInfoBloqueBytes(char* linea, char* bloqueBytes, int* bytes, int* pos);
char* tomarPalabra(char* linea, int* pos);

#endif /* ARCHIVO_H_ */
