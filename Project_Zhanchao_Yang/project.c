/*
 *  Textures and Lighting
Key bindings
  l          Toggle lighting on/off
  t          Change textures(have 2 different combinations texture)
  p          Toggles texture mode modulate/replace
  m          light movement
  a/A        decrease/increase ambient light
  d/D        decrease/increase diffuse light
  s/S        decrease/increase specular light
  e/E        decrease/increase emitted light
  n/N        Decrease/increase shininess
  u/U        Lower/rise light
  x          Toggle axes
arrows(Up,down,left,right)     Change view angle
  PgDn/PgUp  Zoom in and out
  0          Reset view angle
  ESC        Exit
 */
#include "CSCIx229.h"
// shader var
int roll=1;
int mode_s=1;
int shader[]  = {0,0,0,0,0,0,0,0,0,0};
float X=0,Y=0,Z=1;
//
int mode=0;       //  Texture mode
int ntex=0;       //  Cube faces
int axes=0;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=1;      //  Lighting
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  20;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   5;  // Elevation of light
unsigned int texture[30]; //  texture from ex14,ex15 crate,block,water,pi,square

//hw5
int smooth    =   1;  // Smooth/Flat shading
int distance = 5;
int local     =   0;  // Local Viewer Model
int inc       =  10;  // light Ball increment
int move=1;       //  Move light
int m=1;        //mode of orth/perspective
int fov=40;  //inital fov angle from orth/perspective

#define DEF_t  glutGet(GLUT_ELAPSED_TIME)/1000.0

double alpha=0.75;
int aone=1;
int obj_color=0;
int    box=1;    //  Draw sky
int    sky[2];   //  Sky textures
int fire=0;

//set color
void SetColor(float R,float G,float B)
{
   float color[] = {R,G,B,1.0};
   glColor3f(R,G,B);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
}
/*
 *  Draw vertex in polar coordinates with normal, function also define in class library
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
static void ball2(double x,double y,double z,double r,double R,double G,double B)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball with yellow specular
   float yellow[]   = {1.0,1.0,0.0,1.0};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   glColor3f(R,G,B);
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

static void sphere(double x,double y,double z,double r)
{
   const int d=15;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);
   
   glColor3f(1,1,0);
   if (obj_color==1){
      glColor3f(x,y,z);

   }
   //add light
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
 *  Draw a cube, dimensions (dx,dy,dz) and rotated th about the y axis
 */
//house component
static void cube(double a,double b,double c,double d,double e,double f)
{
   //these value according to small/big x,small/big y,small/big z, accordindly
   const double x=a; 
   const double x2=d; 
   const double y=b; 
   const double y2=e;   
   const double z=c;
   const double z2=f;
   glBegin(GL_QUADS);
   glColor3f(0.3,0.22,0.2);
   if (obj_color==1)
   {
     glColor3f(1,1,1);
   }
   //front
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);glVertex3f(x,y, z2);
   glTexCoord2f(1,0);glVertex3f(x2,y, z2);
   glTexCoord2f(1,1);glVertex3f(x2,y2, z2);
   glTexCoord2f(0,1);glVertex3f(x,y2, z2);
   //left
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);glVertex3f(x,y, z);
   glTexCoord2f(1,0);glVertex3f(x,y, z2);
   glTexCoord2f(1,1);glVertex3f(x,y2, z2);
   glTexCoord2f(0,1);glVertex3f(x,y2, z);
   //right
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);glVertex3f(x2,y, z2);
   glTexCoord2f(1,0);glVertex3f(x2,y, z);
   glTexCoord2f(1,1);glVertex3f(x2,y2, z);
   glTexCoord2f(0,1);glVertex3f(x2,y2, z2);
   //top
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);glVertex3f(x,y2, z2);
   glTexCoord2f(1,0);glVertex3f(x2,y2, z2);
   glTexCoord2f(1,1);glVertex3f(x2,y2, z);
   glTexCoord2f(0,1);glVertex3f(x,y2, z);
   //bot
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);glVertex3f(x,y, z2);
   glTexCoord2f(1,0);glVertex3f(x2,y, z2);
   glTexCoord2f(1,1);glVertex3f(x2,y, z);
   glTexCoord2f(0,1);glVertex3f(x,y, z);
   glEnd();

}
static void house(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, int texture_1, int texture_2)
{
//set spectur color for my object before transformation
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //start texture
   glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1); //deafult all white space 
   // add first texture
   glBindTexture(GL_TEXTURE_2D,texture[texture_1]);
   //  Cube-body of house
  //add normal for each side, add texture coordinate 0 to 1, for each vertex, add texcorrdoinate
   glBegin(GL_QUADS);
   //  Front
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(0,0, 1);
   glTexCoord2f(1,0); glVertex3f(1,0, 1);
   glTexCoord2f(1,1); glVertex3f(1,1, 1);
   glTexCoord2f(0,1); glVertex3f(0,1, 1);
   //  Back
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0); glVertex3f(1,0, 0);
   glTexCoord2f(1,0); glVertex3f(0,0, 0);
   glTexCoord2f(1,1); glVertex3f(0,1, 0);
   glTexCoord2f(0,1); glVertex3f(1,1, 0);
   glEnd();

