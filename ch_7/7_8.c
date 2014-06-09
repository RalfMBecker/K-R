#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINE 1000
#define SCREENSIZE 23

int mygetline(char*, int, FILE*);
char* pnumber(char*, int, int*, char*);

// Program counting gets seemingly thrown off when a line is too long to be 
// printed in one screenline. This is no logical error though.
int main(int argc, char* argv[]){

  char line[MAXLINE];
  char fname[MAXLINE];
  int c, lineno, psize, page, forward, screenline;
  char* prog;
  FILE* fp;

  prog = *(argv);
  if (argc == 1){
    fprintf(stderr, "%s: usage %s -p<int> <file_1> {<file_2> ... <file_n>}.\n", 
	    prog, prog);
    exit(1);
  }

  psize = SCREENSIZE; // default page size
  while ( --argc > 0 && (*++argv)[0] == '-' ){
    c = *++argv[0];
    while (c != '\0'){
      switch(c){
      case 'p':
	if ( ( argv[0] = pnumber(++argv[0], psize, &psize, prog) ) == NULL){
	  fprintf(stderr, "%s: aborting.\n", prog);
	  exit(2);
	}
	if (psize > SCREENSIZE){
	  printf("%s.warning: max page size allowed is %d", prog, SCREENSIZE);
	  printf("%s. Page size set to %d", prog, SCREENSIZE);
	  psize = SCREENSIZE;
	}
	c = (*argv)[0];
	break;
      default:
	fprintf(stderr, "%s: Usage %s -pxx <file_1> " 
		"{ <file_2> ... <file_n>}, x any digit .\n", prog, prog);
	exit(1);	
	break;
      }
    }
  }

  if (!argc){
    fprintf(stderr, "%s: No files to process.\n", prog);
    fprintf(stderr, "%s: Usage %s -pxx <file_1> " 
	    "{ <file_2> ... <file_n>}, x any digit .\n", prog, prog);
    exit(1);
  }	

  screenline = 0;
  while (!(argc-- == 0) ){
    strcpy(fname, *argv++);
    if ( ( fp = fopen( fname, "r" ) ) == NULL){
      fprintf(stderr, "%s: could not open file %s.\n", prog, fname);
      continue;
    }
    if (screenline){ // processing not first file
      forward = SCREENSIZE - (screenline%SCREENSIZE)+1;
      while (forward-- > 0){
	screenline++;
	printf("%d\n", screenline);
      }
    }
    page = lineno = forward = screenline = 0;
    printf("%s, page %d\n", fname, ++page);
    screenline++;
    while ( mygetline(line, MAXLINE, fp) > 0 ){
      screenline++;
      if ( (++lineno)%psize == 0){
	forward = SCREENSIZE - (screenline%SCREENSIZE)+1;
	screenline--;
	while (forward-- > 0){
	  screenline++;
	  printf("\n");
	}
	printf("%s, page %d\n", fname, ++page);
	screenline += 2; // make up for the one we subtracted before
	forward = 0;
      }
      printf("%s.%d.%d: %s", fname, lineno, screenline, line);
    } // end processing this file (while loop mygetline())
   
  } // end loop over files

  return 0;
}

char* pnumber(char* s, int d, int* size, char* prog){

  if ( s[0] == '\0'){
    fprintf(stderr, "%s: no digit after option -p.\n", prog);
    return NULL;
  }

  int c, n;

  n = 0;
  while ( ( c = *s ) != '\0' ){
    if (!isdigit(c)){
      fprintf(stderr, "%s: invalid char after option -p.\n", prog);
      return NULL;
    }
    n = 10*n + (c - '0');
    s++;
  }

  *size = n;
  return s;
}

int mygetline(char* line, int max, FILE* fp){

  if (fgets(line, max, fp) == NULL)
    return 0;
  else
    return strlen(line);
}
