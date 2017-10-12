#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>

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
	estructuraBloque bloqueCompuesto;
	struct listaBloqueArchivo * sig;
}listaBloqueArchivo;

typedef struct{
	int tamanio;
	char* tipo;
	t_list* bloques;
}t_archivo;

void cargarTablaArchivo();
void cargarDatos(char* lineaDatos,char* lineaElem, t_archivo* nuevoArchivo, int cantidadCampos);

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

#endif /* ARCHIVO_H_ */
