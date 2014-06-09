#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXOP 100
#define MAXVARNAME 10
#define MAXVARS 100
#define NUMBER '0'

struct var{
  char name[MAXVARNAME];
  double value;
};
struct var vars[MAXVARS];
int varstored = 0;

int getop(char*);
void push(double);
double pop(void);

int main(){

  int type;
  double op2;
  char s[MAXOP];

  printf("***<math.h> functions sin, cos, tan, exp, and pow added***\n");
  printf("***         Variables added         ***\n");
  printf("*** Syntax: '2 test' stores 2 in test\n");
  printf("*** Note: vars cannot be redefined once stored. ***\n");

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
      case 's':
	push(sin(pop()));
	break;
      case 'c':
	push(cos(pop()));
	break;
      case 't':
	push(tan(pop()));
	break;
      case 'e':
	push((double)exp((double)pop()));
	break;
      case 'p':
	op2 = (int)pop();
	push((int)pow((int)pop(), op2));
	break;
      case 'V': /* most actions performed in parse. not ideal. */
	if (varstored){
	  getop(s); /* should be \n. Read, ignore, and continue. */
	  varstored = 0;
	}
	break;
      case 'v': 
	printf("Error: var name length overflow.\n");
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
#include <string.h>

int getch(void);
void ungetch(int);
int parse(char);

int getop(char* s){

  int i, c, cn, sign;

  sign = 1;
  while ( (s[0] = c = getch() ) == ' ' || c == '\t')
    ;
  s[1] = '\0';
  if ( (!isdigit(c) && c != '-') && (c != '.') ){
    if (isalpha(c))
      return parse(c);
    else
      return c;
  }
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

int parse(char c){

  char s[MAXVARNAME + 1]; /* add 1 to check for length overflow */
  const char sin[] = { 's', 'i', 'n', '\0' };
  const char cos[] = { 'c', 'o', 's', '\0' };
  const char tan[] = { 't', 'a', 'n', '\0' };
  const char pow[] = { 'p', 'o', 'w', '\0' };
  const char exp[] = { 'e', 'x', 'p', '\0' };
  static int varspos = 0;

  int i, ct;

  s[0] = c;
  i = 1;
  while ( ( ct = getch() ) != ' ' && ct != '\n' && ct!='\t' && i <= MAXVARNAME)
    s[i++] = ct;
  ungetch(ct);
  s[i] = '\0';

  if ( (s[0] == '-') || (s[0] == '+'  ) )
    return s[0];
  else if (strcmp(s, sin) == 0)
    return 's';
  else if (strcmp(s, cos) == 0)
    return 'c';
  else if (strcmp(s, tan) == 0)
    return 't';
  else if (strcmp(s, exp) == 0)
    return 'e';
  else if (strcmp(s, pow) == 0)
    return 'p';
  else if (strlen(s) == (MAXVARNAME+1)){
    return 'v';
  }
  for (i = 0; i < MAXVARS && strcmp(s, vars[i].name) != 0; i++)
    ;
  if (i < MAXVARS){
    push(vars[i].value);
    return 'V';
  }
  else
    if (varspos == MAXVARS-1){
      printf("Error: vars overflow.\n");
      return '['; /* random unused character */
    }
    else{
      if (sp){
      strcpy(vars[varspos].name, s);
      vars[varspos++].value = pop();
      }
      varstored = 1;
      return 'V';
    }
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
