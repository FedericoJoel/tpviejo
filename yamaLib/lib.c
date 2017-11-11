#include "lib.h"

//---------------logger-------------------

t_log* abrir_logger(char* ruta_archivo, char* nombre_programa,
		t_log_level nivel) {
	return log_create(ruta_archivo, nombre_programa, 1, nivel);
}

char* reg_planificacion_to_string( t_reg_planificacion* data_worker) {
	char* char_data_worker = string_new();
	char* tamanio;

	char* char_availability = int_to_string(data_worker->availability);
	char* char_worker = int_to_string(data_worker->worker);

	char* char_bloques = iterate_bloques_to_string(data_worker->bloques);
	char* cant_bloques = int_to_string(list_size(data_worker->bloques));

	char* char_bloques_asignados = iterate_bloques_to_string(data_worker->bloquesAsignados);
	char* cant_bloques_asignados = int_to_string(list_size(data_worker->bloquesAsignados));

	char* char_job = int_to_string(data_worker->job);
	char* char_temporal_transformacion = data_worker->temporalTransformacion;
	char* char_temp_reduc_local = data_worker->tempReducLocal;
	char* char_temp_reduc_global= data_worker->tempReudcGlobal;
	char* char_ip = data_worker->ip;

	string_append(&char_data_worker, char_availability);
	string_append(&char_data_worker, char_worker);


	string_append(&char_data_worker,cant_bloques);
	string_append(&char_data_worker, char_bloques);

	string_append(&char_data_worker,cant_bloques_asignados);
	string_append(&char_data_worker, char_bloques_asignados);

	string_append(&char_data_worker, char_job);
	tamanio = int_to_string(strlen(char_temporal_transformacion));
	string_append(&char_data_worker,tamanio);
	free(tamanio);
	string_append(&char_data_worker, char_temporal_transformacion);

	tamanio = int_to_string(strlen(char_temp_reduc_local));
	string_append(&char_data_worker,tamanio);
	free(tamanio);
	string_append(&char_data_worker, char_temp_reduc_local);

	tamanio = int_to_string(strlen(char_temp_reduc_global));
	string_append(&char_data_worker,tamanio);
	free(tamanio);
	string_append(&char_data_worker, char_temp_reduc_global);

	tamanio = int_to_string(strlen(char_ip));
	string_append(&char_data_worker,tamanio);
	free(tamanio);
	string_append(&char_data_worker, char_ip);

	free(char_availability);
	free(char_worker);
	free(char_bloques);
	free(cant_bloques);
	free(char_bloques_asignados);
	free(cant_bloques_asignados);
	free(char_job);
	free(char_temp_reduc_global);
	free(char_temp_reduc_local);
	free(char_temporal_transformacion);
	return char_data_worker;
}

char* iterate_bloques_to_string(t_list* bloques){
	char* char_bloques = string_new();

	void _iterate_bloques(int bloque) {
		char* char_bloque = int_to_string(bloque);
		string_append(&char_bloques,char_bloque);
	}

	list_iterate(bloques, (void*) _iterate_bloques);

	return char_bloques;
}

t_list* iterate_bloques_from_string(char* bloques, int cantidad) {
	t_list* lista = list_create();
	int i,puntero;
	puntero = 0;
	char* char_num;
	for (i=0; i< cantidad; i++) {
		char_num  = extraer_string(bloques,puntero, puntero + 3);
		int num = atoi(char_num);
		list_add(lista,(void*) num);
		free(char_num);
		puntero += 4;
	}

	return lista;
}

char* bloque_archivo_to_string(t_bloque_archivo* bloque) {
	char* char_bloque = string_new();

	char* char_bloque_archivo;
	char_bloque_archivo = int_to_string(bloque->bloque_archivo);
	char *char_bytes = int_to_string(bloque->bytes);
	char* char_ruta_temporal = bloque->ruta_temporal;
	char* char_copia0 = copia_to_string(bloque->copia0);
	char* char_copia1 = copia_to_string(bloque->copia1);
	char* char_elegida = int_to_string(bloque->elegida);

	string_append(&char_bloque, char_bloque_archivo);
	string_append(&char_bloque, char_bytes);
	char* tamanio = int_to_string(strlen(char_ruta_temporal));
	string_append(&char_bloque, tamanio);
	free(tamanio);
	string_append(&char_bloque, char_ruta_temporal);
	tamanio = int_to_string(strlen(char_copia0));
	string_append(&char_bloque, tamanio);
	free(tamanio);
	string_append(&char_bloque, char_copia0);
	tamanio = int_to_string(strlen(char_copia1));
	string_append(&char_bloque, tamanio);
	free(tamanio);
	string_append(&char_bloque, char_copia1);
	string_append(&char_bloque, char_elegida);

	free(char_bloque_archivo);
	free(char_bytes);
	free(char_ruta_temporal);
	free(char_copia0);
	free(char_copia1);
	free(char_elegida);
	return char_bloque;
}

