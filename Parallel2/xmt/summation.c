//
//Torbert, March 2013
//
// gcc -fopenmp filename
#include <stdio.h>
#include <omp.h>
//
#define N 8
#define logN 3
//
int main(void)
{
  int rank, size=N, j;
  //
  int data[N] = {3, 1, 4, 1, 5, 9, 2, 6};
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
    tree[logN][rank] = data[rank];  // work O(N)
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
  return 0;
}