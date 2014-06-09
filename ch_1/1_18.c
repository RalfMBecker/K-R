#include <stdio.h>
#include <string.h>

#define MAXLINE 200

int my_getline(char*, int);
void truncate(char*);
void my_trim(char*); 
void reverse(char*);

int main(){

  char line[MAXLINE];

  while ( my_getline(line, MAXLINE) > 0){
    truncate(line);
    my_trim(line);
    if (line[0] != '\n')    
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

/* truncates empty space at end, adding one '\n' instead */
void truncate(char* a){

  int i, stop;
  stop = strlen(a); /* position of trailing '\0' */

  if (stop-- > 0){
    while (stop >= 0 && ( a[stop] == ' ' || a[stop] == '\t' || a[stop] == '\n'))
      --stop;
  }

  a[++stop] = '\n';
  a[++stop] = '\0';

}

/* highly inefficient, but does the job */
void my_trim(char* a){

  int d = strlen(a);

  if (d >1){
    reverse(a);
    truncate(a);
    d = strlen(a);
    a[d-1] = '\0'; /* remove newline added at end of string in truncate */
    reverse(a);
  }

}

/* assumes len > 1 */
void reverse(char* a){

  int i, end, len, temp;
  len = strlen(a);

  if (len%2 == 1)
    end = (len - 1)/2;
  else
    end = len/2;

  len--;
  for (i=0; i < end; i++){
    temp = a[i];
    a[i] = a[len-i];
    a[len-i] = temp;
  }

}
