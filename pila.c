// Librerias
#include "pila.h"


// Funciones

void agregarPila(Salida *elemento, Pila *pila) {
	elemento->next = pila->tope;
	pila->tope = elemento;

	pila->cantElementos++;
};


void sacarPila(Pila *pila) { // DEBERIA DEVOLVER LA SALIDA
	Salida *temp;
	temp = pila->tope;
	pila->tope = (pila->tope)->next;
	temp->next = NULL;
	/*
	free(temp);
	*/
	pila->cantElementos--;
};