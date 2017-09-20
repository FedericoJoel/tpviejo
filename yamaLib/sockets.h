/*
 * Al usar esto tenes que definir en tu .c
 * int tienePermiso(char* autentificacion){} y retornar 0 si no tiene y 1 si tiene permiso
 */
#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <commons/log.h>

/////////////
//PROTOCOLO//
/////////////
///FS - DN///
#define DN_SETBLOQUE 1
#define DN_SETBLOQUEANSW 2
#define DN_GETBLOQUE 3
#define DN_GETBLOQUEANSW 4
//FS - YAMA//
#define FS_READ 5
#define FS_STORE 6
#define YM_READANSW 7
#define YM_STOREANSW 8


///////////
//CLIENTE//
///////////
int conectar(int puerto,char* ip);

////////////
//SERVIDOR//
////////////
int crearServidor(int puerto);
int esperarConexion(int servidor, char* autentificacion);

/////////////////////
//ENVIO DE MENSAJES//
/////////////////////
void enviarMensajeConProtocolo(int conexion, char* mensaje, int protocolo);
void enviarProtocolo(int socket, int protocolo);
int sendIntTo(int socket, int number);

/////////////////////////
//RECEPCION DE MENSAJES//
/////////////////////////
char* esperarMensaje(int conexion);
int recibirProtocolo(int conexion);
void enviarMensaje(int conexion, char* mensaje);


//////////////
//AUXILIARES//
//////////////
int autentificar(int conexion, char* autor);
int esperarConfirmacion(int conexion);
int aceptar(int servidor);
char* header(int numero);




#endif
