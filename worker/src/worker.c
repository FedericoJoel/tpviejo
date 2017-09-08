#include "worker.h"  /* Include the header (not strictly necessary here) */


char FILE_LOG[1024] = "/home/utnso/Escritorio/Proyecto/logWorker.txt";

int main(void) {
	t_log* loggerWork;
	loggerWork = log_create(FILE_LOG, "Worker", true, LOG_LEVEL_TRACE);
	pid_t id = crearFork();
//	getpid(); ID Padre
		if (ejecutarRutina() == 0)
		{
			char* proceso = forkEjecutador(id);
			log_trace(loggerWork, "FIN %s", proceso, "correcto");
		}
		else
		{
			char* proceso = forkEjecutador(id);
			log_trace(loggerWork, "FIN %s", proceso, "incorrecto");
		}
	log_destroy(loggerWork);
	return EXIT_SUCCESS;
}

char* forkEjecutador (pid_t process){
	t_log* logger;
	char FILE_LOG[1024] = "/home/utnso/Escritorio/Proyecto/logFork.txt";

    if(process == 0)
    {
		logger = log_create(FILE_LOG, "FORK", true, LOG_LEVEL_TRACE);
		log_trace(logger, "Child Process Executed");
		log_destroy(logger);
		return "Proceso Hijo";
    }
    if(process > 0)
    {
		logger = log_create(FILE_LOG, "FORK", true, LOG_LEVEL_TRACE);
		log_trace(logger, "Parent Process Executed");
		log_destroy(logger);
		return "Proceso Padre";
    }
	logger = log_create(FILE_LOG, "FORK", true, LOG_LEVEL_TRACE);
	log_trace(logger, "fork failed");
	log_destroy(logger);
	return "Proceso Fallido";
}
