// 
// Torbert, 1 May 2013
// 
// OpenGL Demo, 3-D Example
// 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
//
double pi=3.1415926;
//
int    w=640,h=480;
//
double rho,phi,theta;
double up=1.0;
//
double xc,yc,zc;
double xe,ye,ze;
//
void display(void)
{
   double t;
   //
   glClear(GL_COLOR_BUFFER_BIT); // clear the screen
   glColor3f(0.0,0.0,0.0);
   //
   glutWireTeapot(1.0);
   //
   glutSwapBuffers();
}
void look()
{
   xe=xc+rho*sin(theta)*sin(phi); // y
   ye=yc+rho*cos(theta);          // z
   ze=zc+rho*sin(theta)*cos(phi); // x
   //
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(xe,ye,ze, xc,yc,zc, 0.0,up,0.0);
}
void idle(void)
{
   phi+=0.01;
   //
   look();
   glutPostRedisplay();
}
void mouse(int button,int state,int xscr,int yscr)
{
}
void motion(int xscr,int yscr)
{
}
void mouse_wheel(int wheel,int direction,int xscr,int yscr)
{
}
void keyfunc(unsigned char key,int xscr,int yscr)
{
   if(key=='q')
   {
      exit(0);
   }
}
void reshape(int wscr,int hscr)
{
   GLfloat aspect_ratio;
   //
   w=wscr;
   h=hscr;
   aspect_ratio=(GLfloat)w/(GLfloat)h;
   glViewport(0,0,(GLsizei)w,(GLsizei)h);
   //
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0,aspect_ratio,0.1,500.0);
   //
   look();
}
int main(int argc,char* argv[])
{  
   rho   = 3.1;
   phi   = 0.0;
   theta = 0.5*pi;
   //
   xc=yc=zc=0.0;
   //
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(w,h);
   glutInitWindowPosition(100,50);
   glutCreateWindow("OpenGL Demo");
   //
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   //
   glutDisplayFunc(display);
   glutIdleFunc(idle);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutMouseWheelFunc(mouse_wheel);
   glutKeyboardFunc(keyfunc);
   glutReshapeFunc(reshape);
   //
   glutMainLoop();
   //
   return 0;
}
//
// end of file
//
