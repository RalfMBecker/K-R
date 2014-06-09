#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define HASHSIZE 5
#define MAXLINE 200
#define MAXWORD 100
#define MAXCAT 80

struct nlist{
  struct nlist* next;
  char* name;
  char* defn;
};
static struct nlist *hashtab[HASHSIZE];

struct exclusion{
  struct exclusion *next;
  int from;
  int to;
};

unsigned hash(char*);
struct nlist* lookup(char*);
struct nlist* install(char*, char*);
int undef(char*);
struct nlist* findprior(char*);
char* mystrdup(char*);

int mygetline(char*, int);
int getname(int, char*, char*, int);
int getdefn(int, int, char*, char*, int, int);
int translate(void);
struct exclusion* build_exclusion(int*, struct exclusion*, char*, int);
struct exclusion* ealloc(struct exclusion*, int, int);
struct exclusion* efree(struct exclusion*);
char* getfirst(int*, char*, int*, int);
int in_exc_list(int, struct exclusion*);
int replace(struct nlist**, struct exclusion*, char*);

int main(){

  printf("\nProgram performs #define "
	 "and #undefine functions of precompiler.\n"
         "Testing can be done with 'testfile' and 'testfile1.\n\n");

  int rv, i;

  for (i = 0; i < HASHSIZE; i++)
    hashtab[i] = NULL;

  rv = translate();
  if (rv == -1)
    printf("Error: Translation failed.\n");

  return 0;
}

// assumes no more than 1 comment/line
int translate(void){

  int i, from, c, len, suspended;
  char *cp;
  char name[MAXWORD];
  char defn[MAXWORD];
  char line[MAXLINE];

  const char empty[1] = "\0";
  const char def[] = "#define";
  const char undefn[] = "#undef";

  struct exclusion *exc=(struct exclusion*) malloc(sizeof (struct exclusion));
  exc->next = NULL;
  exc->from = exc->to = 0;
  suspended = 0; // not currently in comment

  // **************************************
  // search for this
  // **************************************

  while (mygetline(line, MAXLINE)>0){
    printf("%s\n", line);
    i = 0;
    len = strlen(line);
    while ( (i < len) && ( c = line[i] ) == ' ' || c == '\t' )
      i++;
    if ( i == len || line[0] == '\0' ){ // 2nd catches only \n case
      printf("translation:\nempty line\n\n");
      continue;
    }

    struct exclusion *tmp;
    if ( !(exc = efree(exc)) )
	 printf("error: can't free exclusion list.\n");
    // build linked list of excluded areas in line first
    if (tmp = build_exclusion(&suspended, exc, line, len))
      exc = tmp;
    else
      continue;

    struct exclusion *test;
    if (exc->next)
      printf("0123456789012345678901234567890123456789012345678901234\n"); 
    for (test = exc; test->next != NULL; test = test->next){
      printf("from = %d\tto = %d\n", test->from, test->to); 
    }

    // ********* check and add exclusions

    // *****************************************************
    // do #define first.
    // allow only #define as first token in line after space
    // this means that if not, check if excluded - or error
    // logic fails for #undef test #define (but probably should)
    // *****************************************************
    from = i;
    if ( cp = strstr(line, def) ){
      if ( !(cp - &line[from]) ){
	from += strlen(def);
	if ( line[from] != ' ' && line[from] != '\t' ){
	  printf("error: #define should be followed by space or tab.\n\n");
	  continue;
	}
	if ( (from = getname(from, name, line, MAXLINE)) == -1 )
	  continue;
	if ( !( ((c = line[from]) == ' ') || (c == '\t') ) ){
	  printf("error: name in #define not followed by space or tab\n");
	  continue;
	}
	if ( getdefn(from, 0, defn, line, MAXLINE, MAXWORD) == -1 )
	  continue;
	if (strcmp(defn, empty) == 0){
	  printf("no definition for %s in line\n\n", name);
	  continue;
	}
	if ( (install(name, defn) == NULL) )
	  printf("error: can't add definition of %s to #define's.\n\n", name);
	printf("translation:\ndefined %s to be %s\n\n", name, defn);
	continue;
      } // end of if in #define part
      else if ( in_exc_list(cp - &line[0], exc) )
	;
      else{
	printf("error: #define should be 1st on line after space.\n\n");
	continue;
      }
    } // end of # define 

    // *****************************************************
    // do #undef next.
    // allow only #undef as first token in line after space
    // this means that if not, check if excluded - or error
    // logic fails for #define test #undef (but probably should)
    // *****************************************************
    if ( cp = strstr(line, undefn) ){
      if ( !(cp - &line[from]) ){
	from += strlen(undefn);
	if ( line[from] != ' ' && line[from] != '\t' ){
	  printf("error: #undef should be followed by space or tab.\n\n");
	  continue;
	}
	if ( (from = getname(from, name, line, MAXLINE)) == -1 )
	  continue;
	if ( undef(name) != 0 ){
	  printf("error: problem undefining %s.\n\n", name);
	  continue;
	}
	printf("translation:\nundefined %s.\n\n", name);
	continue;
      } // end of if in #undef part
      else if ( in_exc_list(cp - &line[0], exc) )
	;
      else{
	printf("error: #undef should be 1st on line after space.\n\n");
	continue;
      }
    } // end of #undef 

    // ******************************************
    // if we get here, need to check for macro replacement

    if ( !replace(hashtab, exc, line)){
      printf("error: problem replacing macro.\n");
      return 0;
    }
    else
      printf("%s\n", line);
  } // end of outer loop to get lines

  return 0;
}

