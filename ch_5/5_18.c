#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);
int gettoken(void);

int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];

int main(){

  printf("\n");
  printf("***File as in KR. Solution to 18 and 20 in 5_20.c.\n");
  printf("\n");

  while (gettoken() != EOF){
    out[0] = '\0'; /* need to clear if we do more than one dcl */

    strcpy(datatype, token);
    out[0];

    dcl();
    if (tokentype != '\n')
      printf("Error: syntax error\n");
    printf("%s: %s %s\n", name, out, datatype);
  }

  return 0;
}

void dcl(void){

  int ns;
  static int i = 0;

  for ( ns = 0; gettoken() == '*'; )
    ns++;
  printf("in dcl() %d; tokentype = %d\n", i++, tokentype);
  dirdcl();
  printf("tokentype now: %d\n", tokentype); 
 while (ns-- > 0)
    strcat(out, " pointer to");
}

void dirdcl(void){

  int type;

  if (tokentype == '('){
    printf("found a (\n");
    dcl();
    printf("returned from dcl() with name[] = %s\n", name);
    if (tokentype != ')')
      printf("Error: missing )\n");
  }
  else if (tokentype == NAME){
    printf("processing a name\n");
    strcpy(name, token);
  }

  while ( (type = gettoken()) == PARENS || type == BRACKETS ){
    if (type == PARENS)
      strcat(out, " function returning");
    else{
      strcat(out, " array");
      strcat(out, token);
      strcat(out, " of");
    }
  }

}

int gettoken(void){

  int c;
  char* p = token;

  while ( (c = getchar()) == ' ' || c == '\t' )
    ;
  if (c == '('){
    if ( ( c = getchar() ) == ')'){
      strcpy(token, "()");
      return tokentype = PARENS;
    }
    else{
      ungetc(c, stdin);
      return tokentype = '(';
    }
  }
  else if (c == '['){
    for (*p++ = c; (*p++ = getchar()) != ']'; )
      ;
    *p = '\0';
    return tokentype = BRACKETS;
  }
  else if (isalpha(c)){
    for (*p++ = c; isalnum(c = getchar()); )
      *p++ = c;
    *p = '\0';
    ungetc(c, stdin);
    return tokentype = NAME;
  }
  else{
    printf("finding a tokentype\n");
    return tokentype = c;
  }

}
