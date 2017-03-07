// Librerias
#include "directorios.h"
#include <errno.h> //ELIMINAR

// Funciones

/* Da la ruta absoluta del directorio donde se está ejecutando el programa*/
char *obtenerDirectorioRaiz(){
  char *abspath = (char *) malloc(sizeof(char) * PATH_MAX);

  if (getcwd(abspath, PATH_MAX) != NULL) {

    printf("Directorio raíz donde se ejecutará el comando: %s\n", abspath);
  } else {
    perror("getcwd() error");
  }

  return abspath;
};

/*
void borraCore(char*nombre) {
  // Incializo variables
  FILE *archivo;
  archivo = fopen(nombre, "rb"); 
  uint16_t *datos;

  // Busco el e_type
  fseek(archivo, sizeof(unsigned char), SEEK_SET);
  fread(&datos, sizeof(uint16_t), 1, archivo);
  printf("Trato de borar el beta\n");
  printf("%hu\n\n\n", *datos);
  if (datos[ET_CORE]) {
    //remove(nombre);
    printf("%d\n", datos[ET_CORE]);
  }
}
*/

/* Cuenta cuantos archivos y sub-directorios posee un directorio y cuantos
    bytes ocupan dichos archivos (sin contar los directorios)*/
void informacionArchivos(Directorio *directorio) {
  // Variables
  struct dirent *fd;
  struct stat info;
  directorio->bytes = 0;
  directorio->cantArchivos = 0;


  // Cuenta solamente los archivos
  while ((fd = readdir(directorio->dir)) != NULL) {
    stat(fd->d_name, &info);
    // No cuenta los archivos ocultos
    if (fd->d_name[0] == '.') {
      continue;
    }
    if (S_ISDIR(info.st_mode)) {
      (directorio->cantArchivos)++;
    } else {
      (directorio->cantArchivos)++;
      directorio->bytes = directorio->bytes + info.st_size;
    }
  }
}

void informacionArchivosRecursivo(Directorio *directorio, Pila *pila) {

  // Variables
  struct dirent *fd;
  struct stat info;
  directorio->bytes = 0;
  directorio->cantArchivos = 0;

  while ((fd = readdir(directorio->dir)) != NULL) {
    
    // No cuenta los archivos ocultos
    if (fd->d_name[0] == '.') {
      continue;
    }
    char abs[PATH_MAX];
    realpath(fd->d_name, abs);
    if (stat(fd->d_name, &info) == -1) {
      if (errno == ENOENT) {
        printf("ENOENT");
      }
      printf("Error scan()\n");
    };
    if (S_ISDIR(info.st_mode)) {

      // Inicializo las variables
      Directorio *recursivo = (Directorio *) malloc(sizeof(Directorio));
      Salida *elemento = (Salida *) malloc(sizeof(Salida));

      realpath(fd->d_name, recursivo->path);

      // Crea el DIR
      recursivo->dir = opendir(recursivo->path);
      // Recursión
      chdir(recursivo->path);
      informacionArchivosRecursivo(recursivo, pila);
      // Regreso el directorio y cierro el DIR
      chdir(directorio->path);
      closedir(recursivo->dir);
      // Actualizo los datos
      (directorio->cantArchivos) += recursivo->cantArchivos + 1;
      (directorio->bytes) += recursivo->bytes;

      // Guarda el elemento en la pila
      creaStr(recursivo, elemento->string);
      elemento->next = NULL;
      agregarPila(elemento, pila);
      // Elimina el Directorio de recursión.
      free(recursivo);
      //printf("Elimino recursivo\n");
    } else {
      (directorio->cantArchivos)++;
      directorio->bytes = directorio->bytes + info.st_size;
    }
  }
}


/* Busca los permisos de un directorio en específico. */
char *permisosDirectorio(struct stat informacion){
  char *permisos = (char *) malloc(sizeof(char) * PERMISOS_MAX);

    strcat(permisos, (S_ISDIR(informacion.st_mode)) ? "d" : "-");
    strcat(permisos, (informacion.st_mode & S_IRUSR) ? "r" : "-");
    strcat(permisos, (informacion.st_mode & S_IWUSR) ? "w" : "-");
    strcat(permisos, (informacion.st_mode & S_IXUSR) ? "x" : "-");
    strcat(permisos, (informacion.st_mode & S_IRGRP) ? "r" : "-");
    strcat(permisos, (informacion.st_mode & S_IWGRP) ? "w" : "-");
    strcat(permisos, (informacion.st_mode & S_IXGRP) ? "x" : "-");
    strcat(permisos, (informacion.st_mode & S_IROTH) ? "r" : "-");
    strcat(permisos, (informacion.st_mode & S_IWOTH) ? "w" : "-");
    strcat(permisos, (informacion.st_mode & S_IXOTH) ? "x" : "-");

    return permisos;
};


