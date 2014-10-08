// 
// Torbert, 5 November 2012
// 
// gcc -lm -lGLU -lglut -o demo demo.c
// 
#include <stdio.h>
#include <math.h>
// 
#include <GL/glut.h>
// 
// center of circle... 
// 
double x=200.0;
double y=150.0;
// 
void displayfunc(void)
{
   double t;
   //
   glClear(GL_COLOR_BUFFER_BIT); // clear the screen
   glColor3f(0.0,0.0,0.0);       // set color to black
   //
   glBegin(GL_POINTS);
   for(t=0.167*M_PI;t<1.833*M_PI;t+=0.01)
   { 
      glVertex2f(x+75.0*cos(t),y+75.0*sin(t));
   }
   glEnd();
   //
   glBegin(GL_LINE_STRIP);
   glVertex2f(x+75.0*cos(0.167*M_PI),y+75.0*sin(0.167*M_PI));
   glVertex2f(x                     ,y                     );
   glVertex2f(x+75.0*cos(1.833*M_PI),y+75.0*sin(1.833*M_PI));
   glEnd();
   //
   glFlush(); // for GLUT_DOUBLE buffering glutSwapBuffers() instead
}
int main(int argc,char* argv[])
{  
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // single buffering
   //
   glutInitWindowSize(400,300);
   glutInitWindowPosition(100,50);
   glutCreateWindow("OpenGL Demo");
   //
   glClearColor(1.0,1.0,1.0,0.0); // red, green, blue, alpha
   glShadeModel(GL_SMOOTH);
   //
   glViewport(0,0,(GLsizei)400,(GLsizei)300);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,400.0,0.0,300.0);
   glMatrixMode(GL_MODELVIEW);
   //
   glutDisplayFunc(displayfunc);    // register callback function
   //
   glutMainLoop();
   //
   return 0;
}
// 
// end of file
// 
