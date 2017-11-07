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

int puerto = 2323;
int protocoloInicio = 0010;
int servidor;
t_list *archivo;
t_list *estados;
t_list *planif;
int BASE = 2;
int job = 1;
t_list *numeros_aleatorios;

int main(void) {
	/*estados = list_create();*/

	//t_list * a = tablaestadosPrueba();
//	 srand(time(NULL));
//	t_list * tabla = tablaPlanificacionCompleta();
//	enviarAMaster(tabla);
//	modificarBloqueTablaEstados(1, 2, 3, 1, 1);

	levantar_logger();

	levantar_config();

	levantar_servidor();

	esperar_masters();

	log_info(logger, "Salir del programa");
	return EXIT_SUCCESS;
}

void transformarBloques(t_list * tabla) {

	int i;
	t_link_element *puntTabla = tabla->head;
	for (i = 0; i < list_size(tabla); i++) {
		int j;
		t_reg_planificacion* reg = puntTabla->data;
		t_link_element *puntBloque = reg->bloquesAsignados->head;
		for (j = 0; j < list_size(reg->bloquesAsignados); j++) {
			agregarEstados((int) puntBloque->data, ETAPA_TRANSFORMACION, ESTADO_EN_PROCESO, reg->worker, reg->job,  1, "pum"); // aca el master y el temporal estan harcodeados
			puntBloque = puntBloque->next;
		}
		puntTabla = puntTabla->next;
	}
}

void modificarBloqueTablaEstados(int bloque, int etapa, int estadoNuevo, int worker, int job) {
	int i;
	t_link_element *puntTabla = estados->head;
	for (i = 0; i < list_size(estados); i++) {
		t_estado* estado = puntTabla->data;
		if (estado->bloque == bloque && estado->nodo == worker && estado->job == job) {
			estado->estado = estadoNuevo;
			estado->etapa = etapa;
			break;
		}
		puntTabla = puntTabla->next;
	}
}

void enviarAMaster(t_list * tabla) {

	transformarBloques(tabla);
	//TODO ACA FALTA ENVIARLE A MASTER LA TABLA
}


int PWL(int worker, int job){
		//

		int wlmax = cargaTrabajoMaxima(job); //la mayor carga de trabajo entre todos los worker. Se debera recorrer la tabla de estados para ver cual es la mayor carga de trabajo
		int wl = cargaTrabajoWorker(worker); //la carga de trabajo actual del worker. Se obtiene de la tabla de estados


	return wlmax - wl;
}


int cargaTrabajoWorker(int worker){

	int i;
	//t_link_element *puntTabla = estados->head;
	int carga = 0;

	for(i=0; i<list_size(estados); i++){
		//t_estado *estado = puntTabla->data;
		t_estado *estado = list_get(estados, i);
		if(estado->nodo == worker && estado->estado == ESTADO_EN_PROCESO)
		{
			carga++;
		}
	}
	return carga;
}

bool repetido(int nodo, t_list * nodosDelJob){
	int i;
		for(i=0; i<list_size(nodosDelJob); i++)
		{
			int nodoYaGuardado = list_get(nodosDelJob, i);
			if(nodo == nodoYaGuardado){
				return true;
			}else {
					continue;
				}
			}
		return false;
}

int cargaTrabajoMaxima(int job){

	int i;

	t_list * nodosDelJob;
	nodosDelJob = list_create();

	for(i=0; i<list_size(estados); i++){
		t_estado *estado = list_get(estados, i);
		if(estado->job == job && !repetido(estado->nodo, nodosDelJob)){
			int nodo = estado->nodo;
			list_add(nodosDelJob, (void*) nodo);
		}
	}

	int maximaCarga = cargaTrabajoWorker((int *)list_get(nodosDelJob, 0));
	for(i=1; i<list_size(nodosDelJob); i++){
		int nodoActual = list_get(nodosDelJob, i);
		int cargaActual = cargaTrabajoWorker(nodoActual);
		if(cargaActual > maximaCarga){
			maximaCarga = cargaActual;
		}
	}
	return maximaCarga;
}

