#!/bin/sh
# Script to add matrixgl entry to ~/.xscreensaver file.
# Copyright (C) Vincent Launchbury 2009 
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

# Make sure ~/.xscreensaver exists
if test ! -e ~/.xscreensaver; then
   echo "Error: File '~/.xscreensaver' does not exist"
   echo "Are you sure you have xscreensaver installed?"
   echo "Try running 'xscreensaver-demo', and then re-run this script"
   exit 1
fi

# If it's empty, user needs to run xscreensaver-demo to generate
if test ! -s ~/.xscreensaver; then
   echo "Error: '~/.xscreensaver' is empty"
   echo "Please run 'xscreensaver-demo' and then re-run this script"
   exit 1
fi

# Exit if the entry is already there
if test `grep matrixgl ~/.xscreensaver|wc -l` -gt 0; then
   echo "Warning: Matrixgl is already in the list of screensavers!"
   echo "Run 'xscreensaver-demo' to select your screensaver."
   exit 0
fi

# Line of last entry
LENTRY=`sed -n /\\\\\\\n/= ~/.xscreensaver | sed $\!d`

# Add entry after last entry
sed "$LENTRY a GL:\t\tmatrixgl -root -C green\t         \\\\n\\\\" ~/.xscreensaver > .tmp-$$-xs

# Overwrite users file
mv -f .tmp-$$-xs ~/.xscreensaver
rm -f .tmp-$$-xs 2>/dev/null

# Print success
echo "Matrixgl successfully added to xscreensaver list."
echo "Use xscreensaver-demo to select matrixgl and edit it's settings"
