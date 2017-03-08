// Librerias
#ifndef LIBRERIAS
  #include "librerias.h"
  #define LIBRERIAS
#endif



// Estructuras

typedef char String[PATH_MAX];


typedef struct {
	

	String *contenido;
	int top;
	int maxElementos;

} Pila;


// Funciones
void iniciarPila(Pila *pila, int cantidadElementos);
void destruyePila(Pila *pila);
int pilaLlena(Pila *pila);
int pilaVacia(Pila *pila);
void pushPila(Pila *pila, String *elemento);
void popPila(Pila *pila, String *str);