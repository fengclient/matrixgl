/* matrixgl - Cross-platform matrix screensaver
 * Copyright (C) Alex Zolotoz 2003. 
 * Based on matrixgl 1.0 (see http://knoppix.ru/matrixgl.shtml)
 * -------------------------------------------
 * Written By: Alex Zolotoz <No active email known> 2003.
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
void make_text(void);
void draw_char(int mode, long num, float light, float x, float y, float z);
void draw_flare(float x,float y,float z);
void draw_text1(void);
void draw_text2(void);
void draw_text3(void);
void scroll(int mode);
void make_change(void);
void cbRenderScene(void);
void MouseFunc(int x, int y);
void cbKeyPressed(unsigned char key, int x, int y);
void ourBuildTextures(void);
void cbResizeScene(int Width, int Height);
void ourInit(void);
void load_texture(void);
void *tmalloc(size_t n);
#ifdef NIX_MODE
char get_ascii_keycode(XEvent *ev);
int htoi(char *str);
#endif

#define clamp(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