//two-side wall texture
   glColor3f(0.7,0.2,0);
   glBindTexture(GL_TEXTURE_2D,texture[4]);

   glBegin(GL_QUADS);
   //left 
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0); glVertex3f(0,0, 0);
   glTexCoord2f(1,0); glVertex3f(0,0, 1);
   glTexCoord2f(1,1); glVertex3f(0,1, 1);
   glTexCoord2f(0,1); glVertex3f(0,1, 0);
   //  right 
   glNormal3f(1,0,0);
   glTexCoord2f(0,0); glVertex3f(1,0, 1);
   glTexCoord2f(1,0); glVertex3f(1,0, 0);
   glTexCoord2f(1,1); glVertex3f(1,1, 0);
   glTexCoord2f(0,1); glVertex3f(1,1, 1);
   glEnd();

   glBegin(GL_QUADS);
   //bottom  
   glColor3f(1,1,0);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0); glVertex3f(0,0, 0); 
   glTexCoord2f(1,0); glVertex3f(1,0, 0); 
   glTexCoord2f(1,1); glVertex3f(1,0, 1); 
   glTexCoord2f(0,1); glVertex3f(0,0, 1); 
   glEnd();
//right roof 
// texture of the roof
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[texture_2]);

   glBegin(GL_TRIANGLES);
   glNormal3f(0.5,0,0);
   //glColor3f(0,1,0);
   glTexCoord2f(0,0); glVertex3f(1,1, 0);
   glTexCoord2f(1,0); glVertex3f(1,1, 1);
   glTexCoord2f(0.5,1); glVertex3f(1,1.5, 0.5);
// left roof
   glNormal3f(-0.5,0,0);
   glTexCoord2f(0,0); glVertex3f(0,1, 0);
   glTexCoord2f(1,0); glVertex3f(0,1, 1);
   glTexCoord2f(0.5,1); glVertex3f(0,1.5, 0.5);
   glEnd();
//back roof 
   glBegin(GL_QUADS);
   //glColor3f(0,0.5,0);
//normal 
   glNormal3f(0,0.5,-0.5);
   glTexCoord2f(0,0); glVertex3f(1,1, 0);
   glTexCoord2f(1,0); glVertex3f(0,1, 0);
   glTexCoord2f(1,1); glVertex3f(0,1.5, 0.5);
   glTexCoord2f(0,1); glVertex3f(1,1.5, 0.5);
// front roof
//normal 
   glNormal3f(0,0.5,0.5);
   glTexCoord2f(0,0); glVertex3f(0,1, 1);
   glTexCoord2f(1,0); glVertex3f(1,1, 1);
   glTexCoord2f(1,1); glVertex3f(1,1.5, 0.5);
   glTexCoord2f(0,1); glVertex3f(0,1.5, 0.5);
   glEnd();

   //door
// texture image appied to door and windows
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]);

   glBegin(GL_QUADS);
  
   //front
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(0.5,0.1, 1.1);
   glTexCoord2f(1,0); glVertex3f(0.7,0.1, 1.1);
   glTexCoord2f(1,1); glVertex3f(0.7,0.4, 1.1);
   glTexCoord2f(0,1); glVertex3f(0.5,0.4, 1.1);
   //left
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0); glVertex3f(0.5,0.1, 1);
   glTexCoord2f(1,0); glVertex3f(0.5,0.1, 1.1);
   glTexCoord2f(1,1); glVertex3f(0.5,0.4, 1.1);
   glTexCoord2f(0,1); glVertex3f(0.5,0.4, 1);
   //right
   glNormal3f(1,0,0);
   glTexCoord2f(0,0); glVertex3f(0.7,0.1, 1.1);
   glTexCoord2f(1,0); glVertex3f(0.7,0.1, 1);
   glTexCoord2f(1,1); glVertex3f(0.7,0.4, 1);
   glTexCoord2f(0,1); glVertex3f(0.7,0.4, 1.1);
   //top
   glNormal3f(0,1,0);
   glTexCoord2f(0,0); glVertex3f(0.5,0.4, 1.1);
   glTexCoord2f(1,0); glVertex3f(0.7,0.4, 1.1);
   glTexCoord2f(1,1); glVertex3f(0.7,0.4, 1);
   glTexCoord2f(0,1); glVertex3f(0.5,0.4, 1);
   //bot
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0); glVertex3f(0.5,0.1, 1.1);
   glTexCoord2f(1,0); glVertex3f(0.7,0.1, 1.1);
   glTexCoord2f(1,1); glVertex3f(0.7,0.1, 1);
   glTexCoord2f(0,1); glVertex3f(0.5,0.1, 1);
   glEnd();

   // window_1 on the front
   glBegin(GL_QUADS);
   glColor3f(0.67f,0.84f,0.9f);
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(0.6,0.6, 1.01);
   glTexCoord2f(1,0); glVertex3f(0.8,0.6, 1.01);
   glTexCoord2f(1,1); glVertex3f(0.8,0.8, 1.01);
   glTexCoord2f(0,1); glVertex3f(0.6,0.8, 1.01);