int cargaTrabajoMinima(int job){

	int i;

	t_list * nodosDelJob;
	nodosDelJob = list_create();

	for(i=0; i<list_size(estados); i++){
		t_estado *estado = list_get(estados, i);
		if(estado->job == job && !repetido(estado->nodo, nodosDelJob)){
			int nodo = estado->nodo;
			list_add(nodosDelJob, (void*) nodo);
		}
	}
	int nodoMinimo;
	int minimaCarga = cargaTrabajoWorker((int *)list_get(nodosDelJob, 0));
	for(i=1; i<list_size(nodosDelJob); i++){
		int nodoActual = list_get(nodosDelJob, i);
		int cargaActual = cargaTrabajoWorker(nodoActual);
		if(cargaActual < minimaCarga){
			minimaCarga = cargaActual;
			nodoMinimo = nodoActual;
		}
	}
	return nodoMinimo;
}



bool nodoYaAsignado(t_list *planif, int nodo){


	int i;
	for (i = 0; i < list_size(planif); i++) {
		t_reg_planificacion *reg = list_get(planif, i);
		if (reg->worker == nodo) {
			return true;
		} else {
			continue;
		}
	}
	return false;
}

void agregarBloque(void* registro, int bloque) {

	t_reg_planificacion* reg = registro;
	list_add(reg->bloques, bloque);
}

t_list * archivoPrueba() {

	char* ip1 = string_new();
	ip1 = "127.0.0.1";
	char* ip2 = string_new();
	ip2 = "127.0.0.1";
	char* ip3 = string_new();
	ip3 = "127.0.0.1";

	archivo = list_create();
	t_bloque_archivo* bloque = malloc(sizeof(t_bloque_archivo));
	bloque->copia0->nodo = 1;
	bloque->copia0->bloque_nodo = 8;
	bloque->copia0->ip = ip1;

	bloque->copia1->nodo = 2;
	bloque->copia1->bloque_nodo = 11;
	bloque->copia1->ip = ip1;

	bloque->bloque_archivo = 0;
	bloque->bytes = 10180;

	t_bloque_archivo* bloque1 = malloc(sizeof(t_bloque_archivo));

	bloque1->copia0->nodo = 2;
	bloque1->copia0->bloque_nodo = 12;
	bloque1->copia0->ip = ip2;
	bloque1->copia1->nodo = 3;
	bloque1->copia1->bloque_nodo = 2;
	bloque1->copia1->ip = ip2;

	bloque1->bloque_archivo = 1;
	bloque1->bytes = 10201;

	t_bloque_archivo* bloque2 = malloc(sizeof(t_bloque_archivo));

	bloque2->copia0->nodo = 1;
	bloque2->copia0->bloque_nodo = 7;
	bloque2->copia0->ip = ip3;
	bloque2->copia1->nodo = 2;
	bloque2->copia1->bloque_nodo = 9;
	bloque2->copia1->ip = ip3;
	bloque2->bloque_archivo = 2;
	bloque2->bytes = 10109;

	list_add(archivo, (void*) bloque);
	list_add(archivo, (void*) bloque1);
	list_add(archivo, (void*) bloque2);

	return archivo;

}

void* buscarRegEnLista(t_list *planif, int nodo) {
	int i;
	for (i = 0; i < list_size(planif); i++) {
		t_reg_planificacion* reg = list_get(planif, i);
		if (reg->worker == nodo) {
			return (void*) reg;
		}
	}
}

void agregarEstados(int bloque, int etapa, int estadoNuevo, int worker, int job, int master, char * temporal){
	t_estado* estado = malloc(sizeof(t_estado));
				estado->nodo = worker;
				estado->bloque = bloque;
				estado->etapa = etapa;
				estado->estado = estadoNuevo;
				estado->job = job;
				estado->master = master;
				estado->temporal = temporal;
				//TODO:: falta job, master y temporal;
				list_add(estados, (void*) estado);
}

