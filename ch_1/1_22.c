#include <stdio.h>
#include <string.h>

#define MAXLINE 200
#define BREAK 30

int my_getline(char*, int);
void truncate(char*);
void copy_ft(char*, char*, int, int);

int main(){

  char line[MAXLINE];
  char line_out[MAXLINE];
  int len, start, end;

  while ( ( len = my_getline(line, MAXLINE) ) > 0){
    truncate(line);
    len = strlen(line);
    start = 0;
    while (len > 0){
      if (len >= BREAK){
	end = start + BREAK;
	len -= BREAK;
      }
      else{
	end = start + len;
	len = 0;
      }

      copy_ft(line_out, line, start, end);
      truncate(line_out);
      printf("%s", line_out);
      start += BREAK + 1;
    }
  }

  return 0;

}

/* copies from to to, beginning at start in from, ending at end in from */
/* no error checking: proper sizes assumed in arguments */
void copy_ft(char* to, char* from, int start, int end){

  if (end < start)
    return;

  int i, j=0;
  
  for (i=start; i <= end; i++)
    to[j++] = from[i];
  if (to[j-1] != '\n')
    to[j++] = '\n';
  to[j] = '\0';
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
