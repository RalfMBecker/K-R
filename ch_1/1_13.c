#include <stdio.h>
#include <string.h>

#define MAXLEN 6
#define MAXARR 100

enum boolean { NO = 0, YES };

int rword(char*);
int max_array_val(int*, int);
void histo_v(int*, int);
void histo_h(int*, int, int);

int main(){

  int i, c, d;
  int a[MAXLEN+1]; /* extra space for all larger than MAXLEN */
  char b[MAXARR];
  b[0]='\0';
  for ( i=0; i <= MAXLEN; i++)
    a[i]=0;

  while ( (c = rword(b)) == NO){
    d = strlen(b);
    if (d > MAXLEN)
      d = 0;
    a[d]++;
  }
  if ( (d = strlen(b)) != 0) /* for word containing EOF, if not only EOF */{
    if (d > MAXLEN)
      d = 0;
    a[d]++;
  }

  //  for ( i=1; i <= MAXLEN; i++)
  //    printf("Words of length %d: %d\n", i, a[i]);
  //  printf("Larger words\t: %d\n\n", a[0]);

  putchar('\n');
  histo_v(a, MAXLEN+1);
  printf("\nHit <enter> to continue.\n");
  while (getchar() != '\n')
    ;
  histo_h(a, MAXLEN, max_array_val(a, MAXLEN));

  return 0;
}

int rword(char* a){

  int c, i;
  i = 0;

  while ( i < MAXARR-1 && (c = getchar()) != EOF &&
	  c != '\t' && c != '\n' && c != ' '){
   a[i++] = c;
  }
  a[i] = '\0';

  return (c == EOF) ? YES : NO;
}

int max_array_val(int* a, int len){

  int i, max=0;

  for (i=0; i < len; i++){
    if ( a[i] > max )
      max = a[i];
  }

  return max;
}

void histo_v(int* a, int len){

  int i, j;

  for (i=1; i < len; i++){
    printf("Length %3d: ", i);
    for (j=1; j <= a[i]; j++)
      printf("*");
    putchar('\n');
  }
  printf("Length %2d+: ", MAXLEN);
  for (j=1; j <= a[0]; j++)
    printf("*");
  putchar('\n');

}

void histo_h(int* a, int len, int max){

  int i, j;

  for (j=max; j>0; j--){
    for (i=1; i<=len; i++){
      if (a[i] >= j)
	printf(" * ");
      else
	printf("   ");
    }
    if (a[0] >= j)
      printf(" * ");
    else
      printf("   ");

    putchar('\n');
  }
  for (i=0; i<=len; i++)
    printf("---");
  putchar('\n');
  for (i=1; i<=len; i++)
    printf("%2d ", i);
  printf("%2d+\n", MAXLEN);
}
