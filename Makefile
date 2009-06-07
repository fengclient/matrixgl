INCLUDE = -I/usr/X11R6/include -I/usr/X11/include
LIBDIR  = -L/usr/X11R6/lib -I/usr/X11/lib

CFLAGS = -O2 -Wall -pedantic -ansi -W -Wno-unused -ggdb
CC = gcc
LIBRARIES = -lX11 -lXmu -lGL -lGLU -lm

Matrix: matrix.o
	@$(CC) matrix.o -o matrixgl $(CFLAGS) $(INCLUDE) $(LIBDIR) $(LIBRARIES)
	@strip matrixgl
	@rm -f *.o

matrix: matrix.c
	@$(CC) -c $(CFLAGS) matrix.c
