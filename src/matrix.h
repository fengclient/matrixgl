/* matrixgl - Cross-platform matrix screensaver
 * Copyright (C) Alexander Zolotov, Eugene Zolotov 2003. 
 * Based on matrixgl 1.0 (see http://knoppix.ru/matrixgl.shtml)
 * -------------------------------------------
 * Written By:  Alexander Zolotov  <nightradio@gmail.com> 2003.
 *       and :  Eugene Zolotov     <sentinel@knoppix.ru> 2003.
 * Modified By: Vincent Launchbury <vincent@doublecreations.com> 2008,2009.
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

/* In windows we use GLUT, and certain functions cannot be static */
#ifdef NIX_MODE
#define nix_static static 
#else 
#define nix_static
#endif

static void make_text(void);
static void draw_char(int mode, long num, float light, float x, float y, float z);
static void draw_flare(float x,float y,float z);
static void draw_text1(void);
static void draw_text2(int mode);
static void scroll(int mode);
static void make_change(void);
nix_static void cbRenderScene(void);
nix_static void MouseFunc(int x, int y);
nix_static void cbKeyPressed(unsigned char key, int x, int y);
nix_static void cbResizeScene(int Width, int Height);
static void ourInit(void);
static void load_texture(void);
static void *tmalloc(size_t n);
#ifdef NIX_MODE
static char get_ascii_keycode(XEvent *ev);
static int htoi(char *str);
#endif

#define clamp(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

