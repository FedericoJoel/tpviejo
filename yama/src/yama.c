#include "yama.h"

int puerto = 2323;
int protocoloInicio = 0010;
int servidor;
t_list *archivo;
t_list *estados;
t_list *planif;
int BASE = 2;


int main(void) {

	prueba();
	levantar_logger();

	levantar_config();

	levantar_servidor();

	esperar_masters();

	log_info(logger,"Salir del programa");
	return EXIT_SUCCESS;
}

struct regPlanificacion{
		int availability;
		int worker;
		t_list *bloques;
		t_list *bloquesAsignados;
		}regPlanificacion;


int PWL(int worker){
		//
		int wlmax = 0; //la mayor carga de trabajo entre todos los worker. Se debera recorrer la tabla de estados para ver cual es la mayor carga de trabajo
		int wl = 0; //la carga de trabajo actual del worker. Se obtiene de la tabla de estados

		return wlmax - wl ;
	}


bool nodoYaAsignado(t_list *planif, int nodo){

	int i;
	for(i=0; i<list_size(planif); i++)
	{
		struct regPlanificacion *reg = list_get(planif, i);
		if(reg->worker == nodo){
			return true;
		}else {
				continue;
			}
		}
	return false;
}

void agregarBloque( void* registro, int bloque){

	struct regPlanificacion *reg = registro;
	list_add(reg->bloques, bloque);
}
t_list * archivoPrueba(){

	char* ip1 = string_new();
	ip1 = "127.0.0.1";
	char* ip2 = string_new();
	ip2 = "127.0.0.1";
	char* ip3 = string_new();
	ip3 = "127.0.0.1";

	archivo = list_create();
	struct Bloque* Bloque0 = malloc (sizeof (struct Bloque));
	(*Bloque0).Copia0.nodo = 1;
	(*Bloque0).Copia0.bloque = 8;
	(*Bloque0).Copia0.ip = ip1;

	(*Bloque0).Copia1.nodo = 2;
	(*Bloque0).Copia1.bloque = 11;
	(*Bloque0).Copia1.ip = ip1;

	(*Bloque0).bloque = 0;
	(*Bloque0).bytes = 10180;

	struct Bloque *Bloque1 = malloc (sizeof (struct Bloque));

	(*Bloque1).Copia0.nodo = 2;
	(*Bloque1).Copia0.bloque = 12;
	(*Bloque1).Copia0.ip = ip2;
	(*Bloque1).Copia1.nodo = 3;
	(*Bloque1).Copia1.bloque = 2;
	(*Bloque1).Copia1.ip = ip2;

	(*Bloque1).bloque = 1;
	(*Bloque1).bytes = 10201;

	struct Bloque *Bloque2 = malloc (sizeof (struct Bloque));

	(*Bloque2).Copia0.nodo = 1;
	(*Bloque2).Copia0.bloque = 7;
	(*Bloque2).Copia0.ip = ip3;
	(*Bloque2).Copia1.nodo = 2;
	(*Bloque2).Copia1.bloque = 9;
	(*Bloque2).Copia1.ip = ip3;
	(*Bloque2).bloque = 2;
	(*Bloque2).bytes = 10109;


	list_add(archivo, (void*) Bloque0);
	list_add(archivo, (void*) Bloque1);
	list_add(archivo, (void*) Bloque2);

	return archivo;

}

void * buscarRegEnLista(t_list *planif, int nodo)
{
	int i;
	for(i=0; i<list_size(planif); i++)
	{
		struct regPlanificacion *reg = list_get(planif, i);
		if(reg->worker == nodo)
		{
			return (void*) reg;
		}
	}
}

