#include "yama.h"

int main(void) {
	//levanta config
	t_config config = config_create(ruta_config);
	int puerto_fs = config_get_int_value(config,"FS_PUERTO");
	char ip_fs = config_get_string_value(config,"FS_IP");

	//intentar conectar a fs, devuelve file descriptor del socket
	int fd_fs = conectar(puerto_fs,ip_fs);

	//enviar handshake
	autentificar(fd_fs,"YAMA");

	//

}
