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
t://matrixgl.git.sourceforge.net/gitroot/matrixgl
AC_INIT([matrixgl], [2.2.2], [vincent@doublecreations.com])
AC_COPYRIGHT([Parts Copyright (C) Vincent Launchbury 2009])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([src/matrix.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile
                 src/Makefile])

# Programs
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Headers
AC_HEADER_STDC

# Library Stuff
AC_TYPE_SIZE_T
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])


# X11 Lib
AC_PATH_X
AC_CHECK_LIB([X11], [XOpenDisplay],, 
             [AC_CHECK_HEADERS(X11/Xlib.h,,
              [echo "Error: X11 not found";exit 1])]
)

#Opengl stuff
AX_CHECK_GLU

# Find man dirs better than automake does.
MAN1_DIR=0
test "$silent" != yes && echo -n "checking for man1 directory... "
for dir in /usr/share/man/man1 \
   /usr/local/share/man/man1 \
   /usr/local/man/man1 \
   /usr/man/man1 \
   /usr/contrib/man/man1; do
   test -d $dir && MAN1_DIR=$dir && break
done
if test -n $MAN1_DIR; then
   test "$silent" != yes && echo "$MAN1_DIR"
else
   test "$silent" != yes && echo "no. Using /usr/share/man/man1"
   MAN1_DIR = "/usr/share/man/man1"
fi
AC_SUBST([MAN1_DIR])


# Let user disable xscreensaver support
AC_ARG_ENABLE(xscreensaver,
              [  --disable-xscreensaver Don't install to or check for xscreensaver],
              [case "${enableval}" in
                no) skip_xscreensaver=true ;;
               yes) skip_xscreensaver=false ;;
                 *) AC_MSG_ERROR(bad value ${enableval} for --disable-xscreensaver) ;;
               esac],
               [skip_xscreensaver=false])
AM_CONDITIONAL(USE_XSCREENSAVER, 
               test "$skip_xscreensaver" != true)

test "$silent" != yes && echo -n "checking if xscreensaver feature is enabled.. "
if test "$skip_xscreensaver" = false; then
   test "$silent" != yes && echo "yes"
else
   test "$silent" != yes && echo "no"
fi

if test "$skip_xscreensaver" = false; then
   # 
   # Find xscreensaver directories
   #
   # First the hack dir
   XSS_HACKS=0
   test "$silent" != yes && echo -n "checking for xscreensaver hack directory... "
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
      test "$silent" != yes && echo "$XSS_HACKS"
   else
      test "$silent" != yes && echo "no"
   fi
   AC_DEFINE_UNQUOTED(XSCREENSAVER_HACKS_DIR, "$XSS_HACKS", [xscreensaver hack directory])

   # Then the config dir
   XSS_CONF=''
   test "$silent" != yes && echo -n "checking for xscreensaver config directory... "
   for dir in /usr/lib/xscreensaver/config \
      /usr/lib/X11/xscreensaver/config \
      /usr/share/xscreensaver/config \
      /usr/local/share/xscreensaver/config ; do
      test -d $dir && XSS_CONF=$dir && break
   done
   if test -n $XSS_CONF; then
      test "$silent" != yes && echo "$XSS_CONF"
   else
      test "$silent" != yes && echo "no"
   fi
   AC_DEFINE_UNQUOTED(XSCREENSAVER_CONFIG_DIR, "$XSS_CONF", [xscreensaver conf directory])

   if test -z "$XSS_HACKS" || test -z "$XSS_CONF"; then
      echo "Error: xscreensaver directories cannot be found."
      echo "Use --disable-xscreensaver or \
If you're sure you have xscreensaver installed, \
please file a bug as detailed in README. Our script creates \
the bug report for you, so please please send it :)"
      exit 1
   fi
   AC_SUBST([XSS_HACKS])
   AC_SUBST([XSS_CONF])
fi

# And we're done
AC_OUTPUT
