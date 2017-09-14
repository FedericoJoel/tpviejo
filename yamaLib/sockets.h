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

int tienePermiso(char* autentificacion);

//manda como header el int que ponemos como protocolo.
void enviarProtocolo(int socket, int protocolo);
//recibimos un header y error si n lo mandan correctamente.
int recibirProtocolo(int conexion);
// BLOQUEANTE devuelve un socket conectado a puerto,ip
int conectar(int puerto,char* ip);
//envia el handshake
int autentificar(int conexion, char* autor);
//
int esperarConfirmacion(int conexion);
int crearServidor(int puerto);
void enviarMensaje(int conexion, char* mensaje);
int esperarConexion(int servidor, char* autentificacion);
char* esperarMensaje(int conexion);
int aceptar(int servidor);
char* header(int numero);	
void enviarMensajeConProtocolo(int conexion, char* mensaje, int protocolo);
int sendIntTo(int socket, int number);

#endif
