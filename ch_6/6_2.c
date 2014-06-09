#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "header.h"

#define MAXWORD 100
#define MAXLINE 100
#define MAXCOMP 20
#define MAXDEFINES 50
#define NCKEYS (sizeof Ckeytab / sizeof Ckeytab[0])
#define NMKEYS (sizeof Cmacros / sizeof Cmacros[0])

// in same node, store, as char**, vars same in first complen chars
// char*'s in words are stored in increasing order
struct tnode {
  char** words;
  int count;
  struct tnode *left;
  struct tnode *right;
};
char line[MAXLINE];
char* defines[MAXDEFINES];
int sdefines = 0; // how many #defines do we have

struct tnode* addtree(struct tnode*, char*, int);
void treeprint(struct tnode*, int);
void swap(char**, int, int);
int binsearch(char*, void*, int, int (*comp)(void*, int, char*));
int keycomp(struct key*, int, char*);
int charcomp(char**, int, char*);
void sort_in(char**, int, char*);

int checkdigit(char*);
int getword(char*, int, int*);
void readword(char*, int, int*);
int quotes(char*, int*); 
int escape(int, int*);
int isoctal(int);
int dquotes(int*);
void leave(char*);

int main(int argc, char* argv[]){

  struct tnode *root;
  char word[MAXWORD];
  int first;
  int i = 0;
  int* position = &i;
  int complen, test, test1, test2, test3;

  FILE *fp;
  char* prog = argv[0];

  if (argc != 3){
    fprintf(stderr, "%s: usage is %s <file.c> <# to compare>.\n", prog, prog);
    exit(1);
  }
  if ( ( fp = fopen(*++argv, "r") ) == NULL ){ 
    fprintf(stderr, "%s: can't open file %s.\n", prog, *argv);
    exit(2);
  }
  else if ( ( checkdigit(*++argv) == -1) || ((complen = atoi(*argv)) <= 0) ) {
    fprintf(stderr, "%s: 2nd argument needs to be a positive integer.\n", prog);
    exit(3);
  }
  else if ( (complen > MAXCOMP) ){
    fprintf(stderr, "%s: max comparison length is %d\n", prog, MAXCOMP);
    exit(4);
  }
  else{
    printf("We print out variables, and functions, except those that ");
    printf("are defined in \nstdio.h or ctype.h.\n");
    if ( (complen > 1) ){
      printf("Names that are the same in the first <%d> ", complen);
      printf("characters are printed together\nto the right.\n");
    }
    printf("\n");
    root = NULL;
    while ( (fgets(line, MAXLINE, fp) != NULL) ){
      while ( (first = getword(word, MAXWORD, position)) != '\n'){
	if ( (first == EOF) )
	  break;
	test1 = binsearch(word, Ckeytab, NCKEYS, 
			   (int (*)(void*, int, char*))(keycomp));
	test2 = binsearch(word, Cmacros, NMKEYS,
			   (int (*)(void*, int, char*))(keycomp));
	test3 = binsearch(word, defines, sdefines,
			   (int (*)(void*, int, char*))(charcomp));
	if ( (test1 == -1) && (test2 == -1) && (test3 == -1) )
	  test = -1;
	else
	  test = 0;
	if ( isalpha(word[0]) && (test == -1) )
	  root = addtree(root, word, complen);
      }
      *position = 0;
    }
  }
  fclose(fp);
  treeprint(root, complen);

  return 0;
}

struct tnode* addtree(struct tnode* p, char* w, int clen){

  int comp;

  if ( (p == NULL) ){
    p = (struct tnode*) malloc(sizeof(struct tnode));
    if ( (p == NULL) )
      leave("cannot allocate struct tnode\n");
    p->words = (char**) malloc(sizeof(char*));
    p->words[0] = (char*) malloc(strlen(w)+1);
    strcpy(p->words[0], w);

    p->count = 1;
    p->left = p->right = NULL;
  }
  else if ( ( (comp = strncmp(w, *(p->words), clen)) == 0) ) {
    if ( !(binsearch(w, p->words, p->count,
		     (int (*)(void*, int, char*))(charcomp)) == -1) )
      return p;
    p->words = realloc(p->words, (p->count + 1) * sizeof(char*));
    sort_in(p->words, ++p->count, w);
  }
  else if ( (comp < 0) )
    p->left = addtree(p->left, w, clen);
  else
    p->right = addtree(p->right, w, clen);

  return p;
}

// use with n being the NEW array size (old == (n-1))
void sort_in(char** array, int n, char* word){

  array[--n] = (char*) malloc( (strlen(word)+1) * sizeof(char));
  if ( ( array[n] == NULL) )
    leave("cannot allocate space to store word\n");
  strcpy(array[n], word);

  while ( (--n >= 0) && (strcmp(word, array[n]) < 0) )
    swap(array, n, n+1);
}

