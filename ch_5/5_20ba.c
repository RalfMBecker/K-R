#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);
int gettoken(void);
int pdatatype(void);
int typevalid(char*);
int pparens(void);

int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];
int error;
int nameset; /* var name already set? -> future name calls are fct args */

int main(){

  printf("\n");
  printf("***Allows for const qualifier and function arguments.\n");
  printf("***Checks most errors I could think of.\n");
  printf("\n");

  while (gettoken() != EOF){
    out[0] = name[0] = '\0';
    error = 0;

    if (pdatatype() == -1){
      printf("Error: expression needs to begin with a valid return type\n");
      while ( getchar() != '\n')
	;
      continue;
    }
    dcl();
    if (error == 1){
      error = 0;
      while (getchar() != '\n')
	;
      continue;
    }
    if (name[0] == '\0'){
      printf("Error: no valid variable/function name in declaration.\n");
      continue;
    }

    if (tokentype != '\n')
      printf("Error: syntax error\n");
    printf("%s: %s %s\n", name, out, datatype);

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
  else if (tokentype == NAME){
    strcpy(name, token);
    nameset = 1;
  }

  while ( (type = gettoken()) == PARENS || type == BRACKETS ){
    if (type == PARENS){
      strcat(out, " function");
      strcat(out, token);
      strcat(out, " returning");
    }
    else{
      strcat(out, " array");
      strcat(out, token);
      strcat(out, " of");  int temp[3];
    }
  }

  // only a closing bracket ) or EOL should follow either PARENS or BRACKETS
  if (type != '\n' && type != ')'){
      printf("Error: syntax error\n");
      error = 1;
    }

}

int gettoken(void){

  int c;
  char* p = token;

  while ( (c = getchar()) == ' ' || c == '\t' )
    ;
  if (c == '('){
    if (pparens() == -1){
      error = 1;
      return tokentype = '\n';
    }
    return tokentype;
  }
  else if (c == '['){
    *p++ = c;
    for ( (c = getchar()) ; c != ']' ; p++, (c = getchar()) ){
      if (c == ' ')
	;
      else if ( isdigit(c) || c == ']' )
	*p = c;
      else{
	printf("Error: invalid expression in brackets.\n");
	error = 1;
	tokentype = '\n';
	return tokentype;
      }
    }

    if (*p != ']')
      *p = ']';
    *++p = '\0';
    printf("%s\n", token);

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

int pdatatype(void){

  int c;

  if (tokentype != NAME)
    return -1;
  strcpy(datatype, token);
  if (strcmp(datatype, "const") == 0){
    while ( (c = getchar()) == ' '){
      if (c == EOF)
	return -1;
    }
    ungetc(c, stdin);
    gettoken();
    if (tokentype != NAME)
      return -1;
    else{
      if (!typevalid(token) || 
	  strcmp(token, "void") == 0 && strcmp(datatype, "const") == 0 )
	return -1;
      strcat(datatype, " ");
      strcat(datatype, token);
    }
  }
  else{
    if (!typevalid(token))
      return -1;
  }

  return 1;
}

int typevalid(char* test){

  if (strcmp(test, "char") == 0 || strcmp(test, "int") == 0 ||
      strcmp(test, "float") == 0 || strcmp(test, "double") == 0 ||
      strcmp(test, "void") == 0)
    return 1;

  return 0;

}

int pparens(void){

  char temp[MAXTOKEN];
  char temp2[MAXTOKEN];  
  int c, nc, i;
  int nstars, nameset;
  char* p = temp;

  while ( (c = getchar()) == ' ' || c == '\t')
    ;
  if ( c == ')' ){
    strcpy(token, "()");
    tokentype = PARENS;
    return 0;
  }
  else if (c == '*'){
      ungetc(c, stdin);
      tokentype = '(';
      return 0;
    }

  if ( !isalpha(c) ){
    printf("Error: invalid expression in parentheses.\n");
    return -1;
  }

  *p++ = c;
  nc = 0;
  while ( isalnum(c = getchar()) ){
    *p++ = c;
    nc++;
  }
  *p = '\0';
  ungetc(c, stdin);

  // is it a name? if yes, put back and find name as usual
  if ( !(typevalid(temp)) && strcmp(temp, "const") !=0 ){
    for ( i = nc; i >=0 ; i--)
      ungetc(temp[i], stdin);
    tokentype = '(';
    return 0;
  }

  // no. so we are parsing argument names 
  strcpy(token, temp);  
  if (strcmp(temp, "const") == 0){
    strcpy(temp, token);
    gettoken();
    if ( !typevalid(token) ){
      printf("Error: invalid function argument.\n");
      return -1;
    }
    else{    
      strcat(temp, " ");
      strcat(temp, token);
    }
  }
  else
    strcpy(temp, token);

  nameset = 0;
  nstars = 0;
  while ( (c = gettoken()) != ',' && c != ')' && c!= '\n' && c != EOF){
    if ( c == ' ')
      continue;
    else if (c == NAME){
      if (nameset){    
	printf("Error: invalid function argument.\n");
	return -1;
      }
      else{
	nameset = 1;
	strcat(temp, " ");
	strcat(temp, token);
      }
    }
    else if (c == '*'){
      if (nameset){
	printf("Error: invalid function argument.\n");
	return -1;
      }
      else
	nstars++;
    }
    else{
      printf("Error: invalid function argument.\n");
      return -1;
    }
  }

  if (c == EOF || c == '\n'){
    printf("Error: invalid function argument.\n");
    return -1;
  }
  if (nstars){
    strcpy(token, "pointer to ");  
    for (i = 1; i < nstars; i++)
      strcat(token, "pointer to ");
    strcat(token, temp);
    strcpy(temp, token);
  }

  if (c == ')'){
    strcpy(token, "(");
    strcat(token, temp);
    strcat(token, ")");
    tokentype = PARENS;
    return 0;
  }

  // parse through other args
  while (c == ','){
    strcat(temp, ", ");

    c = gettoken();
    if (strcmp(token, "const") != 0 && strcmp(token, "void") == 0 &&
	!typevalid(token)){
      printf("Error: invalid function argument.\n");
      return -1;
    }
  
    if (strcmp(token, "const") == 0){
      strcpy(temp2, token);
      gettoken();
      if ( !typevalid(token) ){
	printf("Error: invalid function argument.\n");
	return -1;
      }
      else{
	strcat(temp2, " ");
	strcat(temp2, token);
      }
    }
    else
      strcpy(temp2, token);

    nameset = 0;
    nstars = 0;
    while ( (c = gettoken()) != ',' && c != ')' && c!= '\n' && c != EOF){
      if ( c == ' ')
	continue;
      else if (c == NAME){
	if (nameset){
	  printf("Error: invalid function argument.\n");
	return -1;
	}
      else{
	nameset = 1;
	strcat(temp2, " ");
	strcat(temp2, token);
      }
      }
      else if (c == '*'){
	if (nameset){
	  printf("Error: invalid function argument.\n");
	  return -1;
	}
	else
	  nstars++;
      }
      else{
	printf("Error: invalid function argument.\n");
	return -1;
      }
    }

    if (c == EOF || c == '\n'){
      printf("Error: invalid function argument.\n");
      return -1;
    }
    if (nstars){
      strcpy(token, "pointer to ");  
      for (i = 1; i < nstars; i++)
	strcat(token, "pointer to ");
      strcat(token, temp2);
      strcpy(temp2, token);
    }

    strcat(temp, temp2);

    if (c == ')'){
      strcpy(token, "(");
      strcat(token, temp);
      strcat(token, ")");
      tokentype = PARENS;
      return 0;
    }
  }
    
}    
