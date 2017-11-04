#ifndef DIRECTORIO_H_
#define DIRECTORIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <lib.h>

//DIRECTORIO//
typedef struct{
	int index;
	char nombre[255];
	int padre;
}t_directory;

typedef struct{
	char* path;
	int nivel;
}estructuraAux;

//VACIA UNA POSICION DEL ARRAY DE DIRECTORIO
void vaciarPosicion(t_directory* directorio, int pos);

//DEVUELVE LA POSICION DE UNA CARPETA EN EL ARRAY DEL DIRECTORIO
int buscarPosicionEnDirectorio(t_directory* directorio, char* path);

//ELIMINA EL ULTIMO DIRECTORIO DE UN PATH, DE LA ESTRUCTURA EN MEMORIA
void eliminarUnDirectorio(t_directory* directorio, char** path);

//ELIMINA UN DIRECTORIO FISICO DE LA PC
int eliminarArchivosDeDirectorio(t_directory* directorio);

//COMPRUEBA QUE EL DIRECTORIO EXISTA EN LA ESTRUCTURA DE MEMORIA
int comprobarDirectorio(int nivel, t_directory directorio [], t_directory* unDirectorio);

//AGREGAR DIRECTORIO A STRUCT DE MEMORIA
void agregarPath(t_directory[], char* path);

char* agregarPadreARuta(int, t_directory[]);
int damePosicionDeElemento(char*, t_directory[]);
void guardarRegistro(t_directory [], t_directory*, int, int);
void cargarDirectorio(t_directory []);
void mostrarDirectorio(t_directory* directorio);
void convertirDirectorio(char *, t_directory [], int pos);
int existeDirectorio();
char* listarDirectorio(char* path);

//VACIA EL NOMBRE DE UN DIRECTORIO EN MEMORIA
void vaciarArray(char* nombre, int cantidad);

//VACIA LA ESTRUCTURA EN MEMORIA DEL DIRECTORIO
void vaciarEstructuraDirectorio(t_directory* directorio);

//INVIERTE EL ORDEN DEL ARRAY DE DIRECTORIO
char* invertirOrden(char** argumentos);

void guardarDirectorioEnMemoria(t_directory* directorio);

void escribirEnChar(char nombre[], char* argumentos, int pos);

void ordenarArray(estructuraAux* array);

#endif /* DIRECTORIO_H_ */
