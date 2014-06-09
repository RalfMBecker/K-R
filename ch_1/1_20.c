#include <stdio.h>
#include <string.h>

#define MAXLINE 200
#define TABN 8

int my_getline(char*, int);
void detab(char*, char*);

int main(){

  char line[MAXLINE];
  char line_det[MAXLINE];

  while ( my_getline(line, MAXLINE) > 0){
    printf("%s", line);
    detab(line_det, line);
    printf("%s", line_det);
  }

  return 0;

}

/* assume from is of proper length*/
/* we should check if to will be longer than MAXLENTH. Omitted for clarity. */
/* tab forward from position x will be to (x + (TABN - x%TABN)) */
void detab(char* to, char* from){

  int i, j, k, end, d;

  j = -1;
  for (i=0; (d = from[i]) != EOF && d != '\n'; ++i){
    ++j;
    if (d != '\t')
      to[j] = from[i];
    else{
      end = j+(TABN - j%TABN);
      for (k=j; k < end; k++)
	to[k] = ' ';
      j = end-1;
    }
  }
  if ( d = '\n')
    to[++j] = '\n';
  to[++j] = '\0';
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
