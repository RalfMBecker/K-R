#include <stdio.h>
#include <stdlib.h>

#define MAXOP 100
#define NUMBER '0'

int getop(char*);
void push(double);
double pop(void);

int main(){

  int type;
  double op2;
  char s[MAXOP];

  while ((type = getop(s)) != EOF){
      switch(type){
      case NUMBER:
	push(atof(s));
	     break;
      case '+':
	push(pop() + pop());
	break;
      case '*':
	push(pop() * pop());
	break;
      case '-':
	op2 = pop();
	push(pop() - op2);
	break;
      case '/':
	op2 = pop();
	if (op2 == 0)
	  printf("Error: division by zero.\n");
	else
	  push(pop() / op2);
	break;
      case '%':
	op2 = pop();
	push( (int) pop() % (int) op2 );
	break;
      case '\n':
	printf("\t%.8g\n", pop());
	break;
      default:
	printf("Error: unknown command %s\n", s);
	break;
      }
  }

  return 0;

}  

#define MAXVAL 100

int sp = 0;
double val[MAXVAL];

void push(double f){
  if (sp < MAXVAL)
    val[sp++] = f;
  else
    printf("Error: stack overflow.\n");
}

double pop(void){

  if (sp > 0)
    return val[--sp];
  else{
    printf("Error: stack empty\n");
    return 0.0;
  }
}

#include <ctype.h>

int getch(void);
void ungetch(int);

int getop(char* s){

  int i, c, cn, sign;

  sign = 1;
  while ( (s[0] = c = getch() ) == ' ' || c == '\t')
    ;
  s[1] = '\0';
  if ( (!isdigit(c) && c != '-') && (c != '.') )
    return c;
  else if (c == '-'){
    if ( !isdigit(cn = getch()) ){
      ungetch(cn);
      return c;
    }
    else{
      c = cn;
      s[1] = cn;
      sign = -1;
    }
  }
  i = (sign ==1)?0:1;
  if (isdigit(c))
    while ( isdigit(s[++i] = c = getch()) ) 
      ;
  if (c == '.')
    while ( isdigit(s[++i] = c = getch()) ) 
      ;
  s[i] = '\0';
  if (c != EOF)
    ungetch(c);

  return NUMBER;

}

#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

int getch(void){
  return (bufp > 0)?buf[--bufp]:getchar();
}

void ungetch(int c){

  if (bufp >= BUFSIZE)
    printf("Error: overflow in ungetch too many character\n");
  else
    buf[bufp++] = c;

}
