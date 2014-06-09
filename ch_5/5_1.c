#include <stdio.h>
#include <ctype.h>

#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

int getch(void);
void ungetch(int);
int getint(int*);

int main(){

  int c, p;
  int* pp;

  printf("Version prints the digit read, or \"no digit\" if no digit\n");
  printf("Float 3.2 read as \"3 no digit\"\n");
  printf("no overflow check for largest allowed digit\n\n");
  pp = &p;
  // write value to p using pointer to int* to use simple printf (clever!)
  while ( ( c = getint(pp) ) != EOF)
    (c == 0)?printf("no digit\n"):printf("type:%d, value:%d\n", c, p);
  return 0;

}  

int getint(int* pn){

  int c, sign;

  while (isspace(c = getch()))
    ;
  if (!isdigit(c) && c != EOF && c != '+' && c != '-'){
    //     ungetch(c);
    while ( (c = getch()) != '\n')
      ;
    return 0;
  }
  sign = (c == '-')?-1:1;
  if (c == '+' || c == '-'){ // check for ++, --, +word/-word and such
    if ( (c = getch()) == '\n') // handle "+\n" "-\n" case
      return 0;
    if ( !isdigit(c) && c != EOF){
      while ( (c = getch()) != '\n')
	;
      return 0;
    }
  }
  for (*pn = 0; isdigit(c); c = getch())
    *pn = *pn * 10 + ( c-'0');
  *pn *= sign;

  if (c != EOF)
    ungetch(c);

  return c;

}

int getch(void){
  return (bufp > 0)?buf[--bufp]:getchar();
}

void ungetch(int c){

  if (bufp >= BUFSIZE)
    printf("Error: overflow in ungetch too many character\n");
  else
    buf[bufp++] = c;

}
