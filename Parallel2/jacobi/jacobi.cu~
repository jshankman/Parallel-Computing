//
//Jake Shankman 4/22/2013
//
#include <stdio.h>
#include <stdlib.h>
//
#define N 7

__global__ void jacobi(int* bcuda, float* xcuda, float* x1)
{
  int rank = threadIdx.x; //flat model
  //
    if(rank - 1 < 0)
      {
	x1[rank] = (1.0f/3)*(bcuda[rank] - (-1*xcuda[rank + 1]));
      }
    else if(rank + 1 > N)
      {
	x1[rank] = (1.0f/3)*(bcuda[rank] - (-1*xcuda[rank - 1]));
      }
    else
      {
      x1[rank] = (1.0f/3)*(bcuda[rank] - (-1*xcuda[rank-1] + -1*xcuda[rank + 1]));
      }
}
int allOne(float* matrix)
{
  int i;
  for(i = 0; i < N; ++i)
  {
    if(matrix[i] != 1.0f) // matrix[i] is the same as *(matrix + i)
      return 0;
  }
  return 1;
}
//
int main(void)
{
  int b[N];
  int *bcuda;
  cudaMalloc((void**)&bcuda, sizeof(int)*N);
  //float x0[N], x1[N];
  float *x0 = (float*) malloc(sizeof(float) * N);
  float *x1;
  float *xcuda;
  float *xtmp;
  int i;
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
    x0[i] = rand()/(float)RAND_MAX;
    printf("%f ", x0[i]);
  }
  printf("]\n");
  cudaMalloc((void**)&xcuda, sizeof(float)*N);
  cudaMemcpy(bcuda, b, sizeof(int)*N, cudaMemcpyHostToDevice);
  //xi+1 = Dinverse(b - (R)xi)
  int count = 1;
  while(allOne(x0) != 1)
  {
    cudaMemcpy(xcuda, x0, sizeof(float)*N, cudaMemcpyHostToDevice);
    cudaMalloc((void**)&x1, sizeof(float)*N);
    printf("x%d: [", count);
    //
    dim3 dimGrid(1), dimBlock(N);
    //
    //do following for loop in parallel
    jacobi<<<dimGrid,dimBlock>>>(bcuda,xcuda, x1);
    xtmp = xcuda;
    xcuda = x1;
    cudaFree(xtmp);
    cudaMemcpy(x0, xcuda, sizeof(float)*N, cudaMemcpyDeviceToHost);    
    for(i = 0; i < N; ++i)
    {
      printf("%f ", x0[i]);
    }   
    printf("]\n");
    ++count;
  }
  return 0;
}