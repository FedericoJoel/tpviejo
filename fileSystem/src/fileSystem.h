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

void fs_format();

void fs_rm(char * arg);

void fs_rmd(char * arg);

void ejecutarConsola();

#endif /* FILESYSTEM_H_ */
