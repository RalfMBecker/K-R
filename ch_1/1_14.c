#include <stdio.h>
#include <stdlib.h>

#define ALPHABET 26
#define MAXCOUNT 25

void histo_v(int*, int);

int main(){

  int i, c, d;
  int a[ALPHABET];

  for ( i=0; i < ALPHABET; i++)
    a[i]=0;

  while ( (c = getchar()) != EOF){
    if (isalpha(c)){
      if ( a[ (d = tolower(c) - 'a') ] <= MAXCOUNT)
	a[d]++;
    }
  }

  //  for ( i=0; i < ALPHABET; i++)
  //  printf("%3d: %3d\n", i+'a', a[i]);

  putchar('\n');
  histo_v(a, ALPHABET);

  return 0;
}

void histo_v(int* a, int len){

  int i, j;

  for (i=0; i < len; i++){
    printf("%3d: ", i+'a');
    for (j=1; j <= a[i]; j++)
      printf("*");
    putchar('\n');
    if (i%10 == 0 && i > 0){
      printf("Hit <enter> for more.");
      while ( getchar() != '\n');
    }
  }

}
