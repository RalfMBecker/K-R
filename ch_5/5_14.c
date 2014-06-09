#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINES 100
#define MAXLEN 1000

char* lineptr[MAXLINES];

int readlines(char* lineptr[], int nlines);
void writelines(char* lineptr[], int nlines);
void myqsort(void* lineptr[], int left, int right, 
	     int (*comp)(void*, void*), int reverse, int fold);
int numcmp(char*, char*);

int main(int argc, char* argv[]){

  int nlines, c;
  int numeric = 0;
  int reverse = 0;
  int fold = 0;

  if (argc > 4){
    printf("Error: function used as function -n -r -f\n");
    return 0;
  }
  while (--argc > 0){
    if ( (*++argv)[0] != '-'){
      printf("Error: function used as function -n -r -f\n");;
      return 0;
    }
    else{
      while ( (c = *++argv[0]) )
	switch(c){
	case 'n': 
	  numeric = 1;
	  break;
	case 'r':
	  reverse = 1;
	  break;
	case 'f':
	  fold = 1;
	  break;
	default:
	  printf("Error: function used as function -n -r -f\n");
	  return 0;
	}
    }
  }

  if ( (nlines = readlines(lineptr, MAXLINES))  >= 0){
    myqsort( (void**) lineptr, 0, nlines-1, 
	     ( int (*)(void*, void*))(numeric ? numcmp : strcmp), 
	     reverse, fold );
    printf("\nSorted:\n");
    writelines(lineptr, nlines);
    return 0;
  }
  else{
    printf("Error: line overflow\n");
    return -1;
  }

return 0;

}

int readlines(char* lineptr[], int maxlines){

  int len, nlines;
  char* p, line[MAXLEN];
  int mygetline(char*, int);

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

void myqsort(void* v[], int left, int right, 
	     int (*comp)(void*, void*), int rev , int fold){

  int i, last;
  void swap(void* v[], int i, int j);
  char s1[MAXLEN], s2[MAXLEN];
  char* stoupper(char*);

  if (left >= right)
    return;
  swap(v, left, (left+right)/2);
  last = left;
  for (i = left+1; i <= right; i++){
    if (rev){
      if (fold){
	strcpy(s1, v[i]);
	strcpy(s2, v[left]);
	if ( (*comp)(stoupper(s1), stoupper(s2)) > 0)
	  swap(v, ++last, i);
      }
      else{
	if ( (*comp)(v[i], v[left]) > 0)
	  swap(v, ++last, i);
      }
    }
    else{
      if (fold){
	strcpy(s1, v[i]);
	strcpy(s2, v[left]);
	if ( (*comp)(stoupper(s1), stoupper(s2)) < 0)
	  swap(v, ++last, i);
      }
      else{
	if ( (*comp)(v[i], v[left]) < 0)
	  swap(v, ++last, i);
      }
    }
  }
  swap(v, left, last);
  myqsort(v, left, last-1, comp, rev, fold);
  myqsort(v, last+1, right, comp, rev, fold);

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

char* stoupper(char* s){

  char* p = s;
  int z = -1;

  while ( (p[++z] != '\0') )
    p[z] = toupper(s[z]);

  return s;

}
