#include <stdio.h>
#include "parallel.h"

void func1(int a)
{
  a++;
  printf("%d ", a);
  if(a%10==0)
    printf("\n");
}

void func2(int b)
{
  b++;
  printf("%d ", b);
  if(b%10==0)
    printf("\n");
}

int main(int argc, char* argv[])
{
  INITIALIZE_ENV(5)
  int i, j;
  Parallel<void(int), func1> f1[128];
  for(i=0; i<128; i++)
  {
    f1[i](i);
  }
  Parallel<void(int), func2> f2[128];
  for(j=200; j<328; j++)
  {
    f2[j-200](j);
  }

  return 0;
}