t_list* iniciarReduccion(int worker, int job){

	int i;
	t_list *reduccion = list_create();
	t_link_element *puntTabla = estados->head;
	for (i = 0; i < list_size(estados); i++) {
		t_estado* reg = puntTabla->data;
		if(reg->job == job && reg->nodo == worker){
			agregarEstados(reg->bloque, ETAPA_REDUCCION_LOCAL, ESTADO_EN_PROCESO, worker, job, 1, "pin"); // aca el master y el temporal estan harcodeados
			t_reg_planificacion *regPlani = malloc(sizeof(t_reg_planificacion));
			regPlani->worker = worker;
			regPlani->ip = "as";//harcodeado;
			regPlani->temporalTransformacion = reg->temporal;
			regPlani->tempReducLocal = "asfa";//harcodeado

			list_add(reduccion, (void*) regPlani);
		}
		puntTabla = puntTabla->next;
	}
	return reduccion;

}

void fallaEtapa(int etapa, int job){
			int i;
			t_link_element *puntTabla = estados->head;
			for (i = 0; i < list_size(estados); i++) {
				t_estado* reg = puntTabla->data;
				if(reg->job == job){
					modificarBloqueTablaEstados(reg->bloque, etapa, ESTADO_ERROR, reg->nodo, job);
				}
				puntTabla = puntTabla->next;
			}
}

int terminoEtapaTransformacion(int job, int worker){

	int i;
		t_link_element *puntTabla = estados->head;
		for (i = 0; i < list_size(estados); i++) {
			t_estado* reg = puntTabla->data;
			if(reg->job == job && reg->nodo == worker){
				if(reg->etapa == ETAPA_TRANSFORMACION && reg->estado == ESTADO_EN_PROCESO){
					return 0;
				}
			}
			puntTabla = puntTabla->next;
		}
		return 1;
}

t_list* tablaPlanif() {
	t_list* arch = archivoPrueba2();
	t_list* planificacion = list_create();
	t_list * estados = tablaestadosPrueba();

	int i;

	for (i = 0; i < list_size(arch); i++) {
		t_bloque_archivo *bloque = list_get(arch, i);
		int nBloque = bloque->bloque_archivo;
		int nodocopia0 = bloque->copia0->nodo;
		int nodocopia1 = bloque->copia1->nodo;

		if (!nodoYaAsignado(planificacion, nodocopia0)) {
			t_reg_planificacion* reg = malloc(sizeof(t_reg_planificacion));
			reg->worker = nodocopia0;
			reg->availability = BASE+ PWL(reg->worker, job);
			reg->bloquesAsignados = list_create();
			reg->bloques = list_create();
			//todo setear rutas temporales acumulando en algun lado las que ya se generaron para este master
			reg->temporalTransformacion = string_from_format("/tmp/Master-",int_to_string(i));
			reg->tempReducLocal = string_from_format("/tmp/Master-",int_to_string(i));
			reg->tempReudcGlobal= string_from_format("/tmp/Master-",int_to_string(i));
			reg->ip = "127.0.0.1";
			reg->job = job;
			agregarBloque(reg, nBloque);
			list_add(planificacion, (void*) reg);

		} else {

			t_reg_planificacion* reg = buscarRegEnLista(planificacion,
					nodocopia0);
			agregarBloque(reg, nBloque);
		}


		if (!nodoYaAsignado(planificacion, nodocopia1)) {
			t_reg_planificacion* reg1 = malloc(sizeof(t_reg_planificacion));
			reg1->worker = nodocopia1;
			reg1->availability = BASE+ PWL(reg1->worker, job);
			reg1->bloquesAsignados = list_create();
			reg1->bloques = list_create();
			reg1->job = job;
			reg1->temporalTransformacion = string_from_format("/tmp/Master-",int_to_string(i));
			reg1->tempReducLocal = string_from_format("/tmp/Master-",int_to_string(i));
			reg1->tempReudcGlobal= string_from_format("/tmp/Master-",int_to_string(i));
			reg1->ip = "127.0.0.1";
			agregarBloque(reg1, nBloque);
			list_add(planificacion, (void*) reg1);
		} else {
			t_reg_planificacion* reg1 = buscarRegEnLista(planificacion,
					nodocopia1);
			agregarBloque(reg1, nBloque);
		}
	}

	return planificacion;

}

