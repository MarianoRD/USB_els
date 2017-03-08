// Librerias
#ifndef LIBRERIAS
  #include "librerias.h"
  #define LIBRERIAS
#endif

#include "pila.h"

// Estructuras
typedef struct
{
  char rutaAbs[PATH_MAX];
  char nombre[NAME_MAX];
  DIR *dir;
  int cantArchivos;
  int cantDirectorios;
  struct stat informacion;
  struct group grupo;
  struct passwd usuario;
  char fechaMod[DATE_MAX];
  char fechaAcc[DATE_MAX];
  int bytes;

} Directorio;

// Funciones
void datosDirectorio(Directorio *directorio);
void creaStr(Directorio *directorio, char *str);
void datosHijo(Directorio *directorio, char *nombreArchivoSalida);
void datosSubDirectorio();
void crearReporte(char *rutaSalida, Pila *pila, char*nombreDirectorio, char *nombreArchivoSalida);