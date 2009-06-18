#!/bin/sh

# Script to generate a useful bug report
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

# Note: We assume the user has dialog, but if they don't,
#  it still generates a useful report.

if test $EUID -eq 0; then
   echo "Error: Don't run this as root"
   exit
fi

dialog --clear --title "Generating a Bug Report" \
--msgbox "This script will help you generate a useful bug-report. \
\n\nPLEASE DON'T CLOSE ANY WINDOWS THAT OPEN, they will be closed \
automatically after the script has finished. \
\n\n This may take a while, a dialog will pop up when it's \
finished" 20 80
clear
echo 'WORKING..'

# Grab full uname, for distro info
echo '@@uname@@'                              >bug_report 2>&1
uname -a                                     >>bug_report 2>&1

# Check if direct rendering is working (if available)
echo '@@glxinfo@@'                           >>bug_report 2>&1
glxinfo | grep direct                        >>bug_report 2>&1

# Grab configure errors (if any)
echo '@@config@@'                            >>bug_report 2>&1
./configure                                  >>bug_report 2>&1
# Grab compilation errors (if any)
echo '@@make@@'                              >>bug_report 2>&1
make                                         >>bug_report 2>&1

# If it compiled
if test "$?" -eq 0; then
   # Grab location of xscreensaver dirs (incase they differ in your distro)
   echo '@@xscreensaver-dirs@@'              >>bug_report 2>&1
   find /usr -name xscreensaver -type d      >>bug_report 2>/dev/null
   
   # Now grab fps stats/runtime errors
   echo '@@fps-stats@@'                      >>bug_report 2>&1
   ./src/matrixgl -f2                        >>bug_report 2>&1 & 
   PID=$!
   dialog --clear --pause "Please don't close the matrixgl window, \
it will close automatically after the timer finishes" 20 80 8
   clear
   echo 'WORKING'
   kill $PID

   # Grab install errors, if user lets us run it as root
   dialog --title "Notice" --colors --yes-label "Enter root password" \
--no-label "Continue without root" --yesno "In order for us to \
catch install errors, and add them to this report, you will need \
to enter the root password. If you don't want to enter your password, \
you can continue without it. \n\n\Z1Note: Your password is not  \
stored or added to the report" 20 80
   if test $? -eq 0; then 
      clear
      echo 'Please enter your root password'
      su -c "echo '@@install@@' >>bug_report 2>&1 && make install >>bug_report 2>&1 && exit"
   else 
      echo '@@install@@' >> bug_report
      echo 'User refused to give root pass for install' >> bug_report
   fi

fi



# All done, give user instructions
dialog --clear --title "Finished!" \
--msgbox "Bug Report compiled. Please email \
vincent@doublecreations.com and include a full description of \
the problem, as well as attaching the file 'bug_report'. \
\n\nThe file contains no personal information \
\n\nIf you are a developer, please use the sourceforge \
tracker system at <http://sf.net/projects/matrixgl/> \
instead of emailing the report." 20 80

clear
echo "Email problem to vincent@doublecreations.com and attach \
the file 'bug_report'"
echo "Thank you for helping the project :-)"

