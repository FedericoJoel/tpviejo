#include "lib.h"  /* Include the header (not strictly necessary here) */
#include <stdio.h>

void saludar()    /* Function definition */
{
    puts("hola");
}

void checkStatus (pid_t process)
{

	FILE* arch;

	arch = fopen("C:/log.txt","w+");

	if(process< 0)
    {
		fseek(arch,sizeof("\nfork failed"), SEEK_END);
		fwrite("\nfork failed",sizeof("fork failed"),1,arch);

    }
    if(process > 0)
    {
    	fseek(arch,sizeof("\nParent Process Executed"), SEEK_END);
    	fwrite("\nParent Process Executed",sizeof("Parent Process Executed"),1,arch);
    }

    if(process == 0)
    {
    	fseek(arch,sizeof("\nChild Process Executed"), SEEK_END);
    	fwrite("\nChild Process Executed",sizeof("Child Process Executed"),1,arch);
    }

    fclose(arch);
}
