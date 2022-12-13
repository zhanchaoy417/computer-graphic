/*
 * Simple program to demonstrate generating coordinates
 * using the Lorenz Attractor
 */
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#ifndef RES
#define RES 1
#endif

/*  Lorenz Parameters  */
GLdouble s  = 10;
GLdouble b  = 2.6666;
GLdouble r  = 28;
// th: Azimuth angle, ph: Elevation  angle
int th=0; // x-y axis 
int ph=0; // y-z axis
double dim=2;   // Dimension of orthogonal box

//print function
#define LEN 8192  //  Maximum length of text string
static void
Print(const char* format , ...)
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
//if the program not working throw an error, from ex6
void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}
static void
draw(void)
{
  //clear last draw if exist one ,then load new draw
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  //set th in y-axis and ph in x-axis
  glRotated(ph,1,0,0);
  glRotated(th,0,1,0);

  //main function
   int i;
   /*  Coordinates  */
   double x = 1;
   double y = 1;
   double z = 1;
   /*  Time step  */
   double dt = 0.001;

   //printf("%5d %8.3f %8.3f %8.3f\n",0,x,y,z);
   /*
    *  Integrate 50,000 steps (50 time units with dt = 0.001)
    *  Explicit Euler integration
    */
   //begin draw
   glBegin(GL_LINE_STRIP);
   for (i=0;i<50000;i++)
   {
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;
      //use key (R/r, s/S, b/B) to make change
      //for each step, use ratio 0.02 to adjust the shape then getvertex of x,y,z 
      glVertex3d(0.02*x,0.02*y,0.02*z);
      //with different color in vertex, I keep the pixel between 0-255
      glColor3b( (i/1)%255,(i/2)%255,(i/3)%255);
     // printf("%5d %8.3f %8.3f %8.3f\n",i+1,x,y,z);
   } 
  
   glEnd();
   //draw three line x,y,z in white
    glColor3f(1, 1, 1);
    //begin line draw
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(1,0,0);
    glVertex3d(0,0,0);
    glVertex3d(0,1,0);
    glVertex3d(0,0,0);
    glVertex3d(0,0,1);
    glEnd();

    //label axes
    glRasterPos3d(1,0,0);
    Print("X");
    glRasterPos3d(0,1,0);
    Print("Y");
    glRasterPos3d(0,0,1);
    Print("Z");

//the position to print angle
  glWindowPos2i(5,5);
  Print("View Angle=%d,%d",th,ph);
  ErrCheck("display");
  glutSwapBuffers();
  glFlush();
}


/* new window size or exposure */
static void
reshape(int width, int height)
{
  //GLfloat h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint) RES*width, (GLint) RES*height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //orthogonal projection in GL,take (left,right,up,down,up of zbuf, down of zbuf)
  //  from ex6, aspect ratio of the window
  double r = (height>0) ? (double)width/height : 1;
  glOrtho(-r*dim,+r*dim, -dim,+dim, -dim,+dim);
  //glOrtho(-dim*h,+dim*h, -dim,+dim, -dim, +dim);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

//key function use R/r, S/s, B/b key 
static void
key(unsigned char k, int x, int y)
{
  switch (k) {
  case 's':
    s += 1;
    break;
  case 'S':
    s -= 1;
  case 'b':
    b += 0.1;
    break;
  case 'B':
    b -= 0.1;
  case 'r':
    r += 1;
    break;
  case 'R':
    r -= 1;
    break;
  case 27:  /* Escape */
    exit(0);
    break;
//reset/ defualt all s,b,r values
  case 'd':
    s  = 10;
    b  = 2.6666;
    r  = 28;
  default:
    return;
  }
  glutPostRedisplay();
}
//special key function, use up,down,left,right to change view angle along axies
static void
special(int k, int x, int y)
{ 
  switch (k) {
  case GLUT_KEY_UP:
    ph += 5.0;
    break;
  case GLUT_KEY_DOWN:
    ph -= 5.0;
    break;
  case GLUT_KEY_LEFT:
    th -= 5.0;
    break;
  case GLUT_KEY_RIGHT:
    th += 5.0;
    break;
  default:
    return;
  }
  th %= 360;
  ph %= 360;
  glutPostRedisplay();
}

// main function 
int main(int argc, char *argv[])
{ 
  glutInit(&argc, argv);
  //set up zbufer and color
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

 //create window
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Hw2_Lorenz_Attractor_Zhanchao_Yang");

  glutDisplayFunc(draw);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);

  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}