void tablaPlanif()
{
	t_list *arch = archivoPrueba2();

	planif = list_create();

	int i;
	for(i=0; i<list_size(arch); i++)
	{
		struct Bloque *bloque = list_get(arch, i);
		int nBloque = bloque->bloque;
		int nodoCopia0 = bloque->Copia0.nodo;
		int nodoCopia1 = bloque->Copia1.nodo;

		if(!nodoYaAsignado(planif, nodoCopia0))
		{
			struct regPlanificacion *reg = malloc (sizeof (struct regPlanificacion));
			(*reg).availability = 2;
			(*reg).worker = nodoCopia0;
			(*reg).bloquesAsignados = list_create();
			(*reg).bloques = list_create();
			agregarBloque(reg, nBloque);
			list_add(planif, (void*) reg);

		} else {

			struct regPlanificacion *reg = buscarRegEnLista(planif, nodoCopia0);
			agregarBloque(reg, nBloque);
		}

		if(!nodoYaAsignado(planif, nodoCopia1))
		{
			struct regPlanificacion *reg1 = malloc (sizeof (struct regPlanificacion));
			(*reg1).availability = 2;
			(*reg1).worker = nodoCopia1;
			reg1->bloquesAsignados = list_create();
			(*reg1).bloques = list_create();
			agregarBloque(reg1, nBloque);
			list_add(planif, (void*) reg1);
		} else {
			struct regPlanificacion *reg1 = buscarRegEnLista(planif, nodoCopia1);
			agregarBloque(reg1, nBloque);
		}
	}



}





/*int prueba ()
{

	planif *clock = tablaPlanif();

	archivo *arch = archivoPrueba();


	struct tablaPlanificacion *clock = tabla; //Se apunta clock a la primera posicion del array. Se deberia apuntar a la posicion del worker con mayor disponibilidad

		for(i=0 ; i<list_size(arch) ; i++){ //Este for hace un bucle por cada bloque que se quiera encontrar

			planif *auxclock = clock;
			struct tablaPlanificacion * auxclock = clock; //Se apunta al auxclock a la posicion  apuntada por clock

			if((auxclock->).worker <3){ auxclock += 1;}else{auxclock= tabla;}; //Se le suma a auxclock una posicion

			int encontrado =0;

			while(!encontrado){ //Se entra en un bucle del cual se va a salir cuando se le asigne un bloque al worker apuntado por clock

				if((*clock).availability > 0){

					for(i=0; i<5; i++){ //Busco si el worker en el que esta parado el clock contiene al bloque. Debe hacerse por cada elemento de la lista de bloques que contiene el worker

									if(bloqueABuscar == (*clock).bloques[i]){
										(*clock).bloquesAsignados += 1;
										(*clock).availability -= 1;
										encontrado = 1;//Si lo encuentro se pasara el clock al siguiente worker
										break;
									}
								}

								if(!encontrado){ //Si no se encuentra el bloque en el worker apuntado por clock se manda al auxclock a asignarlo

									int encontradoAux =0;

									while(!encontradoAux){

										if(auxclock != clock){// Se dio una vuelta entera si son iguales
											for(i=0; i<5; i++){ //Busco si el nodo en el que esta parado el auxclock contiene al bloque
													if(bloqueABuscar == (*auxclock).bloques[i]){
															(*auxclock).bloquesAsignados += 1;
															(*auxclock).availability -= 1;
															encontradoAux = 1;
															break;
													}
											}
											if((* auxclock).worker <3){ auxclock += 1;}else{auxclock= tabla;}
										}
										else{
											for(i=0 ; i<3 ; i++){
												clock[i].availability += BASE;
											}
										}
								}
								}

								bloqueABuscar += 1;

				}
				else{
					(*clock).availability = BASE;
					break;
				}
			}

			if((* clock).worker <3){clock += 1;}else{clock= tabla;}

		}


		return 1;
}*/

void buscarBloqueEnWorker(void * clock, void * bloqueABuscar1, int* encontrado)
{
	int i;
	struct regPlanificacion *reg = clock;
	struct Bloque *bloqueABuscar = bloqueABuscar1;
	int size = list_size(reg->bloques);
	for(i=0; i<size; i++){ //Busco si el worker en el que esta parado el clock contiene al bloque. Debe hacerse por cada elemento de la lista de bloques que contiene el worker
		int *bloqueClock = list_get(reg->bloques, i);
		if(bloqueABuscar->bloque == (int)bloqueClock){
			list_add(reg->bloquesAsignados, bloqueClock);
			reg->availability -= 1;
			*encontrado = 1;//Si lo encuentro se pasara el clock al siguiente worker
			break;
		}
	}
}


