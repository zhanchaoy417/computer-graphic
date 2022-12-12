
/*
 * HW3 3D Objects modifty from ex8
 *
 *  Demonstrates how to draw objects in 3D.
 *
 *  Key bindings:
 *  m/M        Cycle through different sets of objects
 *  a          Toggle axes
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
// #pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glut.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif

int th=0;          //  Azimuth of view angle
int ph=0;          //  Elevation of view angle
double zh=0;       //  Rotation of teapot
int axes=1;        //  Display axes
//int mode=0;        //  What to display

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.14159265/180))
#define Sin(x) (sin((x)*3.14159265/180))
#define DEF_D 5
int L=1,M=2,N=4;   //  Brick size
int K=5;           //  Color
typedef struct {float r,g,b;} color_t;
color_t color[7] = {{1.0,1.0,1.0},{0.5,0.5,0.5},{0.5,0.0,0.0},{1.0,0.5,0.0},{1.0,1.0,0.0},{0.0,0.5,0.0},{0.0,0.0,0.5}};

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Check for OpenGL errors, throw error if not working
 */
void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}


/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}
static void sphere(double x,double y,double z,double r)
{
   const int d=15;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);
   glColor3f(1,1,0);

   //  Latitude bands
   for (int ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}
/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
//house component
static void cube(double a,double b,double c,double d,double e,double f)
{
   const double x=a;
   const double x2=d;
      const double y=b;
   const double y2=e;   
   const double z=c;
   const double z2=f;
   glBegin(GL_QUADS);
   glColor3f(0.3,0.22,0.2);
   glVertex3f(x,y, z2);
   glVertex3f(x2,y, z2);
   glVertex3f(x2,y2, z2);
   glVertex3f(x,y2, z2);
   //left
   glVertex3f(x,y2, z2);
   glVertex3f(x,y, z2);
   glVertex3f(x,y, z);
   glVertex3f(x,y2, z);
   //right
   glVertex3f(x2,y2, z2);
   glVertex3f(x2,y, z2);
   glVertex3f(x2,y, z);
   glVertex3f(x2,y2, z);
   //top-bot
   glVertex3f(x,y2, z2);
   glVertex3f(x2,y2, z2);
   glVertex3f(x2,y2, z);
   glVertex3f(x,y2, z);

   glVertex3f(x,y, z2);
   glVertex3f(x2,y, z2);
   glVertex3f(x2,y, z);
   glVertex3f(x,y, z);
   glEnd();
 

}
//use class example cylinder,cone function
void cone(double x,double y,double z,
                 double r,double h,int inc, double R, double G, double B)
{
   glColor3f(R,G,B);
   //float yellow[]={1.0,1.0,0.0,1.0};
  // glMaterialf(GL_FRONT,GL_SHININESS,32.0);
   //glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(-90,1,0,0);
   glScaled(r,r,h);

//rotate traniagles for side part
   glBegin(GL_TRIANGLES);
   for (int th=0;th<=360;th+=inc)
   {
      //part 1
      glNormal3d(Cos(th-0.5*inc),Sin(th-0.5*inc),1);
      glTexCoord2f((th-0.5*inc)/120,0);
      glVertex3d(Cos(th-0.5*inc),Sin(th-0.5*inc),0);
      //part2
      glNormal3d(Cos(th),Sin(th),1);
      glTexCoord2f(th/120,1);
      glVertex3d(0,0,1);
      //part3
      glNormal3d(Cos(th+0.5*inc),Sin(th+0.5*inc),1);
      glTexCoord2f((th+0.5*inc)/120,0);
      glVertex3d(Cos(th+0.5*inc),Sin(th+0.5*inc),0);
   }
   glEnd();
   //base part
   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(0,0,-1);
   glTexCoord2f(0.5,0.5);
   glVertex3d(0,0,0);
   for (int th=0;th<=360;th+=inc)
   {
      glTexCoord2f(0.5*Cos(th+0.5*inc)+0.5,-0.5*Sin(th+0.5*inc)+0.5);
      glVertex3d(Cos(th+0.5*inc),Sin(th+0.5*inc),0);
   }
   glEnd();

   glPopMatrix();
}
static void tree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   glRotated(th,0,1,0);
    cone(0,0,0,0.5,0.5,10,0,1,0);
   cone(0,-0.3,0,0.5,0.5,10,0,1,0);
   cone(0,-0.6,0,0.5,0.5,10,0,1,0);
   cube(-0.1,-0.6,-0.1,0.1, -1,0.1);
   glPopMatrix();

}


