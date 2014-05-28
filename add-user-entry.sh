#!/bin/sh
# Script to add matrixgl entry to ~/.xscreensaver file.
# Copyright (C) 2009, 2010, 2014 Vincent Launchbury
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

# Make sure ~/.xscreensaver exists
if test ! -s ~/.xscreensaver; then
   echo "Please run 'xscreensaver-demo' to create a .xscreensaver file."
   echo "Then, re-run this script to add matrixgl to the list."
   exit 1
fi

# Exit if the entry is already there
if test `grep matrixgl ~/.xscreensaver|wc -l` -gt 0; then
   echo "Matrixgl is already in the list of screensavers."
   echo "Run 'xscreensaver-demo' to select your screensaver."
   exit 0
fi

# Add matrixgl to top of list
cp ~/.xscreensaver ~/.xscreensaver-backup
sed -e '/^programs:/a\
   GL:  matrixgl -root \\n\\' ~/.xscreensaver-backup > ~/.xscreensaver

# Print success
echo "Matrixgl added to the list of screensavers."
echo "Run 'xscreensaver-demo' to select your screensaver."
