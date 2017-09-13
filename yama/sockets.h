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

void enviarProtocolo(int socket, int protocolo);
//te conecta ip y puerto que le pases, devuelve el socket para comunicarse
int conectar(int puerto,char* ip);
int autentificar(int conexion, char* autor);
int esperarConfirmacion(int conexion);
//crea server en el puerto especificado y devuelve socket que va a escuchar conexiones
int crearServidor(int puerto);
void enviarMensaje(int conexion, char* mensaje);
//hace un aceptar() y devuelve socket del cliente
int esperarConexion(int servidor, char* autentificacion);
char* esperarMensaje(int conexion);
//bloquea con accept() y queda esperando clientes
int aceptar(int servidor);
char* header(int numero);	
int recibirProtocolo(int conexion);
void enviarMensajeConProtocolo(int conexion, char* mensaje, int protocolo);
int sendIntTo(int socket, int number);

#endif
