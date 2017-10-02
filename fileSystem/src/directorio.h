#ifndef DIRECTORIO_H_
#define DIRECTORIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>

//DIRECTORIO//
typedef struct{
	int index;
	char nombre[255];
	int padre;
}t_directory;

int comprobarDirectorio(int nivel, t_directory directorio [], t_directory unDirectorio);
void guardarRegistro(t_directory [], t_directory, int, int);
int mostrarDirectorio();
void convertirDirectorio(char *, t_directory []);

#endif /* DIRECTORIO_H_ */