// window_2
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(0.2,0.6, 1.01);
   glTexCoord2f(1,0); glVertex3f(0.4,0.6, 1.01);
   glTexCoord2f(1,1); glVertex3f(0.4,0.8, 1.01);
   glTexCoord2f(0,1); glVertex3f(0.2,0.8, 1.01);
   glEnd();

//steep
//texture for stepp in the front
   glColor3f(1,1,1);
 glBindTexture(GL_TEXTURE_2D,texture[12]);
   cube(0.2,0.1,1,0.5,0.2,1.1);
   cube(0.2,0,1.1,0.5,0.08,1.2);
   cube(0.5,0,1.1,0.8,0.05,1.2);
   cube(0.5,0-0.1,1.2,0.8,0-0.02,1.3);

//texture for light-structure
glColor3f(0.58f,0.29f,0);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
  cube(1.05,0,1,1.1,1.1,1.2);

  glDisable(GL_TEXTURE_2D);
  sphere(1.075,1.1,1.1,0.05);
   //  Undo transformations
   glPopMatrix();
    //  Undo textures
   
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
   if (obj_color==1)
   {
     glColor3f(1,1,1);
    }
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
//for tree part
void cylinder3(double x,double y,double z,double r,double d,double th, double ratio)
{
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,d);
   glRotated(th,1,0,0);
   //  Head & Tail
   glColor3f(1,1,1);
   //bot
     glNormal3f(0,0,1);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,1);
      for (int k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(1*Cos(k)*ratio,Sin(k)*ratio,1);
      }
      glEnd();
   //top
     glNormal3f(0,0,-1);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,-1);
      for (int k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(-1*Cos(k),Sin(k),-1);
      }
     
      glEnd();
   //  Edge
   glColor3f(0.54,0.45,0.38);
   glBegin(GL_QUAD_STRIP);
   for (int k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      //near bot part
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k)*ratio,Sin(k)*ratio,+1);
      //near top part
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
}
static void cube2(double a,double b,double c,double d,double e,double f, double th)
{
   //these value according to small/big x,small/big y,small/big z, accordindly
   glPushMatrix();
   //  Offset and scale
   glRotated(th,1,0,0);
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
   glTexCoord2f(0,0);glVertex3f(x,y, z2);
   glTexCoord2f(1,0);glVertex3f(x2,y, z2);
   glTexCoord2f(1,1);glVertex3f(x2,y2, z2);
   glTexCoord2f(0,1);glVertex3f(x,y2, z2);
   //left
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);glVertex3f(x,y, z);
   glTexCoord2f(1,0);glVertex3f(x,y, z2);
   glTexCoord2f(1,1);glVertex3f(x,y2, z2);
   glTexCoord2f(0,1);glVertex3f(x,y2, z);
   //right
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);glVertex3f(x2,y, z2);
   glTexCoord2f(1,0);glVertex3f(x2,y, z);
   glTexCoord2f(1,1);glVertex3f(x2,y2, z);
   glTexCoord2f(0,1);glVertex3f(x2,y2, z2);
   //top
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);glVertex3f(x,y2, z2);
   glTexCoord2f(1,0);glVertex3f(x2,y2, z2);
   glTexCoord2f(1,1);glVertex3f(x2,y2, z);
   glTexCoord2f(0,1);glVertex3f(x,y2, z);
   //bot
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);glVertex3f(x,y, z2);
   glTexCoord2f(1,0);glVertex3f(x2,y, z2);
   glTexCoord2f(1,1);glVertex3f(x2,y, z);
   glTexCoord2f(0,1);glVertex3f(x,y, z);
   glEnd();
   glPopMatrix();

}
//use class example cylinder,cone function
void cone(double x,double y,double z,
                 double r,double h,int inc, double R, double G, double B)
{
   glColor3f(R,G,B);
   if (obj_color==1)
   {
      glColor3f(1,1,1);
   }
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
void pyramid(double x,double y, double z,double h,double th)
{
    glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
glScaled(h,h,h);
glRotatef(th,1,0,0);
glBegin( GL_TRIANGLES );
glColor3f(0.56,0.93,0.56);

 glNormal3f(0,-2,-4);
 glTexCoord2f(0.5,1); glVertex3f( 0.0, 1, 0.0 );
 glTexCoord2f(0,0); glVertex3f( -1.0, -1.0, 1 );
 glTexCoord2f(1,0);glVertex3f( 1.0, -1, 1);

 glNormal3f(-4,1,-2);
 glTexCoord2f(0.5,1); glVertex3f( 0.0, 1.0, 0.0);
 glTexCoord2f(0,0); glVertex3f( -1.0, -1.0, 1.0);
 glTexCoord2f(1,0); glVertex3f( 0.0, -1.0, -1.0);

 glNormal3f(4,1,-2);
 glTexCoord2f(0.5,1); glVertex3f( 0.0, 1.0, 0.0);
 glTexCoord2f(0,0); glVertex3f( 0.0, -1.0, -1.0);
 glTexCoord2f(1,0); glVertex3f( 1.0, -1.0, 1.0);

glNormal3f(0,4,0);
 glTexCoord2f(0.5,1); glVertex3f( -1.0, -1.0, 1.0);
 glTexCoord2f(0,0); glVertex3f( 0.0, -1.0, -1.0);
 glTexCoord2f(1,0); glVertex3f( 1.0, -1.0, 1.0);
glEnd();
glPopMatrix();

}
//simple tree
static void tree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   obj_color=1;
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1); //deafult all white space 
   glBindTexture(GL_TEXTURE_2D,texture[17]);
   cone(0,0,0,0.5,0.5,10,0,1,0);
   cone(0,-0.3,0,0.5,0.5,10,0,1,0);
   cone(0,-0.6,0,0.5,0.5,10,0,1,0);
   glBindTexture(GL_TEXTURE_2D,texture[18]);
   cube(-0.1,-0.6,-0.1,0.1, -1,0.1);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
    obj_color=0;

}
void leaf(double x,double y, double z,double h,double th)
{
     glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
glScaled(h,h,h);
glRotatef(th,1,0,0);
glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1); //deafult all white space 
   glBindTexture(GL_TEXTURE_2D,texture[17]);
