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

#define TESTING  /* Some functions used for testing */
/*#define WIN32_MODE*/

/* Includes */
#ifdef WIN32_MODE 
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <unistd.h>
#endif

#include <stdio.h>   /* Always a good idea. */
#include <math.h> 
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>   /* OpenGL itself. */
#include <GL/glu.h>  /* GLU support library. */
#include <GL/glut.h> /* GLUT support library. */

#include "matrix.h"  /* Prototypes */
#include "matrix1.h" /* Font data */
#include "matrix2.h" /* Image data */

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
#else
int main(int argc,char **argv) 
{
   char *par;
   char *gms = tmalloc(35); /* Game Mode String */
   int i=0,a=0,s=0;
   if(argc>1) {
      par=argv[1];
      if(!strcmp(par,"-install")) {
         fprintf(stderr, "Error: Feature not implemented");
         exit(1);
         if (getuid()!=0) {
            fprintf(stderr, "Error: Must run -install as root\n");
            exit(1);
         }
         system("cp dcmatrix_gl /usr/bin/dcmatrix_gl.kss");
         system("cp matrixgl.xpm /usr/share/pixmaps/matrixgl.xpm");
         system("mkdir -p /usr/share/applnk/System/ScreenSavers/"); /* Make sure directory exists */
         system("cp matrix.desktop /usr/share/applnk/System/ScreenSavers/DCMatrixgl.desktop");
         printf("Install complete\n");
         exit(0);
      } else if(!strcmp(par,"-remove")) {
         fprintf(stderr, "Error: Feature not implemented");
         exit(1);
         if (getuid()!=0) {
            fprintf(stderr, "Error: Must run -remove as root\n");
            exit(1);
         }
         system("rm -f /usr/bin/dcmatrix_gl.kss");
         system("rm -f /usr/share/pixmaps/matrixgl.xpm");
         system("rm -f /usr/share/applnk/System/ScreenSavers/DCMatrixgl.desktop");
         printf("Uninstall complete\n");
         exit(0);
      } else if(!strcmp(par,"--help")) {
         printf("matrixgl Help\n");
         printf("----------------\n");
         printf("-install  -> Install into screensaver directory\n");
         printf("-remove   -> Remove installed files\n");
         printf("--help    -> Show this help\n");
         printf("--version -> Show version info\n");
         printf("In-Screensaver Keys\n");
         printf(" >'c' - View Credits\n");
         printf(" >'s' - Toggle 3D mode");
         printf(" >'n' - Next Image\n");
         printf(" >'p' - Pause Screensaver\n");
         printf(" >'q' - Quit\n");
         exit(0);
      } else if(!strcmp(par,"--version")) {
         printf("matrixgl Version 1.2B.\n");
         printf(" Based on matrixgl by knoppix.ru\n");
         printf(" Modified by Vincent Launchbury\n");
         exit(0);
      }
   } else {
      printf("matrixgl: Use --help for help and keys\n");
   }   
#endif

#ifdef WIN32_MODE
   glutInit(&argc, &lpCmd);
#else
   glutInit(&argc, argv);
#endif

   /* Allocations for dynamic width */
   text_x = ceil(70 * ((float)glutGet(GLUT_SCREEN_WIDTH)/glutGet(GLUT_SCREEN_HEIGHT)));
   if (text_x % 2 == 1) text_x++;
   if (text_x < 108) text_x=108; /* Sanity check? That'd be a crazy monitor :P */
   speed = tmalloc(text_x);
   text= tmalloc(text_x*(text_y+1));
   text_light = tmalloc(text_x*(text_y+1));
   bump_pic = tmalloc(sizeof(float) * (text_x*(text_y+1)));
   memset(text_light, 253, text_x*(text_y+1));
   /* End allocations */

   /* Init the light tables */
   mode2=0;
   for (i=0; i<2000;i++) {
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

   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
#ifndef WIN32_MODE
   /* Force linux to use same resolution as desktop */
   sprintf(gms,"%dx%d:24@85", glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
   glutGameModeString(gms);
#endif
   glutEnterGameMode();

   /* Register the callback functions */
   glutDisplayFunc(&cbRenderScene);
   glutIdleFunc(&cbRenderScene);
   glutReshapeFunc(&cbResizeScene);
   glutKeyboardFunc(&cbKeyPressed);
   glutPassiveMotionFunc(MouseFunc);   
   ourInit();
   glutMainLoop(); /* Pass off control to OpenGL. */
   return 0;
}


/* Set Up initial column speeds and character mappings */
void make_text(void)
{
   long a;
   srand(10);
   for(a=0;a<(text_x*text_y);a++) text[a]=rand()&63;
   for(a=0;a<text_x;a++) {
      speed[a]=rand()&1;
      if (!a && speed[a]==speed[a-1]) speed[a]=2; /* Collisions become speed 3 */
   }
}


/* Draw character #num on the screen. */
void draw_char(int mode, long num, float light, float x, float y, float z)
{
   float tx,ty;
   long num2,num3;
   if (mode==1) light/=255;
   num2=num/10;
   num3=num-(num2*10);
   ty=(float)num2/7;
   tx=(float)num3/10;
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
   if(!paused && pic_mode==1 && ++pic_fade>255) pic_fade=255;
   if(!paused && pic_mode==2 && --pic_fade<0) pic_fade=0;
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

/* Draw white characters for each column */
void draw_text2(void)
{
   float x,y;
   long p=0;
   for(y=_text_y;y>-_text_y;y--){
      for(x=-_text_x;x<_text_x;x++){
         if(text_light[p]>128 && text_light[p+text_x]<10) 
            draw_char(2, text[p]+1,0.5,x,y, ((x>-_rtext_x-1 && x<_rtext_x )?bump_pic[p]:8));
         p++;
      }
   }
}

/* Draw flares for each column */
void draw_text3(void)
{
   float x,y;
   long p=0;
   for (y=_text_y;y>-_text_y;y--) {
      for (x=-_text_x;x<_text_x;x++) {
         if(text_light[p]>128 && text_light[p+text_x]<10) 
            draw_flare(x,y, ((x>-_rtext_x-1 && x<_rtext_x )?bump_pic[p]:8));
         p++;
      }
   }
}

void scroll(int mode)
{
   static char odd=0;
   unsigned int a, s=0;
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
      if (timer > 158 && pic_offset==(num_pics+1)*(rtext_x*text_y)) {
         pic_offset+=rtext_x*text_y; /* Go from 'knoppix.ru' -> 'Double Creations' */
         timer=100;pic_mode=1; /* Make DC dissapear quickly */
      }
      if(timer>280) {
         timer=-1;  /* back to start */
         pic_offset=(rand()&(num_pics))*(rtext_x*text_y); /* Random 3d image (not credits) */
      }
      timer++;

      for(a=(text_x*text_y)+text_x-1;a>text_x-1;a--) {
         text_light[a]=text_light[a-text_x]; 
      }

      for(a=1;a<text_x;a++) text_light[a]=253; /* clear top line (in light table) */

      for(s=0,a=(text_x*text_y)/2; a<(text_x*text_y); a++){
         if(text_light[a]==255) text_light[s]=text_light[s+text_x]>>1; /*make black bugs in top line */
         s++;if(s>=text_x) s=0;
      }
   }
   odd =!odd;
   if(!mode) glutTimerFunc(60,scroll,mode2);
}


void make_change(void)
{
   unsigned int r=rand()&0xFFFF;
   if (paused) return;
   r>>=3;  
   if(r<(text_x*text_y)) text[r]+=133; /* random character changes) */
   r=rand()&0xFFFF;r>>=7;
   if(r<text_x && text_light[r]!=0) text_light[r]=255; /* white nodes */

   if(mode2) scroll(mode2);
}


void cbRenderScene(void)
{  
   glBindTexture(GL_TEXTURE_2D,1);
   glEnable(GL_BLEND);
   glEnable(GL_TEXTURE_2D);

   glDisable(GL_LIGHTING);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
   glDisable(GL_DEPTH_TEST); 
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity(); 
   glTranslatef(0.0f,0.0f,Z_Off);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glBegin(GL_QUADS); 
   draw_text1();
   glEnd();

   glBindTexture(GL_TEXTURE_2D,2);
   glBegin(GL_QUADS);
   draw_text2();
   glEnd();

   glBindTexture(GL_TEXTURE_2D,3);
   glBegin(GL_QUADS);
   draw_text3();
   glEnd();

   make_change();
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glutSwapBuffers();
} 


void MouseFunc(int x, int y)
{
#ifdef WIN32_MODE
   /* 
    * WIN seems to randomly call passive mouse func when no movement
    * takes place, so we gotta check if coords have infact changed.
    */
   static short xx=0,yy=0, t=0;
   if (!t) {t++;xx=x;yy=y;}
   else if (xx!=x||yy!=y)exit(0);
#else
   static int c=0;
   if(c++>=1) exit(0);
#endif
}


void cbKeyPressed(unsigned char key, int x, int y)
{
   switch (key) {
      case 113: case 81: case 27: /* q,ESC */
         exit(0);
      case 'n': /* n - Next picture. */
         if (classic) break;
         pic_offset+=rtext_x*text_y;
         pic_mode=1;
         timer=10;
         if(pic_offset>(rtext_x*text_y*(num_pics))) pic_offset=0; /* Don't show credits */
         break;
      case 'c': /* Show "Credits" */
         classic=0;
         pic_offset=(num_pics+1)*(rtext_x*text_y);
         pic_mode=1;
         timer=110;
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
         if (!paused) glutTimerFunc(60,scroll,mode2);
         break;
#ifdef TESTING
      case '=':
         Z_Off+=5.0f;
         break;
      case '-':
         Z_Off-=5.0f;
         break;
#endif /* TESTING */
   }
}


void ourBuildTextures(void)
{
   glPixelTransferf(GL_GREEN_SCALE, 1.15f); /* Give green a bit of a boost */
   glBindTexture(GL_TEXTURE_2D,1);
   gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA8, 512, 256, GL_GREEN, GL_UNSIGNED_BYTE, (void *)font);
   glBindTexture(GL_TEXTURE_2D,2);
   gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA8, 512, 256, GL_LUMINANCE,GL_UNSIGNED_BYTE, (void *)font);
   glBindTexture(GL_TEXTURE_2D,3);
   gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA8, 4, 4, GL_LUMINANCE,GL_UNSIGNED_BYTE, (void *)flare);

   /* Some pretty standard settings for wrapping and filtering. */
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
}


void cbResizeScene(int Width, int Height)
{
   glViewport(0, 0, Width, Height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
   glMatrixMode(GL_MODELVIEW);
}


void ourInit(void) 
{
   make_text();
   ourBuildTextures();   
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClearDepth(1.0);
   glDepthFunc(GL_LESS); 
   glShadeModel(GL_SMOOTH);

   /* A handy trick -- have surface material mirror the color. */
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   pic_offset=(rtext_x*text_y)*(rand()&num_pics);
}

/* malloc w/ error checking  */
void *tmalloc(size_t n)
{
   void *p = malloc(n);
   if (!p && n != 0)
      exit(EXIT_FAILURE);
   return p;
}

