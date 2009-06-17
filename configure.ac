# Autoconf file for the matrixgl project 
# Copyright (C) Vincent Launchbury 2009 
# Written By: Vincent Launchbury <vincent@doublecreations.com> 2009.
# -------------------------------------------
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US

AC_PREREQ([2.63])
AC_INIT([matrixgl], [2.2], [vincent@doublecreations.com])
AC_COPYRIGHT([Parts Copyright (C) Vincent Launchbury 2009])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([src/matrix.c])
AC_CONFIG_HEADERS([config.h])

# Programs
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Headers
AC_PATH_X
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# 
#Opengl stuff
#
AC_CHECK_HEADERS(GL/gl.h GL/glu.h GL/glx.h,,
         [AC_CHECK_HEADERS(OpenGL/gl.h OpenGL/glu.h GL/glx.h,,
           [echo "Error: OpenGL is mandatory";exit 1])]
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

# 
# Find xscreensaver directories
#

# First the hack dir
XSS_HACKS=0
echo -n "checking for xscreensaver hack directory... "
for dir in /usr/lib/xscreensaver \
   /usr/lib/misc/xscreensaver \
   /usr/libexec/xscreensaver \
   /usr/local/lib/xscreensaver \
   /usr/local/lib/misc/xscreensaver \
   /usr/local/libexec/xscreensaver \
   /usr/bin/xscreensaver-hacks; do
   test -d $dir && XSS_HACKS=$dir && break
done
if test -n $XSS_HACKS; then
   echo "$XSS_HACKS"
else
   echo "no"
fi
AC_DEFINE_UNQUOTED(XSCREENSAVER_HACKS_DIR, "$XSS_HACKS", [xscreensaver hack directory])

# Then the config dir
XSS_CONF=''
echo -n "checking for xscreensaver config directory... "
for dir in /usr/lib/xscreensaver/config \
   /usr/lib/X11/xscreensaver/config \
   /usr/share/xscreensaver/config \
   /usr/local/share/xscreensaver/config ; do
   test -d $dir && XSS_CONF=$dir && break
done
if test -n $XSS_CONF; then
   echo "$XSS_CONF"
else
   echo "no"
fi
AC_DEFINE_UNQUOTED(XSCREENSAVER_CONFIG_DIR, "$XSS_CONF", [xscreensaver conf directory])

if test -z "$XSS_HACKS" || test -z "$XSS_CONF"; then
   echo "Error: xscreensaver directories cannot be found."
   echo "If you're sure you have xscreensaver installed, \
please file a bug as detailed in README. Our script creates \
the bug report for you, so please please send it :)"
   exit 1
fi
AC_SUBST([XSS_HACKS])
AC_SUBST([XSS_CONF])
# End xscreensaver stuff




AM_PROG_CC_C_O
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])
AC_CONFIG_FILES([Makefile
                 src/Makefile])

# Library Stuff
AC_TYPE_SIZE_T
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_OUTPUT