pyramid(0,0,0,0.1,90);
pyramid(0.05,0,0,0.1,90);
pyramid(0.15,0,0,0.1,90);
pyramid(0.08,0,0.05,0.1,90);
pyramid(0.1,0,0.15,0.1,90);
 glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}
void tree_part(double x,double y, double z,double h,double th,double xh){
 glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
  glScaled(h,h,h);
  glRotatef(th,1,0,0);
  glRotatef(xh,1,0,0);
 for (int i=0;i<=3;i++)
 {
   
   leaf(-0.8+i*0.5,-0.2,0,1,0);
   leaf(-0.8+i*0.5,0,0,1,0);
   leaf(-0.6+i*0.4,0.2,0,1,0);
   leaf(-0.4+i*0.3,0.4,0,1,0);
   leaf(-0.2+i*0.1,0.5,0,1,0);
   leaf(-0.1+i*0.08,0.6,0,1,0);
   leaf(-0.05+i*0.05,0.7,0,1,0);
 }
  glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1); //deafult all white space 
   glBindTexture(GL_TEXTURE_2D,texture[18]);
   cylinder3(-0,-0.7,0,0.4,0.4,90,0.3);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   
}
void tree_part2(double x,double y, double z,double h,double th)
{
    glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
  glScaled(h,h,h);
  glRotatef(th,1,0,0);
   tree_part(0.2,0,0,0.2,-45,0);
   tree_part(0.4,-0.2,0,0.2,-45,0);
   tree_part(0,-0.2,0,0.2,-45,0);

    tree_part(0.2,0,-0.2,0.2,45,0);
   tree_part(0.4,-0.2,-0.2,0.2,45,0);
   tree_part(0,-0.2,-0.2,0.2,45,0);
   //right
     tree_part(0.7,0,-0.1,0.2,0,0);
   tree_part(0.7,-0.2,0,0.2,0,0);
   tree_part(0.7,-0.2,-0.2,0.2,0,0);
//left
  tree_part(-0.3,0,-0.1,0.2,0,0);
   tree_part(-0.3,-0.2,0,0.2,0,0);
   tree_part(-0.3,-0.2,-0.2,0.2,0,0);
   glPopMatrix();
}
static void tree2(double x,double y,double z,
                 double r, double th)
{
   //  Save transformation
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(r,r,r);
//v2 tree
   tree_part2(0,0,0,1,0);
//2
   tree_part2(0.5,0.2,-0.3,0.8,0);
   tree_part2(0,0.2,0,0.8,0);
   tree_part2(-0.5,0.2,0.3,0.8,0);
//3
   tree_part2(0.3,0.4,-0.4,0.5,0);
   tree_part2(0,0.4,0,0.5,0);
   tree_part2(-0.3,0.4,0.4,0.5,0);
//4
for (int i=0;i<=10;i++)
{
    tree_part2(-0.8+i*0.08,0.5,0.3-i*0.1,0.3,0);
    tree_part2(0.8-i*0.08,0.5,0.3-i*0.1,0.3,0);
}
//3
   tree_part2(0.3,0.6,-0.4,0.5,0);
   tree_part2(0,0.6,0,0.5,0);
   tree_part2(-0.3,0.6,0.4,0.5,0);
   //2
   tree_part2(0.5,0.8,-0.3,0.8,0);
   tree_part2(0,0.8,0,0.8,0);
   tree_part2(-0.5,0.8,0.3,0.8,0);
   //1
   tree_part2(0.2,1,-0.3,0.3,0);
   tree_part2(0,1,0,0.3,0);
   tree_part2(0.1,1,0.3,0.3,0);
   glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1); //deafult all white space 
   glBindTexture(GL_TEXTURE_2D,texture[18]);
   cylinder3(0.,-0.5,-0.1,0.4,0.4,270,0.5);
glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}
static void person(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
//set spectur color for my object before transformation
 obj_color=1;
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //start texture
   glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   // add first texture
   glColor3f(1,1,1); //deafult all white space 
   //find the current time
   double t_cur=fmod(DEF_t,1);
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   cylinder1(0,0.2,0,0.1,0.1);
  glBindTexture(GL_TEXTURE_2D, texture[6]);
   cube(-0.1,-0.2,-0.1,0.1,0.2,0.1);
  // cube2(-0.08,-0.3,0,0,-0.2,0.05,-25);
   //cube(0.02,-0.3,0,0.09,-0.2,0.05); 
   //hand change 40-90 in 0.5s, foot change 0- -25 in 0.5s
   if ( t_cur >=0 && t_cur <=0.5 )
{ 
   glBindTexture(GL_TEXTURE_2D, texture[8]);
   cube2(-0.08,-0.3,0,0,-0.2,0.05,0-50*t_cur);
   glBindTexture(GL_TEXTURE_2D, texture[5]);
   cylinder2(-0.1,0.1,0,0.05,0.05,40+100*t_cur);
}
else
{
   glBindTexture(GL_TEXTURE_2D, texture[8]);
   cube2(-0.08,-0.3,0,0,-0.2,0.05,-25+50*t_cur);
   glBindTexture(GL_TEXTURE_2D, texture[5]);
cylinder2(-0.1,0.1,0,0.05,0.05,90-100*t_cur);
}
   sphere(-0.1,0.03,0,0.04);
  if ( t_cur >=0 && t_cur <=0.5 )
{ 
   glBindTexture(GL_TEXTURE_2D, texture[8]);
   cube2(0.02,-0.3,0,0.09,-0.2,0.05,-25+50*t_cur); 
   glBindTexture(GL_TEXTURE_2D, texture[5]);
cylinder2(0.1,0.1,0,0.05,0.05,90-100*t_cur); 
}
else
{
   glBindTexture(GL_TEXTURE_2D, texture[8]);
cube2(0.02,-0.3,0,0.09,-0.2,0.05,0-50*t_cur); 
  glBindTexture(GL_TEXTURE_2D, texture[5]);
   cylinder2(0.1,0.1,0,0.05,0.05,40+100*t_cur); 
} 
   sphere(0.1,0.03,0,0.04);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   obj_color=0;
   
} 
static void obstacle()
{
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
     glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   // add first texture
   glColor3f(1,1,1); //deafult all white space 
   glBindTexture(GL_TEXTURE_2D, texture[10]);
    for (int i=0;i<=5;i++) 
   {
      cylinder2(0,0,0-i*0.3,0.05,0.05,90);
   } 
   glPushMatrix();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
} 
static void sign(double x,double y,double z, double dx,double dy,double dz,
                  double th, int sign_number)
{
   obj_color=1;
//set spectur color for my object before transformation
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   // add first texture
   glColor3f(1,1,1); //deafult all white space 
   if (sign_number==0)
   {glBindTexture(GL_TEXTURE_2D,texture[3]);}
   if (sign_number==1)
   {glBindTexture(GL_TEXTURE_2D,texture[24]);}
   if (sign_number==2)
   {glBindTexture(GL_TEXTURE_2D,texture[25]);}
   if (sign_number==3)
   {glBindTexture(GL_TEXTURE_2D,texture[26]);}

   glColor3f(1,1,1); //deafult all white space 
   cube(0,0,0,0.05,0.8,0.1);
   cylinder1(0.02,0.9,0.1,0.15,0.01);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   obj_color=0;
} 
static void traffic_light(double x,double y,double z, double dx,double dy,double dz,
                  double th)
{
//set spectur color for my object before transformation
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //start texture
   glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1); //deafult all white space 
  glBindTexture(GL_TEXTURE_2D, texture[9]);
  
cube(0,0,0,0.05,1,0.1);
cube(0,1,0,1,1.04,0.1);
cube(0.7,0.9,0.05,1,1.06,0.1);
 glDisable(GL_TEXTURE_2D);