char* respuesta_master_to_string(t_resp_master* respuesta){
	char* char_respuesta = string_new();

	char* char_worker = int_to_string(respuesta->worker);
	char* char_etapa = int_to_string(respuesta->etapa);
	char* char_estado = int_to_string(respuesta->estado);

	string_append(char_respuesta,char_worker);
	string_append(char_respuesta,char_estado);
	string_append(char_respuesta,char_etapa);

	free(char_estado);
	free(char_etapa);
	free(char_worker);

	return char_respuesta;
}

t_resp_master* respuesta_master_from_string(char* char_respuesta){
	t_resp_master* respuesta;
	respuesta = malloc(sizeof(t_resp_master));
	int puntero = 0;

	char* char_worker = extraer_string(char_respuesta,puntero,puntero + 3);
	respuesta->worker = char_worker;
	puntero += 4;

	char* char_estado = extraer_string(char_respuesta,puntero,puntero + 3);
	respuesta->estado = char_estado;
	puntero += 4;

	char* char_etapa = extraer_string(char_respuesta,puntero,puntero + 3);
	respuesta->etapa = char_etapa;
	puntero += 4;

	return respuesta;
}

t_reg_planificacion* reg_planificacion_from_string(char* char_reg) {
	t_reg_planificacion* reg = malloc(sizeof(t_reg_planificacion));
	int puntero;
	puntero = 0;

	char* char_availability = extraer_string(char_reg,puntero,puntero + 3);
	reg->availability = atoi(char_availability);
	puntero += 4;

	char* char_worker = extraer_string(char_reg,puntero,puntero + 3);
	reg->worker = atoi(char_worker);
	puntero += 4;

	char* char_cant_bloques = extraer_string(char_reg,puntero,puntero + 3);
	int cant_bloques = atoi(char_cant_bloques);
	puntero += 4;

	char* char_bloques = extraer_string(char_reg, puntero, puntero + (cant_bloques * sizeof(int)) - 1);
	reg->bloques = iterate_bloques_from_string(char_bloques, cant_bloques);
	puntero += cant_bloques * sizeof(int);

	char* char_cant_bloques_asignados = extraer_string(char_reg,puntero,puntero + 3);
	int cant_bloques_asignados = atoi(char_cant_bloques_asignados);
	puntero += 4;

	char* char_bloques_asignados = extraer_string(char_reg, puntero, puntero + (cant_bloques_asignados * sizeof(int)) - 1);
	reg->bloquesAsignados = iterate_bloques_from_string(char_bloques_asignados, cant_bloques_asignados);
	puntero += cant_bloques_asignados * (sizeof(int));

	char* char_job = extraer_string(char_reg,puntero,puntero + 3);
	reg->job = atoi(char_job);
	puntero += 4;

	char* char_tamanio_temporal_tr = extraer_string(char_reg, puntero, puntero + 3);
	int tamanio_temporal_tr = atoi(char_tamanio_temporal_tr);
	puntero += 4;

	char* char_temporal_tr = extraer_string(char_reg, puntero, puntero + tamanio_temporal_tr -1);
	reg->temporalTransformacion= string_from_format(char_temporal_tr);
	puntero += tamanio_temporal_tr;

	char* char_tamanio_temporal_rl = extraer_string(char_reg, puntero, puntero + 3);
	int tamanio_temporal_rl = atoi(char_tamanio_temporal_rl);
	puntero += 4;

	char* char_temporal_rl = extraer_string(char_reg, puntero, puntero + tamanio_temporal_rl -1);
	reg->tempReducLocal= string_from_format(char_temporal_rl);
	puntero += tamanio_temporal_rl;

	char* char_tamanio_temporal_rg = extraer_string(char_reg, puntero, puntero + 3);
	int tamanio_temporal_rg = atoi(char_tamanio_temporal_rg);
	puntero += 4;

	char* char_temporal_rg = extraer_string(char_reg, puntero, puntero + tamanio_temporal_rg -1);
	reg->tempReudcGlobal= string_from_format(char_temporal_rg);
	puntero += tamanio_temporal_rg;

	char* char_tamanio_ip = extraer_string(char_reg, puntero, puntero + 3);
	int tamanio_ip = atoi(char_tamanio_ip);
	puntero += 4;

	char* char_ip = extraer_string(char_reg, puntero, puntero + tamanio_ip -1);
	reg->ip= string_from_format(char_ip);
	puntero += tamanio_ip;

	return reg;
}

