// Librerias
#ifndef LIBRERIAS
  #include "librerias.h"
  #define LIBRERIAS
#endif



// Estructuras

typedef struct {

	char *string;
	void *next;
} Salida;


typedef struct {
	
	Salida *tope;
	int cantElementos;

} Pila;


// Funciones

void agregarPila(Salida *elemento, Pila *pila);
void sacarPila(Pila *pila);