//three light
int t_i=25;
double t_cur=fmod(DEF_t,t_i);
if ( t_cur >=0 && t_cur <=10 )
{
   ball2(0.8,1,0.06,0.05, 1,0,0); 
}
else{ball2(0.8,1,0.06,0.05,1,1,1); }
if ( t_cur >=10 &&t_cur <=15 )
{   
ball2(0.8+0.05,1,0.06,0.05,1,1,0); 
} 
else{ ball2(0.8+0.05,1,0.06,0.05,1,1,1); }
if ( t_cur >=15 &&t_cur <=25 )
{ 
       ball2(0.8+0.05*2,1,0.06,0.05,0,1,0);  
}
else{ball2(0.8+0.05*2,1,0.06,0.05,1,1,1);  }

   glPopMatrix();
  
} 
static void house2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, int texture_1, int texture_2)
{
//set spectur color for my object before transformation
   obj_color=1;
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //start texture
   glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1); //deafult all white space 
   glBindTexture(GL_TEXTURE_2D,texture[texture_2]);
   cone(0,0,0,0.2,0.4,10,1,0,0);
   glBindTexture(GL_TEXTURE_2D,texture[texture_1]);
   cylinder2(0,-0.2,0,0.2,0.2,90);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   obj_color=0;

}
void trapezoid(double x,double y, double z,double h,double th){
    glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
glScaled(h,h,h);
glRotatef(th,1,0,0);  
 glBegin(GL_QUADS);
   //  Front
   glColor3f(1,1,1);
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(0.5,1, 0.75);
   glTexCoord2f(1,0); glVertex3f(1.5,1, 0.75);
   glTexCoord2f(1,1); glVertex3f(2,0, 1);
   glTexCoord2f(0,1); glVertex3f(0,0, 1);
   //  Back
   glNormal3f(0,0.25,-1);
   glTexCoord2f(0,0); glVertex3f(1.5,1, 0.25);
   glTexCoord2f(1,0); glVertex3f(0.5,1, 0.25);
   glTexCoord2f(1,1); glVertex3f(0,0, 0);
   glTexCoord2f(0,1); glVertex3f(2,0, 0);
   //left 
   glNormal3f(-0.5,0.25,0);
   glTexCoord2f(0,0); glVertex3f(0.5,1, 0.25);
   glTexCoord2f(1,0); glVertex3f(0.5,1, 0.75);
   glTexCoord2f(1,1); glVertex3f(0,0, 1);
   glTexCoord2f(0,1); glVertex3f(0,0, 0);
   //  right 
   glNormal3f(0.5,0.25,0);
   glTexCoord2f(0,0); glVertex3f(1.5,1, 0.75);
   glTexCoord2f(1,0); glVertex3f(1.5,1, 0.25);
   glTexCoord2f(1,1); glVertex3f(2,0, 0);
   glTexCoord2f(0,1); glVertex3f(2,0, 1);

   //bottom
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0); glVertex3f(0,0, 0); 
   glTexCoord2f(1,0); glVertex3f(2,0, 0); 
   glTexCoord2f(1,1); glVertex3f(2,0, 1); 
   glTexCoord2f(0,1); glVertex3f(0,0, 1); 

   //top
    glNormal3f(0,-1,0);
   glTexCoord2f(0,0); glVertex3f(0.5,1, 0.25); 
   glTexCoord2f(1,0); glVertex3f(1.5,1, 0.25); 
   glTexCoord2f(1,1); glVertex3f(1.5,1, 0.75); 
   glTexCoord2f(0,1); glVertex3f(0.5,1, 0.75); 
   glEnd();
   glPopMatrix();
}
static void car(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
//set spectur color for my object before transformation
 obj_color=1;
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //start texture
   glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1); //deafult all white space 
   glBindTexture(GL_TEXTURE_2D,texture[19]);
   trapezoid(0.1,0.2,0.02,0.26,0);
    glBindTexture(GL_TEXTURE_2D,texture[20]);
 cube(0,0,0,0.7,0.2,0.3);
cube(0.3,0.45,0.1,0.35,0.5,0.2);
 glBindTexture(GL_TEXTURE_2D,texture[21]);
cylinder1(0.6,0,0,0.06,0.03);
cylinder1(0.1,0,0,0.06,0.03);
cylinder1(0.1,0,0.3,0.06,0.03);
cylinder1(0.6,0,0.3,0.06,0.03);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
    obj_color=0;

}