t_bloque_archivo* bloque_archivo_from_string(char* char_bloque) {
	t_bloque_archivo* bloque = malloc(sizeof(t_bloque_archivo));
	int puntero;
	puntero = 0;

	char* char_bloque_archivo = extraer_string(char_bloque, puntero, puntero + 3);
	bloque->bloque_archivo = atoi(char_bloque_archivo);
	puntero += 4;

	char* char_bytes = extraer_string(char_bloque, puntero, puntero + 3);
	bloque->bytes = atoi(char_bytes);
	puntero += 4;


	char* char_tamanio_temporal = extraer_string(char_bloque, puntero, puntero + 3);
	int tamanio_temporal = atoi(char_tamanio_temporal);
	puntero += 4;

	char* char_archivo_temporal = extraer_string(char_bloque, puntero, puntero + tamanio_temporal -1);
	bloque->ruta_temporal = string_from_format(char_archivo_temporal);
	puntero += tamanio_temporal;


	char* char_tamanio_copia0 = extraer_string(char_bloque, puntero, puntero + 3);
	int tamanio_copia0 = atoi(char_tamanio_copia0);
	puntero += 4;

	char* char_copia0 = extraer_string(char_bloque, puntero, puntero + tamanio_copia0 - 1);
	bloque->copia0 = copia_from_string(char_copia0);
	puntero += tamanio_copia0;


	char* char_tamanio_copia1 = extraer_string(char_bloque, puntero, puntero + 3);
	int tamanio_copia1 = atoi(char_tamanio_copia1);
	puntero += 4;

	char* char_copia1 = extraer_string(char_bloque, puntero, puntero + tamanio_copia1 - 1);
	bloque->copia1 = copia_from_string(char_copia1);
	puntero += tamanio_copia1;


	char* char_elegida = extraer_string(char_bloque, puntero, puntero + 3);
		bloque->elegida = atoi(char_elegida);

	return bloque;
}


t_copia* copia_from_string(char* char_copia) {
	t_copia* nueva_copia = malloc(sizeof(t_copia));
	int puntero;
	puntero = 0;

	char* char_nodo = extraer_string(char_copia, puntero, puntero + 3);
	nueva_copia->nodo = atoi(char_nodo);
	puntero += 4;

	char* char_bloque_nodo = extraer_string(char_copia, puntero, puntero + 3);
	nueva_copia->bloque_nodo = atoi(char_bloque_nodo);
	puntero += 4;

	char* char_tamanio_ip = extraer_string(char_copia, puntero, puntero + 3);
	puntero += 4;
	int tamanio_ip = atoi(char_tamanio_ip);
	char* char_ip = extraer_string(char_copia, puntero, puntero + tamanio_ip);
	nueva_copia->ip = string_from_format(char_ip);

	free(char_nodo);
	free(char_bloque_nodo);
	free(char_tamanio_ip);
	free(char_ip);

	return nueva_copia;
}


char* copia_to_string(t_copia* copia) {
	char* char_copia = string_new();
	char* char_nodo = int_to_string(copia->nodo);
	char* char_bloque = int_to_string(copia->bloque_nodo);
	char* ip = copia->ip;
	int ip_len = strlen(ip);

	string_append(&char_copia, char_nodo);
	string_append(&char_copia, char_bloque);
	string_append(&char_copia, int_to_string(ip_len));
	string_append(&char_copia, ip);

	free(char_nodo);
	free(char_bloque);
	free(ip);

	return char_copia;
}


char* int_to_string(int numero) {
	char* longitud = string_new();
	char* num_char = string_itoa(numero);
	char* rev = string_reverse(num_char);
	string_append(&longitud, rev);
	free(num_char);
	free(rev);
	string_append(&longitud, "0000");
	longitud = string_substring(longitud, 0, 4);
	longitud = string_reverse(longitud);
	return longitud;
}

char* extraer_string(char* string, int inicio, int fin) {
	if (fin < inicio) {
		char* r = string_new();
		string_append(&r, "\0");
		return r;
	}
	return (string_substring(string, inicio, 1 + fin - inicio));
}


char* generar_string_separador2(char* str1, char* str2, char* separador){
	char* string = string_new();
	string_append(&string,str1);
	string_append(&string,separador);
	string_append(&string,str2);
	return string;
}

void string_append_separador(char** original, char* string_to_add,char* separador) {
	string_append(original,separador);
	string_append(original,string_to_add);
}
