#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 1000

int mygetline(char*, int, FILE*);

int main(int argc, char* argv[]){

  char line[MAXLINE];
  char pattern[MAXLINE];
  char fname[MAXLINE];
  char info[] = "0";
  int c, lineno, except, number, found, foundfile;
  char* prog;
  FILE* fp;

  lineno = except = number = found = foundfile = 0;
  prog = argv[0];

  while (--argc > 0 && (*++argv)[0] == '-'){
    while (c = *++argv[0]){
      switch(c){
      case 'x':
	except = 1;
	break;
      case 'n':
	number = 1;
	break;
      default:
	info[0] = c;
	fprintf(stderr, "%s: illegal option %s.\n", prog, info);
	exit(1);	
	break;
      }
    }
  } // end outermost while loop

  if (argc == 0){
    fprintf(stderr, "%s: usage %s -x -n pattern {<file_1> ... <file_n>}.\n", 
	    prog, prog);
    exit(2);
  }
  else{
    strcpy(pattern, *(argv++));
    argc--;
  }

  if (!argc){
    while ( mygetline(line, MAXLINE, stdin) > 0 ){
      lineno++;
      if ( ( strstr(line, pattern) != NULL) != except ){
	if (number)
	  printf("%d:", lineno);
	printf("%s", line);
	found++;
      }
    }
  }
  else{
    while (!(argc-- == 0)){
      strcpy(fname, *argv++);
      foundfile = 0;
      lineno = 0;
      if ( ( fp = fopen( fname, "r" ) ) == NULL){
	fprintf(stderr, "%s: could not open file %s.\n", prog, fname);
	continue;
      }
      else{
	printf("\n%s:\n", fname);
	while ( mygetline(line, MAXLINE, fp) > 0 ){
	  lineno++;
	  if ( ( strstr(line, pattern) != NULL) != except ){
	    if (number)
	      printf("%s.%d:", fname, lineno);
	    printf("%s", line);
	    found++;
	    foundfile++;
	  }
	}
      } // end loop within this file, aka, coming from else
      printf("%s: Total # of lines found: %d.\n", fname, foundfile);     
   } // end loop over files
  }


  printf("\nTotal # of lines found from all files: %d.\n", found);

  return found;
}

int mygetline(char* line, int max, FILE* fp){

  if (fgets(line, max, fp) == NULL)
    return 0;
  else
    return strlen(line);
}
