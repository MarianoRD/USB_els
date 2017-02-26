// Librerias
#include "librerias.h"

// Funciones

void  manejadorDeSenales(int senal)
{
     char  c;

     signal(senal, SIG_IGN);
     printf("\nPresionaste Ctrl-C\n"
            "¿Realmente quieres salir? [s/n]: ");
     scanf("%c", &c);
     if (c == 's' || c == 'S') {
     	printf("\nHasta luego.\n");
        exit(0);
     } else {
          signal(SIGINT, manejadorDeSenales);
     }
     scanf("%c", &c);
}

void crearPipe(int *pipeFD) {
	// Chequea que se cree el pipe
	if (pipe(pipeFD) == -1) {
		printf("pipe() error.\n");
		exit(-1);
	}
}