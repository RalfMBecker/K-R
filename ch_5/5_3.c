#include <stdio.h>
#include <string.h>

char* mystrcat(char*, char*);
char* mystrncat(char*, char*, int);
int mystrend(char*, char*);
char* mystrncpy(char*, char*, int);
int mystrncmp(char*, char*, int);

int main(){

  char target[100];
  char source[100];
  char c1[] = "Summer";
  char c2[] = "Summertime";
  char c3[] = "Summef";
  char c4[] = "time";

  target[0] = 't';
  target[1] = 'a';
  target[2] = 'r';
  target[3] = 'g';
  target[4] = 'e';
  target[5] = 't';
  target[6] = '\0';

  printf("s = %s, t = %s, mystrncmp(s, t, -1) = %d\n", 
	 c1, c2, mystrncmp(c1, c2, -1));
  printf("s = %s, t = %s, mystrncmp(s, t, 6) = %d\n", 
	 c1, c2, mystrncmp(c1, c2, 6));
  printf("s = %s, t = %s, mystrncmp(s, t, 6) = %d\n", 
	 c1, c3, mystrncmp(c1, c3, 6));
  printf("s = %s, t = %s, mystrncmp(s, t, 4) = %d\n", 
	 c1, c3, mystrncmp(c1, c3, 4));
  printf("s = %s, ", target); 
  printf("t = %s, mystrcat(s, t) = %s\n", c1, mystrcat(target, c1));
  printf("s = %s, t = %s, mystrend(s, t) = %d\n", c2, c1, mystrend(c2, c1));
  printf("s = %s, t = %s, mystrend(s, t) = %d\n", c2, c4, mystrend(c2, c4));
  printf("s = %s, ", target);
  printf("t = %s, mystrncat(s, t, 4) = %s\n", c4, mystrncat(target, c4, 4));
  printf("s = %s, ", target); 
  printf("t = %s, mystrncat(s, t, 2) = %s\n", c4, mystrncat(target, c4, 2));
  strcat(source, c2);
  printf("s = %s, ", target);
  printf("t = %s, mystrncpy(s, t, 6) = %s\n", c2, mystrncpy(target, source, 6));
  printf("s = %s, ", target);
  printf("t = %s, mystrncpy(s, t, 2) = %s\n", c4, mystrncpy(target, c4, 2));



  return 0;
}

char* mystrcat(char* a, char* b){

  char* p = a;

  while(*a != '\0')
    a++;

  while ( (*a++ = *b++) )
    ;

  return p;
}

int mystrend(char* a, char* b){

  int l1, l2, n;
  int i = 0;
  char* p;

  l1 = strlen(a);
  l2 = strlen(b);
  if (l2 > l1)
    return -1;

  n = l1-l2;
  p = a+n;
  while ( (i++ < l2) && (*p == *b) )
    p++, b++;
  if (*p == '\0')
    return 0;
  else
    return 1;

}

char* mystrncat(char* a, char* b, int n){

  if ( (n<0) )
    printf("invalid choice for cat length: ");

  int m;
  char* p = a;

  while(*a != '\0')
    a++;

  m = 0;
  while ( (m++ < n) && (*a++ = *b++) )
    ;

  *a = '\0';

  return p;
}

char* mystrncpy(char* a, char* b, int n){

  if ( (n<0) )
    printf("invalid choice for cp length: ");

  int l1;
  int m = 0;
  char* p = a;

  l1 = strlen(a);
  if (n > l1)
    n = l1;

  while ( (m++ < n) &&  (*a++ = *b++) )
    ;

  return p;

}

int mystrncmp(char* a, char* b, int n){

  if ( (n<0) )
    printf("invalid choice for cmp length: ");

  int l1, l2;
  int m = 0;

  l1 = strlen(a);
  l2 = strlen(b);
  if (l2 > l1)
    return -1;

  for ( ; (m++ < n) && (*a == *b); a++, b++){
    if (*b == '\0')
      return 0;
  }

  return *a - *b;

}
