# We want /usr/bin, not /usr/local/bin
bexecdir = /usr/bin
bexec_PROGRAMS = matrixgl

# Now install to xscreensaver
xssexecdir = @XSS_HACKS@
xssconfdir = @XSS_CONF@
xssexec_PROGRAMS = matrixgl
xssconf_DATA = matrixgl.xml

# Doesn't work on some distros
# I tested.
man_MANS = matrixgl.1

# So we find it ourselves instead
ourman1dir = @MAN1_DIR@
ourman1_DATA = matrixgl.1

# Now make sure it finds Xlib and GL/GLU
matrixgl_SOURCES = matrix.c
matrixgl_CFLAGS = -DNIX_MODE -pedantic -ansi -Wall -W -Wno-unused 
matrixgl_CFLAGS += -I/usr/X11R6/include -I/usr/X11/include
matrixgl_CFLAGS +=  @GLU_CFLAGS@
matrixgl_LDFLAGS =  -L/usr/X11R6/lib -L/usr/X11/lib
matrixgl_LDFLAGS += -L/usr/local/lib/  -L/System/Library/Frameworks/OpenGL.framework/Libraries 
matrixgl_LDFLAGS += -lm @GLU_LIBS@ 


