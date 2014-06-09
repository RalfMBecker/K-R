#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 200
#define MAXLINES 500

int complines(char*, char*);

int main(int argc, char* argv[]){

  FILE *fp1, *fp2;
  int len, i, rv;
  char line1[MAXLINE];
  char line2[MAXLINE];
  char* prog = argv[0];

  if (argc != 3){
    fprintf(stderr, "%s: usage is %s <file1.c> <file2.c>.\n", prog, prog);
    exit(1);
  }
  if ( ( fp1 = fopen(*++argv, "r") ) == NULL ){ 
    fprintf(stderr, "%s: can't open file %s.\n", prog, *argv);
    exit(2);
  }
  else if ( ( fp2 = fopen(*++argv, "r") ) == NULL ){ 
    fprintf(stderr, "%s: can't open file %s.\n", prog, *argv);
    exit(3);
  }
  else{
    i = 1; // 1 based counting
    len = MAXLINES;
    while ( len -- > 0 && 
	    fgets(line1, len, fp1) != NULL &&
	    fgets(line2, len, fp2) != NULL &&
	    (  rv = complines(line1, line2) ) != -1 ){
      i++; 
    }
   if (rv == -1){
      printf("The files differ first in line %d:\n\n", i);
      printf("%s: %s", argv[-1], line1);
      printf("%s: %s", *argv, line2);
    }
   else
     printf("Both files are the same.\n"); 
  }

  fclose(fp1);
  fclose(fp2);
  return 0;
}

int complines(char* line1, char* line2){

  int c, d;
  char *p1, *p2;

  p1 = line1;
  p2 = line2;

  while ( ( c = *(p1++) ) == ( d = *(p2++) ) &&
	  c != EOF && d != EOF && c != '\0' && d != '\0')
    ;

  if (c != d)
    return -1;

  return 0;
}




