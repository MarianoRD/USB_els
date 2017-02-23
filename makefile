# Makefile para compilar el comando els.

# Variables
CC = gcc
CFLAGS = -g -Wall -Wextra
OBJS = 

# Regla all
all: els

# Creacion del ejecutable

els: $(OBJS)
	gcc $(CFLAGS) -o els $(OBJS)

# Creación de los Object-Files (.o)


# Clean

clean: 
	rm -f els $(OBJS)