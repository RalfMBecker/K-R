#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXWORD 100
#define MAXLINE 100
#define HASHSIZE 101

char line[MAXLINE];

// from 6_5.c
struct nlist{
  struct nlist* next;
  char* name;
  char* defn;
};
static struct nlist *hashtab[HASHSIZE];

unsigned hash(char*);
struct nlist* lookup(char*);
struct nlist* install(char*, char*);
int undef(char*);
struct nlist* findprior(char*);
char* mystrdup(char*);

// new, or adjusted, for this program. Last 2 from 6_2.c.
int parse_token(int*);
int readdefn(char*, int, int*);
int add_defn(int*);
int quotes(char*, int*); 
int escape(char, int*);
int dquotes(int*);
void readword(char*, int, int*);
int forward(int*);
int isoctal(int);
void leave(char*);
void opt_error(char*, int, int);

int main(int argc, char* argv[]){

  int l, t;
  int i = 0;
  int* position = &i;
  struct nlist* p;
  int n = 1;

  FILE *fp;
  char* prog = argv[0];

  for (l = 0; l < HASHSIZE; l++)
    hashtab[l] = NULL;

  if ( (argc != 2) && (argc != 3) ){
    fprintf(stderr, "%s: usage is %s <file.c> (-n).\n", prog, prog);
    exit(1);
  }
  else if ( ( fp = fopen(*++argv, "r") ) == NULL ){
    fprintf(stderr, "%s: can't open file %s.\n", prog, *argv);
    fprintf(stderr, "%s: usage is %s <file.c> (-n).\n", prog, prog);
    exit(2);
  }
  else if ( (argc == 3) ){
    if ( ( (t = *(++argv)[0]) != '-') )
      opt_error(prog, t, 0);
    else if ( ( (t = *++argv[0]) != 'n') )
      opt_error(prog, t, 1);
    else
      n = 0;
  }

  printf("A probably more logical, clean solution from August 2013.\n\n");

  l = 0;
  while ( (fgets(line, MAXLINE, fp) != NULL) ){
    if ( (n) ) printf("%d: ", ++l);
    while ( (t = parse_token(position)) != '\n')
      if ( (t == EOF) )
	break;

    *position = 0;
  }
  fclose(fp);

  printf("\nAt end of source code, the following definitions exist:\n");
  for (l=0; l < HASHSIZE; l++)
    for (p = hashtab[l]; p != NULL; p = p->next)
      printf("\t\t%s: %s\n", hashtab[l]->name, hashtab[l]->defn);

  return 0;
}

//***************************
// from 6_2.c
//***************************
int isoctal(int c){

  if ( ((c - '0') >= 0) && ((c - '0') <= 8) )
    return 0;
  else
    return -1;
}

void leave(char* word){

  printf("Error: %s", word);
  exit(-1);

}

//***************************
// from 6_5.c
//***************************

struct nlist* findprior(char* s){

  struct nlist* np;
  struct nlist* np_prior;

  for (np_prior = np = hashtab[hash(s)]; np != NULL; np = np->next){
    if (strcmp(s, np->name) == 0)
      return np_prior;
    np_prior = np;
  }

  return np_prior;
}

struct nlist* lookup(char* s){

  struct nlist* np;

  for (np = hashtab[hash(s)]; np != NULL; np = np->next)
    if (strcmp(s, np->name) == 0)
      return np;

  return NULL;
}

// in linked list rooted at hash(name), find nlist* of same hash value, 
// if any, preceding the name to be undefined; then re-link properly
int undef(char* name){

  struct nlist* p;
  struct nlist* p_prior;

  p = lookup(name);
  if (p == NULL)
    return -1;
  p_prior = findprior(name);

  if (!(p_prior == p))
    p_prior->next = p->next;
  else
    hashtab[hash(name)] = NULL;
  free ( (void*) p->name);
  free ( (void*) p->defn);
  free( (void*) p); // free only frees the space, doesn't undefine

  return 0;
}

struct nlist* install(char* name, char* defn){

  struct nlist* np;
  unsigned hashval;

  if ( (np = lookup(name)) == NULL){
    np = (struct nlist*) malloc(sizeof(struct nlist));
    if (np == NULL || (np->name = mystrdup(name)) == NULL)
      return NULL;
    hashval = hash(name);
    np->next = hashtab[hashval];
    hashtab[hashval] = np;
  }
  else
    free( (void*) np->defn);
  if( (np->defn = mystrdup(defn) ) == NULL)
      return NULL;
  else
    return np;
}

unsigned hash(char* s){

  unsigned hashval;

  for (hashval = 0; *s != '\0'; s++)
    hashval = *s + 31*hashval;

  return hashval%HASHSIZE;
}

char* mystrdup(char* s){

  char* p;

  p = (char*) malloc(strlen(s)+1);
  if (p != NULL)
    strcpy(p, s);
  return p;
}

//***************************
// new, or adjusted functions
//***************************