void sphere2(int n)
{
   for (int i=0;i<n;i++)
   {
       for (int j=0;j<n;j++)
       {
     for (int k=0;k<n;k++)
{
       sphere(i*0.1,j*0.1,k*0.1,0.01);}}}
}
void firework(double x,double y,double z,double r)
{
    obj_color=1;
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(r,r,r);
   int n;
   double t_cur=fmod(DEF_t,1);
   if (t_cur>=0 && t_cur<0.25)
   {
      n=0;sphere2(n);
   }
    if (t_cur>=0.25 && t_cur<0.5)
   {
      n=3;sphere2(n);
   }
    if (t_cur>=0.5 && t_cur<0.75)
   {
      n=6;sphere2(n);
   }
    if (t_cur>=0.75 && t_cur<1)
   {
      n=10;sphere2(n);
   }
   glPopMatrix();
   obj_color=0;
}
void firework2()
{
   int arr[9];
   for (int i=0;i<=8;i++)
   {
      arr[i]=(rand()) %20;
   }
   for (int i=0;i<=2;i++)
   {
  firework(-1+0.1*arr[i*3],1+0.02*arr[i*3+1],-1+0.1*arr[i*3+2],0.5);
   }
}
void mailbox(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
    obj_color=1;
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //start texture
   glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1); //deafult all white space 
   glBindTexture(GL_TEXTURE_2D,texture[22]);
   cube(0,0,0,0.1,1,0.2);
   cube(-1,1,0.1,1,1.5,0.5);
     glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   obj_color=0;
}
void road(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
    obj_color=1;
   float white[] = {1,1,1,1};
   //Emission light
   float black[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
      glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Offset
   //start texture
   glEnable(GL_TEXTURE_2D);
   //texture env,can chnage to original texture or texture with exitsing colors
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1); //deafult all white space 
   glBindTexture(GL_TEXTURE_2D,texture[23]);
   cube(-0.5,-0.02,-2,0.5,-0.01,-2.4);
     glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   obj_color=0;
}

