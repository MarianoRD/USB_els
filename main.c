// Librerías
#include "directorios.h"

// Constantes y Variables Globales


// Declaración de variables
Directorio raiz;
String str;
struct dirent *fdActual;
struct stat statActual;
int pipeRaiz[2];
int pidHijo;
Pila pila;

// Main

int main(int argc, char* argv[]) {

  // Inicializa los valores de las estructuras
  raiz.bytes = 0;
  raiz.cantArchivos = 0;
  raiz.cantDirectorios = 0;

  // Manejador de Señales
  signal(SIGINT, manejadorDeSenales);

  // Inicializo variables
  char nombreArchivoSalida[NAME_MAX];
  strcpy(nombreArchivoSalida, argv[1]);

  // Asegurarse de que hayan ingresado el nombre del archivo de salida
  if (argc < 2) {
    printf("\n");
    printf("Por favor introducir el nombre del archivo donde desea guardar ");
    printf("los datos. \n");
    // Sale con codigo de error
    exit(-1);
  }

  // Obtiene el directorio actual
  if(getcwd(raiz.rutaAbs, PATH_MAX) == NULL) {
    printf("Error con getcwd(), main.c\n");
  };

  // Abre el directorio
  raiz.dir = opendir(raiz.rutaAbs);
  if(raiz.dir == NULL) {
    printf("Error con opendir(), main.c\n");
    exit(-1);
  }

  // Busco los datos del directorio
  datosDirectorio(&raiz);

  // Inicializo la pila
  iniciarPila(&pila, raiz.cantDirectorios);

  // Verifico que pueda escribir en el directorio raiz.
  verificarEscritura(&(raiz.informacion));

  // Agrego raiz a la pila
  creaStr(&raiz, str);
  pushPila(&pila, &str);

  // Reinicio el apuntador de 'raiz'
  rewinddir(raiz.dir);

  /* EN CASO DE CONCURRENCIA --------------------------------------------------<
  // Creo el arreglo con los datos de los hijos
  int hijos[raiz.cantDirectorios];
  */

  // Ciclo
  while((fdActual = readdir(raiz.dir)) != NULL) {
    // Revisa que no sea un directorio oculto
    if (fdActual->d_name[0] == '.') {
      continue;
    }

    // Busca información del archivo
    if (stat(fdActual->d_name, &statActual) < 0) {
      printf("Error stat(), main.c\n");
      exit(-1);
    }

    // Si es directorio
    if (S_ISDIR(statActual.st_mode)) {
      printf("Directorio: %s\n", fdActual->d_name); // quitar() ---------------<
      // Crea el pipe
      pipe(pipeRaiz);
      // Crea un proceso hijo
      pidHijo = fork();
      if(pidHijo == -1) {
        printf("Error fork(), main.c\n");
        exit(-1);
      } else if (pidHijo == 0) { // Hijo
        printf("Hijo creado\n");
        // Cierra la lectura
        close(pipeRaiz[READ]);

        // Crea la estructura
        Directorio hijo;
        // Inicializa las variables de 'hijo'
        strcpy(hijo.nombre, fdActual->d_name);
        realpath(hijo.nombre, hijo.rutaAbs);
        hijo.cantArchivos = 0;
        hijo.cantDirectorios = 0;
        hijo.bytes = 0;
        // Busca la información de los hijos
        datosHijo(&hijo, nombreArchivoSalida);
        // Escribe la información en el pipe
        // Deberia escribir solo el str ---------------------------------------<

        // El hijo termina
        printf("Exit hijo\n"); // Quitar --------------------------------------<
        exit(0);
      } else if (pidHijo > 0) { // Padre
        // Cierra la escritura
        close(pipeRaiz[WRITE]);
        // Espera al hijo
        int estadoReturn;    
        waitpid(pidHijo, &estadoReturn, 0); // Cambiar por waitpid para que no sea bloqueante ------------<
        if (estadoReturn < 0) {
          printf("Hubo un error en la terminación del hijo\n");
        } 
        // Lee la información del pipe

        //

      }
    }
  };

  // Crea el reporte padre
  crearReporte(raiz.rutaAbs, &pila, raiz.nombre, nombreArchivoSalida);
  // Cierro del directorio
  closedir(raiz.dir);
  // Finaliza el programa
  printf("Programa terminó con éxito\n");
  return 0;

}