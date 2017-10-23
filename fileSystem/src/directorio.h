#ifndef DIRECTORIO_H_
#define DIRECTORIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <unistd.h>

//DIRECTORIO//
typedef struct{
	int index;
	char nombre[255];
	int padre;
}t_directory;

int comprobarDirectorio(int nivel, t_directory directorio [], t_directory unDirectorio);
char* agregarPadreARuta(int, t_directory[]);
int damePosicionDeElemento(char*, t_directory[]);
void guardarRegistro(t_directory [], t_directory, int, int);
void mostrarDirectorio(t_directory []);
void convertirDirectorio(char *, t_directory []);
int existeDirectorio();

#endif /* DIRECTORIO_H_ */
