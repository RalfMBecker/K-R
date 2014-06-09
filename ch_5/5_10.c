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
int varspos = 0;

int getop(char*, char*);
void push(double);
double pop(void);

int main(int argc, char* argv[]){

  int type;
  double op2;
  char s[MAXOP];
  char* p;

  printf("***<math.h> functions sin, cos, tan, exp, and pow added***\n");
  printf("***Variables added***\n");
  printf("***Use '@' for multiplication***\n");
  while ( --argc > 0 ){
    type = getop(s, *++argv);
    switch(type){
    case NUMBER:
      push(atof(s));
      break;
    case '+':
      push(pop() + pop());
      break;
    case '@':
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
    case 'V': /* actions performed in parse. not ideal. */
      break;
    default:
      printf("Error: unknown command %s\n", s);
      break;
    }
  }
  printf("\t%.8g\n", pop());

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

int parse(char*);

/* arg 1 receives result; arg 2 is pointer to current argv */
int getop(char* s, char* arg){

  int i, c, sign;

  sign = 1;
  s[0] = c = arg[0];
  s[1] = '\0';
  if ( !isdigit(c) && c != '-' && (c != '.') ){
    if (isalpha(c))
      return parse(arg);
    else
      return c;
  }
  else if (c == '-'){
    if ( !isdigit(arg[1]) )
      return c;
    else{
      s[1] = c = arg[1];
      sign = -1;
    }
  }
  i = (sign == 1)?0:1;  /* to check next */
  if (isdigit(c))
    while ( isdigit(s[i] = arg[i] ) ) 
      ++i;
  if (arg[i] == '.')
    i++;
  while ( isdigit( s[i] = arg[i] ) ) 
    i++;

  return NUMBER;

}

int parse(char* arg){

  char s[MAXVARNAME];
  const char sin[] = { 's', 'i', 'n', '\0' };
  const char cos[] = { 'c', 'o', 's', '\0' };
  const char tan[] = { 't', 'a', 'n', '\0' };
  const char pow[] = { 'p', 'o', 'w', '\0' };
  const char exp[] = { 'e', 'x', 'p', '\0' };

  int i, c;

  s[0] = *arg;
  i = 1;
  while ( (c = *++arg) != '\0' )
    s[i++] = c;
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
  else if (strlen(s) > MAXVARNAME){
    printf("Error: var name length overflow.\n");
    return '['; /* return any illegal symbol */
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
      ++varspos;
      strcpy(vars[varspos].name, s);
      vars[varspos].value = pop();
      return 'V';
    }
}
