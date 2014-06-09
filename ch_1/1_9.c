#include <stdio.h>

int main(){

  enum states { OUT=0, IN };
  int c, state;

  state = OUT;
  c = getchar();

  while (c != EOF){

    if (c != ' '){
      putchar(c);
      if (state == IN)
	state = OUT;
    }
    else{
      if (state == OUT){
	state = IN;
	putchar(c);
      }
     }
    c = getchar();
  }

  return 0;
}
