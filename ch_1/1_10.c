#include <stdio.h>

int main(){

  int c;

  while ( (c = getchar()) != EOF){

    if (c == '\t')
      printf("%c%c", '\\', 't');
    else if (c == '\b')    /* not recognized */
      printf("%c%c", '\\', 'b');
    else if (c == '\\')
      printf("%c%c", '\\', '\\');
    else
      putchar(c);
  }
}