void cylinder(double x,double y,double z,
                 double r,double h,int inc)
{
   glColor3f(1,1,1);
   float yellow[]={1.0,1.0,0.0,1.0};
   glMaterialf(GL_FRONT,GL_SHININESS,32.0);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(-90,1,0,0);
   glScaled(r,r,h/2);

//rotate traniagles for side part
   for (int i=1;i>=-1;i-=2)
   {
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,i);
      for (int th=0;th<=360;th+=inc)
      {
         glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
         glVertex3f(Cos(i*th),Sin(i*th),i);
      }
      glEnd();
   //base part
  glBegin(GL_QUAD_STRIP);
   for (int th=0;th<=360;th+=inc)
   {
      glNormal3f(Cos(th),Sin(th),0);
      glTexCoord2f(th/120,0); glVertex3d(Cos(th),Sin(th),-1);
      glTexCoord2f(th/120,0.5*h/r); glVertex3d(Cos(th),Sin(th),+1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
}
}

void cylinder1(double x,double y,double z,double r,double d)
{
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,d);
   //  Head & Tail
   glColor3f(1,1,1);
   for (int i=1;i>=-1;i-=2)
   {
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,i);
      for (int k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(i*Cos(k),Sin(k),i);
      }
      glEnd();
   }
   //  Edge
   glColor3f(1.00,0.77,0.36);
   glBegin(GL_QUAD_STRIP);
   for (int k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),+1);
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
}
void cylinder2(double x,double y,double z,double r,double d,double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,d);
   glRotated(th,1,0,0);
   //  Head & Tail
   glColor3f(1,1,1);
   for (int i=1;i>=-1;i-=2)
   {
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,i);
      for (int k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(i*Cos(k),Sin(k),i);
      }
      glEnd();
   }
   //  Edge
   glColor3f(1.00,0.77,0.36);
   glBegin(GL_QUAD_STRIP);
   for (int k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),+1);
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
}
void cone2(){
   glBegin(GL_TRIANGLES);
    for (int k=0;k<=360;k+=DEF_D){
      glColor3f(0.0,0.0,1.0);
      glVertex3f(0,0,1);
      glColor3f(0.0,1.0,1.0);
      glVertex3f(Cos(k),Sin(k),0);
      glColor3f(1.0,0.0,0.0);
      glVertex3f(Cos(k+DEF_D),Sin(k+DEF_D),0);
    }
    glEnd();
}
static void house(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   glRotated(th,0,1,0);   //  Cube
  
  
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(0,0, 0);
   glVertex3f(1,0, 0);
   glVertex3f(1,1, 0);
   glVertex3f(0,1, 0);
   //  Back
 
   glVertex3f(0,0, 1);
   glVertex3f(1,0, 1);
   glVertex3f(1,1, 1);
   glVertex3f(0,1, 1);
   //left 
   
   glVertex3f(0,0, 1);
   glVertex3f(0,0, 0);
   glVertex3f(0,1, 0);
   glVertex3f(0,1, 1);
   //  right 
   glVertex3f(1,0, 1);
   glVertex3f(1,0, 0);
   glVertex3f(1,1, 0);
   glVertex3f(1,1, 1);
   //bottom
   glColor3f(1,1,0);
   glVertex3f(1,0, 1);
   glVertex3f(1,0, 0);
   glVertex3f(0,0, 0);
   glVertex3f(0,0, 1);
   glEnd();

//left/right roof 
   glBegin(GL_TRIANGLES);
   glColor3f(0,1,0);
   glVertex3f(1,1, 0);
   glVertex3f(1,1, 1);
   glVertex3f(1,1.5, 0.5);
// 
   glVertex3f(0,1, 0);
   glVertex3f(0,1, 1);
   glVertex3f(0,1.5, 0.5);
   glEnd();
//front-back roof 
   glBegin(GL_QUADS);
   glColor3f(0,0.5,0);
   glVertex3f(0,1, 0);
   glVertex3f(1,1, 0);
   glVertex3f(1,1.5, 0.5);
   glVertex3f(0,1.5, 0.5);
// 
   glVertex3f(0,1, 1);
   glVertex3f(1,1, 1);
   glVertex3f(1,1.5, 0.5);
   glVertex3f(0,1.5, 0.5);
   glEnd();
   

   //door
   glBegin(GL_QUADS);
   glColor3f(1,1,1);
   glVertex3f(0.5,0.1, 1.1);
   glVertex3f(0.7,0.1, 1.1);
   glVertex3f(0.7,0.4, 1.1);
   glVertex3f(0.5,0.4, 1.1);
   //left
   glVertex3f(0.5,0.4, 1.1);
   glVertex3f(0.5,0.1, 1.1);
   glVertex3f(0.5,0.1, 1);
   glVertex3f(0.5,0.4, 1);
   //right
   glVertex3f(0.7,0.4, 1.1);
   glVertex3f(0.7,0.1, 1.1);
   glVertex3f(0.7,0.1, 1);
   glVertex3f(0.7,0.4, 1);
   //top-bot
   glVertex3f(0.5,0.4, 1.1);
   glVertex3f(0.7,0.4, 1.1);
   glVertex3f(0.7,0.4, 1);
   glVertex3f(0.5,0.4, 1);
   glVertex3f(0.5,0.1, 1.1);
   glVertex3f(0.7,0.1, 1.1);
   glVertex3f(0.7,0.1, 1);
   glVertex3f(0.5,0.1, 1);
   glEnd();

   // window_1
   glBegin(GL_QUADS);
   glColor3f(1,1,0);
   glVertex3f(0.6,0.6, 1.01);
   glVertex3f(0.8,0.6, 1.01);
   glVertex3f(0.8,0.8, 1.01);
   glVertex3f(0.6,0.8, 1.01);
// window_2
   glVertex3f(0.2,0.6, 1.01);
   glVertex3f(0.4,0.6, 1.01);
   glVertex3f(0.4,0.8, 1.01);
   glVertex3f(0.2,0.8, 1.01);
   glEnd();
//steep
   cube(0.2,0.1,1,0.5,0.2,1.1);
   cube(0.2,0,1.1,0.5,0.08,1.2);
   cube(0.5,0,1.1,0.8,0.05,1.2);
   cube(0.5,0-0.1,1.2,0.8,0-0.02,1.3);
//light
  cube(1.05,0,1,1.1,1.1,1.2);
  sphere(1.075,1.1,1.1,0.05);
   //  Undo transformations
   glPopMatrix();
}

