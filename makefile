# Makefile para compilar el comando els.

# Variables
CC = gcc
CFLAGS = -g -Wall -Wextra
OBJS = main.o directorios.o archivos.o librerias.o pila.o

# Regla all
all: els

# Creacion del ejecutable

els: $(OBJS)
	gcc $(CFLAGS) -o els $(OBJS)

# Creación de los Object-Files (.o)
main.o: main.c archivos.o directorios.o librerias.o
	gcc $(CFLAGS) -c main.c

archivos.o: archivos.h librerias.o
	gcc $(CFLAGS) -c archivos.c

directorios.o: directorios.h librerias.o
	gcc $(CFLAGS) -c directorios.c

librerias.o: librerias.h
	gcc $(CFLAGS) -c librerias.c

pila.o: pila.h
	gcc $(CFLAGS) -c pila.c

# Clean

clean: 
	rm -f els $(OBJS)