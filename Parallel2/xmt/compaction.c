//
//Torbert, March 2013
//Jake Shankman 3/13/2013
// gcc -fopenmp filename
#include <stdio.h>
#include <omp.h>
//
#define N 8
#define logN 3
//
int main(void)
{
  int rank, size=N, j, a;
  //
  int data[N] = {5, 9, 7, 2, 2, 4, 1, 8};
  int B[N] = {0, 0, 1, 0, 1, 1, 0, 1};
  //
  //
  int tree[logN + 1][N];	    //wastes memory
  //
  omp_set_num_threads(size);
  printf("%d\n", size);
  //
  #pragma omp parallel private(rank) // time O(1)
  {
    rank = omp_get_thread_num();
    printf("%d ", rank);
    //
    tree[logN][rank] = B[rank];  // work O(N)
  }
  printf("\n\n");
  //
  for(j = logN - 1; j >= 0; --j)    // time O(logN)
  {
    size /= 2;
    //
    omp_set_num_threads(size);
    printf("%d %d\n", size, j);
    //
    #pragma omp parallel private(rank)
    {
      rank = omp_get_thread_num();
      printf("%d ", rank);
      //
      //pairwise independent sums... work O(N)
      //
      tree[j][rank] = tree[j+1][2*rank] + tree[j+1][2*rank + 1];
    }
    printf("\n\n");
  }
  printf("%d\n", tree[0][0]);
  //
  printf("\nPrefix calculations...\n");
  //
  int prefix[logN + 1][N];
  //
  prefix[0][0] = tree[0][0];
  int A[tree[0][0]];
  for(j = 1; j <= logN; ++j)
  {
    size *= 2;
    //
    omp_set_num_threads(size);
    printf("%d %d\n", size, j);
    //
    #pragma omp parallel private(rank)
    {
      rank = omp_get_thread_num();
      printf("%d ", rank);
      //
      //
      if(rank == 0)
      {
	prefix[j][rank] = tree[j][rank];
      }
      else if(rank %2 == 1)
      {
	prefix[j][rank] = prefix[j - 1][rank/2];
      }
      else
      {
	prefix[j][rank] = prefix[j - 1][rank/2 - 1] + tree[j][rank];
      }
      //
      //DO SOMETHING!!!!
      if(B[rank] == 1 && size == N)
      {
	A[prefix[j][rank] -1] = data[rank];
	
      }
    }
    printf("\n\n");
  }
  int x;
  for(x = 0; x < N; ++x)
  {
    printf("%d ", prefix[logN][x]);
  }
  printf("\nData\n");
  for(x = 0; x< tree[0][0]; ++x)
  {
    printf("%d ", A[x]);
  }
  printf("\n\n");
  //
  return 0;
}