// fill linked list of areas in quotes or comments (exc: var in calling fct)
// return: NULL for error; new head of linked list else
// keep track of multiline comment in still_in (suspended) through pointer
struct exclusion* build_exclusion(int *still_in, struct exclusion *exc, 
				  char* line, int len){

  struct delimiter{
    char begin[3];
    char end[3];
  };
  struct delimiter delimiter_list[3] = {
    {"\"", "\""},
    {"\'", "\'"},
    {"/*", "*/"} };

  int c, from, to, index, going;
  char *cp;
  struct exclusion *ep;

  // opening comment type 2 in prior line
  from = to = 0;
  if (*still_in){
    cp = strstr(line, "*/");
    if (!cp) // multi-line comment
      return exc;
    else{
      to = (cp - &line[0])+2;
      if ( !(exc = ealloc(exc, from, to)) ){
	  printf("error: can't allocate exclusion on heap.\n");
	  return NULL;
	}
      from = to;
    }
  }

  *still_in = 0;
  going = 1;
  while (going){

    cp = getfirst(&index, line, &from, len);
    switch(index){
    case 3: // if we find '//', can return after processing
      going = 0;
      to = len;
      if ( (ep = ealloc(exc, from, to)) == NULL ){
	printf("error: can't allocate exclusion on heap.\n");
	return NULL;
      }
      else
	return ep;
      break;
    case 0: case 1:
      if ( !(cp = strstr(line+from+1, delimiter_list[index].end)) ){
	if (index == 0 || (index == 1 && !from) || 
	    (index == 1 && line[from-1] != '\\') ){ // note short-circuiting
	  printf("error: found %s, but no matching closing delimiter %s.\n",
		 delimiter_list[index].begin, delimiter_list[index].end); 
	  return NULL;
	}
	else{ // found ' in escaped form (\')
	  from++;
	  going = 1;
	  break;
	}
      }
      to = cp - &line[0];
      if ( !(ep = ealloc(exc, from, to)) ){
	printf("error: can't allocate exclusion on heap.\n");
	return NULL;
      }
      else{
	if (to == len)
	  return ep;
	else{
	  exc = ep;
	  from = to+1;
	  going = 1;
	}
      }
      break;
    case 2: // if multi-line comment, won't find closing match
      cp = strstr(line+from+1, delimiter_list[index].end);
      if (!cp){
	*still_in = 1;
	to = len;
      }
      else{
	*still_in = 0;
	to = (cp - &line[0])+2;
      }
      if ( !(ep = ealloc(exc, from, to)) ){
	printf("error: can't allocate exclusion on heap.\n");
	return NULL;
      }
      if (to == len)
	return ep;
      else{
	  exc = ep;
	  from = to+1;
	  going = 1;
      }
      break;
    case -1: default:
      going = 0;
      break;
    } // end switching through cases
  } // end while going

  return exc;  // if we fall through, send back what was sent
}

// returns pointer to first excluded token (or NULL if none)
// also returns related index into a delimiter list:
// -1: NULL; 0: "; 1: '; 2: /*; 3: //
// from will be adjusted as well to match what was found
char* getfirst(int *index, char *line, int *from, int len){

  char *delimiters[] = { "\"", "\'", "/*", "//" };  

  int i, c, nfrom[4], found[4];
  char *cp, *p[4];

  *index = -1;
  cp = NULL;
  for (i=0; i<4; i++)
    found[i] = 0;

  for (i=0; i<4; i++)
    if ( (p[i] = strstr(line+*from, delimiters[i])) ){ 
      nfrom[i] = p[i] - &line[0];
      found[i] = 1;
    }
    else
      p[i] = &line[len] + 1;

  if ( !(found[0] || found[1] || found[2] || found[3]) )
    return NULL; // no need to change 'from'

  //  for (i = 0; i<4; i++)
  //  if (found[i])
  //    printf("found a %s at %d; index is %d\n", 
  //	     delimiters[i], nfrom[i], i);

  // when equal, both were not found (see else above)
  if (p[0] < p[1]){
    cp = p[0];
    *index = 0;
  }
  else if (p[1] < p[0]){
    cp = p[1];
    *index = 1;
  }
  else
    cp = p[0]; // artificially set high in line to use in comp below

  if (p[2] < p[3]){
    if (p[2] < cp)
      *index = 2;
  }
  else if (p[3] < p[2]){
    if (p[3] < cp)
      *index = 3;
  }

  *from = nfrom[*index];
  return p[*index];
}

