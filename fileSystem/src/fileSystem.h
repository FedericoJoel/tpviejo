#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <commons/temporal.h>
#include <lib.h>
#include <stdint.h>
#include <commons/string.h>
#include <readline/history.h>
#include <pthread.h>
#include <sockets.h>

void fs_format();

void fs_rm(char * arg);

void fs_rename(char * arg);

void fs_mv(char * arg);

void fs_cat(char * arg);

void fs_mkdir(char * arg);

void fs_cpfrom(char * arg);

void fs_cpto(char * arg);

void fs_cpblok(char * arg);

void fs_md5(char * arg);

void fs_ls(char * arg);

void fs_info(char * arg);

void ejecutarConsola();

int str_array_size(char** array);

void str_array_print(char ** array);

#endif /* FILESYSTEM_H_ */