static void Sky(double D)
{
   //  Textured white box dimension (-D,+D)
   glPushMatrix();
   glScaled(D,D,D);
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(0.25,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(0.25,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.00,1); glVertex3f(-1,+1,-1);

   glTexCoord2f(0.25,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(0.50,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.50,1); glVertex3f(+1,+1,+1);
   glTexCoord2f(0.25,1); glVertex3f(+1,+1,-1);

   glTexCoord2f(0.50,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.75,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(0.75,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.50,1); glVertex3f(+1,+1,+1);

   glTexCoord2f(0.75,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1.00,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1.00,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.75,1); glVertex3f(-1,+1,+1);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,sky[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.5,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(0.5,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0,1); glVertex3f(-1,+1,-1);

   glTexCoord2f(1.0,1); glVertex3f(-1,-1,+1);
   glTexCoord2f(0.5,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.5,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1.0,0); glVertex3f(-1,-1,-1);
   glEnd();

   //  Undo
   glDisable(GL_TEXTURE_2D);
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
   //set original is perspect mode
  if (m==1)
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
   //skybox texture
   if (box) 
   {Sky(3*dim);}
   //  Light switch from hw5, initlize light on which =1
   if (!light)
   {      
      glDisable(GL_LIGHTING);
   }
else
   {
      //  Translate intensity to color vectors,distance be 5, same with hw5 distance constant
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
    //  Draw scene
    
   for (int i=-1;i<=1;i++)
   {
            house(-1.5,0,i , 0.4,0.4,0.4 , 45,12+i,12+3+i);  
            house2(-1.7,0+0.4,i-1 , 1,1,1,45,15-i ,12+3+i);
   }
   tree(-1.5,0.5,-2.5,0.5,0.5,0.5,0);
   
   for (int i=-1;i<=1;i++) 
   {
             house(1.1,0,i , 0.4,0.4,0.4 , -45,12+3+i,12+i);
             house2(1.3,0+0.4,i-0.3 , 1,1,1,-45,15-3-i,12+i );
   }
    tree(1.5,0.5,-2.5,0.5,0.5,0.5,0);
   
   for (int i=0;i<=2;i++) 
   {
             tree2(0.6,0+0.2,1.5-i , 0.3,0 );
             tree2(-0.6,0+0.2,1.5-i , 0.3,0 );
   }
  car(0.25,0,-1,0.8,0.8,0.8,90); 
  car(-0.35,0,-1.5,0.8,0.8,0.8,90);
 
 obstacle();
 sign(0.8,0,-1,0.5,0.5,0.5,0,0);
 sign(-0.8,0,-1.5,0.5,0.5,0.5,90,0);
 //cross sign
 sign(0.8,0,-1.6,0.5,0.5,0.5,0,1);
 //yiled sign
 sign(0.7,0,0,0.5,0.5,0.5,0,2);
 //trun sign
 sign(-0.7,0,-1,0.5,0.5,0.5,0,3);

 //sign(1,0,0, 1,1,1,0);
 traffic_light(0,0.2,-3, 1,1,1,0);
 traffic_light(1,0,-2, 1,1,1,90);
 traffic_light(-1,0,-2.5,1,1,1,-90);
 person(-1,0,-2.5, 0.5,0.5,0.5,90);
  person(1,0,-2, 0.5,0.5,0.5,-90);
  //
   person(0,0,1,0.5,0.5,0.5,0);
    person(-0.25,0,1,0.5,0.5,0.5,0);
   person(0.25,0,1,0.5,0.5,0.5,0);
  //firework2();
  //
  for (int i=0;i<=2;i++)
  {
    mailbox(-0.9,0,-0.4+i,0.1,0.1,0.1,0);
    mailbox(0.9,0,-0.4+i,0.1,0.1,0.1,0);
  }
  road(0,0,0,1,1,1,0);
   glUseProgram(shader[mode_s]);
   //  For brick shader set "uniform" variables
   if (mode_s >0)
   {
      float time = roll ? 0.001*glutGet(GLUT_ELAPSED_TIME) : 0;
      int id = glGetUniformLocation(shader[mode_s],"Xcenter");
      glUniform1f(id,X);
      id = glGetUniformLocation(shader[mode_s],"Ycenter");
      glUniform1f(id,Y);
      id = glGetUniformLocation(shader[mode_s],"Zoom");
      glUniform1f(id,Z);
      id = glGetUniformLocation(shader[mode_s],"time");
      glUniform1f(id,time);
   }
if(fire==1){firework2();}
   //  Draw the objects
   glDisable(GL_TEXTURE_2D);
   
   //  Revert to fixed pipeline
   glUseProgram(0);
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
   //  Display function from ex15
   glWindowPos2i(6,6);
   Print("Angle=%d,%d  Dim=%.1f time=%.1f Light=%s Texture=%s",th,ph,dim,DEF_t,light?"On":"Off",mode?"Replace":"Modulate");
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   //  Render the scene and make it visible
   //remove zbufer after display 
   glFlush();
   glutSwapBuffers();
}

//  function when the window is resized
void idle()
{
   //another way prevent object change shape when window resize
   //  Elapsed time in seconds
   double t=glutGet(GLUT_ELAPSED_TIME)/1000.0;
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
   Project(m?fov:0,asp,dim);
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
   {
     th =0;
      ph = 0; 
   }
   //  Toggle axes
   else if (ch == 'x')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l')
      light = 1-light; 
   //  Switch projection mode
   else if (ch == 'p')
      mode = 1-mode; 
   // M         orthorgonal/perceptive mode(optional key)  
   else if (ch == 'M')
      m = 1-m;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   else if (ch == 'm')
      move = 1-move;
//fov,rep key may need for the future
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
   //skybox
   else if (ch=='b')
        box=1-box;
   //firework
    else if (ch=='f')
        fire=1-fire;
   //top scene
   else if (ch=='9')
        ph=30, dim=2.8;
   // shader on/off
   /*else if ( ch== 'B')
    mode_s=1-mode_s;*/
     //  Toggle textures mode,not change
   // else if (ch == 't') ntex = 1-ntex;
   //  Translate shininess power to value (-1 => 0)
      //shininess increase by power of 2
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
    Project(m?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutIdleFunc(move?idle:NULL);

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
    Project(m?fov:0,asp,dim);
}

//below 5 functions, code from ex25, for firework vertex shader 
char* ReadText(char *file)
{
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   int n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}
 void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}


//main function

int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(700,700);
   glutCreateWindow("Project_Zhanchao_Yang");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   // Load textures, texture from ex14,ex15
   texture[0] = LoadTexBMP("door.bmp");
   texture[1] = LoadTexBMP("window.bmp");
   texture[2] = LoadTexBMP("wood.bmp");
   texture[3] = LoadTexBMP("stop.bmp");
   texture[4] = LoadTexBMP("brick.bmp");
   texture[5] = LoadTexBMP("hand.bmp");
   texture[6] = LoadTexBMP("clothes.bmp");
   texture[7] = LoadTexBMP("person_4.bmp");
   texture[8] = LoadTexBMP("leg.bmp");
   texture[9] = LoadTexBMP("post.bmp");
   texture[10] = LoadTexBMP("obstacle.bmp");
   //6 different wall
   texture[11] = LoadTexBMP("brick.bmp");
   texture[12] = LoadTexBMP("crate.bmp");
   texture[13] = LoadTexBMP("block.bmp");
    texture[14] = LoadTexBMP("water.bmp");
    texture[15] = LoadTexBMP("wall_3.bmp");
    texture[16] = LoadTexBMP("wall_2.bmp");
    texture[17] = LoadTexBMP("leaf.bmp");
    texture[18] = LoadTexBMP("branch.bmp");
    texture[19] = LoadTexBMP("car_2.bmp");
    texture[20] = LoadTexBMP("car_1.bmp");
    texture[21] = LoadTexBMP("wheel.bmp");
     texture[22] = LoadTexBMP("mailbox.bmp");
    texture[23] = LoadTexBMP("crosswalk.bmp");
    //3 other traffic sign
   texture[24] = LoadTexBMP("cross_sign.bmp");
     texture[25] = LoadTexBMP("yield_sign.bmp");
    texture[26] = LoadTexBMP("turn_sign.bmp");
   sky[0] = LoadTexBMP("skybox1.bmp");
   
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   shader[1] = CreateShaderProg("simple.vert","simple.frag");
 
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
