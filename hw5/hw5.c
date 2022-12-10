/*
 *  Lighting
 *
 *  Demonstrates basic lighting using a cube, sphere and icosahedron.
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  o          Cycles through objects
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */

//use code from library
#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=40;       //  set to 40 perspective view
double asp=1;     //  Aspect ratio
double dim=3;     //  three dim space
// Light values
int light     =   1;  // Lighting
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // light Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity 
int ambient   =  10;  // Ambient intensity 
int diffuse   =  40;  // set to 40 Diffuse intensity 
int specular  =   0;  // Specular intensity 
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light


/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

//ball for lighting
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball with yellow specular
   float yellow[]   = {1.0,1.0,0.0,1.0};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (int ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

// start my object 
static void sphere(double x,double y,double z,double r)
{
   const int d=15;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);
   
   glColor3f(1,1,0);
   float yellow[]   = {1.0,1.0,0.0,1.0};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
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
   //front
   glNormal3f(0,0,1);
   glVertex3f(x,y, z2);
   glVertex3f(x2,y, z2);
   glVertex3f(x2,y2, z2);
   glVertex3f(x,y2, z2);
   //left
   glNormal3f(-1,0,0);
   glVertex3f(x,y2, z2);
   glVertex3f(x,y, z2);
   glVertex3f(x,y, z);
   glVertex3f(x,y2, z);
   //right
   glNormal3f(1,0,0);
   glVertex3f(x2,y2, z2);
   glVertex3f(x2,y, z2);
   glVertex3f(x2,y, z);
   glVertex3f(x2,y2, z);
   //top
   glNormal3f(0,1,0);
   glVertex3f(x,y2, z2);
   glVertex3f(x2,y2, z2);
   glVertex3f(x2,y2, z);
   glVertex3f(x,y2, z);
   //bot
   glNormal3f(0,-1,0);
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
//set spectur color for my object before transformation
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
  
  //add normal for each side
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glNormal3f(0,0,1);
   glVertex3f(0,0, 1);
   glVertex3f(1,0, 1);
   glVertex3f(1,1, 1);
   glVertex3f(0,1, 1);
      //  Back
   glNormal3f(0,0,-1);
   glVertex3f(0,0, 0);
   glVertex3f(1,0, 0);
   glVertex3f(1,1, 0);
   glVertex3f(0,1, 0);
   //left 
   glNormal3f(-1,0,0);
   glVertex3f(0,0, 1);
   glVertex3f(0,0, 0);
   glVertex3f(0,1, 0);
   glVertex3f(0,1, 1);
   //  right 
   glNormal3f(1,0,0);
   glVertex3f(1,0, 1);
   glVertex3f(1,0, 0);
   glVertex3f(1,1, 0);
   glVertex3f(1,1, 1);
   //bottom
   glColor3f(1,1,0);
   glNormal3f(0,-1,0);
   glVertex3f(1,0, 1);
   glVertex3f(1,0, 0);
   glVertex3f(0,0, 0);
   glVertex3f(0,0, 1);
   glEnd();

//right roof 
   glBegin(GL_TRIANGLES);
   glNormal3f(0.5,0,0);
   glColor3f(0,1,0);
   glVertex3f(1,1, 0);
   glVertex3f(1,1, 1);
   glVertex3f(1,1.5, 0.5);
// left roof
   glNormal3f(-0.5,0,0);
   glVertex3f(0,1, 0);
   glVertex3f(0,1, 1);
   glVertex3f(0,1.5, 0.5);
   glEnd();
//back roof 
   glBegin(GL_QUADS);
   glColor3f(0,0.5,0);
//normal 
   glNormal3f(0,0.5,-0.5);
   glVertex3f(0,1, 0);
   glVertex3f(1,1, 0);
   glVertex3f(1,1.5, 0.5);
   glVertex3f(0,1.5, 0.5);
// front roof
//normal 
   glNormal3f(0,0.5,0.5);
   glVertex3f(0,1, 1);
   glVertex3f(1,1, 1);
   glVertex3f(1,1.5, 0.5);
   glVertex3f(0,1.5, 0.5);
   glEnd();

   //door
   glBegin(GL_QUADS);
   glColor3f(1,1,1);
   //front
   glNormal3f(0,0,1);
   glVertex3f(0.5,0.1, 1.1);
   glVertex3f(0.7,0.1, 1.1);
   glVertex3f(0.7,0.4, 1.1);
   glVertex3f(0.5,0.4, 1.1);
   //left
   glNormal3f(-1,0,0);
   glVertex3f(0.5,0.4, 1.1);
   glVertex3f(0.5,0.1, 1.1);
   glVertex3f(0.5,0.1, 1);
   glVertex3f(0.5,0.4, 1);
   //right
   glNormal3f(1,0,0);
   glVertex3f(0.7,0.4, 1.1);
   glVertex3f(0.7,0.1, 1.1);
   glVertex3f(0.7,0.1, 1);
   glVertex3f(0.7,0.4, 1);
   //top
   glNormal3f(0,1,0);
   glVertex3f(0.5,0.4, 1.1);
   glVertex3f(0.7,0.4, 1.1);
   glVertex3f(0.7,0.4, 1);
   glVertex3f(0.5,0.4, 1);
   //bot
   glNormal3f(0,-1,0);
   glVertex3f(0.5,0.1, 1.1);
   glVertex3f(0.7,0.1, 1.1);
   glVertex3f(0.7,0.1, 1);
   glVertex3f(0.5,0.1, 1);
   glEnd();

   // window_1
   glBegin(GL_QUADS);
   glColor3f(1,1,0);
   glNormal3f(0,0,1);
   glVertex3f(0.6,0.6, 1.01);
   glVertex3f(0.8,0.6, 1.01);
   glVertex3f(0.8,0.8, 1.01);
   glVertex3f(0.6,0.8, 1.01);
// window_2
   glNormal3f(0,0,1);
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
   //  Erase the window and the depth buffer,Enable Z-buffering and Undo previous transformations
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);

   glLoadIdentity();
   //  Perspective /Orthogonal mode
   //from hw4 projection
  if (mode==1)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Flat/smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

  
   //  Light switch, initlize light on which =1
   if (!light)
   {      
      glDisable(GL_LIGHTING);
   }
