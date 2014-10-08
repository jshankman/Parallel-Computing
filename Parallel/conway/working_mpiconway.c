// 
// Jake Shankman 10 Dec 2012
// Conway in Serial
// 
// mpicc -lm -lGLU -lglut -o mpiconway mpiconway.c
// mpirun -np 4 -machinefile hosts.txt a.out
#include <stdio.h>
#include <math.h>
//#include <dos.h>
// 
#include "mpi.h"
#include <GL/glut.h>
#define maxW 400
#define maxH 300
//
//
//
int grid [(maxW/10)][(maxH/10)];
int flag = 0;
int w, h;
int sleep = 100;
int rank,size;
MPI_Status status;
int tag = 0;
void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
	int x, y;
	x = xscr/10;
	y = (maxH - yscr)/10;
	if(grid[x][y] == 0)
	{
	  grid[x][y] = 1;
	  printf("change\n");
	}
	else
	{
	  grid[x][y] = 0;
	}
	glutPostRedisplay();
      }
    }
   else if(button==GLUT_RIGHT_BUTTON)
     if(state ==GLUT_DOWN)
      {
	flag = !flag;
	printf("flag=%i\n", flag);
      }
}
void motionfunc(int xscr,int yscr)
{
   printf("(x,y)=(%d,%d)\n",xscr,yscr);
   mousefunc(GLUT_LEFT_BUTTON, GLUT_DOWN,xscr, yscr);
}
int neighbors(int xpos, int ypos)
{
  int count = 0;
  int i, j;
  
  for(i = -1; i < 2; ++i)
  {
    for(j = -1; j < 2; ++j)
    {
      if( i != 0 || j != 0)
      {
	
	int xnbr=xpos+i;
	int ynbr=ypos+j;
	
	if(xnbr<0) 
	  xnbr=maxW/10-1;
	if(xnbr>maxW/10-1)
	  xnbr=0;
	if(ynbr<0)
	  ynbr = maxH/10 -1;
	if(ynbr>maxH/10 -1)
	  ynbr = 0;
	if(grid[xnbr][ynbr] == 1)
	{
	  ++count;
	}
      }
    }
  }   
  return count;
}
int alive(int xpos, int ypos)
{
  int n = neighbors(xpos, ypos);
  if(grid[xpos][ypos] == 1)
  {
    if(n == 2 || n == 3)
    {
      return 1;
    }
    else
      return 0;
  }
  else
  {
    if(n == 3)
      return 1;
    else
      return 0;
  }
}
void idlefunc()
{
  int worker, b, k;
  int location[size - 1][2];
  
  if(flag == 0)
  {
   usleep(sleep);
   int i=0; 
   int j = 0;
   int next [(maxW/10)][(maxH/10)];
   for(worker = 1; worker < size; ++worker)
   {
     MPI_Send(&i,1,MPI_INT,worker,tag,MPI_COMM_WORLD);
     MPI_Send(&j,1,MPI_INT,worker,tag,MPI_COMM_WORLD);
     MPI_Send(grid,(maxW/10)*(maxH/10),MPI_INT,worker,tag,MPI_COMM_WORLD);
     location[worker - 1][0] = i;
     location[worker - 1][1] = j;
     ++j;
   }
   for(i = 0; i < maxW/10; ++i)
   {
     for(;j < maxH/10; ++j)
     {
      MPI_Recv(&b,1,MPI_INT,MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);     
      k = status.MPI_SOURCE;
      
      next[location[k-1][0]][location[k-1][1]] = b;
      
      MPI_Send(&i,1,MPI_INT,k,tag,MPI_COMM_WORLD);
     MPI_Send(&j,1,MPI_INT,k,tag,MPI_COMM_WORLD);
     MPI_Send(grid,(maxW/10)*(maxH/10),MPI_INT,k,tag,MPI_COMM_WORLD);
      
     location[k - 1][0] = i;
      location[k - 1][1] = j;
     }
     j = 0;
  }
  for(worker = 1; worker < size; ++worker)
   {
     MPI_Recv(&b,1,MPI_INT,MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);     
      k = status.MPI_SOURCE;
      next[location[k-1][0]][location[k-1][1]] = b;
   }
   for(i = 0; i < maxW/10; ++i)
   {
     for(j = 0; j < maxH/10; ++j)
       grid[i][j] = next[i][j];
   }
    glutPostRedisplay();
  }
}
void displayfunc(void)
{ 
   glClear(GL_COLOR_BUFFER_BIT);
   glutSwapBuffers();
   int i, j;
   for(i = 0; i < maxW/10; ++i)
   {
     for(j = 0; j < maxH/10; ++j)
     {
       if(grid[i][j] == 0)
       {
	 glColor3f(1.0, 1.0, 1.0);
       }
       else
       {
	 glColor3f(0.0, 0.0, 0.0);
       }
       int a, b;
       glBegin(GL_POINTS);
       for(a = i*10; a < i*10 + 10; ++a)
       {
	 for(b = j*10; b < j*10 + 10; ++b)
	 {
	   glVertex2f(a, b);
	 }
       }
       glEnd();
    }
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
  int worker, close = -1;
  for(worker = 1; worker < size; ++worker)
  {
    MPI_Send(&close,1,MPI_INT,worker,tag,MPI_COMM_WORLD);
  }
  printf("Window closed.\n");
  MPI_Finalize();
}
int main(int argc,char* argv[])
{ 
  //
  //SET UP GRID HERE!!!!
  //
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  

  
  if(rank == 0)
  {
    int i, j;
    for(i = 0; i < maxW/10; ++i)
      for(j = 0; j< maxH/10; ++j)
      {
      
	  grid[i][j] = rand() % 2;
      }
      
    grid[3][5] = 1;
    grid[4][6] = 1;
    grid[5][5] = 1;
    grid[5][4] = 1;
    grid[5][6] = 1;
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // single buffering
    //
    glutInitWindowSize(400,300);
    glutInitWindowPosition(100,50);
    glutCreateWindow("Jake Shankman's Game of Life");
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
    //CALL IDLE FUNCTION!!!!!
    glutIdleFunc(idlefunc);
    //
    glutMouseFunc(mousefunc);
    glutMotionFunc(motionfunc);
    glutReshapeFunc(reshapefunc);
    glutWMCloseFunc(closefunc);
    //
    //
    glutMainLoop();
  }
  else
  {
    int i, j, a;
    while(1)
    {
      MPI_Recv(&i,1,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
      if(i == -1)
      {
	break;
      }
      MPI_Recv(&j,1,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
      MPI_Recv(grid,(maxW/10)*(maxH/10),MPI_INT,0,tag,MPI_COMM_WORLD,&status);
      a = alive(i, j);
      MPI_Send(&a,1,MPI_INT,0,tag,MPI_COMM_WORLD);
    }
  }
  MPI_Finalize();
  //
   return 0;
}
// 
// end of file
// 
