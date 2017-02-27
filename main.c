// Librerías
#include "archivos.h"
#include "directorios.h"

// Constantes y Variables Globales


// Declaración de variables
char raiz[PATH_MAX];
struct dirent *fd;
struct stat buffer;
int pipeInfo[2];
int pidHijo;

// Main

/*int main(int argc, char* argv[]) {*/
int main(void) {
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
  char *raiz = obtenerDirectorioRaiz();

  // Abre el directorio
  DIR *directorio = opendir(raiz);
  Directorio *directorioStruct;
  directorioStruct = datosDirectorioActual(raiz);

  // Verifico que pueda escribir en el directorio raiz.
  verificarEscritura(&(directorioStruct->informacion));
  // Imprimo los datos del directorio raiz
  imprimeDirectorio("", *directorioStruct);

  // Ciclo del proceso
  while ((fd = readdir(directorio)) != NULL) {

    stat(fd->d_name, &buffer);
    

    // Revisa que no sea un directorio oculto
    if (fd->d_name[0] == '.') {
      continue;
    }
    if (S_ISDIR(buffer.st_mode)) {

      // Abre el pipe (recibe ambas puntas cerradas)
      crearPipe(pipeInfo);

      // Crea un proceso hijo
      pidHijo = fork();
      if(pidHijo == -1) { // Error
        printf("fork() error.\n");
        exit(-1);
      } else if (pidHijo == 0) { // Es hijo
        // Cierra la lectura
        close(pipeInfo[READ]);
        // Busca toda la información
        Directorio *hijo;
        hijo = datosDirectorioRecursivo(fd->d_name);
        // Se la comunica al padre
        //write(pipeInfo[WRITE], &hijo, sizeof(hijo));
        write(pipeInfo[WRITE], hijo->path, sizeof(hijo->path));
        write(pipeInfo[WRITE], &(hijo->cantArchivos), sizeof(hijo->cantArchivos));
        write(pipeInfo[WRITE], &(hijo->informacion), sizeof(hijo->informacion));
        write(pipeInfo[WRITE], &(hijo->grupo), sizeof(hijo->grupo));
        write(pipeInfo[WRITE], &(hijo->usuario), sizeof(hijo->usuario));
        write(pipeInfo[WRITE], &(hijo->fechaMod), sizeof(hijo->fechaMod));
        write(pipeInfo[WRITE], &(hijo->fechaAcc), sizeof(hijo->fechaAcc));
        write(pipeInfo[WRITE], &(hijo->bytes), sizeof(hijo->bytes));
        // El hijo termina
        free(hijo);
        exit(0);
      } else { // Es padre
        // Cierra la escritura
        close(pipeInfo[WRITE]);
        // Espera al hijo
        wait(0);
        // Lee la información del pipe
        Directorio hijo;
        // Empiezo a leer el pipe
        read(pipeInfo[READ], hijo.path, sizeof(hijo.path));
        read(pipeInfo[READ], &(hijo.cantArchivos), sizeof(hijo.cantArchivos));
        read(pipeInfo[READ], &(hijo.informacion), sizeof(hijo.informacion));
        read(pipeInfo[READ], &(hijo.grupo), sizeof(hijo.grupo));
        read(pipeInfo[READ], &(hijo.usuario), sizeof(hijo.usuario));
        read(pipeInfo[READ], &(hijo.fechaMod), sizeof(hijo.fechaMod));
        read(pipeInfo[READ], &(hijo.fechaAcc), sizeof(hijo.fechaAcc));
        read(pipeInfo[READ], &(hijo.bytes), sizeof(hijo.bytes));
        // Cierra el pipe
        close(pipeInfo[READ]);

        // Imprime la información del hijo
        imprimeDirectorio(raiz, hijo);
      }
    
    }
  }

  // Cierra el directorio
  closedir(directorio);

  // Manejador de Señales
  signal(SIGINT, manejadorDeSenales);

  // Fin de impresion.
  printf("\nTerminé\n");
  while(1) {

  };

  // Finaliza
  return 0;

}