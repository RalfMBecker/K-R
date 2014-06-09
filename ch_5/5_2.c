#include <stdio.h>
#include <ctype.h>

#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

int getch(void);
void ungetch(int);
int getdouble(double*);

int main(){

  int c;
  double p;
  double* pp;

  printf("Version prints the float read, or \"no float\" if no float\n");
  printf("Float style \".4\" not allowed\n");
  printf("no overflow check for largest allowed float\n\n");

  pp = &p;
  while ( ( c = getdouble(pp))  != EOF)
    (c == 0)?printf("no float\n"):printf("type:%d, value:%g\n", c, p);
  return 0;

}  

int getdouble(double* pn){

  int c;
  double sign, power;

  while (isspace(c = getch()))
    ;
  if (!isdigit(c) && c != EOF && c != '+' && c != '-'){
    // ungetch(c);
    while ( (c = getch()) != '\n')
      ;
    return 0;
  }
  sign = (c == '-')?-1.0:1.0;
  if (c == '+' || c == '-'){ // check for ++, --, +word/-word and such
    if ( (c = getch()) == '\n') // handle "+\n" "-\n" case
      return 0;
    if ( !isdigit(c) && c != EOF){
      while ( (c = getch()) != '\n')
	;
      return 0;
    }
  }
  //  if (c == '+' || c == '-')
  //    c = getch();
  //  if ( !isdigit(c) && c != EOF ){
  //    ungetch(c);
  //    return 0;
  //  }
  for (*pn = 0.0; isdigit(c); c = getch())
    *pn = *pn * 10.0 + ( c-'0');
  power = 1.0;
  if ( c == '.')
    for (power = 1.0; isdigit( c = getch() ); ){
      *pn = *pn * 10.0 + ( c-'0');
      power *= 10.0;
    }
  *pn *= sign/power;

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
