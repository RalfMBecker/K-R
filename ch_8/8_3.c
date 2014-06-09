#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syscalls.h"

#define MAXWORD 100

int main(){

  printf("**Not fully tested, eg, max of 20 file open and such.\n");
  printf("**Buffering set small so replenishing of buffer is checked.\n");

  int c;

  myFILE *fp1;
  myFILE *fp2;
  char namein[MAXWORD];
  char nameout[MAXWORD];

  strcpy(namein, "8_1.c");
  strcpy(nameout, "testfile");

  myfflush(NULL);
  if ( ( fp1 = myfopen(namein, "r") ) == NULL ){
    fprintf(stderr, "Error: can't open file %s for reading\n", namein);
    exit(1);
  }
  else if ( ( fp2 = myfopen(nameout, "w") ) == NULL ){
    fprintf(stderr, "Error: can't open file %s for writing\n", nameout);
    exit(2);
  }

  while ( ( c = getc(fp1) ) != EOF )
    putc(c, fp2);
	     
  if ( myfclose(fp1) == EOF){
    fprintf(stderr, "Error: can't close file %s.\n", namein); 
    exit(3);
  }
  else if ( myfclose(fp1) == EOF){
    fprintf(stderr, "Error: can't close file %s.\n", nameout);
    exit(4);
  } 

  return 0;
}