void buscarBloqueEnWorker(void * clock, void * bloqueABuscar1, int* encontrado) {
	int i;
	t_reg_planificacion* reg = clock;
	t_bloque_archivo* bloqueABuscar = bloqueABuscar1;
	int size = list_size(reg->bloques);
	for (i = 0; i < size; i++) { //Busco si el worker en el que esta parado el clock contiene al bloque. Debe hacerse por cada elemento de la lista de bloques que contiene el worker
		int *bloqueClock = list_get(reg->bloques, i);
		if (bloqueABuscar->bloque_archivo == (int) bloqueClock) {
			list_add(reg->bloquesAsignados, bloqueClock);
			reg->availability -= 1;
			*encontrado = 1; //Si lo encuentro se pasara el clock al siguiente worker
			break;
		}
	}
}

t_list * archivoPrueba2() {
	char* ip1 = string_new();
	ip1 = "127.0.1.1";
	char* ip2 = string_new();
	ip2 = "127.0.1.1";
	char* ip3 = string_new();
	ip3 = "127.0.1.1";

	archivo = list_create();
	t_bloque_archivo* bloque = malloc(sizeof(t_bloque_archivo));
	bloque->copia0 = malloc(sizeof(t_copia));
	bloque->copia0->nodo = 1;
	bloque->copia0->bloque_nodo = 1;
	bloque->copia0->ip = ip1;

	bloque->copia1 = malloc(sizeof(t_copia));
	bloque->copia1->nodo = 2;
	bloque->copia1->bloque_nodo = 1;
	bloque->copia1->ip = ip2;

	bloque->bloque_archivo = 0;
	bloque->bytes = 1024;

	t_bloque_archivo* bloque1 = malloc(sizeof(t_bloque_archivo));
	bloque1->copia0 = malloc(sizeof(t_copia));
	bloque1->copia0->nodo = 1;
	bloque1->copia0->bloque_nodo = 1;
	bloque1->copia0->ip = ip1;

	bloque1->copia1 = malloc(sizeof(t_copia));
	bloque1->copia1->nodo = 3;
	bloque1->copia1->bloque_nodo = 1;
	bloque1->copia1->ip = ip3;

	bloque1->bloque_archivo = 1;
	bloque1->bytes = 1024;

	t_bloque_archivo* bloque2 = malloc(sizeof(t_bloque_archivo));
	bloque2->copia0 = malloc(sizeof(t_copia));
	bloque2->copia0->nodo = 2;
	bloque2->copia0->bloque_nodo = 1;
	bloque2->copia0->ip = ip2;

	bloque2->copia1 = malloc(sizeof(t_copia));
	bloque2->copia1->nodo = 3;
	bloque2->copia1->bloque_nodo = 1;
	bloque2->copia1->ip = ip3;

	bloque2->bloque_archivo = 2;
	bloque2->bytes = 1024;

	t_bloque_archivo* bloque3 = malloc(sizeof(t_bloque_archivo));
	bloque3->copia0 = malloc(sizeof(t_copia));
	bloque3->copia0->nodo = 1;
	bloque3->copia0->bloque_nodo = 1;
	bloque3->copia0->ip = ip1;

	bloque3->copia1 = malloc(sizeof(t_copia));
	bloque3->copia1->nodo = 2;
	bloque3->copia1->bloque_nodo = 1;
	bloque3->copia1->ip = ip2;

	bloque3->bloque_archivo = 3;
	bloque3->bytes = 1024;

	t_bloque_archivo* bloque4 = malloc(sizeof(t_bloque_archivo));
	bloque4->copia0 = malloc(sizeof(t_copia));
	bloque4->copia0->nodo = 1;
	bloque4->copia0->bloque_nodo = 1;
	bloque4->copia0->ip = ip1;

	bloque4->copia1 = malloc(sizeof(t_copia));
	bloque4->copia1->nodo = 3;
	bloque4->copia1->bloque_nodo = 1;
	bloque4->copia1->ip = ip3;

	bloque4->bloque_archivo = 4;
	bloque4->bytes = 1024;

	t_bloque_archivo* bloque5 = malloc(sizeof(t_bloque_archivo));
	bloque5->copia0 = malloc(sizeof(t_copia));
	bloque5->copia0->nodo = 3;
	bloque5->copia0->bloque_nodo = 3;
	bloque5->copia0->ip = ip3;

	bloque5->copia1 = malloc(sizeof(t_copia));
	bloque5->copia1->nodo = 2;
	bloque5->copia1->bloque_nodo = 1;
	bloque5->copia1->ip = ip2;

	bloque5->bloque_archivo = 5;
	bloque5->bytes = 1024;

	t_bloque_archivo* bloque6 = malloc(sizeof(t_bloque_archivo));
	bloque6->copia0 = malloc(sizeof(t_copia));
	bloque6->copia0->nodo = 1;
	bloque6->copia0->bloque_nodo = 1;
	bloque6->copia0->ip = ip1;

	bloque6->copia1 = malloc(sizeof(t_copia));
	bloque6->copia1->nodo = 2;
	bloque6->copia1->bloque_nodo = 1;
	bloque6->copia1->ip = ip2;

	bloque6->bloque_archivo = 6;
	bloque6->bytes = 1024;

	list_add(archivo, (void*) bloque);
	list_add(archivo, (void*) bloque1);
	list_add(archivo, (void*) bloque2);
	list_add(archivo, (void*) bloque3);
	list_add(archivo, (void*) bloque4);
	list_add(archivo, (void*) bloque5);
	list_add(archivo, (void*) bloque6);

	return archivo;
}


