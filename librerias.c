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

void verificarEscritura(char *str) {
    if(access(str, W_OK) == 0) {
        return;
    } else {
        printf("No se puede crear el reporte en el directorio escogido.\n");
        printf("El programa terminará.\n");
        exit(-1);
    }
}