void swap(char** v, int i, int j){

  char* temp;

  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

void treeprint(struct tnode* p, int clen){

  int t = 0;

  if (p != NULL){
    if (clen > 1)
      t = (p->count > 1);
    treeprint(p->left, clen);
    while ( (p->count-- > 0) )
      printf("%s%s\n", t?"   ":"", *(p->words++));
    treeprint(p->right, clen);
  }
}

int binsearch(char* word, void* tab, int n, 
	       int (*comp)(void*, int, char*)){

  int cond;
  int low, high, mid;

  low = 0;
  high = n-1;
  while ( (low <= high) ){
    mid = (low + high)/2;
    cond = (*comp)(tab, mid, word);
    if ( (cond < 0) )
      high = mid-1;
    else if ( (cond > 0) )
      low = mid+1;
    else
      return mid;
  }

  return -1;
}

int keycomp(struct key* k, int n, char* w){

  return strcmp(w, k[n].word);
}

int charcomp(char** k, int n, char* w){

  return strcmp(w, k[n]);
}

// after function, pos should point to next char to be read
int getword(char* word, int lim, int* pos){

  int c;
  char* w = word;

  while ( ((c = line[*pos]) == ' ') || (c == '\t') )
    ++*pos;

  if ( (c == EOF) ){
    word[0] = '\0';
    return c;
  }

  if( (c == '/') ){ // handle comments
    if ( (c = line[++*pos]) == EOF){
      *w = '\0';
      return EOF;
    }
    if ( (c == '/') ){ // if not, we can fall through
      *w++ = '\n';
      *w = '\0';
      return '\n'; // fast forward in calling routine
    }
  }

  if ( (c == '\"') ){
    if ( (dquotes(pos) == -1) )
      leave("missing closing \" in string constant\n");
  }

  if ( (c == '\'') )
    quotes(line, pos);

  if ( (c == '#') ){ // handle pre-processor lines
    if ( !isalpha(c = line[++*pos]) )
      leave("incorrect pre-compiler syntax\n");
    readword(word, lim, pos);
    if ( (strcmp(word, "define") == 0) ){
      while ( ((c = line[++*pos]) == ' ') || (c == '\t') )
	;
      if ( !isalpha(c) )
	leave("incorrect pre-compiler syntax\n");
      readword(word, lim, pos);
      sort_in(defines, ++sdefines, word);
    }
    word[0] = '\n'; // fast forward in calling routine
    word[1] = '\0';
    return word[0];
  }

  if (!isalpha(c)){
    ++*pos;
    *w++ = c;
    *w = '\0';
    return c;
  }

  readword(w, lim, pos);
  return word[0];

}

void readword(char* w, int lim, int* pos){

  for ( ; lim-- > 0; w++){
    if ( !isalnum( (*w = line[(*pos)++]) ) ){
      break;
    }
  }
  (*pos)--;
  *w = '\0';

}

int checkdigit(char* word){

  int i = 0;

  while ( ( isdigit(word[i++]) ) )
    ;
  if ( (word[i-1] != '\0') )
    return -1;

  return 0;
}

// we leave on error; better return value would be 1 for success; 0 for not
int quotes(char* line, int* pos){

  char c;
 
  if ( ((c = line[++*pos]) == '\'') )
    leave("empty character constant '' illegal");

  if ( (c == '\\') ){
    c = line[++*pos];
    if ( (escape(c, pos) == -1) )
      leave("illegal escape sequence\n");
  }
  if ( ((c = line[++*pos]) != '\'') )
    leave("illegal character constant\n");
 
  return 1;
}

// upon return, pos points at last char of escape sequence
// check for closing ' in calling routine
// we allow (1) letter escape sequences; (2) octal and hex escape sequences
int escape(int c, int* pos){

  int type = -1;

  if ( (c == 'a') || (c == 'b') || (c == 'f') || (c == 'n') || (c == 'r') || 
       (c == 't') || (c == 'v') || (c == '\'') || (c == '\"') || (c == '\\') || 
       (c == '\?') )
    return 0;

  if ( (isoctal(c) == 0) )
    type = 0;
  else if ( (c == 'x') )
    if ( isxdigit(c = line[++*pos]) )
      type = 1;

  if ( (type == -1) )
    return -1;
  else if ( (type == 0) ){
    while ( (isoctal(c = line[++*pos]) == 0) )
      ;
    --*pos;
    return 0;
  }
  else{
    while ( isxdigit(c = line[++*pos]) )
      ;
    --*pos;
    return 0;
  }

}

int isoctal(int c){

  if ( ((c - '0') >= 0) && ((c - '0') <= 8) )
    return 0;
  else
    return -1;
}

// returns 0, and pos on closing " (or -1 if no closing ")
int dquotes(int* pos){

  char c;
  char prior = '\"';

  while ( ((c = line[++*pos]) != '\n') && (c != EOF) )   {
    if ( (c == '\"') && !(prior == '\\') )
      return 0;
    prior = c;
  }

  return -1;
}

void leave(char* word){

  printf("Error: %s", word);
  exit(-1);

}
