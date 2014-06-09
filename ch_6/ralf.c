#include <stdio.h>

int main(){

  int i, j, last;

  j = last = 0;
  for (i = 0; i <= 1000; i++){
    j = sbrk(1);
    printf("%d\t%d\t%d\t%d\n", j>last, j == last+1, last, j );
  last = j;
  }
  return 0;
}
