/* matrixgl - Cross-platform matrix screensaver
 *
 * Copyright (C) 2003 Alexander Zolotov, Eugene Zolotov
 * Copyright (C) 2008, 2009, 2010, 2011, 2012, 2013, 2014 Vincent Launchbury
 *
 * See AUTHORS for a full list of contributors.
 *
 * -------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US
 */

#ifndef MATRIX_H
#define MATRIX_H

/* Windows uses GLUT callbacks, which cannot be static */
#ifdef UNIX_MODE
#  define unix_static static
#else  /* WIN32_MODE */
#  define unix_static
#endif /* WIN32_MODE */

static void draw_char(long num, float light, float x, float y, float z);
static void draw_flare(float x,float y,float z);
static void draw_text1(void);
static void draw_text2(int mode);
static void scroll(void);
static void make_change(void);
unix_static void cbRenderScene(void);
unix_static void cbKeyPressed(unsigned char key, int x, int y);
unix_static void cbResizeScene(int width, int height);
unix_static void MouseFunc(int x, int y);
static void ourInit(void);
static void *tmalloc(size_t n);
#ifdef UNIX_MODE
static char get_ascii_keycode(XEvent *ev);
#endif

/* Get rid of warnings when we need an unused parameter */
#ifdef __GNUC__
#  define unused __attribute__((unused))
#elif defined (__LCLINT__)
#  define unused /*@unused@*/
#else
#  define unused /* */
#endif /* __GNUC__ */

#define clamp(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#endif /* MATRIX_H */

