INCLUDE = -I/usr/X11R6/include -I/usr/X11/include
LIBDIR  = -L/usr/X11R6/lib -L/usr/X11/lib
MACLIBS = -L/usr/local/lib/  -L/System/Library/Frameworks/OpenGL.framework/Libraries 

CFLAGS = -O2 -Wall -pedantic -ansi -W -Wno-unused -DNIX_MODE
CC = gcc
LIBS = -lX11 -lGL -lGLU -lm


Matrix: matrix.c
	$(CC) matrix.c -o matrixgl $(CFLAGS) $(INCLUDE) $(LIBDIR) $(LIBS)
	@strip matrixgl

mac: matrix.c
	$(CC) matrix.c -o matrixgl $(CFLAGS) $(INCLUDE) $(LIBDIR) $(LIBS) $(MACLIBS) -framework OpenGL
	@strip matrixgl