// is the location pointed to by loc excluded by the linked list?
// 0: no; 1: yes
int in_exc_list(int loc, struct exclusion *list){

  struct exclusion *p;

  for (p=list; p->next; p = p->next)
    if ( (loc >= p->from) && (loc <= p->to) )
      return 1; 

  return 0;
}

// no check if replacement makes line too long
int replace(struct nlist **hashtab, struct exclusion *exc, char *line){

  int i, from, nlen, dlen, delta; // delta adjusts line length post rplcmnt
  char first[MAXWORD], second[MAXWORD];
  char *cn, *cd, *cp;
  struct nlist *nlp;
  struct exclusion *ep;

  strcpy(first, line);
  for (i=0; i<HASHSIZE; i++){
    for (nlp = hashtab[i]; nlp != NULL; nlp = nlp->next){
      delta = 0;
      from = 0;
      nlen = strlen(cn = nlp->name);
      dlen = strlen(cd = nlp->defn);
      while (cp = strstr(first+from, cn)){
	  from = cp - &first[0];
	  if (in_exc_list(from+delta, exc)){
	    from += nlen;
	    continue;
	  }
	  strcpy(second, first+from+nlen);
	  first[from]='\0';
	  strncat(first, cd, MAXCAT);
	  strncat(first, second, MAXCAT);
	  from += dlen;
	  delta += nlen - dlen;
      } // inner while
    } // outer while
  } // for loop

  strcpy(line, first);
  return 1;
}

struct exclusion* ealloc(struct exclusion* ep, int from, int to){

  struct exclusion* p;

  if ( !(p = (struct exclusion*) malloc (sizeof (struct exclusion))) )
    return NULL;
  p->from = from;
  p->to = to;
  p->next = ep;
  ep = p;

  return ep;
}

// frees the entire exclusion linked list
struct exclusion* efree(struct exclusion* exc){

  struct exclusion* p;

  while ( (p = exc->next) ){
    free ( (void*) exc );
    exc = p;    
  }

  return exc; // returns old last element of linked list
 }
  
// return -1 for error; search from 'from'
// position returned is one after last space or tab
// function is lax in checking for name lims
int getname(int from, char* word, char* line, int lim){

  if (from > lim)
    return -1;

  int c, i;
  char *w = word;

  i = from;
  while ( (i < lim) && ( c = line[i] ) == ' ' || c == '\t' || c == '\n' )
    i++;
  if ( !isalpha(*w = c) && !(c == '_') ){
    *w = '\0';
    printf("error: invalid name in #define\n");
    return -1;
  }
  for ( ; i < lim && ( isalnum(c = line[i]) || c == '_' ); i++, w++ )
    *w = c;
  *w = '\0';

  return i;
}

// return -1 for error; 0 if ok; search from 'from'
// no check if replacement text is gramatically ok
int getdefn(int from_l, int from_w, char* word, char* l, int lim_l, int lim_w){

  if (from_w > lim_w)
    return -1;

  int c, d, i, j;
  j = from_w;
  char *w = word+j;

  i = from_l;
  while ( (i < lim_l) && ( c = l[i] ) == ' ' || c == '\t' )
    i++, j++; // need to split for recursive call
  for ( ; (i < lim_l) && (j < lim_w) && 
	  ( c = l[i] ) != '\0' && c != '\\'; i++, j++, w++ ){
    if ( (c == '/') && ( (d = l[i+1]) == '/' || d == '*') ){
    *w = '\0';
    break;
    }
    *w = c;
  }
  if (j == lim_w){
    printf("error: allowed length for definition exceeded\n");
    return -1;
  }
  if ( c == '\\'){
    mygetline(l, MAXLINE);
    printf("%s\n", l);
    if ( !getdefn(0, j-1, word, l, MAXLINE, MAXWORD) )
      return 0;
    else
      printf("error in multi-line definition\n");
    return -1;
  }
  *w = '\0';
  return 0;
}

// cuts off final \n; reports lines with ony \n as length 1
int mygetline(char* line, int lim){

  line[0] = '\0';
  int i, c;

  i = 0;
  while ( --lim > 0 && ( c = getchar() ) != EOF && c != '\n')
    line[i++] = c;
  if ( (i == 0) && ( c == '\n') )
    return 1;
  line[i] = '\0';

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

  free( (void*) p->name);
  free( (void*) p->defn);
  if (!(p_prior == p))
    p_prior->next = p->next;
  else
    hashtab[hash(name)] = p->next;
 
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
