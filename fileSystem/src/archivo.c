#include "archivo.h"

char* rutaArchivo = "/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/fileSystem/src/ejemplo.csv";

void mostrarTablaArchivo(){

	t_config* config = config_create(rutaArchivo);
	int tamanioFs = config_get_int_value(config, "TAMANIO");
/*
	int n = 255;
	char str[n];
	FILE *file = fopen(rutaArchivo, "r");
    if(fgets(str,n,file)!=";"){
    	 puts(str);
    	    }
    	    fclose(file);
*/
}
