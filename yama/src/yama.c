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

useconds_t retardo_planificacion = 10000000;
int puerto = 2323;
int protocoloInicio = 0010;
int servidor;
int numeroDeJob = 0;
int numeroDeMaster = 0;
t_list *archivo;
t_list *estados;
t_list *planif;
t_list *cargas_trabajo;
int BASE = 2;
t_list *numeros_aleatorios;
int algoritmo = ALGORITMO_WCLOCK;
bool necesidadRecargaConfig = false;


/*--------------------------- Main ------------------------------*/
int main(void) {

	signal(SIGUSR1, signal_handler);
	printf("Process Id = %d\n",(int)getpid());
	levantar_logger();

	levantar_config();

	srand(time(NULL));
	t_list* tabla = iniciarTransformacion(0);
	list_destroy_and_destroy_elements(tabla, (void*)planificacion_destroy);
	loguear_cargas();
	tabla = iniciarTransformacion(1);
	list_destroy_and_destroy_elements(tabla, (void*)planificacion_destroy);
	loguear_cargas();
	finalizarTransformacion(1,1);
	finalizarTransformacion(1,2);
	finalizarTransformacion(1,3);
	t_list* reduccionLocal = iniciarReduccionLocal(1,1);
	reduccionLocal = iniciarReduccionLocal(1,2);
	reduccionLocal = iniciarReduccionLocal(1,3);
	fallaEtapa(1);
	loguear_cargas();
	tabla = iniciarTransformacion(0);
	list_destroy_and_destroy_elements(tabla, (void*)planificacion_destroy);
	loguear_cargas();

	tabla = iniciarTransformacion(2);
	list_destroy_and_destroy_elements(tabla, (void*)planificacion_destroy);
	tabla = iniciarTransformacion(3);
	list_destroy_and_destroy_elements(tabla, (void*)planificacion_destroy);
	loguear_cargas();
	finalizarTransformacion(1,1);
	finalizarTransformacion(1,2);
	finalizarTransformacion(1,3);



	//reduccionLocal = iniciarReduccionLocal(2,1);
	//reduccionLocal = iniciarReduccionLocal(2,2);
	//reduccionLocal = iniciarReduccionLocal(2,3);

	/*finalizarReduccionLocal(2,1);
		finalizarReduccionLocal(2,2);
		finalizarReduccionLocal(2,2);
		t_list* reduccionGlobal = iniciarReduccionGlobal(2);
		finalizarReduccionGlobal(2);*/
	finalizarReduccionLocal(1,1);
	finalizarReduccionLocal(1,2);
	finalizarReduccionLocal(1,3);
	t_list* reduccionGlobal = iniciarReduccionGlobal(1);
	loguear_cargas();
	finalizarReduccionGlobal(1);
	loguear_cargas();

	enviarAMaster(tabla);
	modificarBloqueTablaEstados(1, 2, 3, 1, 1);

//	 srand(time(NULL));
//	t_list * tabla = tablaPlanificacionCompleta();
//	enviarAMaster(tabla);
//	modificarBloqueTablaEstados(1, 2, 3, 1, 1);



	levantar_servidor();

	esperar_masters();

	log_info(logger, "Salir del programa");
	return EXIT_SUCCESS;
}


/*int abrir_file() {
	char* ruta_file;
	char* linea = NULL;
	size_t len= 0;
	size_t leido;
	FILE* archivo;

	//abrir file y loggear lineas
	ruta_file = RUTA_ARCHIVO_CONFIG;


	archivo = fopen(ruta_file, "r");
	if(archivo == NULL) {
		log_error(logger, "file %s no existe", ruta_file);
		return EXIT_FAILURE;
	}

	while((leido = getline(&linea, &len, archivo)) != -1) {

		char* token = strtok(linea, "=");

		if(string_contains(linea, "FS_IP")){
			char* FS_IP = strtok(NULL, "\n");
			int i=0;
		}else if(string_contains(linea, "YAMA_PUERTO")){
			int YAMA_PUERTO = atoi(strtok(NULL, "\n"));
			int i=0;
		}else if(string_contains(linea, "ALGORTIMO")){
			int ALGORITMO = atoi(strtok(NULL, "\n"));
			int i=0;
		}

	}

	fclose(archivo);
	if(linea){
		free(linea);
	}
	return EXIT_SUCCESS;
}*/
/*--------------------------- Manejas senial ------------------------------*/

