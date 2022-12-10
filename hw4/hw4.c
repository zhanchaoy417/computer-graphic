
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
int mode=0;        //  What to display
double dim=4.0;   //  Size of world
const char* text[] = {"Perpective","Orthogonal","First Perpective"};

double Ex=0;
double Ey=0;
double Ez=0;
//  First Perpective distance start with (-1.0.-1)
double distance=0.01;
double ex2 = 0;
double ey2 = 0;
double ez2 = 0;
double cx=-1;
double cy=0;
double cz=-1;
double cx2;
double cy2;
double cz2;
double asp=1;

//  define Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.14/180))
#define Sin(x) (sin((x)*3.14/180))

int fov=40;

//generate print and project, it may similar to example
//create format array
char fm[100];

static void Pf(const char* arg,...)
{
   char* str=fm;
   //start with lists of parameters
   va_list p;
   va_start(p,arg);
   //vsnprint is print in window and vprintf is print in terminall
   vsnprintf(fm,100,arg,p);
   va_end(p);  
   while(*str)
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*str++);

   
}

/*
 *  Set projection
 */
static void Projection()
{
   //start
   glMatrixMode(GL_PROJECTION);
   //  Undo previous view
   glLoadIdentity();
      switch (mode)
   {
   case 0:
      /* code */
      // r is aspect ratio width/height

      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
      break;
   //change to Znear, Zfar ratio to 3 and set different fov value
      //  Perspective transformation   
   case 1: 
    gluPerspective(fov,asp,dim/4,4*dim);
    break;
   case 2: 
    gluPerspective(fov,asp,dim/4,4*dim);
    break;
   default:
      break;
   }   
   //new view
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
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
static void house(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
  
  
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
switch(mode)
{ case 0:
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
      break;
   case 1:
      Ex = -2*dim*Sin(th)*Cos(ph);
      Ey = +2*dim        *Sin(ph);
       Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
      break;
   case 2:
   //orgianl object position, remove ph angle, ph will be const 0
       ex2 = -2*dim*Sin(th);
       ey2 = -2*dim*0     ;
       ez2 = +2*dim*Cos(th) ;
    //up date object postion when eye postion change
       cx2=cx+ex2;
       cz2=cz+ez2;
      gluLookAt(cx,cy,cz, cx2,ey2,cz2 , 0,1,0);
      break;
}

 
   //  Draw cubes
   for (int i=-1;i<=1;i++)
      for (int j=-1;j<=1;j++)
         for (int k=-1;k<=1;k++)
            house(i,j,k , 0.4,0.4,0.4 , 0);
   //  Decide what to draw
 

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
      Pf("X");
      glRasterPos3d(0.0,len,0.0);
      Pf("Y");
      glRasterPos3d(0.0,0.0,len);
      Pf("Z");
   }
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,5);
   //  Print the text string
   Pf("View Angle=%d,%d FOV=%d Mode=%s Dim=%.01f",th,ph,fov,text[mode],dim);
   //  Render the scene
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int k,int x,int y)
{
if (mode==0 || mode==1)
    switch (k) {
  case GLUT_KEY_RIGHT:
    th += 10;
    break;
  case GLUT_KEY_LEFT:
    th -= 10;
    break;
  case GLUT_KEY_UP:
    ph += 10;
    break;
  case GLUT_KEY_DOWN:
    ph -= 10;
    break;
   case GLUT_KEY_PAGE_UP:
    dim += 0.2;
    break;
  case GLUT_KEY_PAGE_DOWN:
   if (dim>1)
    dim -= 0.2;
    break;
  default:
    return;
   th %= 360;
   ph %= 360;
  }
//at mode2
else
{ 
  switch (k) {
   //update new eye postion each time moving forward/backward with 0.01 ratio
  case GLUT_KEY_UP:
      cx += distance*ex2;
      cz += distance*ez2;
    break;
   //update new eye postion each time moving forward/backward with 0.01 ratio
  case GLUT_KEY_DOWN:
      cx -= distance*ex2;
      cz -= distance*ez2;
    break;
  case GLUT_KEY_RIGHT:
    th += 5;
    break;
  case GLUT_KEY_LEFT:
    th -= 5;
      break;
    case GLUT_KEY_PAGE_DOWN:
    ph -= 5;
      break;
  default:
    return;
   th %= 360;
  }
}
   Projection();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char k,int x,int y)
{
   switch (k) {
      //  Exit on ESC
  case 27:
    exit(0);
    break;
   case 'd':
    th= ph = 0;
    break;
  case 'a':
    axes = 1-axes;
    break;
  case 'm':
    mode = (mode+1)%3;
    break;
  case  '-':
   if (mode==0 || mode==1)
    if (k>1)
     fov -= 1;
   break;
   case '+' :
   if (mode==0 || mode==1)
     if (k<179)
      fov += 1;
    break;
  default:
    return;
  }
   //  Reproject
   Projection();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{

   //aspect ratio r
   asp = (height>0) ? (double)width/height : 1;
   glViewport(0,0, RES*width,RES*height);
     //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //call project func
   Projection();
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
   glutCreateWindow("Hw4_Projections_Zhanchao_Yang");

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
