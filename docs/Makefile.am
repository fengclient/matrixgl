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

EXTRA_DIST = make-help.txt

# Man page
dist_man_MANS = matrixgl.1
# Info page (does html/pdf/ps/..etc automatically)
info_TEXINFOS = matrixgl.texi

# Make HTML documentation single-page
AM_MAKEINFOHTMLFLAGS = --no-headers --no-split

help:
	@cat make-help.txt
