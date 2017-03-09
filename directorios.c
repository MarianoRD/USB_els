// Librerias
#include "directorios.h"
#include <errno.h> // Quitar --------------------------------<

// Funciones

/* 
  Cuenta cuantos archivos y sub-directorios posee un directorio y cuantos
  bytes ocupan dichos archivos (sin contar los directorios)
*/
void informacionArchivos(Directorio *directorio) {
  // Variables
  struct dirent *fd;
  struct stat info;


  // Cuenta solamente los archivos
  while ((fd = readdir(directorio->dir)) != NULL) {
    if(stat(fd->d_name, &info) == -1) {
      printf("Error stat(), informacionArchivos(), directorios.c\n");
    };
    // No cuenta los archivos ocultos
    if (fd->d_name[0] == '.') {
      continue;
    }
    if (S_ISDIR(info.st_mode)) {
      (directorio->cantArchivos)++;
      (directorio->cantDirectorios)++;
    } else {
      (directorio->cantArchivos)++;
      directorio->bytes = directorio->bytes + info.st_size;
    }
  }
}

/* 
  Obtiene todos los datos solicitados de un Directorio (sin recursión)
*/
void datosDirectorio(Directorio *directorio) {
  // Busca la información de los archivos
  informacionArchivos(directorio);
  char ruta[PATH_MAX];
  getcwd(ruta, PATH_MAX);
  // Busca la información del directorio
  if(stat(".", &(directorio->informacion)) == -1) {
    printf("Error stat(), datosDirectorio(), directorios.c\n");
  };
  // Datos del grupo y usuario
  directorio->grupo = *getgrgid(directorio->informacion.st_gid);
  directorio->usuario = *getpwuid(directorio->informacion.st_uid);
  // Fechas
  strftime(directorio->fechaMod, sizeof(directorio->fechaMod), FORMATO_FECHA, localtime(&directorio->informacion.st_mtime));
  strftime(directorio->fechaAcc, sizeof(directorio->fechaAcc), FORMATO_FECHA, localtime(&directorio->informacion.st_atime));

  // Termina la función
  return;
}

/*
  Crea el string que va a ser escrito en el reporte.
*/
void creaStr(Directorio *directorio, char *str){
  // Imprime de la información del directorio
  char permisos[PERMISOS_MAX];
  char archivos[ARCHIVOS_MAX];
  char bytes[BYTES_MAX];

  // Pasa de int a str
  sprintf(archivos, "%d", directorio->cantArchivos);
  sprintf(bytes, "%d", directorio->bytes);

  // Chequea los permisos
  strcat(permisos, (S_ISDIR((directorio->informacion).st_mode)) ? "d" : "-");
  strcat(permisos, ((directorio->informacion).st_mode & S_IRUSR) ? "r" : "-");
  strcat(permisos, ((directorio->informacion).st_mode & S_IWUSR) ? "w" : "-");
  strcat(permisos, ((directorio->informacion).st_mode & S_IXUSR) ? "x" : "-");
  strcat(permisos, ((directorio->informacion).st_mode & S_IRGRP) ? "r" : "-");
  strcat(permisos, ((directorio->informacion).st_mode & S_IWGRP) ? "w" : "-");
  strcat(permisos, ((directorio->informacion).st_mode & S_IXGRP) ? "x" : "-");
  strcat(permisos, ((directorio->informacion).st_mode & S_IROTH) ? "r" : "-");
  strcat(permisos, ((directorio->informacion).st_mode & S_IWOTH) ? "w" : "-");
  strcat(permisos, ((directorio->informacion).st_mode & S_IXOTH) ? "x" : "-");

  // Creo el str
  strcpy(str, "");
  strcat(str, directorio->rutaAbs);
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

  return;
}