t_list * archivoPrueba2(){
	char* ip1 = string_new();
	ip1 = "127.0.1.1";
	char* ip2 = string_new();
	ip2 = "127.0.1.1";
	char* ip3 = string_new();
	ip3 = "127.0.1.1";

	archivo = list_create();
	struct Bloque *Bloque0 = malloc (sizeof (struct Bloque));
	(*Bloque0).Copia0.nodo = 1;
	(*Bloque0).Copia0.bloque = 1;
	(*Bloque0).Copia0.ip = ip1;

	(*Bloque0).Copia1.nodo = 2;
	(*Bloque0).Copia1.bloque = 1;
	(*Bloque0).Copia1.ip = ip2;

	(*Bloque0).bloque= 0;
	(*Bloque0).bytes = 1024;

	struct Bloque *Bloque1 = malloc (sizeof (struct Bloque));
		(*Bloque1).Copia0.nodo = 1;
		(*Bloque1).Copia0.bloque = 1;
		(*Bloque1).Copia0.ip = ip1;

		(*Bloque1).Copia1.nodo = 3;
		(*Bloque1).Copia1.bloque = 1;
		(*Bloque1).Copia1.ip = ip3;

		(*Bloque1).bloque= 1;
		(*Bloque1).bytes = 1024;

	struct Bloque *Bloque2 = malloc (sizeof (struct Bloque));
		(*Bloque2).Copia0.nodo = 2;
		(*Bloque2).Copia0.bloque = 1;
		(*Bloque2).Copia0.ip = ip2;

		(*Bloque2).Copia1.nodo = 3;
		(*Bloque2).Copia1.bloque = 1;
		(*Bloque2).Copia1.ip = ip3;

		(*Bloque2).bloque= 2;
		(*Bloque2).bytes = 1024;

	struct Bloque *Bloque3 = malloc (sizeof (struct Bloque));
		(*Bloque3).Copia0.nodo = 1;
		(*Bloque3).Copia0.bloque = 1;
		(*Bloque3).Copia0.ip = ip1;

		(*Bloque3).Copia1.nodo = 2;
		(*Bloque3).Copia1.bloque = 1;
		(*Bloque3).Copia1.ip = ip2;

		(*Bloque3).bloque= 3;
		(*Bloque3).bytes = 1024;

	struct Bloque *Bloque4 = malloc (sizeof (struct Bloque));
		(*Bloque4).Copia0.nodo = 1;
		(*Bloque4).Copia0.bloque = 1;
		(*Bloque4).Copia0.ip = ip1;

		(*Bloque4).Copia1.nodo = 3;
		(*Bloque4).Copia1.bloque = 1;
		(*Bloque4).Copia1.ip = ip3;

		(*Bloque4).bloque= 4;
		(*Bloque4).bytes = 1024;

	struct Bloque *Bloque5 = malloc (sizeof (struct Bloque));
		(*Bloque5).Copia0.nodo = 3;
		(*Bloque5).Copia0.bloque = 3;
		(*Bloque5).Copia0.ip = ip3;

		(*Bloque5).Copia1.nodo = 2;
		(*Bloque5).Copia1.bloque = 1;
		(*Bloque5).Copia1.ip = ip2;

		(*Bloque5).bloque= 5;
		(*Bloque5).bytes = 1024;

	struct Bloque *Bloque6 = malloc (sizeof (struct Bloque));
		(*Bloque6).Copia0.nodo = 1;
		(*Bloque6).Copia0.bloque = 1;
		(*Bloque6).Copia0.ip = ip1;

		(*Bloque6).Copia1.nodo = 2;
		(*Bloque6).Copia1.bloque = 1;
		(*Bloque6).Copia1.ip = ip2;

		(*Bloque6).bloque= 6;
		(*Bloque6).bytes = 1024;
		list_add(archivo, (void*) Bloque0);
		list_add(archivo, (void*) Bloque1);
		list_add(archivo, (void*) Bloque2);
		list_add(archivo, (void*) Bloque3);
		list_add(archivo, (void*) Bloque4);
		list_add(archivo, (void*) Bloque5);
		list_add(archivo, (void*) Bloque6);


			return archivo;
}

