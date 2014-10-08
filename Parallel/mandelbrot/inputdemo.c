// 
// Torbert, 5 November 2012
// 
// gcc -lm -lGLU -lglut -o inputdemo inputdemo.c
// 
#include <stdio.h>
#include <math.h>
// 
#include <GL/glut.h>
// 
int w,h; 
// 
double x=200.0;
double y=150.0;
// 
void displayfunc(void)
{
   double t;
   //
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0,0.0,0.0);
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
   glutSwapBuffers();
}
void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON)
      if(state==GLUT_DOWN)
      {
         x=1.0*(  xscr);
         y=1.0*(h-yscr);      // pixel y-coordinates inverted
         //
         glutPostRedisplay(); // callback registered displayfunc
      }
}
void motionfunc(int xscr,int yscr)
{
   printf("(x,y)=(%d,%d)\n",xscr,yscr);
}
void keyfunc(unsigned char key,int xscr,int yscr)
{
   printf("key = '%c'\n",key);
}
void closefunc(void)
{
   printf("Window closed.\n");
}
void reshapefunc(int wscr,int hscr)
{
   w=wscr; h=hscr;
   glViewport(0,0,(GLsizei)w,(GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*w,0.0,1.0*h);
   glMatrixMode(GL_MODELVIEW);
}
int main(int argc,char* argv[])
{  
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // double buffering
   //
   glutInitWindowSize(400,300);
   glutInitWindowPosition(100,50);
   glutCreateWindow("OpenGL Demo");
   //
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   //
   glutDisplayFunc(displayfunc);    // register callback functions
   glutIdleFunc(NULL);             // no animation
   glutMouseFunc(mousefunc);
   glutMotionFunc(motionfunc);
   glutKeyboardFunc(keyfunc);
   glutReshapeFunc(reshapefunc);
   glutWMCloseFunc(closefunc);
   //
   glutMainLoop();
   //
   return 0;
}
// 
// end of file
// 
