#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 100
#define MAXLEN 1000

char* lineptr[MAXLINES];

int readlines(char* lineptr[], int nlines);
void writelines(char* lineptr[], int nlines);
int mygetline(char*, int);
void myqsort(void**, int, int, int (*comp)(void*, void*));
int numcmp(char*, char*);

int main(int argc, char* argv[]){

  int nlines;
  int numeric = 0;

  if (argc > 1 && strcmp(argv[1], "\n") == 0)
    numeric = 1;
  if ( (nlines = readlines(lineptr, MAXLINES))  >= 0){
    myqsort( (void**) lineptr, 0, nlines-1, 
	     (int (*)(void*, void*)(numeric ? numcmp : strcmp)) );
    writelines(lineptr, nlines);
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

void writelines(char* lineptr[], int nlines){

  int i;

  for (i = 0; i< nlines; i++){
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

void myqsort(void* v[], int left, int right, int (*comp)(void*, void*) ){

  int i, last;
  void swap(void* v[], int i, int j);

  if (left >= right)
    return;
  swap(v, left, (left+right)/2);
  last = left;
  for (i = left+1; i <= right; i++){
    if ( (*comp)(v[i], v[left]) < 0)
      swap(v, ++last, i);
  }
  swap(v, left, last);
  myqsort(v, left, last-1);
  myqsort(v, last+1, right);

}


int numcmp(char* s1, char* s2){

  double v1, v2;

  v1 = atof(s1);
  v2 = atof(s2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  return 0;

}

void swap(void* v[], int i, int j){

  void* temp;

  temp = v[i];
  v[i] = v[j];
  v[j] = temp;

}
