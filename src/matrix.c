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

#ifndef UNIX_MODE
   #define WIN32_MODE
#endif

/* Includes */
#ifdef WIN32_MODE
   #define _CRT_SECURE_NO_WARNINGS
   #include <windows.h>
   #include <process.h>
#else /* UNIX_MODE */
   #include <X11/Xlib.h>
   #include <X11/Xutil.h>
   #define __USE_XOPEN_EXTENDED
   #include <unistd.h>
   #include <getopt.h>
   #include <time.h>
#endif /* UNIX_MODE */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>   /* OpenGL itself. */
#include <GL/glu.h>  /* GLU support library. */
#ifdef UNIX_MODE
   #include <GL/glx.h>
   #include "vroot.h"
#else /* WIN32_MODE */
   #include <GL/glut.h> /* GLUT support library. */
#endif /* WIN32_MODE */

#include "matrix.h"
#include "data/fonts.h"
#include "data/images.h"
#ifdef UNIX_MODE
   #include "config.h"  /* Autoconf stuff */
#endif /* UNIX_MODE */

/* Global Variables */
#define rtext_x 90
#define text_y 70
int text_x;
unsigned char *speeds; /* Speed of each column (0-2) */

struct glyph {
   unsigned char num;   /* Character number (0-59) */
   unsigned char alpha; /* Alpha modifier */
   float z;             /* Cached Z coordinate */
} *glyphs;

int pic_offset;            /* Which image to show */
long timer=40;             /* Controls pic fade in/out */
int pic_fade=0;            /* Makes all chars lighter/darker */
int classic=0;             /* classic mode (no 3d) */
int paused=0;
int num_pics=11 -1;         /* # 3d images (0 indexed) */
GLenum color=GL_GREEN;     /* Color of text */
int rain_intensity=1;      /* Intensity of digital rain */


#ifdef UNIX_MODE
Display                 *dpy;
GLint                   att[] = {GLX_RGBA, GLX_DOUBLEBUFFER, None};
XVisualInfo             *vi;
XWindowAttributes       gwa;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XEvent                  xev;
int x,y;
#endif

/* FPS Stats */
time_t last;               /* Time since FPS was last polled */
int showfps=0;             /* Whether to output FPS stats or not */
unsigned long sleeper = 0; /* Per-frame sleeper for frame-limiting */
float fps=0;               /* Last polled FPS */
int frames=0;              /* # frames shown since last FPS poll */
int fpoll=1;               /* How often to poll the frame per seconds */
float maxfps=32.0;         /* Default maximum FPS */


#ifdef WIN32_MODE
/*
 * int main(int argc, char **argv)
 */