void datosHijo(Directorio *directorio, char *nombreArchivoSalida) {
  // Guarda la ruta inicial de ejecución
  char rutaInicial[PATH_MAX];
  getcwd(rutaInicial, PATH_MAX);
  // Inicializo variables
  struct dirent *fd;
  struct stat stat_;
  String strHijo;
  Pila pila;
  int bytes = 0;
  int archivos = 0;
  // Abro el directorio
  directorio->dir = opendir(directorio->rutaAbs);
  // Busca los datos
  chdir(directorio->rutaAbs);
  datosDirectorio(directorio);
  // Inicializo la pila
  iniciarPila(&pila, (directorio->cantDirectorios)*10);
  // Crea su string
  creaStr(directorio, strHijo);


  // Reinicio el apuntador de 'raiz'
  rewinddir(directorio->dir);
  // Busca sus sub-directorios
  while((fd = readdir(directorio->dir)) != NULL) {
    // Revisa que no sea un directorio oculto
    if (fd->d_name[0] == '.') {
      continue;
    }
    // Busca información del archivo
    if (stat(fd->d_name, &stat_) < 0) {
      printf("Error stat(), datosHijo(), directorios.c\n");
      exit(-1);
    }

    // Si es directorio
    if (S_ISDIR(stat_.st_mode)) {
      char path[PATH_MAX];
      realpath(fd->d_name, path);
      // Empieza la función recursiva
      datosSubDirectorio(path, &pila, &bytes, &archivos);
    }
  }
  // Agrega a la pila del hijo el primer string
  pushPila(&pila, &strHijo);
  // Crea el reporte
  crearReporte("/tmp", &pila, directorio->nombre, nombreArchivoSalida);
  // Cierra el directorio
  closedir(directorio->dir);
  // Regresa al directorio original
  chdir(rutaInicial);
  // Totaliza los archivos y bytes
  directorio->bytes = directorio->bytes + bytes;
  directorio->cantArchivos = directorio->cantArchivos + archivos;
  return;
  
}

void crearReporte(char *rutaSalida, Pila *pila, char *nombreDirectorio, char *nombreArchivoSalida) {
  // Inicializa las variables
  char nombreArchivo[NAME_MAX];
  String str;
  // Guarda la ruta inicial de ejecución
  char rutaInicial[PATH_MAX];
  getcwd(rutaInicial, PATH_MAX);
  // Cambia a la carpeta de salida
  chdir(rutaSalida);
  // Crea el nombre del archivo
  strcpy(nombreArchivo, "");
  strcat(nombreArchivo, nombreArchivoSalida);
  strcat(nombreArchivo, "-");
  strcat(nombreArchivo, nombreDirectorio);
  // Abre el archivo
  FILE *archivo = fopen(nombreArchivo, "w");
  // Escribe el archivo
  while(!pilaVacia(pila)) {
    popPila(pila, &str);
    fprintf(archivo, "%s", str);
    strcpy(str, "");
  }
  // Cierra el archivo
  fclose(archivo);
  // Regresa al directorio inicial
  chdir(rutaInicial);

  return;
}

void datosSubDirectorio(char *path, Pila *pila, int *bytes, int *archivos) {
  // Guarda la ruta inicial de ejecución
  char rutaInicial[PATH_MAX];
  getcwd(rutaInicial, PATH_MAX);
  // Cambio el directorio
  chdir(path);
  // Inicializo las variables
  Directorio directorio;
  struct stat stat_;
  struct dirent *fd;
  directorio.bytes = 0;
  directorio.cantArchivos = 0;
  directorio.cantDirectorios = 1;
  strcpy(directorio.rutaAbs, path);
  String str;
  // Abro el directorio
  directorio.dir = opendir(directorio.rutaAbs);
  // Busco los datos del directorio
  datosDirectorio(&directorio);
  // Reinicio el apuntador del directorio
  rewinddir(directorio.dir);

  // Hago la recursión
  while((fd = readdir(directorio.dir)) != NULL) {
  // Revisa que no sea un directorio oculto
  if (fd->d_name[0] == '.') {
    continue;
  }
  // Busca información del archivo
  if (stat(fd->d_name, &stat_) < 0) {
    printf("Error stat(), datosSubDirectorio(), directorios.c\n");
    exit(-1);
  }

  // Si es directorio
  if (S_ISDIR(stat_.st_mode)) {
    char path[PATH_MAX];
    realpath(fd->d_name, path);
    // Entra en la recursión
    datosSubDirectorio(path, pila, bytes, archivos);
  }
}
  // Cierra el directorio
  closedir(directorio.dir);
  // Regresa al directorio original
  chdir(rutaInicial);
  // Guarda la información en la pila
  *bytes = *bytes + directorio.bytes;
  *archivos = *archivos + directorio.cantArchivos;
  creaStr(&directorio, str);
  pushPila(pila, &str);

  return;
}