t_link_element * buscarWorkerMayorAvailability(t_list * planificacion){
	int i;
	t_link_element * mayor = planificacion->head;
	t_link_element * actual = planificacion->head->next;

	for(i=1; i<list_size(planificacion); i++){
		t_reg_planificacion * regActual = actual->data;
		t_reg_planificacion * regMayor = mayor->data;
		if(regActual->availability > regMayor->availability){
			mayor= actual;
		}
		actual = actual->next;
	}

	return mayor;
}

t_list * tablaPlanificacionCompleta(t_list* arch)
{

	t_list * planificacion = tablaPlanif();

	t_link_element *nodoMayorAvailability = buscarWorkerMayorAvailability(planificacion); // Se empieza apuntando al worker de mayor availability TODO: desampatar por el historial
	t_link_element *clock = nodoMayorAvailability;
	t_link_element *auxclock = nodoMayorAvailability;
	t_link_element *head = nodoMayorAvailability;

	t_reg_planificacion* ultimoReg = list_get(planificacion,list_size(planificacion) - 1);
	int i, bloqueNumero = 0;

	t_bloque_archivo* bloqueABuscar = list_get(arch, bloqueNumero);
	for (i = 0; i < list_size(arch); i++) { //Este for hace un bucle por cada bloque que se quiera encontrar

		auxclock = clock;
		t_reg_planificacion* reg = auxclock->data;
		auxclock = reg->worker != ultimoReg->worker ? auxclock->next : planificacion->head; //Se le suma a auxclock una posicion
		//todo hay que solucionar esto

		int encontrado = 0;
		t_reg_planificacion* regClock = clock->data;

		while (!encontrado && bloqueNumero<list_size(arch)) { //Se entra en un bucle del cual se va a salir cuando se le asigne un bloque al worker apuntado por clock

			if (regClock->availability > 0) {

				buscarBloqueEnWorker((void *) regClock, (void *) bloqueABuscar,
						&encontrado);

				//Si no se encuentra el bloque en el worker apuntado por clock se manda al auxclock a asignarlo
				if (!encontrado) {
					int encontradoAux = 0;
					while (!encontradoAux) {
						// Se dio una vuelta entera si son iguales
						if (auxclock != planificacion) {
							t_reg_planificacion* regAuxClock = malloc(sizeof(t_reg_planificacion));
							regAuxClock = auxclock->data;

							buscarBloqueEnWorker((void*) regAuxClock,
									(void *) bloqueABuscar, &encontradoAux);

							auxclock =reg->worker != ultimoReg->worker ? auxclock->next : planificacion->head;
						} else {
							for (i = 0; i < list_size(planificacion); i++) {
								t_reg_planificacion* regP = list_get(
										planificacion, i);
								regP->availability += BASE;
							}
						}
					}
				}

				bloqueNumero++;
				bloqueABuscar = list_get(arch, bloqueNumero);

			} else {
				regClock->availability = BASE;
				break;
			}
		}

		clock = regClock->worker != ultimoReg->worker ? clock->next : planificacion->head;

	}

	return planificacion;

}


