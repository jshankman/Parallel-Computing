//
//Jake Shankman 4/22/2013
//
#include <stdio.h>
#include <stdlib.h>
//
#define N 7

int allOne(double* matrix)
{
  int i;
  for(i = 0; i < N; ++i)
  {
    if(matrix[i] != 1.0) // matrix[i] is the same as *(matrix + i)
      return 0;
  }
  return 1;
}
//
int main(void)
{
  int b[N];
  //double x0[N], x1[N];
  double *x0 = (double*) malloc(sizeof(double) * N);
  double *x1;
  double *xtmp;
  int i, j;
  printf("x0: [");
  for (i = 0; i < N; ++i)
  {
    if(i == 0 || i == N-1)
    {
      b[i] = 2;
    }
    else
    {
      b[i] = 1;
    }
    x0[i] = rand()/(double)RAND_MAX;
    printf("%f ", x0[i]);
  }
  printf("]\n");

  //xi+1 = Dinverse(b - (R)xi)
  int count = 1;
  while(allOne(x0) != 1)
  {
    x1 = (double*) malloc(sizeof(double) * N);
    printf("x%d: [", count);
    for(i = 0; i < N; ++i)
    {
      if(i - 1 < 0)
      {
	x1[i] = (1.0/3)*(b[i] - (-1*x0[i + 1]));
      }
      else if( i + 1 > N)
      {
	x1[i] = (1.0/3)*(b[i] - (-1*x0[i - 1]));
      }
      else
      {
      x1[i] = (1.0/3)*(b[i] - (-1*x0[i-1] + -1*x0[i + 1]));
      }
      printf("%f ", x1[i]);
    }
    printf("]\n");
    xtmp = x0;
    x0 = x1;
    free(xtmp);
    ++count;
  }
  return 0;
}