//Jake Shankman
//Parallel 2 Period 7
//2/15/13
#include <stdio.h>
#include <math.h>
int main(void)
{
  char command[150];
  //Floor
  double floor = 0.333333;
  //Light
  double lx=0.000000;
  double ly=1.000000;
  double lz=-0.500000;
  //Eye
  double ex = 0.500000;
  double ey = 0.500000;
  double ez = -1.000000;
  //
  //Screen
  double z = 0.000000;
  //
  double array[3][7];
  //Sphere 0
  array[0][0] = 0.0;
  array[0][1] = 0.500000;
  array[0][2] = 0.166667;
  array[0][3] = 0.166667;
  array[0][4] = 0.0;
  array[0][5] = 0.0;
  array[0][6] = 255.0;
  
  //Sphere 1
  array[1][0] = 0.833333;
  array[1][1] = 0.500000;
  array[1][2] = 0.500000;
  array[1][3] = 0.166667;
  array[1][4] = 0.0;
  array[1][5] = 255.0;
  array[1][6] = 0.0;

  //Sphere 2
  array[2][0] = 0.333333;
  array[2][1] = 0.666667;
  array[2][2] = 0.666667;
  array[2][3] = 0.333333;
  array[2][4] = 255.0;
  array[2][5] = 0.0;
  array[2][6] = 0.0;

  FILE *fout;
  int frame;
  for(frame = 0; frame < 200; ++frame)
  {
     fout=fopen("tmpimg.ppm","w");
     fprintf(fout, "P3\n 400 300\n 255\n");
     int i, j;
  for(j = 0; j < 300; ++j)
  {
    for(i = 0; i < 400; ++i)
    {
    double px = 1.0*i/400;
    double py = 1-(1.0*j/300);
    double pz = 0.0;
    
    double dx = px - ex;
    double dy = py - ey;
    double dz = pz - ez;
    
    double magnitude = sqrt(dx*dx + dy*dy + dz*dz);
    
    dx /= magnitude;
    dy /= magnitude;
    dz /= magnitude;
    
    int sphere = -1;
    int s;
    double tmin;
    for(s = 0; s < 3; ++s)
    {
      double a, b, c;
      a = 1.0;
      b = 2*(dx*(ex - array[s][0]) + dy*(ey - array[s][1]) + dz*(ez - array[s][2]));
      c = pow((ex - array[s][0]), 2) + pow((ey - array[s][1]), 2) + pow((ez - array[s][2]), 2) -(array[s][3] * array[s][3]);
      double t;
      if(pow(b, 2) - 4*a*c > 0)
      {
	double t1 = (-b - sqrt(b*b - 4*a*c))/(2 * a);
	double t2 = (-b + sqrt(b*b - 4*a*c))/(2 * a);
	t = fmin(t1, t2);
	if(sphere < 0 || t < tmin)
	{
	sphere = s;
	tmin = t;
	}
      }
    }
    if(sphere == -1)
    {
      
 //    floor calculations
      
      tmin=(floor-ey)/dy;
      
      if(tmin > 0.0)
      {
	double xi, yi, zi;
	xi = ex + tmin*dx;
	yi = floor;
	zi = ez + tmin*dz;
	double red, green, blue;
	if(((int)(xi/0.1 + 9999) + (int)(zi/0.1 + 9999)) %2 == 0)
	{
	  red = 255.0;
	  green = 0.0;
	  blue =  255.0;
	}
	else
	{
	red = 255.0;
	green = 255.0;
	blue = 0.0;
	}
	double Lx = lx - xi;
	double Ly = ly - yi;
	double Lz = lz - zi;
    
	double magnitude = sqrt(Lx*Lx + Ly*Ly + Lz*Lz);
    
	Lx /= magnitude;
	Ly /= magnitude;
	Lz /= magnitude;
	int sphere = -1;
	int s;
	double tmin;
	for(s = 0; s < 3; ++s)
	{
	  double a, b, c;
	  a = 1.0;
	  b = 2*(Lx*(xi - array[s][0]) + Ly*(yi - array[s][1]) + Lz*(zi - array[s][2]));
	  c = pow((xi - array[s][0]), 2) + pow((yi - array[s][1]), 2) + pow((zi - array[s][2]), 2) -(array[s][3] * array[s][3]);
	  double t;
	  if(pow(b, 2) - 4*a*c > 0)
	  {
	    double t1 = (-b - sqrt(b*b - 4*a*c))/(2 * a);
	    double t2 = (-b + sqrt(b*b - 4*a*c))/(2 * a);
	    t = fmin(t1, t2);
	    if(sphere < 0 || t < tmin)
	    {
	    sphere = s;
	    tmin = t;
	    }
	  }
	}
	red *= 0.4;
	green *= 0.4;
	blue *= 0.4;
	if(sphere == -1)
	{
	  double nx = 0;
	  double ny = 1;
	  double nz = 0;
	  double dot = (nx * Lx) + (ny * Ly) + (nz * Lz);
	  if(dot > 0)
	  {
	    red += 0.6*dot*255.0;
	    blue += 0.6*dot*0;
	    green += 0.6*dot*255.0;
	  }
	  fprintf(fout, "%d %d %d\n", (int)red, (int)green, (int)blue);
	}
	else
	{
	  fprintf(fout, "%d %d %d\n", (int) red, (int) green, (int) blue);
	}
      }
      else
      {
	fprintf(fout, "0 0 0\n");
      }
    }
    else
    {
      //in here perform checking for shadow/light source
      double red = array[sphere][4];
      double green = array[sphere][5];
      double blue = array[sphere][6];
      double xi, yi, zi;
	xi = ex + tmin*dx;
	yi = ey + tmin*dy;
	zi = ez + tmin*dz;
	double Lx = lx - xi;
	double Ly = ly - yi;
	double Lz = lz - zi;
    
	double magnitude = sqrt(Lx*Lx + Ly*Ly + Lz*Lz);
    
	Lx /= magnitude;
	Ly /= magnitude;
	Lz /= magnitude;
	int sp = sphere;
	int sphere = -1;
	int s;
	double tmin;
	for(s = 0; s < 3; ++s)
	{
	  double a, b, c;
	  a = 1.0;
	  b = 2*(Lx*(xi - array[s][0]) + Ly*(yi - array[s][1]) + Lz*(zi - array[s][2]));
	  c = pow((xi - array[s][0]), 2) + pow((yi - array[s][1]), 2) + pow((zi - array[s][2]), 2) -(array[s][3] * array[s][3]);
	  double t;
	  if(pow(b, 2) - 4*a*c > 0)
	  {
	    double t1 = (-b - sqrt(b*b - 4*a*c))/(2 * a);
	    double t2 = (-b + sqrt(b*b - 4*a*c))/(2 * a);
	    t = fmin(t1, t2);
	    if((sphere < 0 || t < tmin ) && t > 0)
	    {
	    sphere = s;
	    tmin = t;
	    }
	  }
	}
	red *= 0.4;
	green *= 0.4;
	blue *= 0.4;
	if(sphere == -1)
	{
	  double nx = (xi - array[sp][0])/array[sp][3];
	  double ny = (yi - array[sp][1])/array[sp][3];
	  double nz = (zi - array[sp][2])/array[sp][3];
	  double dot = (nx * Lx) + (ny * Ly) + (nz * Lz);
	  if(dot > 0)
	  {
	    red += 0.6*dot*array[sp][4];
	    blue += 0.6*dot*array[sp][6];
	    green += 0.6*dot*array[sp][5];
	  }
	  fprintf(fout, "%d %d %d\n", (int)red, (int)green, (int)blue);
	}
	else
	{
	  fprintf(fout, "%d %d %d\n", (int) red, (int) green, (int) blue);
	}
    }
    
    }
    
   }
  
  fclose(fout); 
  array[0][0] += 0.005;
  sprintf(command,"convert tmpimg.ppm frame%03d.png",frame);
  system(command);
  }
  
  
  return 0;
}




//solve quadratic in the if statement as a method and return result.... saves time for doing it multiple times
solveQuadratic(double a, double b, double c)
{
  //put cast ray solve quadratic code in here
  //need to pass array, eye, distance, t and sphere
}
double* normalize(double x, double y, double z)
{
  //double* array = result... then can treat as an array
  double mag = sqrt((x*x) + (y*y) + (z*z));
  x /= mag;
  y /= mag;
  z /= mag;
  double array[3];
  array[0] = x;
  array[1] = y;
  array[2] = z;
  return array;
}
double dotProduct(double nx, double ny, double nz, double Lx, double Ly, double Lz)
{
  //takes arguements of the xyz components of both arrays, multiplies and returns a double
  return (nx * Lx) + (ny * Ly) + (nz * Lz);
}

//
//
//end of file
//
//