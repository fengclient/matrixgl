AC_INIT([matrixgl], [2.1.1], [vincent@doublecreations.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/matrix.c])
AC_PROG_CC
AC_PROG_INSTALL
AC_PATH_X

AC_CHECK_HEADERS(GL/gl.h GL/glu.h,,
         [AC_CHECK_HEADERS(OpenGL/gl.h OpenGL/glu.h,,
           [echo "OpenGL is mandatory";exit 1])]
)

if test `uname -s` != Darwin;
then
AC_CHECK_LIB(GL, glInitNames,,
     AC_CHECK_LIB(MesaGL,glInitNames,,
            echo "OpenGL is mandatory";exit 1)
)

dnl Check for GLU (Original or Mesa)
AC_CHECK_LIB(GLU, gluLookAt, ,
     AC_CHECK_LIB(MesaGLU,gluLookAt,,
     echo "incomplete OpenGL (no GLU)";exit 1)
)
fi

AM_PROG_CC_C_O
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])
AC_CONFIG_FILES([Makefile
                 src/Makefile])

AC_OUTPUT
