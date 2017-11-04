#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "nodo.h"

//ARCHIVO//

typedef struct{
	char* bloqueCopia;
	char* nodoBloque;
	char* bloqueCopia1;
	char* nodoBloque1;
	char* nombreTamanioBloque;
	int tamanioBloque;
}estructuraBloque;

typedef struct{
	int tamanio;
	char* tipo;
	t_list* bloques;
}t_archivo;

void cargarTablaArchivo(t_archivo* nuevoArchivo, char* rutaArchivo);
void cargarDatos(char* lineaDatos,char* lineaElem, t_archivo* nuevoArchivo, int cantidadCampos);
void eliminarNodoDeArchivo(t_archivo* archivo, int id);
void eliminarBloqueDeArchivo(t_archivo* archivo, char* numeroBloque, char* numeroCopia);

estructuraBloque* crearBloque(char* copiaBloque, char* copiaBloque1, char* nodoBloque,
		char* nodoBloque1, char* tamanioBytes, int tamanioBloque);

//devuelve el tamanio total del archivo
int dameTamanio(char* linea, int elemento);

//devuelve el tipo del archivo
char* dameTipo(char* linea, int elemento);

//cuenta caracteres separados por coma
int contarCaracteres(char* linea, int pos);

//toma la palabra separada entre comas
char* tomarPalabra(char* linea, int* pos);

//cuenta la cantidad de columnas de la tabla
int contarCampos(char* linea);

//devuelve la lista de bloques que tiene ese archivo
char* dameBloque (char* linea, int elemento);

//retorna la posicion de un elemento
int dameElemento(char* linea, int elemento);

//RETORNA EL NODO QUE POSSE UN [NODO, BLOQUE]
char* dameNodo(char* nodoBloque);

#endif /* ARCHIVO_H_ */
