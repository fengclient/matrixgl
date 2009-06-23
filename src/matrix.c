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

#ifndef NIX_MODE
   #define WIN32_MODE
#endif

/* Includes */
#ifdef WIN32_MODE 
   #include <windows.h>
#else /* NIX_MODE */
   #include <X11/Xlib.h>
   #define __USE_XOPEN_EXTENDED
   #include <unistd.h>
   #include <getopt.h>
   #include <time.h>
#endif /* NIX_MODE */

#include <stdio.h>   /* Always a good idea. */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
   #include <OpenGL/gl.h>
   #include <OpenGL/glu.h>
#else 
   #include <GL/gl.h>   /* OpenGL itself. */
   #include <GL/glu.h>  /* GLU support library. */
#endif
#ifdef NIX_MODE
   #include <GL/glx.h>
   #include "vroot.h"
#else /* WIN32_MODE */
   #include <GL/glut.h> /* GLUT support library. */
#endif /* WIN32_MODE */

#include "matrix.h"     /* Prototypes */
#include "matrix1.h"    /* Font data */
#include "matrix2.h"    /* Image data */
#include "../config.h"  /* Autoconf stuff */

/* Global Variables */
float Z_Off = -89.0f;
unsigned char flare[16]={0,0,0,0,0,180,0}; /* Node flare texture */
#define rtext_x 90
#define _rtext_x rtext_x/2
#define text_y 70
#define _text_y text_y/2
#define _text_x text_x/2
int text_x;
unsigned char *speed;      /* Speed of each column (0-2) */
unsigned char *text;       /* Random Characters (0-62) */
unsigned char *text_light; /* Alpha 255-white, 254-none */
float *bump_pic;           /* Z-Offset (calculated from pic from matrix2.h) */

int pic_offset;            /* Which image to show */
long timer=40;             /* Controls pic fade in/out */
int mode2=1;               /* Initial speed boost (inits the light table) */
int pic_mode=1;            /* 1-fade in; 2-fade out (controls pic_fade) */
int pic_fade=0;            /* Makes all chars lighter/darker */
int classic=0;             /* classic mode (no 3d) */
int paused=0;
int num_pics=9 -1;         /* # 3d images (0 indexed) */
GLenum color=GL_GREEN;     /* Color of text */


#ifdef NIX_MODE
Display                 *dpy;
Window                  root;
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
time_t last;
float usefps=0.0;
unsigned long sleeper = 0;
float fps=0;
int frames=0;
int fpoll=2;


#ifdef WIN32_MODE
/* 
 * int main(int argc, char **argv)
 */
