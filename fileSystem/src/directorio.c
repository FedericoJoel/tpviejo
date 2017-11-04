#include "directorio.h"

char* rutaDirectorio =
		"/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/directorio.dat";

int buscarPosicionEnDirectorio(t_directory* directorio, char* ruta){
	int posicion = 0;
	for(posicion = 0; posicion < 99; posicion++){
		if(string_equals_ignore_case(directorio[posicion].nombre, ruta)){
			break;
		}
	}
	if(posicion != 0){
		return posicion;
	}else{
		return -1;
	}
}

void eliminarUnDirectorio(t_directory* directorio, char** path){
	int cantidadDeElementos = cantidadElementos(path);
	int pos = 1;
	int posicionAux = 0;
	char* ruta = string_new();
	string_append(&ruta, "/");
	string_append(&ruta, path[pos]);
	int posicion = buscarPosicionEnDirectorio(directorio, ruta);
	pos++;
	for(; pos < cantidadDeElementos; pos++){
		ruta = string_new();
		string_append(&ruta, "/");
		string_append(&ruta, path[pos]);
		posicionAux = buscarPosicionEnDirectorio(directorio, ruta);
		if (directorio[posicion].index == directorio[posicionAux].padre){
			posicion = posicionAux;
		}else{
			break;
		}
	}
	if(pos == cantidadDeElementos){
		vaciarPosicion(directorio, posicion);
		free(ruta);
	}else{
		printf("error");
		free(ruta);
	}
}

void vaciarPosicion(t_directory* directorio, int pos){
	directorio[pos].index = 0;
	vaciarArray(directorio[pos].nombre, 255);
	directorio[pos].padre = 0;
}

void cargarDirectorio(t_directory* directorio) {
	int pos = 0;
	char * linea = string_new();
	size_t len = 0;
	FILE * archivo = fopen(rutaDirectorio, "r");

	while (((getline(&linea, &len, archivo)) != EOF) && pos <= 99) {
		convertirDirectorio(linea, directorio);
		pos++;
	}

	fclose(archivo);
	free(linea);
}

void mostrarDirectorio(t_directory* directorio){
	int pos = 0;
	printf("index \t | \t nombre \t | \t padre \n");
	printf("----------------------------------------------------\n");
	for(pos = 0; pos < 99; pos++){
		printf("%d \t | \t %s \t \t | \t %d \n", directorio[pos].index,
						directorio[pos].nombre, directorio[pos].padre);
	}

}

