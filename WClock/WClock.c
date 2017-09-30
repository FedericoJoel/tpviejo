#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BASE 2; // valor que se debe ingresar en el archivo de configuracion o por consola y es arbitrario

struct tablaDeEstados{
		int nodo;
		int bloque;
		char* estado;
	}tablaDeEstados;

	struct tablaPlanificacion{
		int availability;
		int worker;
		int bloques[5];
		int bloquesAsignados;
		}tablaPlanificacion;


	int PWL(int worker){
		//
		int wlmax = 0; //la mayor carga de trabajo entre todos los worker. Se debera recorrer la tabla de estados para ver cual es la mayor carga de trabajo
		int wl = 0; //la carga de trabajo actual del worker. Se obtiene de la tabla de estados

		return wlmax - wl ;
	}

	struct tablaPlanificacion* inicializarTablaDePlanificacion(){

		struct tablaPlanificacion array[3];

			int i;
			for(i=0 ; i<3 ; i++){

				array[i].worker = i+1;
				array[i].availability = BASE + PWL(array[i].worker);
				array[i].bloquesAsignados = 0;

			}

			array[0].bloques[0] = 0;
			array[0].bloques[1] = 1;
			array[0].bloques[2] = 3;
			array[0].bloques[3] = 4;
			array[0].bloques[4] = 6;

			array[1].bloques[0] = 0;
			array[1].bloques[1] = 2;
			array[1].bloques[2] = 3;
			array[1].bloques[3] = 5;
			array[1].bloques[4] = 6;

			array[2].bloques[0] = 1;
			array[2].bloques[1] = 2;
			array[2].bloques[2] = 4;
			array[2].bloques[3] = 5;
			array[2].bloques[4] = 1;

			return array;

	}

int main (){
	struct tablaPlanificacion* tabla;
	tabla = inicializarTablaDePlanificacion();


	int bloqueABuscar = 0;

	int i;

	struct tablaPlanificacion *clock = tabla; //Se apunta clock a la primera posicion del array. Se deberia apuntar a la posicion del worker con mayor disponibilidad

	for(i=0 ; i<6 ; i++){ //Este for hace un bucle por cada bloque que se quiera encontrar

		struct tablaPlanificacion * auxclock = clock; //Se apunta al auxclock a la posicion  apuntada por clock
		if((* auxclock).worker <3){ auxclock += 1;}else{auxclock= tabla;}; //Se le suma a auxclock una posicion

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
}