int __stdcall WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR lpCmd,int nShow)
{
   int argc=1,i;
   char win[100];
   char *cfile = malloc(strlen(win)+25);
   FILE *config;
   GetWindowsDirectory(win, 100);

   /* We don't support preview on windows */
   if(lpCmd[1]=='p') exit(EXIT_SUCCESS);
   /* Run the config dialog */
   if(lpCmd[1]=='c') {
      sprintf(cfile, "%s\\matrixgl_config.exe", win);
      _execl(cfile, cfile, NULL);
      exit(EXIT_SUCCESS);
   }

   glutInit(&argc, &lpCmd);
   srand(GetTickCount());

   /* Read in config */
   sprintf(cfile, "%s\\matrixgl_config", win);
   config = fopen(cfile, "r");
   if (config) {
      char vals[4];
      fgets(vals, 4, config);
      /* Static mode */
      if (vals[0]!='0') cbKeyPressed('s', 0, 0);
      /* Show credits on startup */
      if (vals[1]!='0' && !classic) cbKeyPressed('c', 0, 0);
      /* Set color */
      if (vals[2]=='1') color=GL_GREEN;
      if (vals[2]=='2') color=GL_RED;
      if (vals[2]=='3') color=GL_BLUE;
   }
   free(cfile);
#else /* UNIX_MODE */
int main(int argc,char **argv)
{
   int i, opt;
   enum {PREVIEW, ROOT, FS, WINDOWED} wuse=WINDOWED; /* Windown type */
   Window wid=0;      /* ID of window, used to grab preview size */

   static struct option long_opts[] =
   {
      {"static",    no_argument,       0, 's'},
      {"credits",   no_argument,       0, 'c'},
      {"color",     required_argument, 0, 'C'},
      {"help",      no_argument,       0, 'h'},
      {"version",   no_argument,       0, 'v'},
      {"window-id", required_argument, 0, 'W'},
      {"root",      no_argument,       0, 'R'},
      {"fs",        no_argument,       0, 'F'},
      {"fullscreen",no_argument,       0, 'F'},
      {"fps",       optional_argument, 0, 'f'},
      {"limit",     required_argument, 0, 'l'},
      {"intensity", required_argument, 0, 'i'},
      {0, 0, 0, 0}
   };
   int opti = 0;
   while ((opt = getopt_long_only(argc, argv, "schvi:l:f::C:FRW:", long_opts, &opti))) {
      if (opt == EOF) break;
      switch (opt) {
         case 'W':
            wuse=PREVIEW;
            wid=strtol(optarg, NULL, 16);
            break;
         case 'R':
            wuse=ROOT;
         case 'F':
            wuse=FS;
            break;
         case 's':
            cbKeyPressed('s', 0, 0);
            break;
         case 'c':
            if (classic) break; /* Can't be used with 's' */
            cbKeyPressed('c', 0, 0); /* Start at credits */
            break;
         case 'C':
            if (!strcmp("green", (char *)optarg))
               color=GL_GREEN;
            else if (!strcmp("red", (char *)optarg))
               color=GL_RED;
            else if (!strcmp("blue", (char *)optarg))
               color=GL_BLUE;
            else {
               fprintf(stderr, "Error: Color must be green, red or blue\n");
               color=GL_GREEN;
            }
            break;
         case 'i':
            rain_intensity = clamp(atoi(optarg), 1, 50);
            break;
         case 'f':
            showfps=1;
            if (optarg) fpoll=clamp(atoi(optarg), 1, 20);
            else if (optind>0 && optind<argc && atoi(argv[optind]))
               fpoll=clamp(atoi(argv[optind]), 1, 20);
            break;
         case 'l':
            maxfps=clamp(atoi(optarg), 1, 200);
            break;
         case 'h':
            fputs("Usage: matrixgl [OPTIONS]...\n\
3D Matix Screensaver based on The Matrix Reloaded\n\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  -C --color=COL         Set color to COL (must be green, red or blue)\n\
  -c --credits           Show the credits on startup\n\
  -F --fs --fullscreen   Run in fullscreen window\n\
  -f --fps[=SEC]         Print fps stats every SEC seconds (default: 2)\n\
  -h --help              Show the help screen\n",
               stdout);
            fputs("\
  -i --intensity=NUM     Set intensity of the digital rain (default: 1)\n\
  -l --limit=LIM         Limit framerate to LIM fps (default: 32)\n\
  -s --static            Run in static mode (no 3D images)\n\
  -v --version           Print version info\n\
  --allow-root           Allow to be run as root\n\n\
Report bugs to <" PACKAGE_BUGREPORT ">\n\
For complete documentation, run: info matrixgl\n",
               stdout);
            exit(EXIT_SUCCESS);
         case 'v':
            fputs("matrixgl version " VERSION "\n\
Copyright (C) 2003 Alexander Zolotov, Eugene Zolotov\n\
Copyright (C) 2008-2014 Vincent Launchbury\n\n\
License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n",
               stdout);
            exit(EXIT_SUCCESS);
      }
   }

   srand(time(NULL));

   /* Set up X Window stuff */
   dpy = XOpenDisplay(NULL);
   if(dpy == NULL) {
      fprintf(stderr, "Can't connect to X server\n");
      exit(EXIT_FAILURE);
   }
   swa.event_mask = KeyPressMask;
   if (wuse==FS) {
      swa.event_mask |= PointerMotionMask;
   }
   /* Bypass window manager for fullscreen */
   swa.override_redirect = (wuse==FS)?1:0;

   /* Use screen dimensions */
   y = DisplayHeight(dpy, DefaultScreen(dpy));
   x = DisplayWidth(dpy, DefaultScreen(dpy));

   /* Take height of preview win in xscreensaver */
   if (wuse==PREVIEW) {
      XGetWindowAttributes(dpy, wid, &gwa);
      x = gwa.width;
      y = gwa.height;
   /* Or scale down for windowed mode */
   } else if (wuse==WINDOWED) {
      x/=2;
      y/=2;
   }

   /* Create window, and map it */
   win = XCreateWindow(dpy, DefaultRootWindow(dpy),
       0, 0, x, y, 0, 0, InputOutput,
       CopyFromParent, CWEventMask | CWOverrideRedirect, &swa);
   XMapRaised(dpy, win);

   /* Handle fullscreen.. */
   if (wuse==FS) {
      Pixmap blank;
      Cursor cursor;
      XColor c;

      /* Since we bypassed the window manager,
       * we have to steal keyboard/mouse control */
      XGrabKeyboard(dpy, win, 1, GrabModeSync, GrabModeAsync, CurrentTime);
      XGrabPointer(dpy, win, 1, PointerMotionMask, GrabModeAsync,
         GrabModeAsync, None, None, CurrentTime);

      /* X has no function to hide the cursor,
       * so we have to create a blank one */
      blank = XCreatePixmapFromBitmapData(dpy, win,"\000", 1, 1, 0, 0, 1);
      cursor = XCreatePixmapCursor(dpy, blank, blank, &c, &c, 0, 0);
      XFreePixmap (dpy, blank);
      XDefineCursor(dpy, win, cursor);
   }
   XStoreName(dpy, win, "Matrixgl " VERSION);

   /* Set up glX */
   vi = glXChooseVisual(dpy, 0, att);
   glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
   glXMakeCurrent(dpy, win, glc);
#endif /* UNIX_MODE */

/* Allocations for dynamic width */
#ifdef WIN32_MODE
   text_x = ceil(text_y * ((float)glutGet(GLUT_SCREEN_WIDTH)
      / glutGet(GLUT_SCREEN_HEIGHT)));
#else /* UNIX_MODE */
   text_x = ceil(text_y * ((float)x/y));
#endif /* UNIX_MODE */

   /* Start at rand pic */
   pic_offset=(rtext_x*text_y)*(rand()%num_pics);

   /* Initializations */
   if (text_x & 1) text_x++;
   if (text_x < 90) text_x=90; /* Sanity check */
   speeds = tmalloc(text_x);
   glyphs = tmalloc(sizeof(struct glyph) * (text_x * text_y));
   for (i=0; i<text_x*text_y; i++) {
      glyphs[i].alpha = 253;
      glyphs[i].num   = rand()%60;
      glyphs[i].z     = 0;
   }

   /* Init the light tables */
   for (i=0; i<500;i++) {
      make_change();
      scroll();
   }

#ifdef UNIX_MODE
   ourInit();
   cbResizeScene(x,y);
   while(1) {
      /* FPS and frame limiting */
      if (!frames) time(&last);
      else if (time(NULL)-last >= fpoll) {
         fps = frames/(time(NULL)-last);
         if (showfps) {
            printf("FPS:%5.1f (%ld) (%3d frames in %2d seconds)\n",
               fps, sleeper, frames, (int)(time(NULL)-last));
            fflush(stdout); /* Don't buffer fps stats (do not remove!) */
         }
         frames=-1;

         /* Limit framerate */
         if (fps > maxfps) {
            /* Framerate too fast. Calculate how many microseconds per-frame
             * too fast it's going, and add that to the per-frame sleeper */
            sleeper += 1000000 / maxfps - 1000000 / fps;
            if (sleeper > 1000000) sleeper = 1000000;
         } else {
            /* Framerate too slow. Calculate how many microseconds per-frame
             * too slow it's going, and minus that from the per-frame sleeper */
            sleeper -= clamp(1000000 / fps - 1000000 / maxfps, 0, sleeper);
         }
      }

      /* Check events */
      if (XCheckWindowEvent(dpy, win, KeyPressMask, &xev)) {
         cbKeyPressed(get_ascii_keycode(&xev), 0, 0);
      }
      if (XCheckWindowEvent(dpy, win, PointerMotionMask, &xev)) {
         MouseFunc(xev.xmotion.x, xev.xmotion.y);
      }

      /* Update viewport on window size change */
      XGetWindowAttributes(dpy, win, &gwa);
      glViewport(0, 0, gwa.width, gwa.height);

      /* Render frame */
      cbRenderScene();
      usleep(sleeper);
      glFinish();
      frames++; /* Finished drawing a frame */
   }
#else /* WIN32_MODE */
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   glutEnterGameMode();
   /* Register the callback functions */
   glutDisplayFunc(&cbRenderScene);
   glutIdleFunc(&cbRenderScene);
   glutReshapeFunc(&cbResizeScene);
   glutKeyboardFunc(&cbKeyPressed);
   glutPassiveMotionFunc(MouseFunc);
   ourInit();
   glutMainLoop(); /* Pass off control to OpenGL. */
#endif /* WIN32_MODE */
   return 0;
}

/* Draw character #num on the screen. */
static void draw_char(long num, float light, float x, float y, float z)
{
   /* The font texture is a grid of 10x6 characters. Texture coords are
    * normalized to [0,1] and (s,t) is the top-left texel of the character
    * #num. The division by 7 ensures that rows don't evenly line up. */
   float s = (float)(num%10) / 10;
   float t = 1 - (float)(num/10)/7;

   glColor4f(0.9, 0.4, 0.3, light/255);

   glTexCoord2f(s,     t);       glVertex3f(x,   y,   z);
   glTexCoord2f(s+0.1, t);       glVertex3f(x+1, y,   z);
   glTexCoord2f(s+0.1, t-0.166); glVertex3f(x+1, y-1, z);
   glTexCoord2f(s,     t-0.166); glVertex3f(x,   y-1, z);
}

/* Draw flare around white characters */
static void draw_flare(float x,float y,float z)
{
   glColor4f(0.9,0.4,0.3,.75);

   glTexCoord2f(0,    0);    glVertex3f(x-1, y+1, z);
   glTexCoord2f(0.75, 0);    glVertex3f(x+2, y+1, z);
   glTexCoord2f(0.75, 0.75); glVertex3f(x+2, y-2, z);
   glTexCoord2f(0,    0.75); glVertex3f(x-1, y-2, z);
}

/* Draw green text on screen */
static void draw_text1(void)
{
   int x, y, i=0, b=0;

   /* For each character, from top-left to bottom-right of screen */
   for (y=text_y/2; y>-text_y/2; y--) {
      for (x=-text_x/2; x<text_x/2; x++, i++) {
         int light = clamp(glyphs[i].alpha + pic_fade, 0, 255);
         int depth = 0;

         /* If the coordinate is in the range of the 3D picture, set depth */
         if (x >= -rtext_x/2 && x<rtext_x/2) {
            depth=clamp(pic[b+pic_offset]+(pic_fade-255), 0, 255);
            b++;

            /* Make far-back pixels darker */
            light-=depth; if (light<0) light=0;
         }

         glyphs[i].z = (float)(255-depth)/32; /* Map depth (0-255) to coord */
         draw_char(glyphs[i].num, light, x, y, glyphs[i].z);
      }
   }
}

/* Draw white characters and flares for each column */
static void draw_text2(int mode)
{
   int x, y, i=0;

   /* For each character from top-left to bottom-right of screen,
    * excluding the bottom-most row. */
   for (y=text_y/2-1; y>-text_y/2; y--) {
      for (x=-text_x/2; x<text_x/2; x++, i++) {
         /* Highlight visible characters directly above a black stream */
         if (glyphs[i].alpha && !glyphs[i+text_x].alpha) {
            if (!mode) {
               /* White character */
               draw_char(glyphs[i].num, 127.5, x, y, glyphs[i].z);
            } else {
               draw_flare(x, y, glyphs[i].z);
            }
         }
      }
   }
}

static void scroll(void)
{
   int i, speed, col=0;
   static char odd=0;

   /* Only scroll the slowest columns every second scroll() */
   odd = !odd;

   /* Scroll columns */
   for (speed=odd; speed <= 2; speed++) {
      for (i=text_x*text_y-1; i>=text_x; i--) {
         if (speeds[col] >= speed) glyphs[i].alpha=glyphs[i-text_x].alpha;
         if (++col >=text_x) col=0;
      }
   }

   /* Clear top line in light table */
   for(i=0; i<text_x; i++) glyphs[i].alpha=253;

   /* Make black bugs in top line */
   for(col=0,i=(text_x*text_y)/2; i<(text_x*text_y); i++) {
      if (glyphs[i].alpha==255) glyphs[col].alpha=glyphs[col+text_x].alpha>>1;
      if (++col >=text_x) col=0;
   }

   /* 3D picture transitions */
   if (!classic) {
      timer++;

      if (timer < 250) {
         /* Fading in */
         if ((pic_fade+=3)>255) pic_fade=255;
      } else {
         /* Fading out */
         if ((pic_fade-=3) < 0) pic_fade = 0;

         /* Transition credits from knoppix.ru -> doublecreations */
         if (pic_offset==(num_pics+1)*(rtext_x*text_y)) {
            pic_offset+=rtext_x*text_y;
            timer=120;
         }
      }

      /* Go to next picture */
      if (timer>400) {
         cbKeyPressed('n', 0, 0);
      }
   }
}

static void make_change(void)
{
   int i;
   for (i=0; i<rain_intensity; i++) {
      /* Random character changes */
      int r=rand() % (text_x * text_y);
      glyphs[r].num = rand()%60;

      /* White nodes (1 in 5 chance of doing anything) */
      r=rand() % (text_x * 5);
      if (r<text_x && glyphs[r].alpha!=0) glyphs[r].alpha=255;
   }
}

unix_static void cbRenderScene(void)
{
   glMatrixMode(GL_MODELVIEW);
   glTranslatef(0.0f,0.0f,-89.0F);
   glClear(GL_COLOR_BUFFER_BIT);

   glBindTexture(GL_TEXTURE_2D,1);
   glBegin(GL_QUADS);
      draw_text1();
   glEnd();

   glBindTexture(GL_TEXTURE_2D,2);
   glBegin(GL_QUADS);
      draw_text2(0);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,3);
   glBegin(GL_QUADS);
      draw_text2(1);
   glEnd();

   if (!paused) {
      make_change();
      scroll();
   }

   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);

