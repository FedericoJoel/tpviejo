#include "yama.h"

int main(void) {

	levantar_logger();

	levantar_config();

	levantar_servidor();

	esperar_masters();

	log_info(logger,"Salir del programa");
	return EXIT_SUCCESS;
}


void levantar_config(void) {
	log_info(logger,"Levantando configuracion");
	config = config_create(ruta_config);
	log_info(logger,"Configuracion levantada");
}


void levantar_logger(void) {
	logger = abrir_logger(ruta_log,nombre_programa,LOG_LEVEL_INFO);
	log_info(logger,"Logger iniciado");
}

int levantar_servidor(void) {
	log_info(logger, "levantando servidor YAMA");
	socket_server = crearServidor(config_get_int_value(config,"YAMA_PUERTO"));
	if (socket_server < 0) {
			log_error(logger, "No se pudo levantar el servidor");
			return EXIT_FAILURE;
		}
	return EXIT_SUCCESS;
}

void esperar_masters(void) {

	int max_fds;
	int select_status;

	max_fds = socket_server;

	while(1){
		construir_fds(&max_fds,socket_clientes);
		log_info(logger,"esperando conexiones");
		select_status = select(max_fds + 1, &fds_masters, NULL, NULL, NULL);

		if(select_status < 0){
			perror("select failed");
			log_error(logger,"fallo select");
			return;
		}else if(select_status > 0) {
			leer_cambios_select();
		}
	}

}

void construir_fds(int* max_actual,int conectados[]){
	int i;

	FD_ZERO(&fds_masters);
	FD_SET(socket_server,&fds_masters);
	for(i=0; i < clientes_max; i++){
		if (conectados[i] != 0) {
			FD_SET(conectados[i],&fds_masters);
			if(*max_actual < conectados[i]){
				*max_actual = socket_clientes[i];
			}
		}
	}
}


void leer_cambios_select(){
	int i;
	//nuevo cliente
	if (FD_ISSET(socket_server,&fds_masters)) {
		log_info(logger,"se recibio una nueva conexion");
		recibir_nuevo_master();
	}
	//recibo data de uno ya conectado
	for (i = 0; i < clientes_max; i++) {
		if (FD_ISSET(socket_clientes[i],&fds_masters)) {
			log_info(logger,"se recibio data de un master conectado");
			recibir_data_de_master(i);
		}
	}
}

void recibir_nuevo_master() {
	int i;
	int nuevo_cliente;

	//conecto cliente en un nuevo socket
	if((nuevo_cliente = esperarConexion(socket_server,AUTH)) < 0){
		log_error(logger,"no se pudo crear conexion");
	}

	//recorro array para encontrarle un lugar vacio al nuevo master
	for (i = 0; (i < clientes_max) && (nuevo_cliente != -1); i ++){
		if (socket_clientes[i] == 0) {
			log_info(logger,"Conexion aceptada:   FD=%d; Slot=%d",nuevo_cliente,i);
			socket_clientes[i] = nuevo_cliente;

			// Enviamos un mensaje con su lugar en la lista
			protocolo = YM_MS_OKCONN;
			enviarMensajeConProtocolo(nuevo_cliente, "holaa",protocolo);
			nuevo_cliente = -1;
		}
	}
	//no queda lugar en el array de conexiones, libero ese socket
	if (nuevo_cliente != -1) {
		log_error(logger, "No queda lugar para nuevo master: %d, servidor muy ocupado", nuevo_cliente);
		close(nuevo_cliente);
	}
}

void recibir_data_de_master(int posicion) {
	int proto_msg;
	char* mensaje;

	proto_msg = recibirProtocolo(socket_clientes[posicion]);

	if (proto_msg == MS_YM_DESCONECTAR) { //se desconecto el cliente
		log_info(logger,"se desconecto el master %d",socket_clientes[posicion]);
		close(socket_clientes[posicion]);
		socket_clientes[posicion] = 0;
	} else { //recibimos un mensaje
		mensaje = esperarMensaje(socket_clientes[posicion]);
		log_info(logger,"recibimos mensaje %s del master %d",mensaje,socket_clientes[posicion]);
	}
}
