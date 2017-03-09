// Librerias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <langinfo.h>
#include <elf.h>
#include <libgen.h>

// Macros
#define PATH_MAX 4096    /* # chars in a path name including nul */
#define NAME_MAX 255    /* # chars in a file name */
#define READ 0  // Pipe read
#define WRITE 1 // Pipe write
#define DATE_MAX 12 /* chars in date */
#define PERMISOS_MAX 11
#define ARCHIVOS_MAX 21
#define BYTES_MAX 21
#define STR_MAX 6000
#define FORMATO_FECHA nl_langinfo(D_FMT)

// Funciones

void manejadorDeSenales(int);
void verificarEscritura(char *str);
int verificarPermisosRX(char *str);