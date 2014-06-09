#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syscalls.h"

#define MAXWORD 100

int main(){

  printf("**Not fully tested, eg, max of 20 file open and such.\n");
  printf("**Buffering set small so replenishing of buffer is checked.\n");
  printf("**myfseek not extensively tested.\n");
  printf("**Added filelength after directory chapter. Easy then.\n\n");

  int c;
  myFILE *fpi1, *fpi2, *fpo1, *fpo2;
  char namein1[MAXWORD];
  char namein2[MAXWORD];
  char nameout1[MAXWORD];
  char nameout2[MAXWORD];

  strcpy(namein1, "8_1.c");
  strcpy(namein2, "8_1.c");
  strcpy(nameout1, "testfile1");
  strcpy(nameout2, "testfile2");

  // testing myfopen, myfclose, putc, getc, and briefly myfseek
  myfflush(NULL);

  if ( ( fpi1 = myfopen(namein1, "r") ) == NULL ){
    fprintf(stderr, "Error: can't open file %s for reading\n", namein1);
    exit(1);
  }
  if ( ( fpi2 = myfopen(namein2, "r") ) == NULL ){
    fprintf(stderr, "Error: can't open file %s for reading\n", namein2);
    exit(2);
  }
  else if ( ( fpo1 = myfopen(nameout1, "w") ) == NULL ){
    fprintf(stderr, "Error: can't open file %s for writing\n", nameout1);
    exit(3);
  }
  else if ( ( fpo2 = myfopen(nameout2, "w") ) == NULL ){
    fprintf(stderr, "Error: can't open file %s for writing\n", nameout2);
    exit(4);
  }

  printf("now: filelength(%s) = %ld\tfilelength(%s) = %ld\n", 
	 namein1, filelength(namein1), nameout1, filelength(nameout1));

  while ( ( c = getc(fpi1) ) != EOF )
    putc(c, fpo1);
  //  if (lseek(fpi2->fd, 19, 0) != -1){
  if ( myfseek(fpi2, 19, 0) != EOF){
    while ( ( c = getc(fpi2) ) != EOF )
      putc(c, fpo2);
  }

  printf("now: filelength(%s) = %ld\tfilelength(%s) = %ld\n", 
	 namein1, filelength(namein1), nameout1, filelength(nameout1));

  if ( myfclose(fpi1) == EOF){
    fprintf(stderr, "Error: can't close file %s.\n", namein1); 
    exit(5);
  }
  if ( myfclose(fpi2) == EOF){
    fprintf(stderr, "Error: can't close file %s.\n", namein2); 
    exit(6);
  }
  else if ( myfclose(fpo1) == EOF){
    fprintf(stderr, "Error: can't close file %s.\n", nameout1);
    exit(7);
  }     
  else if ( myfclose(fpo2) == EOF){
    fprintf(stderr, "Error: can't close file %s.\n", nameout2);
    exit(8);
  }     

  printf("now: filelength(%s) = %ld\tfilelength(%s) = %ld\n", 
	 namein1, filelength(namein1), nameout1, filelength(nameout1));
  printf("\t-->note the effect of flushing when closing the files.\n");

  return 0;
}
