#include <stdio.h>
#include <string.h>

#include "syscalls.h"

int main(){

  int i, *it;
  char ralf[] = "Ralf M. Becker";
  char ralf1[] = "Ralf M. Becker12345678901234567890";
  char* ralft;
  int test[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  int* testt;

  i = 5;
  it = mymalloc(sizeof(int));
  ralft = mymalloc( (sizeof ralf) + 1);
  testt = mymalloc(sizeof test);

  *it = 10;
  strcpy(ralft, ralf);
  for (i = 0; i < 9; i++)
    testt[i] = test[i];
  for (i = 0; i < 9; i++){
    printf("%d: %s %d\t", testt[i], ralft, *it);
    printf("at: %p %p %p\n", &it, &ralft, &testt);
  }
  printf("After copying in string too long for char array:\n");
  strcpy(ralft, ralf1);
  for (i = 0; i < 9; i++){
    printf("%d: %s %d\t", testt[i], ralft, *it);
    printf("at: %p %p %p\n", &it, &ralft, &testt);
  }

  return 0;
}