int existeDirectorio() {
	char * linea = string_new();
	size_t len = 0;
	FILE * archivo = fopen(rutaDirectorio, "r");
	if ((getline(&linea, &len, archivo)) != EOF) {
		free(linea);
		fclose(archivo);
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
}

void convertirDirectorio(char * linea, t_directory directorio[]) {
	t_directory* unDirectorio = malloc(sizeof(t_directory));

	int pos = 0;
	int termino = -1;
	int aux = 0;
	int nivel = -1;
	int largoNombre = 0;
	int largo = string_length(linea);

	vaciarArray(unDirectorio->nombre, 255);

	for (pos = 0; pos < largo; pos++) {
		if (string_starts_with(linea, "/")) {
			if (linea[pos] == '\n') {
				nivel++;
				unDirectorio->padre = nivel;
				unDirectorio->nombre[pos] = '\0';
				largoNombre = string_length(unDirectorio->nombre);
				termino = 0;
			} else {
				if (pos != 0 && linea[pos] == '/') {
					int limpiar = string_length(unDirectorio->nombre);
					int lugar = 0;
					for (lugar = 0; lugar <= limpiar; lugar++) {
						unDirectorio->nombre[lugar] = '\0';
					}
					unDirectorio->padre = (nivel + 1);
					comprobarDirectorio(nivel, directorio, unDirectorio);
					unDirectorio->nombre[0] = '/';
					aux = 1;
					nivel++;
				} else {
					unDirectorio->nombre[aux] = linea[pos];
					aux++;
				}
			}
		} else {
			if (string_equals_ignore_case(&linea[(pos + 1)], "/")) {
				termino = -1;
				break;
			} else {
				if (linea[pos] != '\n') {
					unDirectorio->nombre[aux] = linea[pos];
					aux++;
				} else {
					unDirectorio->nombre[aux] = '\0';
					if (string_equals_ignore_case(unDirectorio->nombre,
							"root")) {
						unDirectorio->padre = nivel;
						largoNombre = string_length(unDirectorio->nombre);
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
		printf("Error");
	}
	free(unDirectorio);
}

int comprobarDirectorio(int nivel, t_directory directorio[],
		t_directory* unDirectorio) {
	int reg = 0;
	for (reg = 0; reg <= 99; reg++) {
		if ((string_equals_ignore_case(unDirectorio->nombre,
				directorio[reg].nombre)
				&& directorio[reg].padre == unDirectorio->padre)
				|| ((string_equals_ignore_case(unDirectorio->nombre, "root"))
						&& nivel == unDirectorio->padre))
			return reg;
	}
//	printf("No existe directorio, el mismo se creara \n");
	for (reg = 0; reg <= 99; reg++) {
		if (directorio[reg].nombre[0] == '\0'
				&& unDirectorio->nombre[0] != '\0') {
			char* mkdir = string_new();
			directorio[reg].index = reg;
			string_append(&mkdir, "mkdir /root");
			string_append(&mkdir, agregarPadreARuta(unDirectorio->padre, directorio));
			string_append(&mkdir, unDirectorio->nombre);
			system(mkdir);
			free(mkdir);
			printf("se creó el directorio en: %s \n", unDirectorio->nombre);
			return reg;
		}
	}
	//RETURN PARA IGNORAR EL WARNING
	return EXIT_FAILURE;
}

char* agregarPadreARuta(int padre, t_directory directorio[]) {
	int pos = 0;
	char* path = string_new();
	char* pathAnterior = string_new();
	while (pos <= 99 && padre != 0) {
		if (directorio[pos].index == padre) {
			string_append(&path, directorio[pos].nombre);
			while (directorio[pos].padre != 0) {
				pathAnterior = agregarPadreARuta(directorio[pos].padre,
						directorio);
				pos = damePosicionDeElemento(pathAnterior, directorio);
				string_append(&pathAnterior, path);
			}

			break;
		}
		pos++;
	}
	if (string_ends_with(pathAnterior, path)) {
		return pathAnterior;
	}
	return path;
}

int damePosicionDeElemento(char* nombre, t_directory directorio[]) {
	int pos = 0;
	while (pos <= 99) {
		if (string_equals_ignore_case(directorio[pos].nombre, nombre)) {
			break;
		}
		pos++;
	}
	return directorio[pos].index;
}

void guardarRegistro(t_directory directorio[], t_directory* unDirectorio,
		int reg, int largo) {
	int pos;
	for (pos = 0; pos <= largo; pos++) {
		directorio[reg].nombre[pos] = unDirectorio->nombre[pos];
	}
	if(reg == 0){
		directorio[reg].padre = -1;
	}else{
		directorio[reg].padre = unDirectorio->padre;
	}
}

void borrarContenido(char* path){
	char* rmContenido = string_new();

	string_append(&rmContenido, "rm -r ");
	string_append(&rmContenido, path);
	string_append(&rmContenido, "/*");
	system(rmContenido);

	free(rmContenido);
}

void borrarCarpeta(char* path){
	char* rmdir = string_new();

	string_append(&rmdir, "rmdir ");
	string_append(&rmdir, path);
	system(rmdir);

	free(rmdir);
}

int eliminarArchivosDeDirectorio(t_directory* directorio) {
	int pos = 0;
	char* lista = string_new();

	while (pos < 99) {
		if (directorio[pos].padre == 0 && directorio[pos].nombre[0] != '\0') {

			char* path = string_new();

			string_append(&path, "/root");
			string_append(&path, directorio[pos].nombre);

			string_append(&lista, listarDirectorio(path));

			borrarContenido(path);
			borrarCarpeta(path);

			printf("comando: rm -r \n");
			printf("comando: rmdir \n");
			printf("se borro el directorio: %s \n", path);

			free(path);
		}
		pos++;
	}
	//LISTA CON LOS ARCHIVOS ELIMINADOS
	lista[string_length(lista) - 1] = '\0';
	if (pos == 99) {
		borrarContenido("/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/archivo/*");
		printf("se borro el directorio: metadata/archivo");
		vaciarEstructuraDirectorio(directorio);
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
}

void vaciarEstructuraDirectorio(t_directory* directorio){
	int pos = 0;
	for (pos = 0; pos < 99; pos++){
		vaciarPosicion(directorio, pos); //255 cantidad de espacios del array de char
	}
}

void vaciarArray(char* nombre, int cantidad){
	int pos = 0;
	for (pos = 0; pos < cantidad; pos++){
		nombre[pos] = '\0';
	}
}

char* listarDirectorio(char* path){
	DIR *d;
	struct dirent *dir;
	char* listaDirectorio = string_new();

	d = opendir(path);
	if (d)
	{
		while ((dir = readdir(d)) != NULL && dir->d_name[0] != '.')
	    {
			if(dir->d_type == '\004'){
				char* pathHijo = string_duplicate(path);
				string_append(&pathHijo, "/");
				string_append(&pathHijo, dir->d_name);
				string_append(&listaDirectorio, listarDirectorio(pathHijo));
			}else{
				string_append(&listaDirectorio, dir->d_name);
				string_append(&listaDirectorio, ".csv");
				string_append(&listaDirectorio, ",");
			}
	    }
		closedir(d);
	}
	return listaDirectorio;
}

void guardarDirectorioEnMemoria(t_directory* directorio){
	char* argumentos;
	char* ruta;
	int pos = 99;
	estructuraAux structAux[99];
	for(;pos >= 0; pos--){
		ruta  = string_new();
		argumentos = string_new();
		if(directorio[pos].nombre[0] == '/'){
			if(directorio[pos].padre != -1 || string_equals_ignore_case(directorio[pos].nombre, "/root") ){
				string_append(&argumentos, directorio[pos].nombre);
				int posAux = directorio[pos].padre;
				while(directorio[posAux].padre != -1 && directorio[pos].padre != -1){
					string_append(&argumentos, directorio[posAux].nombre);
					posAux = directorio[posAux].padre;
				}
			char** argumentosSeparados = string_split(argumentos, "/");
			string_append(&ruta, invertirOrden(argumentosSeparados));
			structAux[pos].nivel = directorio[pos].padre;
			structAux[pos].path = string_duplicate(ruta);
			}
		}else{
			structAux[pos].nivel = -1000;
			structAux[pos].path = string_new();
		}
	}
	ordenarArray(structAux);
	char* archivoRuta = string_duplicate("/home/utnso/Escritorio/Git/tp-2017-2c-LaYamaQueLlama/metadata/directorio1.dat");
	FILE* archivo = fopen(archivoRuta, "w");
	for(pos = 0; pos < 99; pos++){
		if (!(string_is_empty(structAux[pos].path))){
			fputs(structAux[pos].path, archivo);
			fputc('\n', archivo);
		}
	}
	fclose(archivo);
	free(ruta);
	free(argumentos);
}

void ordenarArray(estructuraAux* array){
	int pos = 99;
	int posAux = 0;
	char* argumentos[99];
	for(;pos >= 0; pos--){
		if(!(string_is_empty(array[pos].path)) ){
			argumentos[posAux] = string_duplicate(array[pos].path);
		}
	}
}

char* invertirOrden(char** argumentos){
	int cantidad = cantidadElementos(argumentos);
	char* path = string_new();
	cantidad--;
	int pos = 0;
	for(;pos <= cantidad; cantidad--){
		string_append(&path, "/");
		string_append(&path, argumentos[cantidad]);
	}
	return path;
}
