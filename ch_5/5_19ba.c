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
  printf("***File as in KR. Solution to 19 in 5_19.c.\n");
  printf("\n");

  int type;
  char temp[MAXTOKEN];

  while (gettoken() != EOF){
    //    out[0] = '\0'; 
    strcpy(out, token);
    while ( (type = gettoken()) != '\n'){

      if (type == PARENS || type == BRACKETS)
	strcat(out, token);
      else if (type == '*'){
	sprintf(temp, "(*%s)", out);
	strcpy(out, temp);
      }
      else if (type == NAME){
	sprintf(temp, "%s %s", token, out);
	strcpy(out, temp);
      }
      else
	printf("invalid input at %s\n", token);
    }

    printf("%s\n", out);
  }

  return 0;
}

void dcl(void){

  int ns;

  for ( ns = 0; gettoken() == '*'; )
    ns++;
  dirdcl();
  while (ns-- > 0)
    strcat(out, " pointer to");
}

void dirdcl(void){

  int type;

  if (tokentype == '('){
    dcl();
    if (tokentype != ')')
      printf("Error: missing )\n");
  }
  else if (tokentype == NAME)
    strcpy(name, token);

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
  else
    return tokentype = c;

}
