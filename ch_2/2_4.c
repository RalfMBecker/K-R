#include <stdio.h>
#include <string.h>

#define MAXSTRING 100

void squeeze(char*, char*);
int any(char*, char*);

int main(){

  char a[] = { 'R', 'a', 'l', 'f', ' ', 'B', 'e', 'c', 'k', 'e', 'r', '\0' };
  char b[] = { 'B', 'e', 'c', 'k', 'e', 'r', 'l', '\0' };
  int d;

  d = any(a, b);
  printf("\n\t%d\n", d);

  printf("%s\n", a);
  squeeze(a, b);
  printf("%s\n\n", a);

  printf("\t%d\n", any(a, b));

  return 0;
}

int any(char* a, char* s){

  int i, j, len;

  len = strlen(s);
  for (i = 0; a[i] != '\0'; ++i){
    for (j = 0; (s[j] != '\0') && (a[i] != s[j]) ; ++j)
      ;
    if (j != len){
      break;
    }
  }

  return i;
}


void squeeze(char* a, char* s){

  int i, j, k, len;

  len = strlen(s);
  for (i = j = 0; a[i] != '\0'; i++){
    for (k = 0; (s[k] != '\0') && (a[i] != s[k]); k++)
      ;
    if (k == len)
      a[j++] = a[i];
  }

  a[j]='\0';
}