else
   {
      //  Translate intensity to color vectors
       //if turn on the light, we have ambient,diffuse,specture,meterial 4 color
   //and materical color define in each object itself
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      //  Draw light position as white ball ( no lighting here)
      // next step is set ball postion ,and the ball will rotate and remain same height
      //use cos/sin to rotate and radius==1
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);

      //  enable normal vectors and Enable lighting for the light ball
      glEnable(GL_NORMALIZE);
      glEnable(GL_LIGHTING);
      
      //  add Location of viewer
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //in prevous we have emision,shinyness,spectular color material
      // Now sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      
      //  Enable light off if done
    //  Set ambient, diffuse, specular components and position of light 0 which light off
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   
   //  Draw my object
   house(0,0,0 , 1,1,1 , 0);

   //  Draw axes with length 2.5
   // remove light effect for next opeartion
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      const double len=2.5;  
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

   //  Display parameters print function from ex13,which print all value
   //and set initlize window position
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }

   //  Render the scene and make it visible
   //remove zbufer after display 
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //another way prevent object change shape when window resize
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

//two key function,modify from ex13, and I change the value for most variable
//key name related the variable name to easy understand, such d refer dufuse color

void special(int key,int x,int y)
{
   //   arrow key - change angle by 10 
   if (key == GLUT_KEY_RIGHT)
      th += 10;
   else if (key == GLUT_KEY_LEFT)
      th -= 10;
   else if (key == GLUT_KEY_UP)
      ph += 10;
   else if (key == GLUT_KEY_DOWN)
      ph -= 10;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   // let angles in +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   //use project from library
   Project(mode?fov:0,asp,dim);
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
   else if (ch == 'x')
      axes = 1-axes;
      //  Smooth color model
   else if ( ch == 'X')
      smooth = 1-smooth;
   //  Toggle lighting
   else if (ch == 'l')
      light = 1-light;
   //local viewer
   else if (ch == 'L')
    local = 1-local;
   //  Switch projection mode
   else if (ch == 'p')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light ball elevation
   else if (ch=='u')
      ylight -= 0.05;
   else if (ch=='U')
      ylight += 0.05;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 10;
   else if (ch=='A' && ambient<100)
      ambient += 10;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 10;
   else if (ch=='D' && diffuse<100)
      diffuse += 10;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 10;
   else if (ch=='S' && specular<100)
      specular += 10;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 10;
   else if (ch=='E' && emission<100)
      emission += 10;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //Flip
   else if(ch=='t')
       one = -one;
   //  Translate shininess power to value (-1 => 0)
   //shininess increase by power of 2
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
//  object remain same shape when the window change
void reshape(int width,int height)
{
   //  object remain same shape when the window change
   asp = (height>0) ? (double)width/height : 1;
   //without "RES" may cause reshape problem
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   Project(mode?fov:0,asp,dim);
}


/*
 *  Start up GLUT and tell it what to do,same main function used for previous hw
 */
int main(int argc,char* argv[])
{
 
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("hw5_Lighting_Zhanchao_Yang");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //without it may cause reshape problem
   glutIdleFunc(idle);
   //  throw errow
   glutMainLoop();
   return 0;
}
