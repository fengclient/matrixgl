#!/bin/sh
# Script to generate a useful bug report
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

# Intro message
echo "This tool will help you generate a useful bug-report."
echo ">>> PLEASE DON'T CLOSE ANY WINDOWS THAT OPEN <<<"
sleep 1
echo " "

# Grab full uname, for distro info
echo "@@uname@@"                              >bug_report 2>&1
uname -a                                     >>bug_report 2>&1
echo "Grabbing uname............................ [Done]"

# Check if direct rendering is working (if available)
echo "@@glxinfo@@"                           >>bug_report 2>&1
echo -n 'Grabbing direct rendering information..... '
glxinfo 2>>bug_report | grep direct          >>bug_report 2>&1
if test "$?" -eq 0; then
   echo "[Done]"
else
   echo "[Failed!]"
fi

# Grab location of xscreensaver dirs (incase they differ in your distro)
echo "@@xscreensaver-dirs@@"              >>bug_report 2>&1
echo -n "Finding xscreensaver directories.......... "
find /usr -maxdepth 3                        \
   -path /usr/sbin      -prune -o            \
   -path /usr/include   -prune -o            \
   -path /usr/games     -prune -o            \
   -path /usr/src       -prune -o            \
   -path /usr/tmp       -prune -o            \
   -path /usr/portage   -prune -o            \
   -path /usr/kde       -prune -o            \
   -path /usr/share/doc -prune -o            \
   -name 'xscreensaver*' -type d -print   >>bug_report 2>/dev/null
echo "[Done]"

# Run autoreconf (if running a snapshot)
echo '@@releasetype@@'                          >>bug_report 2>&1
if test ! -e configure; then
   echo 'snapshot'                              >>bug_report 2>&1
   echo '@@autoreconf@@'                        >>bug_report 2>&1
   echo -n "Generating autotools files................ "
   autoreconf -i                                >>bug_report 2>&1
   if test "$?" -eq 127; then
      echo "[Failed!]"
      echo "Error: Autotools is needed to build matrixgl snapshots."
      echo "Please install autoconf, automake and libtool, and"
      echo "then try again. If you can't do this, you can download"
      echo "the latest stable release, and test it without autotools."
      echo "If all else fails, you can still email a text description"
      echo "of the problem to vincent@doublecreations.com. "
      echo "Thanks for helping the project!"
      exit
   else
      echo "[Done]"
   fi
else
   echo 'stable-release (best guess)'            >>bug_report 2>&1
fi

# Grab configure errors (if any)
echo "@@config@@"                            >>bug_report 2>&1
echo -n "Configuring matrixgl...................... "
./configure                                  >>bug_report 2>&1
echo "[Done]"

# Clean, to make sure it rebuilds again
echo -n "Cleaning matrixgl......................... "
make clean >/dev/null 2>&1
echo "[Done]"

# Grab compilation errors (if any)
echo "@@make@@"                              >>bug_report 2>&1
echo -n "Compiling matrixgl........................ "
make                                         >>bug_report 2>&1

# If it compiled
if test "$?" -eq 0; then
   echo "[Done]"

   # Now grab any runtime errors, and fps stats
   echo "@@fps-stats@@"                      >>bug_report 2>&1
   # subshell to avoid "Terminated" messages getting printed
   (
      ./src/matrixgl -f2                     >>bug_report 2>&1 &
      PID=$!
      echo -n "Running matrixgl.......................... "
      sleep 6
      kill $PID                              >/dev/null 2>&1
   )
   echo "[Done]"

   # Grab install errors, if user lets us run it as root
   echo " "
   echo ">>> We need to run as root to check for install errors <<<"
   echo "Please enter your root password if prompted (it is not stored)"
   echo '@@install@@'                                       >>bug_report 2>&1

   # This tries to be as portable as possible in two regards:
   #
   #  - Specifiying the root username is required for various su(1)
   #  implementations where the -c option doesn't mean to run a command. By
   #  passing -c after the username, such implementations treat it as an option
   #  to the shell.
   #  - The "/bin/sh -c" part is required in case root's login shell isn't
   #  bourne-compatible (e.g [t]csh). A bourne-compatible shell is required for
   #  the output redirection to work.
   su root -c "/bin/sh -c 'make install >>bug_report 2>&1'"
   if test "$?" -ne 0; then
      echo 'su returned non-zero status'                    >>bug_report 2>&1
   fi
else
   echo "[Failed]"
fi

echo "@@FINISHED@@" >> bug_report 2>&1


# All done, give user instructions
echo " "
echo "--------------------------"
echo "Bug Report compiled. Please email vincent@doublecreations.com"
echo "and include a full description of the problem, as well as "
echo "attaching the file 'bug_report'. "

echo " "
echo "Thank you for helping the project :-)"