#ifdef WIN32_MODE
   glutSwapBuffers();
#else /* UNIX_MODE */
   glXSwapBuffers(dpy, win);
#endif /* UNIX_MODE */
}

unix_static void MouseFunc(int x, int y)
{
   /* A work around for really buggy mouse drivers (on Windows) that:
    *  a) Randomly generate a mouse event when the coords haven't changed and
    *  b) Even worse, generate a mouse event with different coords, when the
    *  mouse hasn't moved.
    */
   static short xx=0,yy=0, t=0;
   if (!t) {t++;xx=x;yy=y;}
   else if (abs(xx-x)>8||abs(yy-y)>8)exit(EXIT_SUCCESS);
}

unix_static void cbKeyPressed(unsigned char key, unused int x, unused int y)
{
   if (!key) return;
   switch (key) {
      case 'q': case 'Q': case 27: /* 27==ESC */
#ifdef UNIX_MODE
         glXMakeCurrent(dpy, None, NULL);
         glXDestroyContext(dpy, glc);
         XDestroyWindow(dpy, win);
         XCloseDisplay(dpy);
         XFree(vi);
#endif /* UNIX_MODE */
         free(speeds);
         free(glyphs);
         exit(EXIT_SUCCESS);
      case 'n': /* n - Next picture. */
         if (classic || paused) break;
         pic_offset+=rtext_x*text_y;
         pic_offset%=(rtext_x*text_y)*num_pics;
         timer=0;
         break;
      case 'c': /* Show "Credits" */
         classic=0;
         timer = 0;
         pic_offset=(num_pics+1)*(rtext_x*text_y);
         timer=70;
         break;
      case 's': /* Stop 3D Images, classic style. */
         pic_fade=0;
         classic=!classic;
         break;
      case 'p': /* Pause */
         paused=!paused;
         break;
   }
}

