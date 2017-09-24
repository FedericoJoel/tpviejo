#include "lib.h"


//---------------logger-------------------

t_log* abrir_logger(char* ruta_archivo, char* nombre_programa,t_log_level nivel){
	return  log_create(ruta_archivo,nombre_programa,1,nivel);
}
