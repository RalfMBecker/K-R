#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAXSTRING 100

void reverse(char* );
void itoa(int, char*);
char* itob(int, char*, int);

int main(){

  int i, j, b;
  char a[MAXSTRING];

  i = 25;
  itoa(i, a);
  printf("i = %d, itoa = %s\n", i, a);
  i = -13;
  itoa(i, a);
  printf("i = %d, itoa = %s\n", i, a);

  b = 2;
  i = 27;
  itob(i, a, b);
  printf("i = %d, itob(%d, a, %d) = %s\n", i, i, b, a);
  b = 16;
  itob(i, a, b);
  printf("i = %d, itob(%d, a, %d) = %s\n", i, i, b, a);

  return 0;
}

/* no special letters, like chars, used for numbers */
/* all digits separated for readability */
char* itob(int n, char* s, int b){

  if (b < 2){
    printf("Error: Invalid base.\n");
    return NULL;
  }

  int i, sign, x, len;

  /* get base descriptor. */
  char a[MAXSTRING] = "";
  char h[MAXSTRING / 2];
  const char col[] =":";
  const char com[] =",";
  const char min[] ="-";

  s[0] = 'b';
  s[1] = '\0';
  itoa(b, h);
  strcat(s, strcat(h, col));

  if ( (sign = n) < 0)
    n = -n;
  i = 0;
  do {
    x = n%b;
    itoa(x, h);
    strcat(a, strcat(h, com));
  } while ( (n/=b) > 0);

  len = strlen(a);
  a[len-1] = '\0'; /* cut out last comma */  
  if (sign < 0)
    strcat(a, min);

  reverse(a);
  strcat(s, a);

  return s;

}

void itoa(int n, char* s){

  int i, sign;

  if ( (sign = n) < 0)
    n = -n;
  i = 0;
  do {
    s[i++] = n%10 + '0';
  } while ( (n/=10) > 0);
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';

  reverse(s);
}

void reverse(char* s){

  int c, i, j;

  for (i =0, j = strlen(s)-1; i < j; i++, j--){
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}
