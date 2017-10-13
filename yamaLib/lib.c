#include "lib.h"


//---------------logger-------------------

t_log* abrir_logger(char* ruta_archivo, char* nombre_programa,t_log_level nivel){
	return  log_create(ruta_archivo,nombre_programa,1,nivel);
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
