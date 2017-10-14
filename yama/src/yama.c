#include "yama.h"

//nodos hardcodeados -------------------------------
	t_list lista_de_nodos_recibidos; //lista de nodos que nos llega de filesystem
	t_list lista_de_nodos_respuesta; //nodos que vamos a enviar a master

	t_bloque_archivo bloque_mock_0;
	t_bloque_archivo bloque_mock_1;
	t_bloque_archivo bloque_mock_2;

	t_copia copia_0_bloque_0;
	t_copia copia_1_bloque_0;
	t_copia copia_0_bloque_1;
	t_copia copia_1_bloque_1;
	t_copia copia_0_bloque_2;
	t_copia copia_1_bloque_2;

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

	//el select te pide que le pases en el primer parametro cual es el filedescriptor mas alto
	// dentro de tu array de sockets(socket_cliente), a mi siempre me puso 4 para el socket del server
	// y 5,6,etc para los que iban llegando, de arranque el server es el unico asi que va ese.
	max_fds = socket_server;

	while(1){

		// en cada iteracion hay que blanquear(FD_ZERO) el fds_masters, que es un bitarray donde queda guardado
		// que sockets tienen cambios despues del select y tambien volver a setear(FD_SET) los filedescriptors en el fds_set.
		//ademas de volver a calcular cual es el filedescriptor mas alto
		construir_fds(&max_fds,socket_clientes);

		log_info(logger,"esperando conexiones");

		select_status = select(max_fds + 1, &fds_masters, NULL, NULL, NULL);
		//select_status puede ser:
		// < 0 => error
		// == 0 => no paso nada
		// > 0 => hubo algun cambio en los sockets o entro otro nuevo

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
	//TODO handlear si el cliente se desconecta abruptamente(seguramente hay que usar alguna signal o algo asi)
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
			log_info(logger,"Conexion aceptada: FD=%d posicion=%d",nuevo_cliente,i);
			socket_clientes[i] = nuevo_cliente;

			// Enviamos un protocolo de confirmacion
			protocolo = YM_MS_OKCONN;
			enviarProtocolo(nuevo_cliente, protocolo);
			nuevo_cliente = -1;
		}
	}
	//no queda lugar en el array de conexiones, libero ese socket
	if (nuevo_cliente != -1) {
		log_error(logger, "No queda lugar para nuevo master: %d, servidor muy ocupado", nuevo_cliente);
		//mandar protocolo al cliente antes de rechazarlo
		enviarProtocolo(nuevo_cliente, YM_MS_ERRORCONN);
		close(nuevo_cliente);
	}
}

void recibir_data_de_master(int posicion) {
	int proto_msg;
	char* mensaje;

	//defino todas las copias de 3 bloques de archivo que estan desparramadas en 3 nodos
		copia_0_bloque_0.nodo = 0;
		copia_0_bloque_0.bloque_nodo = 0;
		copia_0_bloque_0.ip = string_from_format("127.0.0.1");

		copia_1_bloque_0.nodo = 1;
		copia_1_bloque_0.bloque_nodo = 2;
		copia_1_bloque_0.ip = string_from_format("127.0.0.1");

/*		copia_0_bloque_1.nodo = 0;
		copia_0_bloque_1.bloque_nodo = 2;
		copia_0_bloque_1.ip = string_from_format("127.0.0.1");

		copia_1_bloque_1.nodo = 2;
		copia_1_bloque_1.bloque_nodo = 1;
		copia_1_bloque_1.ip = string_from_format("127.0.0.1");

		copia_0_bloque_2.nodo = 1;
		copia_0_bloque_2.bloque_nodo = 0;
		copia_0_bloque_2.ip = string_from_format("127.0.0.1");

		copia_1_bloque_2.nodo = 2;
		copia_1_bloque_2.bloque_nodo = 3;
		copia_1_bloque_2.ip = string_from_format("127.0.0.1");
*/
		//defino los bloques del archivo que tienen las copias
		bloque_mock_0.bloque_archivo = 0;
		bloque_mock_0.copia0 = &copia_0_bloque_0;
		bloque_mock_0.copia1 = &copia_1_bloque_0;
		bloque_mock_0.bytes = 12;
/*
		bloque_mock_1.bloque_archivo = 1;
		bloque_mock_1.copia0 = &copia_0_bloque_1;
		bloque_mock_1.copia1 = &copia_1_bloque_1;
		bloque_mock_1.bytes = 25;

		bloque_mock_2.bloque_archivo = 2;
		bloque_mock_2.copia0 = &copia_0_bloque_2;
		bloque_mock_2.copia1 = &copia_1_bloque_2;
		bloque_mock_2.bytes = 65;
*/
		//lista de bloques que me llegan
		list_add(&lista_de_nodos_recibidos,(void*) &bloque_mock_0);
	//	list_add(&lista_de_nodos_recibidos,(void*) &bloque_mock_1);
	//	list_add(&lista_de_nodos_recibidos,(void*) &bloque_mock_2);

		//---------------------------------------------------


	proto_msg = recibirProtocolo(socket_clientes[posicion]);

	switch(proto_msg) {
		case MS_YM_DESCONECTAR:// se desconecta master
			log_info(logger,"se desconecto el master %d",socket_clientes[posicion]);
			close(socket_clientes[posicion]);
			socket_clientes[posicion] = 0;
			break;
		case MS_YM_INICIO_JOB:// inicia un job un master
			log_info(logger,"iniciar job de master %d",socket_clientes[posicion]);
			mensaje = esperarMensaje(socket_clientes[posicion]);
			log_info(logger,"ruta de archivo %s",mensaje);
			//todo enviar ruta a filesystem
			//esperar que envie struct de nodos
			//por ahora hardcodeamos la estructura que recibe

			//todo aplicar algoritmo sobre lo que recibo de filesystem
			//mockeo las copias que se eligen
			bloque_mock_0.elegida = 0;
			bloque_mock_0.ruta_temporal = string_from_format("/tmp/master%d-temp%d",0,0); //todo crear estructura que lleve cuenta de los archivos temporales creados y los master uqe se conectaron??
		/*	bloque_mock_1.elegida = 1;
			bloque_mock_1.ruta_temporal = string_from_format("/tmp/master%d-temp%d",0,1);
			bloque_mock_2.elegida = 0;
			bloque_mock_2.ruta_temporal = string_from_format("/tmp/master%d-temp%d",0,2);
*/
			list_add(&lista_de_nodos_respuesta,(void*) &bloque_mock_0);
	//		list_add(&lista_de_nodos_respuesta,(void*) &bloque_mock_1);
	//		list_add(&lista_de_nodos_respuesta,(void*) &bloque_mock_2);
			// se manda la lista de bloques ya modificados a master
			enviar_transformacion(socket_clientes[posicion], &lista_de_nodos_respuesta);

			break;
	}
}


void enviar_transformacion(int master, t_list* lista_bloques){

	void _iterate_bloques(t_bloque_archivo* bloque){
		char* copia_string = bloque_archivo_to_string(bloque);
		enviarMensaje(master,copia_string);
	}

	//envio protocolo de transformacion
	enviarProtocolo(master,YM_MS_TRANSFORMACION);
	//envio cantidad de bloques que voy a mandar
	enviarMensaje(master,int_to_string(list_size(lista_bloques)));
	//itero bloques y los mando serializados
	list_iterate(lista_bloques,(void*) _iterate_bloques);
}




























