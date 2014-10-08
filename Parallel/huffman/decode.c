//#Jake Shankman
//Period 5
//9/5/2012
//Decode


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct TreeNode
{
	char symbol;
	int frequency;
	//
	struct TreeNode* left;
	struct TreeNode* right;
	//
} Node;


int main(int arr,char* array[])
{
  FILE* fin;
  char code[10000];
  char message[10000];
  char prob[10000];
  int n, j;
  int line;
  char ch, key;
  int i, k;
  int total;
  
  if(arr < 2)
  {
    printf("Give me a file!\n");
    return 0;
  }
  char* file = array[1];
  

  fin = fopen(file, "r");
  if(!fin)
  {
    printf("File not found!\n");
    return 0;
  }
  //NULL THE ARRAY
  for(i = 0; i <10000 ; i++)
  {
    code[i] = message[i] = prob[i] = '\0';
  }
  
  //GET NUMBER OF LINES
  n = 0;

  while(1)
  {
    fread(&ch, sizeof(char), 1, fin);
    //
    if(ch == '\n') 
      break;
    //
    n*=10;
    n+= (int)(ch-'0');
   
  }
  
  //BUILD KEY
  for(k = 1; k <= n; k++)
  {
    fread(&key, sizeof(char), 1, fin);
    j = 1;
    while(1)
    {
      fread(&ch, sizeof(char), 1, fin);
      if(ch == '\n')
	break;
      //BINARY CONVERSION
      j*=2;
      if(ch == '1')
      {
	j += 1;
      }
      
    }
    printf("j=%d\t", j);
    printf("key=%c\n", key);
    code[j] = key;
  }

  
  //DECODE MESSAGE
  
  j = 1;
  k = 0;
  while(!(feof(fin)))
  {
    fread(&ch, sizeof(char), 1, fin);
    if (ch == '\n')
      break;
    j*=2;
    if(ch == '1')
      j++;
    if(code[j] != '\0')
    {
      message[k] = code[j];
      k++;
      j = 1;
    }
  }
  printf("Message: %s\n", message );
  fclose(fin);	//close file
  //PROBABILITY; Total
  total = strlen(message);
  Node* tree[total];
  for(i = 0; i < total; i++)
    tree[i] = NULL;
  
  printf("\nminbit Calculations!\n");
  printf("total=%i\n", total);
  for(i = 0; i < total; i++)
  {
    if(message[i] != '\0')
    {
      int z;
      int q;
      q = 0;
      for(z = 0; z < total; z++)
      {
	if(tree[z] != '\0')
	{
	  if(tree[z] ->symbol == message[i])
	  {
	    tree[z] -> frequency++;
	    q = 1;
	  } 
	}
      }
      if(tree[i] == '\0' && q == 0)
      {
	Node* temp;
	temp = (Node*)malloc(sizeof(Node));
	temp -> symbol = message[i];
	temp -> frequency = 1;
	tree[i] = temp;
      }
    }
      
    }
    int minbits;
    minbits = 0;
    for(i = 0; i < total; i++)
    {
      if(tree[i] != '\0')
      {
	minbits += (int)(-1*(log((double)(tree[i] -> frequency)/total)/log(2)));
	printf("%c\t%d\n", tree[i] -> symbol, tree[i] -> frequency);
      }
     }
  printf("Minimum # of bits=\t%i\n", minbits);
  return 0;
}



//end of file