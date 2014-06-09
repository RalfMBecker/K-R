#include <stdio.h>
#define MAXLINE 50
#define MAXPRINT 15

int my_getline(char* a, int);

int main(){

  int len;
  char line[MAXLINE];

  printf("Chosen to print only lines longer than %d characters.\n\n", MAXPRINT);

  while ( (len = my_getline(line, MAXLINE)) > 0){
      if (len >= MAXPRINT)
	printf("%s", line);
    }

  return 0;

}

int my_getline(char* s, int lim){

  int c, i;

  for (i=0; i < lim-1 && ( c = getchar()) != EOF && c!= '\n'; i++)
    s[i] = c;
  if ( c == '\n'){
    s[i] = c;
    ++i;
  }
  s[i] = '\0';

  return i;

}
