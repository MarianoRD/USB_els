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

void verificarEscritura(struct stat *informacion) {
    
    // Guardo el ID del usuario
    int uid = getuid();
    int gid = getgid();
	
    if (gid == informacion->st_gid) {
        if (S_IWGRP) {
            return;
        }
    }
    if (uid == informacion->st_uid) {
        if (S_IWUSR) {
            return;
        }
    }
    if (((uid != informacion->st_uid) && (gid != informacion->st_gid))) {
        if (S_IWOTH) {
            return;
        }
    }

    // Caso que no se pueda escribir
    printf("No se puede escirbir en el directorio donde se desea guardar el reporte.\n");
    exit(-1);
}