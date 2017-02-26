// Librerias
#ifndef LIBRERIAS
  #include "librerias.h"
  #define LIBRERIAS
#endif

// Estructuras
typedef struct
{
  char path[PATH_MAX];
  DIR *dir;
  int cantArchivos;
  struct stat informacion;
  struct group grupo;
  struct passwd usuario;
  char fechaMod[DATE_MAX];
  char fechaAcc[DATE_MAX];
  int bytes;

} Directorio;

// Funciones
char *obtenerDirectorioRaiz();
void informacionArchivos(Directorio *directorio);
void permisosDirectorio(struct stat informacion);
Directorio *datosDirectorioActual(char *path);
void imprimeDirectorio(char *abspath, Directorio directorio);
Directorio *datosDirectorioRecursivo(char *path);
void informacionArchivosRecursivo(Directorio *directorio);