char* generar_ruta_aleatoria(){

	time_t rawtime;
	  struct tm * timeinfo;
	  char *str_aleatorio= string_new();
	  char *temporal = string_new();

	  time (&rawtime);
	  timeinfo = localtime (&rawtime);

	  char* random_string = string_itoa(rand()% 99);
	  strftime (str_aleatorio,7,"%M%S",timeinfo);

	  string_append(&temporal, RUTA_TEMPORAL);
	  string_append(&temporal, str_aleatorio);
	  string_append(&temporal, random_string);

	  return temporal;
}

t_list * tablaestadosPrueba() {
	estados = list_create();

	t_estado* estado0 = malloc(sizeof(t_estado));
	estado0->job = 1;
	estado0->master = 1;
	estado0->nodo = 1;
	estado0->bloque = 8;
	estado0->etapa = 0;
	estado0->temporal = generar_ruta_aleatoria();
	estado0->estado = 0;

	t_estado *estado1 = malloc(sizeof(t_estado));
	estado1->job = 1;
	estado1->master = 1;
	estado1->nodo = 3;
	estado1->bloque = 2;
	estado1->etapa = 0;
	estado1->temporal = generar_ruta_aleatoria();
	estado1->estado = 0;

	t_estado *estado2 = malloc(sizeof(t_estado));
	estado2->job = 1;
	estado2->master = 1;
	estado2->nodo = 2;
	estado2->bloque = 9;
	estado2->etapa = 0;
	estado2->temporal = generar_ruta_aleatoria();
	estado2->estado = 0;


	t_estado* estado3 = malloc(sizeof(t_estado));
	estado3->job = 1;
	estado3->master = 1;
	estado3->nodo = 1;
	estado3->bloque = 8;
	estado3->etapa = 0;
	estado3->temporal = generar_ruta_aleatoria();
	estado3->estado = 0;

	list_add(estados, estado0);
	list_add(estados, estado1);
	list_add(estados, estado2);
	list_add(estados, estado3);

	return estados;
}

void levantar_config(void) {
	log_info(logger, "Levantando configuracion");
	config = config_create(ruta_config);
	log_info(logger, "Configuracion levantada");
}

void levantar_logger(void) {
	logger = abrir_logger(ruta_log, nombre_programa, LOG_LEVEL_INFO);
	log_info(logger, "Logger iniciado");
}

int levantar_servidor(void) {
	log_info(logger, "levantando servidor YAMA");
	socket_server = crearServidor(config_get_int_value(config, "YAMA_PUERTO"));
	if (socket_server < 0) {
		log_error(logger, "No se pudo levantar el servidor");
		return EXIT_FAILURE;
	}
	socket_clientes = list_create();
	return EXIT_SUCCESS;
}

int conectar_con_fs() {
	int proto_recibido;

	char* ip_fs = config_get_string_value(config,"FS_IP");
	int puerto_fs = config_get_int_value(config,"FS_PUERTO");

	log_info(logger,"conectando con proceso FILESYSTEM");
	socket_fs = conectar(puerto_fs,ip_fs);

	proto_recibido = recibirProtocolo(socket_fs);

	if(proto_recibido == FS_YM_ERRORCONN) {
		log_error(logger,"no se pudo conectar a filesystem. Reintentar mas tarde");
		return EXIT_FAILURE;
	}
	else log_info(logger,"conexion establecida con FILESYTEM");
	return EXIT_SUCCESS;
}

void esperar_masters(void) {

	int max_fds;
	int select_status;

	//el select te pide que le pases en el primer parametro cual es el filedescriptor mas alto
	// dentro de tu array de sockets(socket_cliente), a mi siempre me puso 4 para el socket del server
	// y 5,6,etc para los que iban llegando, de arranque el server es el unico asi que va ese.
	max_fds = socket_server;

	while (1) {

		// en cada iteracion hay que blanquear(FD_ZERO) el fds_masters, que es un bitarray donde queda guardado
		// que sockets tienen cambios despues del select y tambien volver a setear(FD_SET) los filedescriptors en el fds_set.
		//ademas de volver a calcular cual es el filedescriptor mas alto
		construir_fds(&max_fds);

		log_info(logger, "esperando conexiones");

		select_status = select(max_fds + 1, &fds_masters, NULL, NULL, NULL);
		//select_status puede ser:
		// < 0 => error
		// == 0 => no paso nada
		// > 0 => hubo algun cambio en los sockets o entro otro nuevo

		if (select_status < 0) {
			perror("select failed");
			log_error(logger, "fallo select");
			return;
		} else if (select_status > 0) {
			leer_cambios_select();
		}
	}

}

