#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXTOKEN 100

enum ttpye { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);
int gettoken(void);

void readword(char*, int);
void putword(char*);
int readdigit(char*, int);
int checkargs(void);
int check_array_args(int);
int typevalid(char*);
int etypevalid(char*);
int getdatatype(char*, int);
int iscomma(void);
int addpointers(char*);
void forward(int*);
void leave(char*);

int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char returntype[MAXTOKEN];
char out[1000];
char word[MAXTOKEN];

int main(){

  printf("\n");
  printf("***Exercise 5_18/5_20:\n");
  printf("\t\tAlternative solution from August 2013\n");
  printf("***Spaces ignored/error check (leaves when error encountered)\n");
  printf("***Function & array args added (with validity check)\n");
  printf("\n");

  while ( ( gettoken() ) != EOF){
    out[0] = '\0';

    if ( (etypevalid(token) == -1) )
      leave("expression needs to start with <type> or <const type>\n");
    getdatatype(token, 0);
    strcpy(returntype, datatype);

    dcl();
    if (tokentype != '\n')
      leave("terminal\n");
    printf("%s: %s %s\n", name, out, returntype);
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
      leave("missing )\n");
  }
  else if (tokentype == NAME)
    strcpy(name, token);

  while ( (type = gettoken()) == PARENS || type == BRACKETS ){
    if (type == PARENS){
      strcat(out, " function");
      strcat(out, token);
      strcat(out, " returning");
    }
    else{
      strcat(out, " array");
      strcat(out, token);
      strcat(out, " of");
    }
  }
}

int gettoken(void){

  int c;
  int* pc = &c;
  char* p = token;

  forward(pc);
  if (c == '('){
    forward(pc);
    if ( ( c == ')') ){
      strcpy(token, "()");
      token[0] = '\0';
      return tokentype = PARENS;
    }
    else if (isalpha(c)){
      readword(word, c);
      if ( (etypevalid(word) == -1) ){
	putword(word);
	return tokentype = '(';
      }
      if ( ( getdatatype(word, 1) == -1) ) // should never happen
	leave("invalid function argument\n");
      strcpy(token, "(");
      strcat(token, datatype);

      if ( (checkargs() == 1) ) // checkargs() just leaves though
	return tokentype = PARENS;
      else
	leave("invalid function argument\n");
    }
    else{ // handles case such as int (*name())
      ungetc(c, stdin);
      return tokentype = '(';
    }
  }
  else if (c == '['){

    forward(pc);
    if ( (c == ']') || (isdigit(c)) ){
      check_array_args(c);
      return tokentype = BRACKETS;
    }
    else
      leave("invalid array argument\n");

    return tokentype = BRACKETS; // we never get here - avoid gcc warning
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

  return -1; // we never get here - to suppress -Wall warning
}

// returns 1 if success
// function should return -1 for further processing
// instead, we "leave" with error comment
int checkargs(void){

  int c;

  // process initial data before entering logic of while loop
  c = iscomma(); // also checks to prevent ',)'
  if ( (!isalpha(c)) && ( !(c == ')') ) )
    leave("invalid function argument\n");

  while( (c != ')') ){

    readword(word, c);
    if ( ( getdatatype(word, 1) == -1) )
      leave("invalid function argument\n");
    else
      strcat(token, datatype);

    c = iscomma();
    if ( (!isalpha(c)) && ( !(c == ')') ) )
      leave("invalid function argument\n");

  }

  strcat(token, ")");

  return 1;
}

void forward(int* pc){

  while ( ( (*pc = getchar()) == ' ') || (*pc == '\t') )
    ;

}

void leave(char* text){

  printf("syntax error: %s\n", text);
  exit(-1);

}

void readword(char* word, int c){

  int i = 0;
  word[0] = c;

  while ( isalnum( c = getchar() ) )
    word[++i] = c;
  word[++i] = '\0';
  ungetc(c, stdin);

}

void putword(char* word){

  int l = strlen(word);

  while ( (--l >= 0) )
    ungetc(word[l], stdin);

}

// returns next non-space char after a comma (if any)
int iscomma(void){

  int c;
  int* pc = &c;

  forward(pc);
  if ( (c == ',') ){
    forward(pc);
    if ( (c == ')') )
      leave(", followed ) in function argument list\n");
    strcat(token, ", ");
  }

  return c;
}

int typevalid(char* test){

  if ( strcmp(test, "char") == 0 || strcmp(test, "int") == 0 ||
       strcmp(test, "long") == 0 || strcmp(test, "float") == 0 || 
       strcmp(test, "double") == 0 ||  strcmp(test, "void") == 0 )
    return 1;

  return -1;

}

int etypevalid(char* test){

  if ( (typevalid(test) == 1) || (strcmp(test, "const") == 0) )
    return 1;

  return -1;
}

int getdatatype(char* word, int infct){

  int c;
  int* pc = &c;
  char tbuf[MAXTOKEN];
  tbuf[0] = datatype[0] = '\0';

  if (strcmp(word, "const") != 0){
    if ( (typevalid(word) == 1) ){
      if ( (infct) && (addpointers(tbuf) == 1) )
	strcpy(datatype, tbuf);
      strcat(datatype, word);
      return 0;
    }
    else
      (infct)?leave("invalid function argument\n"):
	leave("invalid return type\n");
  }

  forward(pc);
  readword(word, c);
  if ( (typevalid(word) == 1) ){
    if ( (infct) && (addpointers(tbuf) == 1) )
      strcpy(datatype, tbuf);
    strcat(datatype, "const ");
    strcat(datatype, word);
    return 0;
  }
  else
    (infct)?leave("invalid function argument\n"):
      leave("invalid return type\n");

  return -1; // fct if properly used never gets here. To suppress -Wall warning
   
}

int addpointers(char* tbuf){

  int c;
  int* pc = &c;
  tbuf[0] = '\0';

  forward(pc);
  if ( ( c == '*' ) ){
    strcpy(tbuf, "pointer to ");
    forward(pc);

    while ( (c == '*') ){
      strcat(tbuf, "pointer to ");
      forward(pc);
    }
    ungetc(c, stdin);
    return 1;
  }

  ungetc(c, stdin);
  return -1;

}

// should return -1 for failure, 1 for success; we just leave instead
int readdigit(char* digit, int c){

  int i = 0;
  int* pc = &c;

  if ( (!isdigit(c)) || ( c == '0') )
    leave("invalid array argument\n");
  word[0] = c;
  forward(pc);

  while ( isdigit(c) ){
    word[++i] = c;
    forward(pc);
  }
  word[++i] = '\0';
  if ( (c != ']') )
    leave("invalid array argument\n");
  ungetc(c, stdin);

  return 1;
}

// should return -1 for failure, 1 for success; we just leave instead
int check_array_args(int c){

  int* pc = &c;

  // process first dimension separately as it involves one strcpy
  if ( (c == ']' ) )
    strcpy(token, "[]");
  else{
    readdigit(word, c); // also checks if followed by ']'
    forward(pc);
    strcpy(token, "[");
    strcat(token, word);
    strcat(token, "]");
  }
  forward(pc);

  while ( (c == '[') ){

    forward(pc);
    if ( (c == ']' ) )
      strcat(token, "[]");
    else{
      readdigit(word, c);
      forward(pc);
      strcat(token, "[");
      strcat(token, word);
      strcat(token, "]");
    }
    forward(pc);
  }

  ungetc(c, stdin);

  return 1;
}


