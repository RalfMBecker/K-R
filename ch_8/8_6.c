#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "alloc.h"

int main(){

  printf("Recall that for **, we should use dynamic arrays, not calloc().\n");

  int times, i; 
  int ralf[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  int ralf1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
		  1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		  1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		  1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  int* ralft;

  times = 10;
  /*
  printf("\nUsing calloc():\n");
  ralft = (int*) calloc(times, sizeof(int));
  for (i = 0; i < times; i++)
    ralft[i] = ralf[i];
  for (i = 0; i < times; i++)
    printf("%d:\t%d\tat: %p\n", i, ralft[i], &ralft[i]);
  printf("After allocating array that is too big:\n");
  for (i = 0; i < 4*times; i++)
    ralft[i] = ralf1[i];
  for (i = 0; i < 4*times; i++)
    printf("%d:\t%d\tat: %p\n", i, ralft[i], &ralft[i]);
  */
  printf("\n\nUsing mycalloc():\n");
  ralft = (int*) mycalloc(times, sizeof(int));
  printf("Confirm that allocated array was zeroed out:\n");
  for (i = 0; i < times; i++)
    printf("%d:\t%d\tat: %p\n", i, ralft[i], &ralft[i]);
  printf("After copying in:\n");
  for (i = 0; i < times; i++)
    ralft[i] = ralf[i];
  for (i = 0; i < times; i++)
    printf("%d:\t%d\tat: %p\n", i, ralft[i], &ralft[i]);

  printf("After allocating array that is too big:\n");
  for (i = 0; i < (int) (3*times) -2; i++)
    ralft[i] = ralf1[i];
  for (i = 0; i < (int) (3*times) -2; i++)
    printf("%d:\t%d\tat: %p\n", i, ralft[i], &ralft[i]);
  printf("\t**Segementation fault for any larger size.\n");


  return 0;
}
