/*
 *  
 * Paul Kubala Assignment 4.
 *   I tried making a pac man inspired theme with my own shape
 * being a Ghost that was made by making cones and cylinders
 * something not discussed in class.
 *   I make a yellow sphere for pacman and different scales and translations
 * of cubes built in class to make the pots for pacman to eat
 * and the boundies of the scene.
 * I have added a perspective mode as well as animated a dot 
 * for the pacman 
 * Some of the code from example 9 was used for perspective. I did add
 * a feature where the display can be viewed from different angles at the 
 * start.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int mode=1;       //  Projection Mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int axes=0;       //  Display axes      
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
int x = 1;
int move=1;       //  Move light
int light=1;      //  Lighting



// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light




//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

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
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}




// Used Cubes from ex8 2.c but added the functionallity
// to pass color as variables
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double r, double g,
                 double b)
{
   // From EX13
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);


   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(r,g,b);
   glNormal3f( 0, 0, 1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(r,g,b);
   glNormal3f( 0, 0,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(r,g,b);
   glNormal3f(+1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(r,g,b);
   glNormal3f(-1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(r,g,b);
   glNormal3f( 0,+1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(r,g,b);
   glNormal3f( 0,-1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

/*
 *  Draw vertex in polar coordinates
 */
/*static void Vertex(double th,double ph)
{
   glColor3f(1 , 1 , 0);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}*/


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


static void sphereVertex(double th, double ph, double c)
{
    glColor3f(c , c , c);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
   x++;
}
// Vertext used to draw ghost
static void Vertex_ghost(double th,double ph)
{
   glColor3f(1 , 0 , 0);
   //glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}
// Vertext helper function to draw cylinder or 
// the base part of the Ghost
static void Vertex_cyn(double th, double rad)
{

   glColor3f(1 , 0 , 0);
   glVertex3d(Cos(th), rad, Sin(th));
}
/*
Draw a Pacman at (x,y,z)
with radius (r)
*/

static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}








static void pacman(double x, double y, double z, double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}


// Draw a Clyinder for the base of the Ghost under the sphere
// or the head
static void cylinder(double x, double y, double z, double r)
{
   const int d=5;
   int th;

   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(r,r/4,r);
   int rad = 2;


      glBegin(GL_TRIANGLE_STRIP);
      for (th=0;th<=360;th+=d)
      {
         glVertex3d(0, rad, 0);
         Vertex_cyn(th, rad);
         Vertex_cyn(th+d, rad);

         Vertex_cyn(th, -rad);
         Vertex_cyn(th + d, -rad);
         glVertex3d(0, -rad,0);

      }
      glEnd();
      glPopMatrix();

}


/*
Draw a Ghost Head
*/
static void ghost(double x, double y, double z, double r)
{
   const int d=5;
  
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);


   //  Latitude bands
   for (ph=d;ph<=90-2*d;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex_ghost(th,ph);
         Vertex_ghost(th,ph+d);
      }
      glEnd();
   }

   //Continueing Body

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex_ghost(0,90);
   for (th=0;th<=360;th+=d)
   {
      Vertex_ghost(th,90-d);
   }
   glEnd();


   //  Undo transformations
   glPopMatrix();
}

// Draw tenticals or cones for the ghost
static void tentical(double x, double y, double z, double r)
{

   const int d=10;
  
   int th;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(2*r,r/2,2*r);

   glBegin(GL_TRIANGLE_FAN);
   Vertex_ghost(0,-90);
   for (th=0;th<=(360*4);th+=d)
   {
      Vertex_ghost(th,(90-d));
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();

}

/*
 *  Draw a sphere from ex8.c
 *     at (x,y,z)
 *     radius (r)
 */
static void sphere2(double x,double y,double z,double r, double col)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {

         sphereVertex(th,ph,col);
         sphereVertex(th,ph+d,col);
      }
      glEnd();
   }

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
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);
   
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }




 //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);






   // Draw cubes for Pac Man

   cube(0,0,0 , 0.025,0.025,0.025 , 0, 1, 1, 1);
   cube(.25,0,0 , 0.025,0.025,0.025 , 0, 1, 1, 1);
   cube(.5,0,0 , 0.025,0.025,0.025 , 0, 1, 1, 1);
   cube(.75,0,0 , 0.025,0.025,0.025 , 0, 1, 1, 1);
   cube(1,0,0 , 0.025,0.025,0.025 , 0, 1, 1, 1);
   // Gap to go behind Ghost
   cube(2.0,0,0 , 0.025,0.025,0.025 , 0, 1, 1, 1);
   cube(2.35,-.5,0, 0.025,0.025,0.025 , 0, 1, 1, 1);
   cube(2.35,-.75,0, 0.025,0.025,0.025 , 0, 1, 1, 1);


   // Draw the Boundry
   // Upper Barier
   cube(0.5, .5, 0, 2.25, 0.1, 0.025 , 0, 0, 0, 1);
   // Vertical Left Wall
   cube(-1.75, -1.65, .1, .1, 2.25, 0.025 , 0, 0, 0, 1);
   // Vertical Right Wall
   cube(2.75, -.90, .1, .1, 1.5, 0.025 , 0, 0, 0, 1);
   // Bottom Wall
   cube(0.5, -1, .1, 1.5, 0.5, .025, 0, 0, 0, 1); 
   
   // Draw Pacman
   pacman(-.5,0,0,.2);

   // Blinking dot 
   if (x%2==0)
      sphere2(2.35,-0.03,0,.1,1);
   else
      sphere2(2.35,-0.03,0,.1,0);
   x++;

   // Draw Ghost  
   ghost(1.5,0,0,.2);
   cylinder(1.5,-.05,0,.2);
   tentical(1.5,-.21,.11,.3);
   tentical(1.5,-.21,-.11,.3);
   tentical(1.4,-.21,0,.3);
   tentical(1.6,-.21,0,.3);




  
 
   // White text
   glColor3f(1,1,1);
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,5);
   //  Display parameters
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Print the text string
   Print("Angle=%d,%d",th,ph);
   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
     //  Update projection
   Project();
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
   // Look to the east 
   else if (ch == 'e' || ch == 'E')
      {th = 0;
      th += 45;}
   // View to the West 
   else if (ch == 'w' || ch == 'W')
      {
         th = 0;
         th -= 45;
   }
   // View to the North 
   else if(ch == 'n' || ch == 'N'){
      th = 0;
      th = 90;

   }
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}



/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Objects");
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
