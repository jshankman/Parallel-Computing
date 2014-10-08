// Note to self, for loops faster in gcc and mpicc
//
// Jake Shankman 8 Nov 2012
// Mandelbrot Set
// 
// gcc -lm -lGLU -lglut -o demo demo.c
// 
#include <stdio.h>
#include <math.h>
#include "mpi.h"
// 
#include <GL/glut.h>
#define maxW 400.0
#define maxH 300.0
//
#include <complex.h>
//
//globally used stuff
double xMin = -2.0;
double xMax = 2.0;
double yMin = -1.5;
double yMax = 1.5;
int w, h;
double x = 200.0;
double y = 150.0;
int maxLoop = 100;
//MPI stuff
int rank,size;
MPI_Status status;
int tag = 0;
//
void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
	double dx, dy;
	dx = (xMax - xMin);
	dy = (yMax - yMin);
	
	double x1 = xMin + dx * xscr/maxW ;
	double y1 = yMin + dy * (yscr)/maxH ;
	
	dx *= 0.5;
	dy *= 0.5;
	
	xMin = x1 - 0.5*dx;
	xMax = x1 + 0.5*dx;
	
	yMin = y1 - 0.5*dy;
	yMax = y1 + 0.5*dy;
	
	glutPostRedisplay();
      }
    }
   else if(button==GLUT_RIGHT_BUTTON)
     if(state ==GLUT_DOWN)
      {
	maxLoop *= 2;
	glutPostRedisplay();
	//printf("\nloop doubled\n");
      }
}
void motionfunc(int xscr,int yscr)
{
   printf("(x,y)=(%d,%d)\n",xscr,yscr);
}
void displayfunc(void)
{
   //printf("\nxMin=%f\nxMax=%f\nyMin=%f\nyMax=%f\n", xMin, xMax, yMin, yMax);
   glClear(GL_COLOR_BUFFER_BIT);
   glutSwapBuffers();
   double a, b;
   int k, i, j, loop, worker;
   int location[size-1][2];
  for(worker = 1; worker < size; ++worker)
  {
      a = (xMax - xMin)*(1.0 *i/maxW) + xMin;
      b = (yMax - yMin)*(1.0 *(maxH - j)/maxH) + yMin;
      
      MPI_Send(&a,1,MPI_DOUBLE,worker,tag,MPI_COMM_WORLD);
      MPI_Send(&b,1,MPI_DOUBLE,worker,tag,MPI_COMM_WORLD);
      MPI_Send(&maxLoop,1,MPI_INT,worker,tag,MPI_COMM_WORLD);
      location[worker - 1][0] = i;
      location[worker - 1][1] = j;
      ++j;
  }
   for(i = 0; i < maxW; ++i)
  {
    for(j = 0; j < maxH; ++j)
    {
      MPI_Recv(&loop,1,MPI_INT,MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);     
      k = status.MPI_SOURCE;
      
      if(loop == maxLoop)
      {
	glColor3f(0.0, 0.0, 0.0);
      }
      else
      {
	glColor3f(0.0, pow(1-(1.0*loop/maxLoop), 3), 1 - pow(1 - (1.0*loop/maxLoop), 3));
      }
      //do OpenGL stuff
      glBegin(GL_POINTS);
      glVertex2f(location[k-1][0], location[k-1][1]);
      glEnd();
      
      a = (xMax - xMin)*(1.0 *i/maxW) + xMin;
      b = (yMax - yMin)*(1.0 *(maxH - j)/maxH) + yMin;
      
      MPI_Send(&a,1,MPI_DOUBLE,k,tag,MPI_COMM_WORLD);
      MPI_Send(&b,1,MPI_DOUBLE,k,tag,MPI_COMM_WORLD);
      MPI_Send(&maxLoop,1,MPI_INT,k,tag,MPI_COMM_WORLD);
      
      location[k - 1][0] = i;
      location[k - 1][1] = j;
    }
    j = 0;
    glutSwapBuffers();
   }
   for(worker = 1; worker < size; ++worker)
   {
     MPI_Recv(&loop,1,MPI_INT,MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);     
      k = status.MPI_SOURCE;
      
      if(loop == maxLoop)
      {
	glColor3f(0.0, 0.0, 0.0);
      }
      else
      {
	glColor3f(0.0, pow(1-(1.0*loop/maxLoop), 3), 1 - pow(1 - (1.0*loop/maxLoop), 3));
      }
      //do OpenGL stuff
      glBegin(GL_POINTS);
      glVertex2f(location[k-1][0], location[k-1][1]);
      glEnd();
   }
   glutSwapBuffers();
   // for GLUT_DOUBLE buffering glutSwapBuffers() instead
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
void closefunc(void)
{
   printf("\nWindow closed.\n");
   double d = 0;
   int worker, exit = -1;
   for(worker = 1; worker < size; ++worker)
   {
     MPI_Send(&d,1,MPI_DOUBLE,worker,tag,MPI_COMM_WORLD);
     MPI_Send(&d,1,MPI_DOUBLE,worker,tag,MPI_COMM_WORLD);
     MPI_Send(&exit,1,MPI_INT,worker,tag,MPI_COMM_WORLD);
   }
   MPI_Finalize();
   
}
int main(int arr,char* array[])
{  
  MPI_Init(&arr,&array);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  if(rank == 0) // manager
  {
    glutInit(&arr,array);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // single buffering
    //
    glutInitWindowSize(400,300);
    glutInitWindowPosition(100,50);
    glutCreateWindow("Jake Shankman's Mandelbrot Set");
    //
    glClearColor(1.0,1.0,1.0,0.0); // red, green, blue, alpha
    glShadeModel(GL_SMOOTH);
    //
    glViewport(0,0,(GLsizei)400,(GLsizei)300);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,400.0,0.0,300.0);
    glMatrixMode(GL_MODELVIEW);
    
    glutDisplayFunc(displayfunc); // register callback function
    glutIdleFunc(NULL);
    //
    glutMouseFunc(mousefunc);
    glutMotionFunc(motionfunc);
    glutReshapeFunc(reshapefunc);
    glutWMCloseFunc(closefunc);
    //
    glutMainLoop();
  }
  else //worker
  {
    double a, b;
    int maxLoop, loop;
    complex z;
    while(1)
    {
      MPI_Recv(&a,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD,&status);
      MPI_Recv(&b,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD,&status);
      MPI_Recv(&maxLoop,1,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
      if(maxLoop == -1)
	break;
      z = 0;
      for(loop = 0; loop < maxLoop; ++loop)
      {
	if(cabs(z) > 2.0)
	{
	  break;
	}
	z = (z*z) + (a + (b*I));
      }
      MPI_Send(&loop,1,MPI_INT,0,tag,MPI_COMM_WORLD);
      
    }
  }
  MPI_Finalize();
   //
   return 0;
// 
// end of file
// 
}