static void brick(int m, int n,int thick,float R,float G,float B)
{
   //  Set all colors
   float black[] = {0,0,0,1};
   float color[] = {R,G,B,1};
   float shiny = 16;
   glColor3f(R,G,B);
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,color);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   glScaled(0.2,0.2,0.2);
   //  Outside dimensions
   float Ox = 2.5*m;
   float Oy = 2.5*n;
   float Oz = thick?3:1;
   //  Inside dimensions
   float Ix = Ox-1;
   float Iy = Oy-1;
   float Iz = Oz-1;

   //  Top and sides
   glBegin(GL_QUADS);
   //  Outside front
   glNormal3f( 0, 1, 0);
   glVertex3f(-Ox,+Oy,+Oz);
   glVertex3f(+Ox,+Oy,+Oz);
   glVertex3f(+Ox,+Oy,-Oz);
   glVertex3f(-Ox,+Oy,-Oz);
   //  Outside back
   glNormal3f( 0,-1, 0);
   glVertex3f(+Ox,-Oy,+Oz);
   glVertex3f(-Ox,-Oy,+Oz);
   glVertex3f(-Ox,-Oy,-Oz);
   glVertex3f(+Ox,-Oy,-Oz);
   //  Outside right
   glNormal3f( 1, 0, 0);
   glVertex3f(+Ox,+Oy,+Oz);
   glVertex3f(+Ox,-Oy,+Oz);
   glVertex3f(+Ox,-Oy,-Oz);
   glVertex3f(+Ox,+Oy,-Oz);
   //  Outside left
   glNormal3f(-1, 0, 0);
   glVertex3f(-Ox,+Oy,-Oz);
   glVertex3f(-Ox,-Oy,-Oz);
   glVertex3f(-Ox,-Oy,+Oz);
   glVertex3f(-Ox,+Oy,+Oz);
   //  Outside top
   glNormal3f( 0, 0, 1);
   glVertex3f(-Ox,+Oy,+Oz);
   glVertex3f(-Ox,-Oy,+Oz);
   glVertex3f(+Ox,-Oy,+Oz);
   glVertex3f(+Ox,+Oy,+Oz);
   //  Inside front
   glNormal3f( 0,-1, 0);
   glVertex3f(-Ix,+Iy,-Oz);
   glVertex3f(+Ix,+Iy,-Oz);
   glVertex3f(+Ix,+Iy,+Iz);
   glVertex3f(-Ix,+Iy,+Iz);
   //  Inside back
   glNormal3f( 0,+1, 0);
   glVertex3f(+Ix,-Iy,-Oz);
   glVertex3f(-Ix,-Iy,-Oz);
   glVertex3f(-Ix,-Iy,+Iz);
   glVertex3f(+Ix,-Iy,+Iz);
   //  Inside right
   glNormal3f(-1, 0, 0);
   glVertex3f(+Ix,+Iy,-Oz);
   glVertex3f(+Ix,-Iy,-Oz);
   glVertex3f(+Ix,-Iy,+Iz);
   glVertex3f(+Ix,+Iy,+Iz);
   //  Inside left
   glNormal3f(+1, 0, 0);
   glVertex3f(-Ix,-Iy,-Oz);
   glVertex3f(-Ix,+Iy,-Oz);
   glVertex3f(-Ix,+Iy,+Iz);
   glVertex3f(-Ix,-Iy,+Iz);
   //  Inside top
   glNormal3f( 0, 0,-1);
   glVertex3f(+Ix,+Iy,+Iz);
   glVertex3f(+Ix,-Iy,+Iz);
   glVertex3f(-Ix,-Iy,+Iz);
   glVertex3f(-Ix,+Iy,+Iz);
   glEnd();
   //  Bottom
   glBegin(GL_QUAD_STRIP);
   glNormal3f( 0, 0,-1);
   glVertex3f(+Ix,+Iy,-Oz);
   glVertex3f(+Ox,+Oy,-Oz);
   glVertex3f(+Ix,-Iy,-Oz);
   glVertex3f(+Ox,-Oy,-Oz);
   glVertex3f(-Ix,-Iy,-Oz);
   glVertex3f(-Ox,-Oy,-Oz);
   glVertex3f(-Ix,+Iy,-Oz);
   glVertex3f(-Ox,+Oy,-Oz);
   glVertex3f(+Ix,+Iy,-Oz);
   glVertex3f(+Ox,+Oy,-Oz);
   glEnd();


   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Set view angle
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);
   //  Decide what to draw
   //house(0,0,0 , 1,1,1 , 0);
   //house(0,1.5,0 , 0.4,0.4,0.2 , 90);
   
   cone(0,0,0,0.5,0.5,10,0,1,0);
   cone(0,-0.3,0,0.5,0.5,10,0,1,0);
   cone(0,-0.6,0,0.5,0.5,10,0,1,0);
   cube(-0.1,-0.6,-0.1,0.1, -1,0.1);
   //face,body
      //brick(M,N,L,color[K].r,color[K].g,color[K].b);

   //  White
   glColor3f(1,1,1);
   //  Draw axes
   if (axes)
   {
      const double len=2;  //  Length of axes
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,5);
   //  Print the text string
   Print("Angle=%d,%d",th,ph);
   //  Render the scene
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 10 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 10;
   //  Left arrow key - decrease angle by 10 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 10;
   //  Up arrow key - increase elevation by 10 degrees
   else if (key == GLUT_KEY_UP)
      ph += 10;
   //  Down arrow key - decrease elevation by 10 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 10;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection
   const double dim=2.5;
   //aspect ratio r
   double r = (height>0) ? (double)width/height : 1;
   glOrtho(-dim*r,+dim*r, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  GLUT calls this routine when there is nothing else to do
 */
void idle()
{
   double time = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*time,360);
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 500x500
   glutInitWindowSize(500,500);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Hw3_Objects_Zhanchao_Yang");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