/* Obtiene toda la información del directorio desde donde se está ejecutando el programa. */
void datosDirectorioActual(Directorio *directorio){

  directorio->dir = opendir(directorio->path);
  informacionArchivos(directorio);
  stat(".", &(directorio->informacion));
  directorio->grupo = *getgrgid(directorio->informacion.st_gid);
  directorio->usuario = *getpwuid(directorio->informacion.st_uid);
  strftime(directorio->fechaMod, sizeof(directorio->fechaMod), FORMATO_FECHA, localtime(&directorio->informacion.st_mtime));
  strftime(directorio->fechaAcc, sizeof(directorio->fechaAcc), FORMATO_FECHA, localtime(&directorio->informacion.st_atime));

}

/* Obtiene toda la información del directorio desde donde se está ejecutando el programa. */
Directorio *datosDirectorioRecursivo(char *outputFile, char *path){

  // Inicializa el directorio
  Directorio *directorio = (Directorio *) malloc(sizeof(Directorio));
  Pila *pila = (Pila *) malloc(sizeof(Pila));
  Salida elementoPila;
  // Inicializa las variables
  realpath(path, (directorio->path));
  printf("Path directorio (datosDirectorioRecursivo): %s\n", (directorio->path));
  directorio->dir = opendir(directorio->path);
  /* ---------------------------------------- PRUEBA */
  chdir(directorio->path);
  /*               */
  informacionArchivosRecursivo(directorio, pila);
  chdir(directorio->path);
  stat(".", &(directorio->informacion));
  directorio->grupo = *getgrgid(directorio->informacion.st_gid);
  directorio->usuario = *getpwuid(directorio->informacion.st_uid);
  strftime(directorio->fechaMod, sizeof(directorio->fechaMod), FORMATO_FECHA, localtime(&directorio->informacion.st_mtime));
  strftime(directorio->fechaAcc, sizeof(directorio->fechaAcc), FORMATO_FECHA, localtime(&directorio->informacion.st_atime));

  // Agrego el directorio a la pila
  creaStr(directorio, elementoPila.string);
  /*
  strcpy(elementoPila.string, str);
  */
  elementoPila.next = NULL;
  agregarPila(&elementoPila, pila);

  // Imprimo el reporte del hijo
  creaReporte(outputFile, path, pila, 1);
  // Libero el espacio de la pila
  free(pila);
  // Retorno
  return directorio;
}

void creaReporte(char *archivo, char *nombre, Pila *pila, int hijo){

  FILE *archSalida;

  if (hijo == 1) {
    chdir("/tmp");
  }

  // Creo el nombre del archivo a guardar
  strcat(archivo, "-");
  strcat(archivo, nombre);
  //strcat(archivo, ".txt");
  archSalida = fopen(archivo, "w");

  while (pila->cantElementos > 0) {
    // Escribe en el archivo
    fprintf(archSalida, "%s", pila->tope->string);
    printf("%s\n", pila->tope->string);
    sacarPila(pila);
  }

  fclose(archSalida);
}

char *creaStr(Directorio *directorio, char *str){
  // Imprime de la información del directorio
  char *permisos;
  char archivos[ARCHIVOS_MAX];
  char bytes[BYTES_MAX];
  permisos = permisosDirectorio(directorio->informacion);
  sprintf(archivos, "%d", directorio->cantArchivos);
  sprintf(bytes, "%d", directorio->bytes);

  // Creo el str
  strcpy(str, "");
  strcat(str, directorio->path);
  strcat(str, " | ");
  strcat(str, permisos);
  strcat(str, " | ");
  strcat(str, (directorio->usuario).pw_name);
  strcat(str, " | ");
  strcat(str, (directorio->grupo).gr_name);
  strcat(str, " | ");
  strcat(str, directorio->fechaMod);
  strcat(str, " | ");
  strcat(str, directorio->fechaAcc);
  strcat(str, " | ");
  strcat(str, archivos);
  strcat(str, " | ");
  strcat(str, bytes);
  strcat(str, "\n");
  // Libero memoria
  free(permisos);
  printf("String: %s\n", str);
  return str;
}