// after function, pos should point to next char to be read
// returning 0 = keep reading at pos; '\n' = next line; EOF = end 
int parse_token(int* pos){

  char word[MAXWORD];
  int c, t;

  c = forward(pos);

  if ( (c == EOF) )
    return c;

  // by processing comments, ', and " first, we ensure no #define in those
  if( (c == '/') ){ // handle comments
    putc(c, stdout);
    if ( ((c = line[++*pos]) == EOF) || (c == '\n') )
      leave("illegal syntax\n"); // only partial correct syntax check
    putc(c, stdout);
    if ( (c == '/') ){ // if not, we can fall through
      if ( (c = line[++*pos]) == EOF)
	return c;
      c = forward(pos);
      t = readdefn(word, MAXWORD, pos);
      if ( (strlen(word) != '\0') ) 
	printf("%s\n", word);
      return t;
    }
  }

  if ( (c == '\"') ){
    if ( (dquotes(pos) == -1) )
      leave("missing closing \" in string constant\n");
    return 0;
  }

  if ( (c == '\'') ){
    quotes(line, pos);
    return 0;
  }
 
  if ( (c == '#') ){ // handle pre-processor lines
    putc(c, stdout);
    if ( !isalpha(c = line[++*pos]) )
      leave("incorrect pre-processor syntax\n");
    readword(word, MAXWORD, pos);
    printf("%s", word); // note that it has at least one char
    if ( (strcmp(word, "define") == 0) ){
      t = add_defn(pos);
    }
    else{
      t = readdefn(word, MAXWORD, pos);
      if ( (strlen(word) != 0) )
	printf("%s\n", word);
    }
    return t;
  }

  if ( (c == '\n') ){
    putc(c, stdout);
    return c;
  }

  if ( !isalpha(c) ){
    putc(c, stdout);
    ++*pos;
  }
  else{
    readword(word, MAXWORD, pos);
    if ( (strlen(word) != 0) ){
      if ( (lookup(word) == NULL) )     
	printf("%s", word);
      else
	printf("%s", lookup(word)->defn);
    }
  }

  return 0;

}

int forward(int* pos){

  int c;

  while ( ((c = line[*pos]) == ' ') || (c == '\t') ){
    putc(c, stdout);
    ++*pos;
  }

  return c;
}

// upon return, points to right after word read
void readword(char* w, int lim, int* pos){

  for ( ; lim-- > 0; w++){
    if ( !isalnum( (*w = line[*pos]) ) )
      break;
    ++*pos;
  }
  *w = '\0';

}

int readdefn(char* w, int lim, int* pos){

  int t;

  for ( ; lim-- > 0; w++){
    if ( ( (*w = line[(*pos)++]) == '\n') ){
      t = '\n';
      break;
    }
    else if ( (*w == EOF) ){
      t = EOF;
      break;
    }
  }

  *w = '\0';
  return t;
}

int add_defn(int* pos){

  char word[MAXWORD], definition[MAXWORD];
  int c, t;

  c = forward(pos);
  if ( !isalpha(c) )
    leave("incorrect pre-processor syntax\n");
  readword(word, MAXWORD, pos);
  printf("%s", word);
  c = forward(pos);
  t = readdefn(definition, MAXWORD, pos);
  if ( (strlen(definition) == 0) )
    leave("pre-processor syntax error: #define without a definition\n");
  printf("%s\n", definition);
  install(word, definition);

  return t;
}

// we leave on error; better return value would be 1 for success; 0 for not
int quotes(char* line, int* pos){

  char c;

  putc('\'', stdout); 
  if ( ((c = line[++*pos]) == '\'') )
    leave("empty character constant '' illegal");

  putc(c, stdout);
  if ( (c == '\\') ){
    c = line[++*pos];
    if ( (escape(c, pos) == -1) )
      leave("illegal escape sequence\n");
  }
  if ( ((c = line[++*pos]) != '\'') )
    leave("illegal character constant\n");
  putc(c, stdout);

  ++*pos;
  return 1;
}

// upon return, pos points at last char of escape sequence
// check for closing ' in calling routine
// we allow (1) letter escape sequences; (2) octal and hex escape sequences
int escape(char c, int* pos){

  int type = -1;

  if ( (c == 'a') || (c == 'b') || (c == 'f') || (c == 'n') || (c == 'r') || 
       (c == 't') || (c == 'v') || (c == '\'') || (c == '\"') || (c == '\\') || 
       (c == '\?') ){
    putc(c, stdout);
    return 0;
  }

  if ( (isoctal(c) == 0) ){
    putc(c, stdout);
    type = 0;
  }
  else if ( (c == 'x') ){
    printf("x");
    if ( isxdigit(c = line[++*pos]) ){
      putc(c, stdout);
      type = 1;
    }
  }

  if ( (type == -1) )
    return -1;
  else if ( (type == 0) ){
    while ( (isoctal(c = line[++*pos]) == 0) )
      putc(c, stdout);
    --*pos;
    return 0;
  }
  else{
    while ( isxdigit(c = line[++*pos]) )
      putc(c, stdout);
    --*pos;
    return 0;
  }
}

// returns 0, and pos on closing " (or -1 if no closing ")
int dquotes(int* pos){

  char c;
  char prior = '\"';

  putc('\"', stdout);
  while ( ((c = line[++*pos]) != '\n') && (c != EOF) )   {
    putc(c, stdout);
    if ( (c == '\"') && !(prior == '\\') ){
      ++*pos;
      return 0;
    }
    prior = c;
  }

  return -1;
}

// t = 0: no '-'; t = 1: '-' followed by illegal option w
void opt_error(char* prog, int w, int t){

  fprintf(stderr, "%s: invalid option \'", prog);
  if ( (t == 0) )
    putc(w, stderr);
  else if ( (t == 1) ){
    fprintf(stderr, "-");
    putc(w, stderr);
  }
  else{
    fprintf(stderr, "%s: invalid use of function opt_error()\n", prog);
    exit(4);
  }

  fprintf(stderr, "\'.\n");
  fprintf(stderr, "%s: usage is %s <file.c> (-n).\n", prog, prog);
  exit(3);
}