unix_static void cbResizeScene(int width, int height)
{
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);
   glMatrixMode(GL_MODELVIEW);
}

static void ourInit(void)
{
   unsigned char flare[16]={0,0,0,0,0,180,0}; /* Node flare texture */
   int i;

   /* Set up column speeds */
   for(i=0;i<text_x;i++) {
      speeds[i]=rand()&1;
      /* If the column on the left is the same speed, go faster */
      if (i && speeds[i]==speeds[i-1]) speeds[i]=2;
   }

   /* Create texture mipmaps */
   glPixelTransferf(GL_GREEN_SCALE, 1.15f); /* Give green a bit of a boost */
   glBindTexture(GL_TEXTURE_2D,1);
   gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA8, 512, 256, color,
      GL_UNSIGNED_BYTE, (void *)font);
   glBindTexture(GL_TEXTURE_2D,2);
   gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA8, 512, 256, GL_LUMINANCE,
      GL_UNSIGNED_BYTE, (void *)font);
   glBindTexture(GL_TEXTURE_2D,3);
   gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA8, 4, 4, GL_LUMINANCE,
      GL_UNSIGNED_BYTE, (void *)flare);

   /* Use GL_DECAL so texels aren't multipied by the colors on the screen, e.g
    * the black set by calls to glClear(GL_COLOR_BUFFER_BIT) */
   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

   glEnable(GL_BLEND);
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_LIGHTING);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

/* malloc w/ error checking  */
static void *tmalloc(size_t n)
{
   void *p = malloc(n);
   if (!p && n != 0)
      exit(EXIT_FAILURE);
   return p;
}

#ifdef UNIX_MODE
static char get_ascii_keycode(XEvent *ev)
{
   char key;
   int count = XLookupString((XKeyEvent *)ev, &key, 1, NULL, NULL);

   /* We only care about single-character key presses */
   if (count == 1) {
      return key;
   }

   return 0;
}
#endif /* UNIX_MODE */

