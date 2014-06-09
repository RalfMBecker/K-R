#include <stdio.h>
#include <string.h>

#define MAXLINE 200
#define TABN 8

int my_getline(char*, int);
void detab(char*, char*);
void entab(char*, char*);

int main(){

  char line[MAXLINE];
  char line_det[MAXLINE];

  while ( my_getline(line, MAXLINE) > 0){
    printf("%s", line);
    entab(line_det, line);
    printf("%s", line_det);
  }

  return 0;

}

/* let n be number of spaces to be replaced by tab/space combos; */
/* i = current position; k = TABN - i%TABN; m = n-k. Then: */
/* if (i+n)/TABN == i/TABN, then spaces enough: tabs = 0, spaces = n */ 
/* else: tabs == 1{if k>0 or (k=0 and n>0)} + m/TABN; spaces == m%TABN */
/* note that because a tab can fast forward, effective position is tracked */
/* this setup answers the additional question in KR */
/* assume from is of proper length*/
/* we should check if to will be longer than MAXLENGTH. Omitted for clarity. */
void entab(char* to, char* from){

  int i, j, k, spaces_total, tabs, spaces, end_tabs, end_spaces;
  int m, position, d;

  j = -1;
  position = 0;
  for (i=0; (d = from[i]) != EOF && d != '\n'; ++i){
    ++j;
    if (d != ' '){
      to[j] = from[i];
      ++position;
    }
    else{
      spaces_total = 1;
      while ( from[++i] == ' ')
	spaces_total++;
      --i;
      /* see above for following calculations */
      if ( (position+spaces_total)/TABN == position/TABN ){
	tabs = 0;
	spaces = spaces_total;
      }
      else{
	m = spaces_total - (TABN - position%TABN);
	tabs = m/TABN + ( ((m > 0) || ( (m == 0) && spaces_total > 0) )?1:0 );
	spaces = m%TABN;
      }

    //printf("%d\t%d\t%d\t%d\t\%d\n", j, position, spaces_total, tabs, spaces);
      end_tabs = j + tabs;
      end_spaces = j + tabs + spaces;
      for (k=j; k < end_tabs; k++)
	to[k] = '\t';
      for (k=end_tabs; k < end_spaces; k++)
	to[k] = ' ';
      j = end_spaces-1;

      position += spaces_total;
    }
  }
  if ( d = '\n')
    to[++j] = '\n';
  to[++j] = '\0';
}

/* assume from is of proper length*/
/* we should check if to will be longer than MAXLENGTH. Omitted for clarity. */
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
