// Librerias
#include "pila.h"


// Funciones

void iniciarPila(Pila *pila, int cantidadElementos) {
  String *contenido;

  /* Allocate a new array to hold the contenido. */

  contenido = (String *)malloc(sizeof(String)* (cantidadElementos+1));

  if (contenido == NULL) {
    printf("Error inicializando la pila\n");
    exit(-1);  /* Exit, returning error code. */
  }

  pila->contenido = contenido;
  pila->maxElementos = cantidadElementos;
  pila->top = -1;  // Pila vacia
}

void destruyePila(Pila *pila) {
	// Libera memoria
	free(pila->contenido);

	// Reinicia los valores inicializados
	pila->contenido = NULL;
  	pila->maxElementos = 0;
  	pila->top = -1;  // Pila vacia
}

int pilaLlena(Pila *pila) {
	if (pila->top == (pila->maxElementos)) {
		return 1;
	} else {
		return 0;
	}
}

int pilaVacia(Pila *pila) {
	if (pila->top == -1) {
		return 1;
	} else {
		return 0;
	}
}

void pushPila(Pila *pila, String *elemento) {
	// Chequea que la pila no esté llena
	if (pilaLlena(pila)) {
    	printf("La pila está llena, no se puede agregar otro elemento.\n");
    	exit(1);
  	}
 
  	// Agrega el elemento
  	(pila->top)++;
  	strcpy(pila->contenido[pila->top], *elemento);
 
}


void popPila(Pila *pila, String *str) { // DEBERIA DEVOLVER LA SALIDA
	// Chequea que la pila no esté vacía
	if (pilaVacia(pila)) {
    	printf("La pila está vacía, no se puede sacar un elemento.\n");
    	exit(1);
  	}

  	// Saca el elemento
  	pila->top--;
  	strcpy(*str, (pila->contenido[(pila->top)+1]));
};