#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXWORD 100

struct tnode {
  char* word;
  int count;
  struct tnode *left;
  struct tnode *right;
};

struct tnode2 {
  int count;
  char** words;
  int size; // number of words in words
  struct tnode2* left;
  struct tnode2* right;
};

struct tnode* addtree(struct tnode*, char*);
void treeprint(struct tnode*);
int getword(char*, int);
struct tnode *talloc(void);
char* mystrdup(char*);

struct tnode2* addtree2(struct tnode2*, int, char*);
void treeprint2(struct tnode2*);
struct tnode2* tnodetotnode2(struct tnode*);
struct tnode2* root2;


int main(){

  struct tnode *root;
  char word[MAXWORD];

  root = NULL;
  root2 = NULL;
  while (getword(word, MAXWORD) != EOF)
    if (isalpha(word[0]))
      root = addtree(root, word);

  root2 = tnodetotnode2(root);
  treeprint2(root2);

  return 0;
}

struct tnode* addtree(struct tnode* p, char* w){

  int cond;

  if ( p == NULL){
    p = talloc();
    p->word = mystrdup(w);
    p->count = 1;
    p->left = p->right = NULL;
  }
  else if ( ( cond = strcmp(w, p->word) ) == 0 )
    p->count++;
  else if (cond < 0)
    p->left = addtree(p->left, w);
  else
    p->right = addtree(p->right, w);

  return p;
}

void treeprint(struct tnode* p){

  if (p != NULL){
    treeprint(p->left);
    printf("%4d %s\n", p->count, p->word);
    treeprint(p->right);
  }
}

struct tnode2* addtree2(struct tnode2* p, int n, char* w){

  if ( p == NULL){
    p = (struct tnode2*) malloc(sizeof(struct tnode2));

    p->words = (char**) malloc(sizeof(char*));
    p->words[0] = (char*) malloc(strlen(w)+1);
    strcpy(p->words[0], w);

    p->count = n;
    p->size = 1;
    p->left = p->right = NULL;
  }
  else if ( p->count == n ){
    p->words = realloc(p->words, (p->size + 1) * sizeof(char*));
    p->words[p->size] = (char*) malloc(strlen(w)+1);  
    strcpy(p->words[p->size], w);
    (p->size)++;
  }
  else if ( p->count < n )
    p->left = addtree2(p->left, n, w);
  else
    p->right = addtree2(p->right, n, w);

  return p;
}

void treeprint2(struct tnode2* p){

  int i, c;

  if (p!= NULL){
    treeprint2(p->left);
    for ( i = 0; i < p->size; i++){
      printf("%d: ", p->count);
      printf("%s\n", p->words[i]);
    }
    treeprint2(p->right);
  }
}

struct tnode2* tnodetotnode2(struct tnode* ps){

  if (ps != NULL){
    tnodetotnode2(ps->left);
    root2 = addtree2(root2, ps->count, ps->word);
    tnodetotnode2(ps->right);
  }

  return root2;
}

struct tnode* talloc(void){

  return (struct tnode*) malloc(sizeof(struct tnode));
}

char* mystrdup(char* s){

  char* p;

  p = (char*) malloc(strlen(s)+1);
  if (p != NULL)
    strcpy(p, s);

  return p;
}

int getword(char* word, int lim){

  int c;
  char* w = word;

  while (isspace( c = getchar() ) )
    ;
  if (c != EOF)
    *w++ = c;
  if (!isalpha(c)){
    *w = '\0';
    return c;
  }

  for ( ; lim-- > 0; w++){
    if ( !isalnum( *w = getchar() ) ){
      ungetc(*w, stdin);
      break;
    }
  }
  *w = '\0';

  return word[0];

}
