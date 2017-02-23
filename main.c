// Directivas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Manejo de señales
#include <dirent.h> // Operaciones con directorios
#include <sys/types.h>
  
 // Librerías propias


// Declaración de funciones

// Constantes y Variables Globales
#define NAME_MAX 255    /* # chars in a file name */
#define PATH_MAX 4096    /* # chars in a path name including nul */
#define READ 0  // Pipe read
#define WRITE 1 // Pipe write



// Main

int main(int argc, char* argv[]) {
/*
  // Asegurarse de que hayan ingresado el nombre del archivo de salida
  if (argc < 2) {
    printf("\n");
    printf("Por favor introducir el nombre del archivo donde desea guardar ");
    printf("los datos. \n");
    // Sale con codigo de error
    exit(-1);
  }
*/
  // Obtiene el directorio donde se está trabajando
  char cwd[1024];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
       fprintf(stdout, "Current working dir: %s\n", cwd);
   else
       perror("getcwd() error");
   return 0;
/*
  // Abre el directorio
  DIR *directorio = opendir();
  opendir()
  }
*/
  // Finaliza
  return 0;

}
