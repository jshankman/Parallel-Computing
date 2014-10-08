//Jake Shankman
//Parallel 2 Period 7
//1/30/13
#include <stdio.h>
int main(void)
{
  FILE *fout = fopen("ray0.ppm", "w");
  fprintf(fout, "P3\n 400 300\n 255\n");
  int j;
  for(j = 0; j < 400*300; ++j)
  {
   fprintf(fout, "255 100 100\n");
  }
  
  
  return 0;
}