# Copyright (C) 2009, 2010, 2011, 2012, 2013, 2014 Vincent Launchbury
# Written By: Vincent Launchbury <vincent@doublecreations.com>
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

# Remember to update configure.ac's AC_CONFIG_FILES when adding new directories
SUBDIRS = data

bin_PROGRAMS = matrixgl

if USE_XSCREENSAVER
# Install binary to XScreenSaver hacks directory
xssexecdir = @XSS_HACKS@
xssexec_PROGRAMS = matrixgl
endif

matrixgl_SOURCES = matrix.c
noinst_HEADERS = matrix.h vroot.h

# Now make sure it finds Xlib and GL/GLU
matrixgl_CFLAGS = -DUNIX_MODE @WARN_CFLAGS@ @X11_CFLAGS@ @GLU_CFLAGS@
matrixgl_LDADD = -lm -lX11 @GLU_LIBS@

help:
	@cat ../docs/make-help.txt