void construir_fds(int* max_actual) {
	int i;

	FD_ZERO(&fds_masters);
	FD_SET(socket_server, &fds_masters);
	for (i = 0; i < list_size(socket_clientes); i++) {
		int socket_actual = get_socket_posicion(i);
		FD_SET(socket_actual, &fds_masters);
		if (*max_actual < socket_actual) {
			*max_actual = socket_actual;
		}
	}
}

void leer_cambios_select() {
	int i;

	//nuevo cliente
	if (FD_ISSET(socket_server, &fds_masters)) {
		log_info(logger, "se recibio una nueva conexion");
		recibir_nuevo_master();
	}

	//recibo data de uno ya conectado
	//TODO handlear si el cliente se desconecta abruptamente(seguramente hay que usar alguna signal o algo asi)
	for (i = 0; i < list_size(socket_clientes); i++) {
		int socket_actual = get_socket_posicion(i);
		if (FD_ISSET(socket_actual, &fds_masters)) {
			log_info(logger, "se recibio data de un master conectado");
			recibir_data_de_master(i);
		}
	}
}

void recibir_nuevo_master() {
	int nuevo_cliente;

	//conecto cliente en un nuevo socket
	if ((nuevo_cliente = esperarConexion(socket_server)) < 0) {
		log_error(logger, "no se pudo crear conexion");
	}

	list_add(socket_clientes,(void*) nuevo_cliente);

	// Enviamos un protocolo de confirmacion
	protocolo = YM_MS_OKCONN;
	enviarProtocolo(nuevo_cliente, protocolo);
	nuevo_cliente = -1;
}

void recibir_data_de_master(int posicion) {
	int proto_msg,desconectado;

	//defino todas las copias de 3 bloques de archivo que estan desparramadas en 3 nodos
	copia_0_bloque_0.nodo = 0;
	copia_0_bloque_0.bloque_nodo = 0;
	copia_0_bloque_0.ip = string_from_format("127.0.0.1");

	copia_1_bloque_0.nodo = 1;
	copia_1_bloque_0.bloque_nodo = 2;
	copia_1_bloque_0.ip = string_from_format("127.0.0.1");

	copia_0_bloque_1.nodo = 0;
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

	//defino los bloques del archivo que tienen las copias
	bloque_mock_0.bloque_archivo = 0;
	bloque_mock_0.copia0 = &copia_0_bloque_0;
	bloque_mock_0.copia1 = &copia_1_bloque_0;
	bloque_mock_0.bytes = 12;

	bloque_mock_1.bloque_archivo = 1;
	bloque_mock_1.copia0 = &copia_0_bloque_1;
	bloque_mock_1.copia1 = &copia_1_bloque_1;
	bloque_mock_1.bytes = 25;

	bloque_mock_2.bloque_archivo = 2;
	bloque_mock_2.copia0 = &copia_0_bloque_2;
	bloque_mock_2.copia1 = &copia_1_bloque_2;
	bloque_mock_2.bytes = 65;

	//lista de bloques que me llegan
	list_add(&lista_de_nodos_recibidos, (void*) &bloque_mock_0);
	list_add(&lista_de_nodos_recibidos, (void*) &bloque_mock_1);
	list_add(&lista_de_nodos_recibidos, (void*) &bloque_mock_2);

	//---------------------------------------------------

	proto_msg = recibirProtocolo(get_socket_posicion(posicion));

	switch (proto_msg) {
	case MS_YM_DESCONECTAR:	// se desconecta master
		desconectado = get_socket_posicion(posicion);
		log_info(logger, "se desconecto el master %d", desconectado);
		close(desconectado);
		list_remove(socket_clientes,posicion);
		break;
	case MS_YM_INICIO_JOB:	// inicia un job un master
		atender_inicio_job(posicion);
		break;
	case FIN_TRANSF_NODO:
		atender_fin_transf_bloque(posicion);
		break;
	case FIN_TRANSFORMACION:
		atender_fin_transformacion(posicion);
	}
}

