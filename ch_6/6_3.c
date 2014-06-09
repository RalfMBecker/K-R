#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXWORD 1000

struct tnode {
  char* word;
  char* lines;
  struct tnode *left;
  struct tnode *right;
};
int linenum;

struct tnode* addtree(struct tnode*, char*);
void treeprint(struct tnode*);
int getword(char*, int);
struct tnode *talloc(void);
char* mystrdup(char*);
void reverse(char*);
void itoa(int, char*);

int main(){

  struct tnode *root;
  char word[MAXWORD];

  printf("\n***Reports multiple occurances in one line for each occurance.\n");
  printf("***This could be easily adjusted in 'addtree'.\n\n");

  linenum = 0;
  root = NULL;
  while (getword(word, MAXWORD) != EOF)
    if (isalpha(word[0])){
      root = addtree(root, word);
      if (root == NULL){
	printf("Critical Error: aborting.\n");
	return 0;
      }
    }
  printf("\n");
  treeprint(root);

  return 0;
}

struct tnode* addtree(struct tnode* p, char* w){

  int cond, len;
  char* q;
  char s[MAXWORD];

  if ( p == NULL){
    p = talloc();
    p->word = mystrdup(w);
    itoa(linenum, s);
    len = strlen(s);
    p->lines = (char*) malloc(len+2);
    strcpy(p->lines, s);
    strcat(p->lines, " ");
    p->left = p->right = NULL;
  }
  else if ( ( cond = strcmp(w, p->word) ) == 0 ){
    itoa(linenum, s);
    len = strlen(p->lines) + strlen(s);
    q = (char*) realloc(p->lines, len+2);
    if (q == NULL){
      printf("Error: overflow. Too many lines in which a word occurs.\n");
      return NULL;
    }
    p->lines = q;
    strcat(p->lines, s);
    strcat(p->lines, " ");
  }
  else if (cond < 0)
    p->left = addtree(p->left, w);
  else
    p->right = addtree(p->right, w);

  return p;
}

void treeprint(struct tnode* p){

  int len, i;

  if (p != NULL){
    treeprint(p->left);
    printf("%s: ", p->word);
    printf("%s\n", p->lines);

    treeprint(p->right);
  }
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

  while (isspace( c = getchar() ) && c !='\n' )
    ;
  if ( c == '\n' )
    linenum++;
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

void reverse(char* s){

  int c, i, j;

  for (i = 0, j = strlen(s)-1; i < j; i++, j--){ 
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void itoa(int n, char* s){

  int i;

  i = 0;
  do{
    s[i++] = n%10 +'0';
  } while ( (n /= 10) > 0);
  s[i] ='\0';
  reverse(s);
}
