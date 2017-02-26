// Librerias
#include "archivos.h"

// Funciones

/* Busca los permisos de un archivo en específico. */
void permisosArchivo(struct stat informacion){
    printf( (S_ISDIR(informacion.st_mode)) ? "d" : "-");
    printf( (informacion.st_mode & S_IRUSR) ? "r" : "-");
    printf( (informacion.st_mode & S_IWUSR) ? "w" : "-");
    printf( (informacion.st_mode & S_IXUSR) ? "x" : "-");
    printf( (informacion.st_mode & S_IRGRP) ? "r" : "-");
    printf( (informacion.st_mode & S_IWGRP) ? "w" : "-");
    printf( (informacion.st_mode & S_IXGRP) ? "x" : "-");
    printf( (informacion.st_mode & S_IROTH) ? "r" : "-");
    printf( (informacion.st_mode & S_IWOTH) ? "w" : "-");
    printf( (informacion.st_mode & S_IXOTH) ? "x" : "-");
};

/* Imprime la información de un archivo */
void ImprimeInfoArchivo(struct dirent *fd, char *abspath) {
  // Inicializo la Estructura del archivo
  Archivo archivo;

  // Busco toda la informacion del archivo
  strcpy((archivo.path), abspath);
  archivo.fd = *fd;
  stat(fd->d_name, &archivo.informacion);
  archivo.grupo = *getgrgid(archivo.informacion.st_gid);
  archivo.usuario = *getpwuid(archivo.informacion.st_uid);
  strftime(archivo.fechaMod, sizeof(archivo.fechaMod), FORMATO_FECHA, localtime(&archivo.informacion.st_mtime));
  strftime(archivo.fechaAcc, sizeof(archivo.fechaAcc), FORMATO_FECHA, localtime(&archivo.informacion.st_atime));

  // Imprime de la información del archivo
  printf("%s/%s ", archivo.path, archivo.fd.d_name); // Dirección del archivo
  printf("| ");
  permisosArchivo(archivo.informacion); // Permisos del archivo
  printf(" ");
  printf("| %s ", (archivo.usuario).pw_name); // User-ID
  printf("| %s ", (archivo.grupo).gr_name); // Group-ID
  printf("| %s ", archivo.fechaMod); // Fecha de Modificación
  printf("| %s ", archivo.fechaAcc); // Fecha de Acceso
  printf("\n");
};