void enviar_transformacion(int master, t_list* lista_bloques) {

	void _iterate_bloques(t_reg_planificacion* worker_planificado) {
		char* copia_string = reg_planificacion_to_string(worker_planificado);
		enviarMensaje(master, copia_string);
	}

	//envio protocolo de transformacion
	enviarProtocolo(master, YM_MS_TRANSFORMACION);
	//envio cantidad de bloques que voy a mandar
	enviarMensaje(master, int_to_string(list_size(lista_bloques)));
	//itero bloques y los mando serializados
	list_iterate(lista_bloques, (void*) _iterate_bloques);
}

void atender_inicio_job(int posicion) {
	char* mensaje;
	char* char_archivo;
	int cliente = get_socket_posicion(posicion);
	log_info(logger, "iniciar job de master %d", cliente);
	mensaje = esperarMensaje(cliente);
	log_info(logger, "ruta de archivo %s", mensaje);
	//enviar_ruta_fs(mensaje);

	//char_archivo = esperarMensaje(cliente);

	//todo mapear archivo a lista de struct
	//por ahora hardcodeamos la estructura que recibe


	t_list* archivo_planificado = tablaPlanificacionCompleta(&lista_de_nodos_recibidos);
//
//	//mockeo las copias que se eligen
//	bloque_mock_0.elegida = 0;
//	bloque_mock_0.ruta_temporal = string_from_format("/tmp/master%d-temp%d", 0,
//			0); //todo crear estructura que lleve cuenta de los archivos temporales creados y los master uqe se conectaron??
//	bloque_mock_1.elegida = 1;
//	bloque_mock_1.ruta_temporal = string_from_format("/tmp/master%d-temp%d", 0,
//			1);
//	bloque_mock_2.elegida = 0;
//	bloque_mock_2.ruta_temporal = string_from_format("/tmp/master%d-temp%d", 0,
//			2);

//	list_add(&lista_de_nodos_respuesta, (void*) &bloque_mock_0);
//	list_add(&lista_de_nodos_respuesta, (void*) &bloque_mock_1);
//	list_add(&lista_de_nodos_respuesta, (void*) &bloque_mock_2);
	// se manda la lista de bloques ya modificados a master
	enviar_transformacion(cliente, archivo_planificado);

	transformarBloques(archivo_planificado);
}

void atender_fin_transf_bloque(int posicion) {
	int cliente = get_socket_posicion(posicion);
	char* char_bloque = esperarMensaje(cliente);
	int bloque = atoi(char_bloque);
	modificarBloqueTablaEstados(bloque,ETAPA_TRANSFORMACION,ESTADO_FINALIZADO_OK,posicion,0);//TODO que mierda le paso en los ultimos 2
	log_info(logger, "finalizo la transformacion del bloque %d del master %d",
			bloque, socket_clientes[posicion]);
	int estado = terminoEtapaTransformacion(0,posicion);//todo aca deberia ir el worker y el job

	if(estado) {
		t_list* reg = iniciarReduccion(posicion,0);//todo lo mismo que el anteiror
	}

	//TODO marcar en tabla *de estados que termino la transformacion del bloque, si no hay ninguna otra transformacion en curso en ese nodo del worker,arranco reduccion
}

void atender_fin_transformacion(int posicion) {
	//TODO marcar en tabla de estados la etapa en que se encuentra??
	log_info(logger, "finalizaron las transformaciones de master %d",
			socket_clientes[posicion]);
	//enviar_reduccion_local(posicion);
}

void enviar_ruta_fs(char* mensaje) {
	enviarMensajeConProtocolo(socket_fs,mensaje,YM_FS_RUTA);
}

void enviar_reduccion_local(int posicion) {

}

int get_socket_posicion(int posicion) {
	return (int) list_get(socket_clientes,posicion);
}

