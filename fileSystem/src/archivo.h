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
	char* bloqueCopia; //BLOQUE0COPIA0 nro bloque nro copia // guardo la ip del nodobloque copia en t_copia.ip
	char* nodoBloque; //[Nodo1,33] nodo 1 bloque 33 funcion dameIDNodo guardo en t_copia.nodo , damebloquedelnodo lo guardo en t_copia.bloque_nodo
	char* bloqueCopia1; //lo mismo
	char* nodoBloque1;//lo mismo
	char* nombreTamanioBloque; //BLOQUE0BYTES //de aca tengo que tomar el nro y guardarlo en t_bloque_archivo bloque_archivo
	int tamanioBloque; //TamañoBloque guardo en t_bloque_archivo bytes
}estructuraBloque;

//TODO enviar lista de t_bloque_archivo tiene la info de un bloque que contiene las dos copias y su info.
//funciones para serializar y deserializar bloquearchivofromstring y bloquearchivotostring.
typedef struct{
	int tamanio;
	char* tipo;
	t_list* bloques;
}t_archivo;

void vaciarArchivo(char* path);

void mostrarArchivo(t_archivo* nuevoArchivo, char* rutaArchivo);
void cargarTablaArchivo(t_archivo* nuevoArchivo, char* rutaArchivo);
void cargarDatos(char* lineaDatos,char* lineaElem, t_archivo* nuevoArchivo, int cantidadCampos);
void eliminarNodoDeArchivo(t_archivo* archivo, int idNodo, int idBloque);
void eliminarBloqueDeArchivo(t_archivo* archivo, char* numeroBloque, char* numeroCopia);
char* informacionNodoBloque(t_archivo* archivo);

void guardarArchivoEnArchivo(t_archivo* archivo, char* path);

char* informacionNodoBloque(t_archivo* archivo);

void vaciarListaNodos(t_list* lista);

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
