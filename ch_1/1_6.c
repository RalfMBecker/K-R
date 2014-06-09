#include <stdio.h>

int main(){

  int c;
  int d;

  c = getchar();
  while((d = (c != EOF)) == 1){
    printf("\n(c != EOF) = %d.\n",d); 
    putchar(c);
    c = getchar();
  }
  printf("\n(c != EOF) = %d.\n\n",d); 

  printf("The value of EOF is %d.\n", EOF);

  return 0;
}
