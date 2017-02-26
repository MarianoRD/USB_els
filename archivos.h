// Librerias
#ifndef LIBRERIAS
  #include "librerias.h"
  #define LIBRERIAS
#endif

// Estructura

typedef struct
{
  char path[PATH_MAX];
  struct dirent fd;
  struct stat informacion;
  struct group grupo;
  struct passwd usuario;
  char fechaMod[DATE_MAX];
  char fechaAcc[DATE_MAX];

} Archivo;

// Funciones
void permisosArchivo(struct stat informacion);
void ImprimeInfoArchivo(struct dirent *fd, char *abspath);
