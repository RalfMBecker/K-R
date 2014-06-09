#include <stdio.h>
#define albert is really    really fat oh yes // comment
// #define albert redefine
//#define albert redefine
#include <string.h>
#include <stdlib.h>

#define HASHSIZE 101
#define MAXLINE 200
#define MAXWORD 100
#define MAXLINES 500

struct nlist{
  struct nlist* next;
  char* name;
  char* defn;
};

static struct nlist *hashtab[HASHSIZE];
int inquotes; // we assume syntactically correct quotes...
int incomment; // ...and comments
int defined; // added a precompiler definition
char line[MAXLINE];

unsigned hash(char*);
struct nlist* lookup(char*);
struct nlist* install(char*, char*);
int undef(char*);
struct nlist* findprior(char*);
char* mystrdup(char*);
void tokenfunction(void);

int mygetline(int);
int getword(int, char*, int);
int pdefine(int, char*, int);

int main(){

  int i, from, to, c;
  char word[MAXWORD];

  for (i = 0; i < HASHSIZE; i++)
    hashtab[i] = NULL;
  inquotes = 0;
  incomment = 0;
  defined = 0;

  mygetline(MAXLINE);
  printf("%s", line);
  from = to = 0;
  while ( to >= 0 ){
    to = getword(from, word, MAXLINE);
    if (!( ( c = line[to] ) == EOF || c == '\n')){
      printf("position = %d, word = %s, end position = %d\n", from, word, to);
      from = to+1;
    }
    else
      break;
  }


  return 0;
}

int pdefine(int from, char* word, int lim){

  int to;
  char temp[MAXWORD];

  word[0] = '#';
  word[1] = '\0';
  to = getword(from, temp, lim);
  if (!(strcmp(word, "#define") == 0)){
    strcat(word, temp);
    defined = 0;
    return to;
  }


}
  

// return -1 for error; end position of word in line else
int getword(int from, char* word, int lim){

  if (from > lim)
    return -1;

  int c, i, to;
  char* w = word;

  i = from;
  while ( ( c = line[i] ) == ' ' || c == '\t' )
    i++;
  if ( c == '#' ){
    to = pdefine(i+1, word, lim);
    if (defined == 0)
      return to;

  }
  if (c != EOF)
    *w++ = c;
  if (!isalpha(c)){
    *w = '\0';
    return i;
  }
  i++;
  for (; i < lim; i++, w++){
    if (!isalnum(*w = line[i])){
      i--;
      break;
    }
  }
  *w = '\0';

  return i;
}


int mygetline(int lim){

  line[0] = '\0';
  int i, c;

  i = 0;
  while ( --lim > 0 && ( c = getchar() ) != EOF && c != '\n')
    line[i++] = c;
  if ( c == '\n' )
    line[i++] = c;
  line[i] = c;

  return i;
}
 



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

int undef(char* name){

  struct nlist* p;
  struct nlist* p_prior;

  p = lookup(name);
  if (p == NULL)
    return -1;
  p_prior = findprior(name);

  if (!(p_prior == p))
    p_prior->next = p->next;
  p->name[0] = '\0';
  p->defn[0] = '\0';
  p->next = NULL;
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

void tokenfunction(void){

  int char[MAXWORD];
#undef MAXWORD
  int char2[MAXWORD];

}
