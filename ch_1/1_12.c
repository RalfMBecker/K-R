#include <stdio.h>

#define MAXARR 1000
enum boolean { NO=0, YES }; /* EOF reached */

int rword(char*);
void wword(char*);

int main(){

  printf("Hit ctrl-d twice to escape.\n\n");

  int c;
  char a[MAXARR];
  *a = '\0';

  while ( rword(a) == NO){ 
    if ( a[0] != '\0'){
      wword(a);
      putchar('\n');
    }
  }
  if ( a[0] != '\0'){
    wword(a);
    putchar('\n');
  }
  return 0;
}

int rword(char* a){

  int c, i;
  i = 0;

  while ( i < MAXARR-1 && (c = getchar()) != EOF &&
	  c != '\t' && c != '\n' && c != ' '){
   a[i++] = c;
  }
  a[i] = '\0';

  return (c == EOF) ? YES : NO;
}

void wword(char* a){

  int c;

  while ( (c = *a++) != '\0')
    putchar(c);
}
