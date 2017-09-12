#ifndef WORKER_H_
#define WORKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <lib.h>
#include <commons/log.h>

// Lista de workers que debe tener
t_log* logger;
pthread_mutex_t mutex_log;

char* forkEjecutador (pid_t process);

int ejecutarScript(char* script, int (*procesar_std)());

#endif /* WORKER_H_ */