int __stdcall WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR lpCmd,int nShow)
{
   int argc=1;
   int i=0,a=0,s=0;
   if(lpCmd[1]=='p' || lpCmd[1]=='c') exit(0);
   glutInit(&argc, &lpCmd);
   srand(GetTickCount());
   pic_offset=(rtext_x*text_y)*(rand()%num_pics); /* Start at rand pic */
#else /* NIX_MODE */
int main(int argc,char **argv) 
{
   int i=0,a=0,s=0;
   int opt;
   short ierror=0;    /* Install Error */
   short allowroot=0; /* If they insist... */
   int wuse=2;        /* Window method 2-windowed, 1-fs, 0-preview */
   Window wid=0;      /* ID of window, used to grab preview size */

   static struct option long_opts[] =
   {
      {"static",    no_argument,       0, 's'},
      {"credits",   no_argument,       0, 'c'},
      {"color",     required_argument, 0, 'C'},
      {"install",   no_argument,       0, 'i'},
      {"remove",    no_argument,       0, 'u'},
      {"help",      no_argument,       0, 'h'},
      {"version",   no_argument,       0, 'v'},
      {"window-id", required_argument, 0, 'W'},
      {"root",      no_argument,       0, 'F'},
      {"fs",        no_argument,       0, 'F'},
      {"fullscreen",no_argument,       0, 'F'},
      {"allow-root",no_argument,       0, 'Z'},
      {"fps",       required_argument, 0, 'f'},
      {0, 0, 0, 0}
   };
   int opti = 0;
   pic_offset=(rtext_x*text_y)*(rand()%num_pics); /* Start at rand pic */
   while ((opt = getopt_long_only(argc, argv, "sciuhvf:C:FW:Z", long_opts, &opti))) {
      if (opt == EOF) break;
      switch (opt) {
         case 'Z':
            if (!opti) break; /* Short opt not allowed */
            allowroot=1;
            break;
         case 'W':
            wuse=0;
            wid=htoi(optarg);
            break;
         case 'F':
            wuse=1;
            break;
         case 's':
            pic_fade=0;
            pic_offset=0;
            pic_mode=!pic_mode;
            classic=1;
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
         case 'f':
            usefps=atof(optarg);
            /*fpoll=clamp(atoi(optarg), 1, 20);*/
            break;
         case 'i':
            fputs("\
Error: This install method is deprecated.\n\
Please use '$make install' in the source \
directory instead.\n",
               stderr);
            exit(EXIT_FAILURE);
         case 'u':
            fputs("\
Error: This uninstall method is deprecated.\n\
Please use '$make uninstall' in the source \
directory instead.\n",
               stderr);
            exit(EXIT_FAILURE);
         case 'h':
            fputs("Usage: matrixgl [OPTIONS]...\n\
3D Matix Screensaver based on The Matrix Reloaded\n\
 -C --color=COL         Set color to COL (must be green, red or blue)\n\
 -c --credits           Show the credits on startup\n\
 -F --fs --fullscreen   Run in fullscreen window\n\
 -f --fps=FPS           Print fps stats and limit to FPS fps.\n\
 -h --help              Show the help screen\n\
 -i --install           Install to xscreensaver (deprecated)\n",
               stdout);
            fputs("\
 -u --remove            Remove from xscreensaver\n\
 -s --static            Run in static mode (no 3D images)\n\
 -v --version           Print version info\n\
 --allow-root           Allow to be run as root\n",
               stdout);
            fputs("Long options may be passed with a single dash.\n\n\
In-Screensaver Keys:\n\
 'c' - View creidts\n\
 's' - Toggle static mode (3D)\n\
 'n' - Next image\n\
 'p' - Pause screensaver\n\
 'q' - Quit\n\n\
Report bugs to <" PACKAGE_BUGREPORT ">\n\
Home Page: http://www.sourceforge.net/projects/matrixgl/\n",
             stdout);
            exit(0);
         case 'v':
            fputs("matrixgl version " VERSION "\n\
Based on matrixgl 1.0 (see http://knoppix.ru/matrixgl.shtml) \n\
Written By:  Alexander Zolotov  <nightradio@gmail.com> 2003.\n\
      and :  Eugene Zolotov     <sentinel@knoppix.ru> 2003.\n\
Modified By: Vincent Launchbury <vincent@doublecreations.com> 2008,2009.\n\n",
               stdout);
            fputs("Send bug reports to <" PACKAGE_BUGREPORT ">\n\
To assist us best, please run the script ./gen-bug-report.sh in the \
source directory and follow the instructions, before sending your \
bug report.\n",
               stdout);
            exit(0);
      }
   }
   /* Don't run as root, unless specifically requested */
   if (!allowroot && getuid()==0) {
      fprintf(stderr, "Error: You probably don't want to run this as "
         "root.\n (Use --allow-root if you really want to..)\n");
      exit(0);
   }
#endif /* NIX_MODE */

#ifdef WIN32_MODE
   glutInit(&argc, &lpCmd);
#else /* NIX_MODE */
   srand(time(NULL));
#endif


#ifdef NIX_MODE
   /* Set up X Window */
   dpy = XOpenDisplay(NULL);
   if(dpy == NULL) {
      fprintf(stderr, "Can't connect to X server\n");
      exit(0); 
   }
   root = DefaultRootWindow(dpy);
   vi = glXChooseVisual(dpy, 0, att);
   swa.colormap = XCreateColormap(dpy, root, vi->visual, AllocNone);
   swa.event_mask = KeyPressMask;
   y = DisplayHeight(dpy, DefaultScreen(dpy));
   x = DisplayWidth(dpy, DefaultScreen(dpy));
   /* Take height of preview win in xscreensaver */
   if (!wuse) {
      XGetWindowAttributes(dpy, wid, &gwa);
      x = gwa.width;
      y = gwa.height;
   }
   if (wuse==2) {
      x/=2;
      y/=2;
   }
   win = XCreateWindow(dpy, root, 0, 0, x, y, 0, vi->depth, InputOutput, 
       vi->visual, CWColormap | CWEventMask, &swa);
   XMapWindow(dpy, win);
   XStoreName(dpy, win, "Matrixgl Screensaver");
   glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
   glXMakeCurrent(dpy, win, glc);
#endif /* NIX_MODE */

/* Allocations for dynamic width */
#ifdef WIN32_MODE
   text_x = ceil(70 * ((float)glutGet(GLUT_SCREEN_WIDTH) 
      / glutGet(GLUT_SCREEN_HEIGHT)));
#else /* NIX_MODE */
   text_x = ceil(70 * ((float)x/y));
#endif /* NIX_MODE */

   /* Initializations */
   if (text_x % 2 == 1) text_x++;
   if (text_x < 90) text_x=90; /* Sanity check */
   speed = tmalloc(text_x);
   text= tmalloc(text_x*(text_y+1));
   text_light = tmalloc(text_x*(text_y+1));
   bump_pic = tmalloc(sizeof(float) * (text_x*(text_y+1)));
   memset(text_light, 253, text_x*(text_y+1));
   /* End allocations */

   /* Init the light tables */
   mode2=0;
   for (i=0; i<500;i++) {
      make_change();

      for(a=(text_x*text_y)+text_x-1;a>text_x;a--)
         text_light[a]=text_light[a-text_x]; 

      for(a=1;a<text_x;a++) text_light[a]=253; /* Clear top line */

      for(s=0,a=(text_x*text_y)/2; a<(text_x*text_y); a++){
         if(text_light[a]==255) text_light[s]=text_light[s+text_x]>>1;
         s++;if(s>=text_x) s=0;
      }
   }
   mode2=1;

#ifdef NIX_MODE
   ourInit();
   cbResizeScene(x,y);
   while(1) {
      /* FPS and frame limiting */
      if (!frames) time(&last);
      else if (time(NULL)-last >= fpoll) {
         fps = frames/(time(NULL)-last);
         if (usefps) {
            printf("FPS:%5.1f (%ld) (%3d frames in %2d seconds) - goal: %f\n", 
		   fps, sleeper, frames, (int)(time(NULL)-last), usefps);

	    /* Limit framerate */
	    if (fps > usefps) {
	      sleeper+=3000;
	    } else if (sleeper >= 3000) {
	      sleeper-=1000;
	    }
	    if (sleeper > 1000000) sleeper=100000;
	    printf("sleeper %ld\n", sleeper);
	    fflush(stdout); /* Don't buffer fps stats (do not remove!) */
	 }
	 frames=-1;
      }


      /* Check events */
      if(XCheckWindowEvent(dpy, win, KeyPressMask, &xev)) {
         cbKeyPressed(get_ascii_keycode(&xev),0,0);
      }
      XGetWindowAttributes(dpy, win, &gwa);
      glViewport(0, 0, gwa.width, gwa.height);
      /* Render frame */
      cbRenderScene();
      usleep(sleeper);
      glFinish();
      scroll(0);
      if (usefps) frames++;
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
void draw_char(int mode, long num, float light, float x, float y, float z)
{
   float tx,ty;
   long num2;
   if (mode==1) light/=255;
   num2=num/10;
   ty=(float)num2/7;
   tx=(float)num/10;
   glColor4f(0.9,0.4,0.3,light);

   glTexCoord2f(tx, ty); glVertex3f( x, y, z); 
   glTexCoord2f(tx+0.1, ty); glVertex3f( x+1, y, z);
   glTexCoord2f(tx+0.1, ty+0.166); glVertex3f( x+1, y-1, z);
   glTexCoord2f(tx, ty+0.166); glVertex3f( x, y-1, z);
}


/* Draw flare around white characters */
void draw_flare(float x,float y,float z)
{
   glColor4f(0.9,0.4,0.3,.75);

   glTexCoord2f(0, 0); glVertex3f( x-1, y+1, z); 
   glTexCoord2f(0.75, 0); glVertex3f( x+2, y+1, z);
   glTexCoord2f(0.75, 0.75); glVertex3f( x+2, y-2, z);
   glTexCoord2f(0, 0.75); glVertex3f( x-1, y-2, z);
}

/* Draw green text on screen */
void draw_text1(void)
{
   float x,y;
   long p=0,b=0;
   int c,c_pic;
   if(!paused && pic_mode==1 && (pic_fade+=3)>255) pic_fade=255;
   if(!paused && pic_mode==2 && (pic_fade-=3)<0) pic_fade=0;
   for(y=_text_y;y>-_text_y;y--){
      for(x=-_text_x;x<_text_x;x++){
         if (x>-_rtext_x-1 && x<_rtext_x ) {
            /* Main text */
            c=text_light[p]-(text[p]>>1);
            c+=pic_fade;if(c>255) c=255; 
            /* 3D Image offsets */
            c_pic=pic[b+pic_offset]-(255-pic_fade); 
            if(c_pic<0) {c_pic=0;} c-=c_pic;if(c<0) {c=0;}

            if(!paused)bump_pic[p]=(float)(255-c_pic)/32;
            if(c>10 && text[p]) draw_char(1, text[p]+1,c,x,y, bump_pic[p]); 
            b++;
         } else {
            c=text_light[p]-(text[p]>>1);c+=pic_fade;if(c>255) c=255;
            if (c>10 && text[p]) draw_char(1, text[p]+1, c,x,y,8);
         }
         p++;
      }
   }
}

/* Draw white characters and flares for each column */
void draw_text2(int mode)
{
   float x,y;
   long p=0;
   for(y=_text_y;y>-_text_y;y--){
      for(x=-_text_x;x<_text_x;x++){
         if(text_light[p]>128 && text_light[p+text_x]<10) {
            /* White character */
            if (!mode) {
               draw_char(2, text[p]+1,0.5,x,y, 
                  ((x>-_rtext_x-1 && x<_rtext_x )?bump_pic[p]:8));
            /* Flare */
            } else {
               draw_flare(x,y, 
                  ((x>-_rtext_x-1 && x<_rtext_x )?bump_pic[p]:8));
            }
         }

         p++;
      }
   }
}

void scroll(int mode)
{
   static char odd=0;
   int a, s=0;
   if (paused)return;
   for(a=(text_x*text_y)+text_x-1;a>text_x-1;a--){
      if(speed[s]) text_light[a]=text_light[a-text_x]; 
      s++;if(s>=text_x) s=0;    
   }
   for(s=0,a=(text_x*text_y)+text_x-1;a>text_x-1;a--){
      if(speed[s]>1) text_light[a]=text_light[a-text_x]; 
      s++;if(s>=text_x) s=0;    
   }

   if (odd) {
      if(timer==0 && !classic)  pic_mode=1;  /* pic fade in */
      if(timer>10) {mode2=0;mode=0;} /* Initialization */
      if(timer>140 && timer<145 && !classic) pic_mode=2; /* pic fade out */
      if (timer > 140 && pic_offset==(num_pics+1)*(rtext_x*text_y)) {
         pic_offset+=rtext_x*text_y; /* Go from 'knoppix.ru' -> 'DC' */
         timer=70;pic_mode=1; /* Make DC dissapear quickly */
      }
      if(timer>220) {
         timer=-1;  /* back to start */
         pic_offset+=rtext_x*text_y; /* Next pic */
         if(pic_offset>(rtext_x*text_y*(num_pics))) pic_offset=0; 
      }
      timer++;

      for(a=(text_x*text_y)+text_x-1;a>text_x-1;a--) {
         text_light[a]=text_light[a-text_x]; 
      }

      /* Clear top line in light table */
      for(a=1;a<text_x;a++) text_light[a]=253;

      for(s=0,a=(text_x*text_y)/2; a<(text_x*text_y); a++){
         /* Make black bugs in top line */
         if(text_light[a]==255) text_light[s]=text_light[s+text_x]>>1;
         s++;if(s>=text_x) s=0;
      }
   }
   odd =!odd;
#ifdef WIN32_MODE
   if(!mode) glutTimerFunc(60,scroll,mode2);
#endif
}


void make_change(void)
{
   int r=rand()&0xFFFF;
   if (paused) return;
   r>>=3;  
   if(r<(text_x*text_y)) text[r]+=133; /* random character changes) */
   r=rand()&0xFFFF;r>>=7;
   if(r<text_x && text_light[r]!=0) text_light[r]=255; /* white nodes */

#ifdef WIN32_MODE
   if(mode2) scroll(mode2);
#endif
}


void cbRenderScene(void)
{  
   glBindTexture(GL_TEXTURE_2D,1);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, 
      GL_NEAREST_MIPMAP_LINEAR);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glMatrixMode(GL_MODELVIEW);
   glTranslatef(0.0f,0.0f,Z_Off);
   glClear(GL_COLOR_BUFFER_BIT);

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

   make_change();
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);

#ifdef WIN32_MODE
   glutSwapBuffers();
#else /* NIX_MODE */
   glXSwapBuffers(dpy, win); 
#endif /* NIX_MODE */
} 


void MouseFunc(int x, int y)
{
   static short xx=0,yy=0, t=0;
   if (!t) {t++;xx=x;yy=y;}
   else if (xx!=x||yy!=y)exit(0);
}


void cbKeyPressed(unsigned char key, int x, int y)
{
   switch (key) {
      case 'X':
         break; /* Do nothing */
      case 113: case 81: case 27: /* q,ESC */
#ifdef NIX_MODE
         glXMakeCurrent(dpy, None, NULL);
         glXDestroyContext(dpy, glc);
         XDestroyWindow(dpy, win);
         XCloseDisplay(dpy);
#endif /* NIX_MODE */
         exit(0); 
      case 'n': /* n - Next picture. */
         if (classic) break;
         pic_offset+=rtext_x*text_y;
         pic_mode=1;
         timer=10;
         if(pic_offset>(rtext_x*text_y*(num_pics))) pic_offset=0; 
         break;
      case 'c': /* Show "Credits" */
         classic=0;
         pic_offset=(num_pics+1)*(rtext_x*text_y);
         pic_mode=1;
         timer=70;
         break;
      case 's': /* Stop 3D Images, classic style. */
         if (!classic) {
            pic_fade=0;
            pic_offset=0;
         } 
         pic_mode=!pic_mode;
         classic=!classic;
         break;
      case 'p': /* Pause */
         paused=!paused;
#ifdef WIN32_MODE
         if (!paused) glutTimerFunc(60,scroll,mode2);
#endif /* WIN32_MODE */
         break;
   }
}




void cbResizeScene(int Width, int Height)
{
   glViewport(0, 0, Width, Height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,200.0f);
   glMatrixMode(GL_MODELVIEW);
}


void ourInit(void) 
{
   /* Set up column speeds and character mappings */
   long a;
   for(a=0;a<(text_x*text_y);a++) text[a]=rand()&63;
   for(a=0;a<text_x;a++) {
      speed[a]=rand()&1;
      if (!a && speed[a]==speed[a-1]) speed[a]=2; /* Collisions goto speed 3 */
   }

   /* Make Textures */
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

   /* Some pretty standard settings for wrapping and filtering. */
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glShadeModel(GL_SMOOTH);

   /* A handy trick -- have surface material mirror the color. */
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   glEnable(GL_BLEND);
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_LIGHTING);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
}

/* malloc w/ error checking  */
void *tmalloc(size_t n)
{
   void *p = malloc(n);
   if (!p && n != 0)
      exit(EXIT_FAILURE);
   return p;
}

#ifdef NIX_MODE
int htoi(char *str)
{
  int i,sum=0,d,sl;
  sl=strlen(str);

  if(str[0]!='0' || str[1]!='x') return -1;
  for(i=2;i<sl;i++){
    d=0;
    if(str[i]>='0' && str[i]<='9') d=(int)(str[i]-'0');
    if(str[i]>='A' && str[i]<='F') d=(int)(str[i]-'A'+10);
    if(str[i]>='a' && str[i]<='f') d=(int)(str[i]-'a'+10);
    sum+=d;
    sum=sum<<4;
  }

  return(sum>>4);
}

char get_ascii_keycode(XEvent *ev)
{
   char keys[256], *s;
   int count;
   KeySym k;

   if (ev) {
      count = XLookupString((XKeyEvent *)ev, keys, 256, &k,NULL);
      keys[count] = '\0';
      if (count == 0) {
         s = XKeysymToString(k);
         strcpy(keys, (s)?s:"");
      }
      if (count==1) return *keys;
   }
   return 'X';
}
#endif

