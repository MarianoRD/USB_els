// Librerias
#ifndef LIBRERIAS
  #include "librerias.h"
  #define LIBRERIAS
#endif

#include "pila.h"

// Estructuras
typedef struct
{
  char path[PATH_MAX];
  char nombre[NAME_MAX];
  DIR *dir;
  int cantArchivos;
  struct stat informacion;
  struct group grupo;
  struct passwd usuario;
  char fechaMod[DATE_MAX];
  char fechaAcc[DATE_MAX];
  int bytes;
  int absoluto;

} Directorio;

// Funciones
char *obtenerDirectorioRaiz();
void informacionArchivos(Directorio *directorio);
char *permisosDirectorio(struct stat informacion);
void datosDirectorioActual(Directorio *directorio);
void creaReporte(char *archivo, char* nombre, Pila *pila, int hijo);
Directorio *datosDirectorioRecursivo(char *outputFile, char *path);
void informacionArchivosRecursivo(Directorio *directorioA, Pila *pila);
void borraCore(char*nombre);
char *creaStr(Directorio *directorio, char *str);
void imprimeDirectorio(char *abspath, Directorio directorio);