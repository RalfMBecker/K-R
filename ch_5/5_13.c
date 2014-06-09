#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINES 100
#define MAXLEN 1000

char* lineptr[MAXLINES];

int readlines(char* lineptr[], int nlines);
void writelines(char* lineptr[], int nlines, int from);
int mygetline(char*, int);

int main(int argc, char* argv[]){

  int nlines, from, i;
  char s[20];

  if (argc > 2){
    printf("Error: usage is tail -nnn\n");
    return 0;
  }
  else if (argc == 1)
    from = 10;
  else{
    if ( (*++argv)[0] != '-'){
      printf("Error: usage is tail -nnn\n");
      return 0;
    }
    i = 0;
    while ( isdigit( s[i] = (*argv)[i+1] ) )
      i++;
    if ( (*argv)[i+1] != '\0'){
      printf("Error: usage is tail -nnn\n");
      return 0;
    }
    s[i] = '\0';
    from = atoi(s);
  }

  if ( (nlines = readlines(lineptr, MAXLINES))  >= 0){
    printf("\nTail -%d:\n", from);
    from = (from > nlines)?0:(nlines-from);
    writelines(lineptr, nlines, from);
    return 0;
  }
  else{
    printf("Error: line overlow\n");
    return 1;
  }

return 0;

}

int readlines(char* lineptr[], int maxlines){

  int len, nlines;
  char* p, line[MAXLEN];

  nlines = 0;
  while ( ( len = mygetline(line, MAXLEN) ) > 0)
    if (nlines >= maxlines || (p = (char*) 
			       malloc(len*sizeof(char)) ) == NULL)
   return -1;
  
  else{
    line[len-1] = '\0';
    strcpy(p, line);
    lineptr[nlines++] = p;
  }

return nlines;

}

void writelines(char* lineptr[], int nlines, int from){

  int i;

  if (from > nlines)
    from = nlines;

  for (i = from; i< nlines; i++){
    printf("%s\n", lineptr[i]);
    free(lineptr[i]);
  }

}

int mygetline(char* s, int lim){

  int c, i;

  for (i = 0; i < lim-1 && ( c = getchar() ) != EOF && c!= '\n'; i++)
    s[i] = c;
  if (c == '\n'){
    s[i] = c;
    ++i;
  }
  s[i] = '\0';

  return i;

}
