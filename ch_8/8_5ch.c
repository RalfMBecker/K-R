#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syscalls.h"

#define MAXWORD 100

int main(){

  int c;

  myFILE *fp1;
  FILE *fp2;
  char name[MAXWORD];

  strcpy(name, "8_1.c");
  if ( ( fp1 = myfopen(name, "r") ) == NULL ){
    fprintf(stderr, "Error: can't open file %s for reading\n", name);
    exit(1);
  }
  strcpy(name, "testfile");
  if ( ( fp2 = fopen(name, "w") ) == NULL ){
    fprintf(stderr, "Error: can't open file %s for writing\n", name);
    exit(2);
  }

  while ( ( c = getchar() ) != EOF )
         putc(c, fp2);

  return 0;
}
