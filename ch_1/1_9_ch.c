#include <stdio.h>
#define MAXLINE 50

int my_getline(char* a, int);
void copy(char*, char*);

int main(){

  int len, max=0;
  char line[MAXLINE];
  char longest[MAXLINE];

  while ( (len = my_getline(line, MAXLINE)) > 0){
      if (len > max){
	max = len;
	copy(longest, line);
      }
    }

  if ( max > 0)
    printf("%s", longest);
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

void copy(char* to, char* from){

  while ( (*to++ = *from++) != '\0')
    ;

}
