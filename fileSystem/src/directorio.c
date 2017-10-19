#include "directorio.h"

char* rutaDirectorio =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/fileSystem/src/directorio.dat";

int mostrarDirectorio() {
	t_directory directorio[100];
	int pos = 0;
	char * linea = string_new();
	size_t len = 0;

	FILE * archivo = fopen(rutaDirectorio, "r");
	printf("index \t | \t nombre \t | \t padre \n");
	printf("----------------------------------------------------\n");
	while (((getline(&linea, &len, archivo)) != EOF) && pos <= 99) {
		convertirDirectorio(linea, directorio);
		printf("%d \t | \t %s \t \t | \t %d \n", directorio[pos].index,
				directorio[pos].nombre, directorio[pos].padre);
		pos++;
	}

	fclose(archivo);
	if (pos == 99 || linea == NULL) {
		if (linea)
			free(linea);
		return EXIT_SUCCESS;
	} else {
			free(linea);
		return EXIT_FAILURE;
	}
}

void convertirDirectorio(char * linea, t_directory directorio[]) {
	t_directory unDirectorio;
	int pos0;
	int pos;
	int termino = 0;
	int aux = 0;
	int nivel = -1;
	int largoNombre = 0;
	int limpiar = 0;
	int largo = string_length(linea);

	for (pos = 0; pos < largo; pos++) {
		if (string_starts_with(linea, "/")) {
			if (linea[pos] == '\n') {
				nivel++;
				unDirectorio.padre = nivel;
				unDirectorio.nombre[pos] = '\0';
				largoNombre = string_length(unDirectorio.nombre);
				termino = 0;
			} else {
				if (pos != 0 && linea[pos] == '/') {
					limpiar = string_length(unDirectorio.nombre);
					int lugar;
					for (lugar = 0; lugar <= limpiar; lugar++){
						unDirectorio.nombre[lugar] = '\0';
					}
					unDirectorio.padre = (nivel + 1);
					comprobarDirectorio(nivel, directorio, unDirectorio);
					unDirectorio.nombre[0] = '/';
					aux = 1;
					nivel++;
				} else {
					unDirectorio.nombre[aux] = linea[pos];
					aux++;
				}
			}
		} else {
			if (string_equals_ignore_case(&linea[(pos + 1)], "/")) {
				termino = -1;
				break;
			} else {
				if (linea[pos] != '\n') {
					unDirectorio.nombre[aux] = linea[pos];
					aux++;
				} else {
					unDirectorio.nombre[aux] = '\0';
					if (string_equals_ignore_case(unDirectorio.nombre,
							"root")) {
						unDirectorio.padre = nivel;
						largoNombre = string_length(unDirectorio.nombre);
						termino = 0;
						break;
					} else {
						termino = -1;
					}
				}
			}
		}
	}
	if (termino == 0) {
		int reg = comprobarDirectorio(nivel, directorio, unDirectorio);
		guardarRegistro(directorio, unDirectorio, reg, largoNombre);
	} else {
		//loguear error
		printf("Error");
	}
	for (pos0 = 0; pos0 < 255; pos0++) {
		unDirectorio.nombre[pos0] = '\0';
	}
}

int comprobarDirectorio(int nivel, t_directory directorio[],
		t_directory unDirectorio) {
	int reg = 0;
	for (reg = 0; reg <= 99; reg++) {
		if ((string_equals_ignore_case(unDirectorio.nombre,
				directorio[reg].nombre)
				&& directorio[reg].padre == unDirectorio.padre)
				|| ((string_equals_ignore_case(unDirectorio.nombre, "root"))
						&& nivel == unDirectorio.padre))
			return reg;
	}
//	printf("No existe directorio, el mismo se creara \n");
	for (reg = 0; reg <= 99; reg++) {
		if (directorio[reg].nombre[0] == '\0') {
			directorio[reg].index = reg;
			return reg;
		}
	}
	//RETURN PARA IGNORAR EL WARNING
	return -1;
}

void guardarRegistro(t_directory directorio[], t_directory unDirectorio,
		int reg, int largo) {
	int pos;
	for (pos = 0; pos <= largo; pos++) {
		directorio[reg].nombre[pos] = unDirectorio.nombre[pos];
	}
	directorio[reg].padre = unDirectorio.padre;

}
