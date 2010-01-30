bin_PROGRAMS = matrixgl

if USE_XSCREENSAVER
# Now install to xscreensaver
xssexecdir = @XSS_HACKS@
xssconfdir = @XSS_CONF@
xssexec_PROGRAMS = matrixgl
dist_xssconf_DATA = matrixgl.xml
endif

dist_man_MANS = matrixgl.1

matrixgl_SOURCES = matrix.c
noinst_HEADERS = matrix.h matrix1.h matrix2.h vroot.h

# Now make sure it finds Xlib and GL/GLU
matrixgl_CFLAGS = -DNIX_MODE -pedantic -ansi -Wall -W
matrixgl_CFLAGS += -I/usr/X11R6/include -I/usr/X11/include
matrixgl_CFLAGS +=  @GLU_CFLAGS@ -lX11
matrixgl_LDFLAGS =  -L/usr/X11R6/lib -L/usr/X11/lib -L/usr/local/lib/
matrixgl_LDFLAGS += -lm @GLU_LIBS@ 


