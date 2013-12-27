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
matrixgl_CFLAGS = -DNIX_MODE -pedantic -ansi -Wall -W @x_includes@ @GLU_CFLAGS@
matrixgl_LDADD = -lm -lX11 @GLU_LIBS@