int prueba()
{
	tablaPlanif();

	/*int h;
	for(h=0; h< list_size(planif); h++)
	{
		struct regPlanificacion *r = list_get(planif, h);
		int i;
		for (i=0; i<list_size(r->bloques); i++)
		{
			int *bloque = list_get(r->bloques, i);
			int j = 5;
		}
	}*/


	t_list *arch = archivoPrueba2();
	t_link_element *clock = planif->head;
	t_link_element *auxclock = planif->head;
	t_link_element *head = planif->head;

	struct regPlanificacion *ultimoReg = list_get(planif, list_size(planif)-1);
	int i,bloqueNumero = 0;

	struct Bloque *bloqueABuscar = list_get(arch, bloqueNumero);
	for(i=0 ; i<list_size(arch); i++){ //Este for hace un bucle por cada bloque que se quiera encontrar


		auxclock = clock;
		struct regPlanificacion *reg = auxclock->data;
		auxclock = reg->worker != ultimoReg->worker ? auxclock->next : head; //Se le suma a auxclock una posicion
		//todo hay que solucionar esto

		int encontrado =0;
		struct regPlanificacion *regClock = clock->data;

		while(!encontrado){ //Se entra en un bucle del cual se va a salir cuando se le asigne un bloque al worker apuntado por clock

			if(regClock->availability > 0){

				buscarBloqueEnWorker((void *)regClock, (void *) bloqueABuscar, &encontrado);

				//Si no se encuentra el bloque en el worker apuntado por clock se manda al auxclock a asignarlo
				if(!encontrado)
				{
					int encontradoAux =0;
					while(!encontradoAux)
					{
						// Se dio una vuelta entera si son iguales
						if(auxclock != planif)
						{
							struct regPlanificacion *regAuxClock = auxclock->data;

							buscarBloqueEnWorker((void*) regAuxClock, (void *) bloqueABuscar, &encontradoAux);

							auxclock = reg->worker != ultimoReg->worker ? auxclock->next : head;
						}
						else{
							for(i=0 ; i<list_size(planif) ; i++)
							{
								struct regPlanificacion *regP = list_get(planif, i);
								regP->availability += BASE;
							}
						}
					}
				}

				bloqueNumero++;
				bloqueABuscar = list_get(arch, bloqueNumero);



						}
						else{
							regClock->availability = BASE;
							break;
						}
					}

					clock = regClock->worker != ultimoReg->worker ? clock->next : head;

				}


				return 1;

}



t_list * tablaEstadosPrueba(){
	estados = list_create();

	struct Estado *Estado0 = malloc (sizeof (struct Estado));
	(*Estado0).job = 1;
	(*Estado0).master = 1;
	(*Estado0).nodo = 1;
	(*Estado0).bloque = 8;
	(*Estado0).etapa = 0;
	(*Estado0).temporal = "/tmp/j1n1b8";
	(*Estado0).estado = 0;

	struct Estado *Estado1 = malloc (sizeof (struct Estado));
	(*Estado1).job = 1;
	(*Estado1).master = 1;
	(*Estado1).nodo = 3;
	(*Estado1).bloque = 2;
	(*Estado1).etapa = 0;
	(*Estado1).temporal = "/tmp/j1n1b8";
	(*Estado1).estado = 0;

	struct Estado *Estado2 = malloc (sizeof (struct Estado));
	(*Estado2).job = 1;
	(*Estado2).master = 1;
	(*Estado2).nodo = 2;
	(*Estado2).bloque = 9;
	(*Estado2).etapa = 0;
	(*Estado2).temporal = "/tmp/j1n1b8";
	(*Estado2).estado = 0;

	list_add(estados, Estado0);
	list_add(estados, Estado1);
	list_add(estados, Estado2);

	return estados;
}

void roundRobin(){

}

int atenderMaster(){
	servidor = crearServidor(puerto);
	char* aux = "";
	int master = esperarConexion(servidor, aux);
	int protocolo = recibirProtocolo(master);
	chequearProtocolo(protocolo);
	char* mensaje = esperarMensaje(master);
	return 1;
}

void chequearProtocolo(int protocolo){
	if(protocolo == protocoloInicio){

		char* scriptReductor = esperarMensaje(servidor);
		char* rutaArchivo = esperarMensaje(servidor);
		char* rutaAlmacenamiento = esperarMensaje(servidor);
		printf("esto anda\n");

	} else {

		perror("no entra al if del chequeo de protocolos");

	}

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

			// Enviamos un mensaje con su lugar en la lista
			protocolo = YM_MS_OKCONN;
			enviarMensajeConProtocolo(nuevo_cliente, "holaa",protocolo);
			nuevo_cliente = -1;
		}
	}
	//no queda lugar en el array de conexiones, libero ese socket
	if (nuevo_cliente != -1) {
		log_error(logger, "No queda lugar para nuevo master: %d, servidor muy ocupado", nuevo_cliente);
		//todo mandar algun protocolo al cliente antes de rechazarlo
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
