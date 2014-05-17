dnl Autoconf file for the matrixgl project 
dnl Copyright (C) Vincent Launchbury 2009,2010 
dnl Written By: Vincent Launchbury <vincent@doublecreations.com> 2009-2010.
dnl -------------------------------------------
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl 
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl 
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US

AC_PREREQ([2.63])
t://matrixgl.git.sourceforge.net/gitroot/matrixgl
AC_INIT([matrixgl], [2.3.1], [vincent@doublecreations.com])
AC_COPYRIGHT([Parts Copyright (C) Vincent Launchbury 2009,2010])
AC_CONFIG_AUX_DIR([build])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([src/matrix.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 docs/Makefile])
dnl /usr/local doesn't work on some distros I tested,
dnl  also fixes the man issue.
AC_PREFIX_DEFAULT([/usr])

dnl Programs
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_MAKE_SET

dnl We need a cpp compiler to link libglu
AC_PROG_CXX
if test -z "$ac_ct_CXX"; then
   echo "***************************************************************"
   echo "FATAL ERROR: A c++ compiler cannot be found on your system."
   echo "You will need to install one for matrixgl to work. Check your "
   echo "package manager for g++ or similar. Check the dependencies"
   echo "section of the README file for more information."
   echo "***************************************************************"
   exit 1
fi

dnl Headers
AC_HEADER_STDC

dnl Library Stuff
AC_TYPE_SIZE_T
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])


dnl X11 Lib
AC_PATH_X([X11], [X11/Xlib.h], [XOpenDisplay])
if test "X$no_x" = "Xyes"; then
   echo "***************************************************************"
   echo "FATAL ERROR: X11 header files and/or libraries cannot be found."
   echo "They are sometimes in a seperate 'dev' package (e.g libx11-dev)."
   echo "Check the dependencies section of the README file for more"
   echo "information."
   echo "***************************************************************"
   exit 1
fi
AC_SUBST(x_includes)
AC_SUBST(x_libraries)

dnl Opengl stuff
AX_CHECK_GLU

if test "X$no_gl" = "Xyes"; then
   echo "***************************************************************"
   echo "FATAL ERROR: OpenGL header files and/or libraries cannot be found."
   echo "They are sometimes in a seperate 'dev' package (e.g mesa-common-dev"
   echo "or libgl1-mesa-dev). Check the dependencies section of the README "
   echo "file for more information."
   echo "***************************************************************"
   exit 1
fi
if test "X$no_glu" = "Xyes"; then
   echo "***************************************************************"
   echo "FATAL ERROR: The OpenGL Utility Library (GLU) header files and/or"
   echo "libraries cannot be found. They are sometimes in a seperate 'dev'"
   echo "package (e.g libglu1-mesa-dev). Check the dependencies section of"
   echo "the README file for more information."
   echo "***************************************************************"
   exit 1
fi


dnl Let user disable XScreenSaver support
AC_ARG_WITH([xscreensaver],
            [AS_HELP_STRING([--without-xscreensaver],
                            [disable support for XScreenSaver])],
            [],
            [with_xscreensaver=yes])
AM_CONDITIONAL([USE_XSCREENSAVER],
               [test x"$with_xscreensaver" = x"yes"])

AC_MSG_CHECKING([if xscreensaver support is enabled])
AC_MSG_RESULT([$with_xscreensaver])


##
## Find xscreensaver directories
##
if test x"$with_xscreensaver" = x"yes"; then

   # Find the hack dir (i.e screensaver binaries)
   XSS_HACKS=''
   AC_MSG_CHECKING([for xscreensaver hack directory])
   for dir in /usr/lib/xscreensaver \
      /usr/lib/misc/xscreensaver \
      /usr/libexec/xscreensaver \
      /usr/local/lib/xscreensaver \
      /usr/local/lib/misc/xscreensaver \
      /usr/local/libexec/xscreensaver \
      /usr/bin/xscreensaver-hacks; do
      test -d $dir && XSS_HACKS=$dir && break
   done
   if test -n "$XSS_HACKS"; then
      AC_MSG_RESULT($XSS_HACKS)
   else
      AC_MSG_RESULT([no])
   fi
   AC_SUBST([XSS_HACKS])

   # Find the config dir (for per-screensaver XML config)
   XSS_CONF=''
   AC_MSG_CHECKING([for xscreensaver config directory])
   for dir in /usr/lib/xscreensaver/config \
      /usr/lib/X11/xscreensaver/config \
      /usr/share/xscreensaver/config \
      /usr/local/share/xscreensaver/config \
      /usr/share/xscreensaver ; do # PLD Linux
      test -d $dir && XSS_CONF=$dir && break
   done
   if test -n "$XSS_CONF"; then
      AC_MSG_RESULT($XSS_CONF)
   else
      AC_MSG_RESULT([no])
   fi
   AC_SUBST([XSS_CONF])

   if test -z "$XSS_HACKS" || test -z "$XSS_CONF"; then
      echo "Error: xscreensaver directories cannot be found."
      echo "Use --disable-xscreensaver or \
if you're sure you have xscreensaver installed, \
file a bug as detailed in the README. Our script creates \
the bug report for you, so please send it :)"
      exit 1
   fi
fi

dnl And we're done
AC_OUTPUT

dnl Output a success message
echo "----------------------------------------------------------"
echo "Package configuration was successful, type 'make' to build"
echo "----------------------------------------------------------"