void signal_handler(int signal){
	log_info(logger, "Pedido de recarga de configuracion");
	necesidadRecargaConfig = true;
}

/*--------------------------- Manejo de Bloques Tabla de Estados ------------------------------*/

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

void loguear_actualizacion_estado(int bloque, char* etapa, char* estadoNuevo, int worker, int job, int master, char* temporal) {
	log_info(logger, "Estado actualizado: Job: %d, master: %d, worker: %d, bloque: %d, etapa: %s, estado*: %s, temporal: %s.",job , master, worker, bloque, etapa, estadoNuevo, temporal);
}
void loguear_agregar_estado(int bloque, char* etapa, char* estadoNuevo, int worker, int job, int master, char* temporal) {
	log_info(logger, "Estado agregado: Job: %d, master: %d, worker: %d, bloque: %d, etapa*: %s, estado: %s, temporal: %s.",job , master, worker, bloque, etapa, estadoNuevo, temporal);
}
void loguear_cargas(){
	int i;
	for(i=0 ; i<list_size(cargas_trabajo); i++){
			t_carga_trabajo* reg = list_get(cargas_trabajo, i);
			log_info(logger, "job: %d, Worker: %d, carga: %d",reg->job,reg->worker, reg->carga, reg->tareasAsignadas);
		}
}
void agregarBloque(void* registro, int bloque) {

	t_reg_planificacion* reg = registro;
	list_add(reg->bloques, (void *)bloque);
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

				if(estados == NULL){
					estados =list_create();
					list_add(estados, (void*) estado);
				}else{
				list_add(estados, (void*) estado);
				}
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


/*--------------------------- Funciones de algoritmo planificacion ------------------------------*/
int PWL(int worker, int job){
		//

	if(algoritmo == ALGORITMO_WCLOCK){
		int wlmax = cargaTrabajoMaxima(job); //la mayor carga de trabajo entre todos los worker. Se debera recorrer la tabla de estados para ver cual es la mayor carga de trabajo
		int wl = cargaTrabajoWorker(worker); //la carga de trabajo actual del worker. Se obtiene de la tabla de estados
		return wlmax - wl;
	}else if(algoritmo == ALGORITMO_CLOCK){
			return 0;
		}
}


int cargaTrabajoWorker(int worker){

	int i;
	int carga=0;

	if(cargas_trabajo == NULL || list_is_empty(cargas_trabajo)){ // Si no hay estados por ser el primero
		return 0;
	}

	for(i=0; i<list_size(cargas_trabajo); i++){
		t_carga_trabajo *carga_trabajo = list_get(cargas_trabajo, i);
		if(carga_trabajo->worker == worker){
			carga += carga_trabajo->carga;
		}
	}
	return carga;
}

bool repetido(int nodo, t_list * nodosDelJob){
	int i;
		for(i=0; i<list_size(nodosDelJob); i++)
		{
			int nodoYaGuardado = (int)list_get(nodosDelJob, i);
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
		int maximaCarga = -1;
		t_carga_trabajo* nodoMaximo;
		if(cargas_trabajo == NULL || list_is_empty(cargas_trabajo)){
				return 0;
			}

		for(i=0; i<list_size(cargas_trabajo); i++){
			t_carga_trabajo* nodoActual = list_get(cargas_trabajo, i);
			if(nodoActual->job == job){
				int cargaActual = cargaTrabajoWorker(nodoActual->worker);
				if(cargaActual > maximaCarga){
					maximaCarga = cargaActual;
					nodoMaximo = nodoActual;
				}
			}
		}
		return maximaCarga;


}

int cargaTrabajoMinima(int job){

	int i;
	int minimaCarga = 100000000;
	t_carga_trabajo* nodoMinimo;
	if(cargas_trabajo == NULL || list_is_empty(cargas_trabajo)){
		return 0;
	}

	for(i=0; i<list_size(cargas_trabajo); i++){
		t_carga_trabajo* nodoActual = list_get(cargas_trabajo, i);
		if(nodoActual->job == job){
			int cargaActual = cargaTrabajoWorker(nodoActual->worker);
			if(cargaActual < minimaCarga){
				minimaCarga = cargaActual;
				nodoMinimo = nodoActual;
			}
		}
	}
	return nodoMinimo->worker;

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

void* buscarRegEnLista(t_list *planif, int nodo) {
	int i;
	for (i = 0; i < list_size(planif); i++) {
		t_reg_planificacion* reg = list_get(planif, i);
		if (reg->worker == nodo) {
			return (void*) reg;
		}
	}
	return 0;
}

t_link_element * buscarWorkerMayorAvailability(t_list* planificacion){
	int i;

	if(list_size(planificacion)<2){ //Si no hay contra quien comparar retorna el primero, por las dudas verifico
		return planificacion->head;
	}
	t_reg_planificacion* mayor = list_get(planificacion, 0);
	t_reg_planificacion* puntero_mayor;

	for(i=1; i<list_size(planificacion); i++){
		t_reg_planificacion* actual = list_get(planificacion, i);
		if(actual->availability > mayor->availability){
			mayor= actual;
		}
	}

	/*Esto es porque cuando hicimos el algoritmo eramos unos giles*/
	t_link_element* puntero= planificacion->head;
	for(i=0; i<list_size(planificacion); i++){
		puntero_mayor = puntero->data;
		if(puntero_mayor->worker == mayor->worker){
			return puntero;
		}
		puntero = puntero->next;
	}

}

void agregar_carga(int job, int worker, int carga){
	t_carga_trabajo* carga_trabajo = malloc(sizeof(t_carga_trabajo));
	carga_trabajo->worker = worker;
	carga_trabajo->job = job;
	carga_trabajo->carga = carga;
	list_add(cargas_trabajo, (void*) carga_trabajo);
}


void crear_cargas_inicio(t_list * planificacion, int job){
	int i;
	for(i=0; i<list_size(planificacion); i++){
		t_reg_planificacion* regPlani = list_get(planificacion, i);
		if(cargas_trabajo == NULL){
			cargas_trabajo = list_create();
		}
		agregar_carga(job, regPlani->worker, 0);
	}
}

void actualizar_cargas_inicio(t_list * planificacion){
	int i, j;
	for(i=0; i<list_size(planificacion); i++){
		t_reg_planificacion* regPlani = list_get(planificacion, i);
		for(j=0; j<list_size(cargas_trabajo); j++){
			t_carga_trabajo* carga_trabajo = list_get(cargas_trabajo, j);
			if(regPlani->job == carga_trabajo->job && regPlani->worker == carga_trabajo->worker){
				carga_trabajo->carga += list_size(regPlani->bloquesAsignados);
				int h=8;
				break;
			}
		}
	}
}

void actualizar_cargas_reduccion_global(int workerEncargado,int cantTemporales, int job){
	int i;
		for(i=0; i<list_size(cargas_trabajo); i++){
			t_carga_trabajo* carga_trabajo = list_get(cargas_trabajo, i);
			if(workerEncargado == carga_trabajo->worker && job == carga_trabajo->job){
				//carga_trabajo->carga += ceil(cantTemporales/2);
				carga_trabajo->carga += cantTemporales/2;
			}
		}
}

void planificacion_destroy(t_reg_planificacion *self) {
	free(self);
}
void carga_destroy(t_carga_trabajo *self) {
	free(self);
}

void actualizar_cargas_fin_job(int job){
	int i;
		for(i=0; i< list_size(cargas_trabajo); i++){
			t_carga_trabajo* carga_trabajo = list_get(cargas_trabajo, i);
			if(job == carga_trabajo->job){
				list_remove_and_destroy_element(cargas_trabajo, i, (void*)carga_destroy);
				i--;
			}
		}
}

void asignar_availability(t_list* planificacion){
	int i;
	for(i=0; i< list_size(planificacion); i++){
		t_reg_planificacion* regPlani = list_get(planificacion, i);
		regPlani->availability = BASE+ PWL(regPlani->worker, regPlani->job);
		int h=9;
	}

}
t_list * tablaPlanificacionCompleta(t_list* arch, int job)
{
	usleep(retardo_planificacion);
	t_list * planificacion = tablaPlanif(job);
	crear_cargas_inicio(planificacion, job);
	asignar_availability(planificacion);
	t_link_element *nodoMayorAvailability;

	if(cargas_trabajo == NULL || list_is_empty(cargas_trabajo)){
		nodoMayorAvailability = planificacion->head;
	}else{
		nodoMayorAvailability = buscarWorkerMayorAvailability(planificacion); // Se empieza apuntando al worker de mayor availability TODO: desampatar por el historial
	}
	t_link_element *clock = nodoMayorAvailability;
	t_link_element *auxclock = nodoMayorAvailability;
	t_link_element *head = nodoMayorAvailability;

	t_reg_planificacion* ultimoReg = list_get(planificacion,list_size(planificacion) - 1);
	int i, j, bloqueNumero = 0;

	t_bloque_archivo* bloqueABuscar = list_get(arch, bloqueNumero);
	for (i = 0; i < list_size(arch); i++) { //Este for hace un bucle por cada bloque que se quiera encontrar

		auxclock = clock;
		t_reg_planificacion* reg = auxclock->data;
		auxclock = reg->worker != ultimoReg->worker ? auxclock->next : planificacion->head; //Se le suma a auxclock una posicion


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
							for (j = 0; j < list_size(planificacion); j++) {
								t_reg_planificacion* regP = list_get(
										planificacion, j);
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



/*--------------------------- Transformacion------------------------------*/

void recorrerTablaEstados(t_list* lista){
	int i;
	for (i = 0; i < list_size(lista); i++) {
				t_estado* estado= list_get(lista, i);
				int j=3;
			}
}

/*Agrega a la tabla de estados los campos correspondientes y genera la estructura ha enviar a master*/
//todo: ver como cargar ip y puerto
t_list* iniciarTransformacion(int master){
	if(necesidadRecargaConfig){
		log_info(logger, "Recarga de configuracion"); //todo
		necesidadRecargaConfig = false;
	}
	numeroDeJob++;
	int job = numeroDeJob;
	int i, j;
	t_list *archivo = archivoPrueba2();
	t_list *tabla = tablaPlanificacionCompleta(archivo, job);
	actualizar_cargas_inicio(tabla);
	char *rutaReducGlobal = '\0';
	char *rutaReducLocal = '\0';

	for (i = 0; i < list_size(tabla); i++) {
		t_reg_planificacion* reg = list_get(tabla, i);
		/*Completo los campos faltantes*/
		reg->tempReducLocal = rutaReducLocal;
		reg->tempReducGlobal = rutaReducGlobal;
		reg->job = job; //Asigno el numero de job

		t_list* temporales= list_create();
		j = 0;
		for (j = 0; j < list_size(reg->bloquesAsignados); j++){
			/*Asigno temporales transformacion a cada nodo*/
			char* rutaTransf= generar_ruta_aleatoria();
			list_add(temporales, (void*) rutaTransf);
			/*Modifico tabla de estados*/
			int bloqueActual = (int)list_get(reg->bloquesAsignados, j);
			agregarEstados(bloqueActual, ETAPA_TRANSFORMACION, ESTADO_EN_PROCESO, reg->worker, job, master, rutaTransf);
			loguear_agregar_estado(bloqueActual, "transformacion", "en proceso", reg->worker, reg->job, master, rutaTransf);
		}
		reg->temporalesTransformacion = temporales;

	}

	recorrerTablaEstados(estados);
	return tabla;
}



/*Verifica si termino la etapa*/
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

/*Modifica la tabla de estados para indicar que la etapa finalizo correctamente*/
void finalizarTransformacion(int job, int nodo){
	int i;
			t_link_element *puntTabla = estados->head;
			for (i = 0; i < list_size(estados); i++) {
				t_estado* reg = puntTabla->data;
				if(reg->job == job && reg->nodo == nodo && reg->etapa == ETAPA_TRANSFORMACION){
					reg->estado = ESTADO_FINALIZADO_OK;
					loguear_actualizacion_estado(reg->bloque,"transformacion", "finalizado", reg->nodo, reg->job, reg->master, reg->temporal);
				}
				puntTabla = puntTabla->next;
			}
}

char* recuperar_ip(int nodo){
	//todo: recorrer la lista de fs y recuperar la ip
	return "192.198.1.1:5000";
}
/*---------------------------Reduccion Local------------------------------*/

/*Agrega a la tabla de estados los campos correspondientes y genera la estructura ha enviar a master*/
t_list* iniciarReduccionLocal(int job, int nodo){

	int i;
	t_list *reduccion = list_create();
	t_link_element *puntTabla = estados->head;

	char* rutaReducLocal= generar_ruta_aleatoria();
	t_reg_planificacion *regPlani = malloc(sizeof(t_reg_planificacion));

	regPlani->worker = nodo;
	regPlani->ip = recuperar_ip(nodo);
	regPlani->tempReducLocal = rutaReducLocal;

	/*Completo los campos faltantes*/
	//todo: verificar si esto es correcto
	regPlani->bloques = list_create();
	regPlani->bloquesAsignados = list_create();
	regPlani->tempReducGlobal = '\0';
	regPlani->availability = -1;

	regPlani->temporalesTransformacion = list_create();
	regPlani->tempReducLocal = generar_ruta_aleatoria();

	for (i = 0; i < list_size(estados); i++) {
		t_estado* reg = puntTabla->data;
		if(reg->job == job && reg->nodo == nodo && reg->etapa == ETAPA_TRANSFORMACION){
			list_add(regPlani->temporalesTransformacion, (void*) reg->temporal);
			agregarEstados(reg->bloque, ETAPA_REDUCCION_LOCAL, ESTADO_EN_PROCESO, nodo, job, reg->master, rutaReducLocal);
			loguear_agregar_estado(reg->bloque, "reduccion local", "en proceso", nodo, job, reg->master, rutaReducLocal);
		}
		puntTabla = puntTabla->next;
	}
	list_add(reduccion, (void*) regPlani);
	recorrerTablaEstados(estados);
	return reduccion;

}

/*Verifica si termino la etapa*/
bool terminoEtapaReduccionLocal(int job){

	int i;
		t_link_element *puntTabla = estados->head;
		for (i = 0; i < list_size(estados); i++) {
			t_estado* reg = puntTabla->data;
			if(reg->job == job){
				if(reg->etapa == ETAPA_REDUCCION_LOCAL && reg->estado == ESTADO_EN_PROCESO){
					return false;
				}
			}
			puntTabla = puntTabla->next;
		}
		return true;
}

/*Modifica la tabla de estados para indicar que la etapa finalizo correctamente*/
void finalizarReduccionLocal(int job, int nodo){
	int i;
			t_link_element *puntTabla = estados->head;
			for (i = 0; i < list_size(estados); i++) {
				t_estado* reg = puntTabla->data;
				if(reg->job == job && reg->nodo == nodo && reg->etapa == ETAPA_REDUCCION_LOCAL){
					reg->estado = ESTADO_FINALIZADO_OK;
					loguear_actualizacion_estado(reg->bloque, "reduccion local", "finalizado", reg->nodo, reg->job, reg->master, reg->temporal);
				}
				puntTabla = puntTabla->next;
			}
}

t_list* nodosDelJob(int job){


	t_list * nodosDelJob;
		nodosDelJob = list_create();
		int i;
		for(i=0; i<list_size(estados); i++){
			t_estado *estado = list_get(estados, i);
			if(estado->job == job && !repetido(estado->nodo, nodosDelJob)){
				int nodo = estado->nodo;
				list_add(nodosDelJob, (void*) nodo);
			}
		}
		return nodosDelJob;
}

/*---------------------------Reduccion Global------------------------------*/

/*Agrega a la tabla de estados los campos correspondientes y genera la estructura ha enviar a master*/
t_list* iniciarReduccionGlobal(int job){

	int i;
	int cantReduccionesLocales = 0;
	t_list *reduccion = list_create();
	t_link_element *puntTabla = estados->head;

	char* rutaReducGlobal;
	char* rutaAleatoria= generar_ruta_aleatoria();
	t_list* nodos = nodosDelJob(job);

	int nodoEncargado = cargaTrabajoMinima(job);

	for (i = 0; i < list_size(nodos); i++) {
		int nodoActual = (int)list_get(nodos, i);
		t_reg_planificacion *regPlani = malloc(sizeof(t_reg_planificacion));
		bool encontrado = false;
		while (!encontrado) {
			encontrado = true;
			t_estado* reg = puntTabla->data;
			if(reg->job == job && reg->etapa == ETAPA_REDUCCION_LOCAL && reg->nodo == nodoActual){
				encontrado = true;
				if(reg->nodo == nodoEncargado){
					rutaReducGlobal = rutaAleatoria;
				}else{
					rutaReducGlobal = '\0';
				}
				/*Completo los campos que faltan*/
				regPlani->temporalesTransformacion = list_create();
				regPlani->bloques = list_create();
				regPlani->bloquesAsignados = list_create();
				regPlani->availability = -1;

				regPlani->tempReducLocal = reg->temporal;
				regPlani->worker = reg->nodo;
				regPlani->ip = recuperar_ip(reg->nodo);
				regPlani->job = reg->job;

				regPlani->tempReducGlobal = rutaReducGlobal;

			}
			puntTabla = puntTabla->next;
		}
		list_add(reduccion, (void*) regPlani);
	}

	puntTabla = estados->head;
	for (i = 0; i < list_size(estados); i++) {
		t_estado* reg = puntTabla->data;
		/*Modifico la tabla de estados*/
		if(reg->job == job && reg->etapa == ETAPA_REDUCCION_LOCAL){
			if(reg->nodo == nodoEncargado){
				rutaReducGlobal = rutaAleatoria;
			}else{
				rutaReducGlobal = '\0';
			}
			agregarEstados(reg->bloque, ETAPA_REDUCCION_GLOBAL, ESTADO_EN_PROCESO, reg->nodo, job, reg->master, rutaReducGlobal); // aca el master esta harcodeados
			loguear_agregar_estado(reg->bloque, "reduccion global", "en proceso", reg->nodo, job, reg->master, rutaReducGlobal);
		}
		puntTabla = puntTabla->next;
	}

	actualizar_cargas_reduccion_global(nodoEncargado, list_size(nodos), job);

	return reduccion;

}

/*Modifica la tabla de estados para indicar que la etapa finalizo correctamente*/
void finalizarReduccionGlobal(int job){
	int i;
			t_link_element *puntTabla = estados->head;
			for (i = 0; i < list_size(estados); i++) {
				t_estado* reg = puntTabla->data;
				if(reg->job == job && reg->etapa == ETAPA_REDUCCION_GLOBAL){
					reg->estado = ESTADO_FINALIZADO_OK;
					loguear_actualizacion_estado(reg->bloque, "reduccion global", "finalizado", reg->nodo, reg->job, reg->master, reg->temporal);
				}
				puntTabla = puntTabla->next;
			}
			actualizar_cargas_fin_job(job);
}


/*--------------------------- Replanificacion ------------------------------*/
void replanificar(int job, int planificacion)
{
	//TODO: enviar mensaje a  mastar para que termine un job y en caso de que planificacion sea PEDIDO_TRANSFORMACION entonces debera pedir
	// nuevamente que planifiquemos y si es PEDIDO_ABORTO no lo haga

	if(planificacion == PEDIDO_TRANSFORMACION){
		log_info(logger, "Se ha caido un nodo realizando la etapa de transformacion, se realizara una re-planificacion del job %d",job);
	}else{
		log_info(logger, "Se ha caido un nodo realizando la etapa de reduccion, se realizara un aborto del job %d", job);
	}

}


void fallaEtapa(int nodo){

			int i,j, replanificacion;
			t_list *jobs = list_create();
			for (i = 0; i < list_size(estados); i++) {
				t_estado *estado = list_get(estados, i);

				if(estado->nodo == nodo && estado->estado == ESTADO_EN_PROCESO){
					if(estado->etapa == ETAPA_TRANSFORMACION )
					{
						 replanificacion = PEDIDO_TRANSFORMACION;

					} else {
						 replanificacion = PEDIDO_ABORTO;

					}
					//TODO:: Probar que funciones lo de los jobs repetidos
					bool jobRepetido = false;
					for (j = 0; j < list_size(jobs); j++) {
							int* job = list_get(jobs, j);
							if (estado->job == (int)job) {
								jobRepetido = true;
								break;
							}
						}
					if(!jobRepetido){
						list_add(jobs, (void*) estado->job);
						actualizar_cargas_fin_job(estado->job);
						replanificar(estado->job, replanificacion);
					}
					modificarBloqueTablaEstados(estado->bloque, estado->etapa, ESTADO_ERROR, estado->nodo, estado->job);
				}
			}

}




/*--------------------------- Modelos para testing ------------------------------*/
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

void cargar_archivo(int bloque_archivo, int nodo_copia_0, int bloque_copia_0, int nodo_copia_1, int bloque_copia_1, int bytes_ocupados){
	t_bloque_archivo* bloque = malloc(sizeof(t_bloque_archivo));
	bloque->copia0->nodo = nodo_copia_0;
	bloque->copia0->bloque_nodo = bloque_copia_0;
	bloque->copia0->ip = recuperar_ip(nodo_copia_0);

	bloque->copia1->nodo = nodo_copia_1;
	bloque->copia1->bloque_nodo = bloque_copia_1;
	bloque->copia1->ip = recuperar_ip(nodo_copia_1);

	bloque->bloque_archivo = bloque_archivo;
	bloque->bytes = bytes_ocupados;

	list_add(archivo, (void *)bloque);
}


t_list* tablaPlanif(int job) {
	t_list* arch = archivoPrueba2();
	t_list* planificacion = list_create();
	//t_list * estados = tablaestadosPrueba();

	int i;

	for (i = 0; i < list_size(arch); i++) {
		t_bloque_archivo *bloque = list_get(arch, i);
		int nBloque = bloque->bloque_archivo;
		int nodocopia0 = bloque->copia0->nodo;
		int nodocopia1 = bloque->copia1->nodo;
		if(nodocopia0 != -1){
			if (!nodoYaAsignado(planificacion, nodocopia0)) {
				t_reg_planificacion* reg = malloc(sizeof(t_reg_planificacion));
				reg->worker = nodocopia0;
				reg->availability = 0;
				reg->bloquesAsignados = list_create();
				reg->bloques = list_create();

				reg->job = job;
				agregarBloque(reg, nBloque);
				list_add(planificacion, (void*) reg);


			} else {

		/*if (!nodoYaAsignado(planificacion, nodocopia0)) {
			t_reg_planificacion* reg = malloc(sizeof(t_reg_planificacion));
			reg->worker = nodocopia0;
			reg->availability = BASE+ PWL(reg->worker, job);
			reg->bloquesAsignados = list_create();
			reg->bloques = list_create();

			reg->temporalTransformacion = string_from_format("/tmp/Master-",int_to_string(i));
			reg->tempReducLocal = string_from_format("/tmp/Master-",int_to_string(i));
			reg->tempReudcGlobal= string_from_format("/tmp/Master-",int_to_string(i));
			reg->ip = "127.0.0.1";
			reg->job = job;
			agregarBloque(reg, nBloque);
			list_add(planificacion, (void*) reg);

		} else {*/


				t_reg_planificacion* reg = buscarRegEnLista(planificacion,
						nodocopia0);
				agregarBloque(reg, nBloque);
			}
		}



		if(nodocopia1 != -1){
			if (!nodoYaAsignado(planificacion, nodocopia1)) {
				t_reg_planificacion* reg1 = malloc(sizeof(t_reg_planificacion));
				reg1->worker = nodocopia1;
				reg1->availability = 0;
				reg1->bloquesAsignados = list_create();
				reg1->bloques = list_create();
				reg1->job = job;
				agregarBloque(reg1, nBloque);
				list_add(planificacion, (void*) reg1);
			} else {
				t_reg_planificacion* reg1 = buscarRegEnLista(planificacion,
						nodocopia1);
				agregarBloque(reg1, nBloque);
			}

		/*if (!nodoYaAsignado(planificacion, nodocopia1)) {
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
			agregarBloque(reg1, nBloque);*/

		}
	}

	return planificacion;

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



/*--------------------------- Comunicaciones ------------------------------*/

void enviarAMaster(t_list * tabla) {

	transformarBloques(tabla);
	//TODO ACA FALTA ENVIARLE A MASTER LA TABLA
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


	t_list* archivo_planificado = tablaPlanificacionCompleta(&lista_de_nodos_recibidos, posicion); //En posicion se debe mandar el numero de job
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
		t_list* reg = iniciarReduccionLocal(posicion,0);//todo lo mismo que el anteiror
	}

	//TODO marcar en tabla *de estados que termino la transformacion del bloque, si no hay ninguna otra transformacion en curso en ese nodo del worker,arranco reduccion
}

void atender_fin_transformacion(int posicion) {
	//TODO marcar en tabla de estados la etapa en que se encuentra??
	log_info(logger, "finalizaron las transformaciones de master %d",
			socket_clientes[posicion]);

	/*if(terminoEtapaTransformacion(job, worker)){
		iniciarReduccionLocal(job, worker)
	}*/
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

