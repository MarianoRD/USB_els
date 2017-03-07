// Librerias
#ifndef LIBRERIAS
  #include "librerias.h"
  #define LIBRERIAS
#endif



// Estructuras

typedef struct Salida Salida;

struct Salida {

	char string[STRING_MAX];
	Salida *next;
};


typedef struct {
	
	Salida *tope;
	int cantElementos;

} Pila;


// Funciones

void agregarPila(Salida *elemento, Pila *pila);
void sacarPila(Pila *pila);