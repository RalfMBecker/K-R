#include <stdio.h>
#include <string.h>

#include "alloc.h"

//#include "syscalls.h"

int main(){

  printf("Just check if command runs error free.\n\n");

  char name[] = "Ralf Becker";
  int len, n, *p;

  len = strlen(name);
  printf("Allocating 1 more int...\n");
  p = mymalloc(sizeof(int));
  printf("Free list starting at: %p\n", p);
  printf("Allocating 1 more int...\n");
  p = mymalloc(sizeof(int));
  printf("Free list starting at: %p\n", p);
  printf("Allocating 1 more int...\n");
  p = mymalloc(sizeof(int));
  printf("Free list starting at: %p\n", p);

  
  printf("Adding char[] of size %d to free list using bfree...\n", len);
  n = mybfree(name, len);
  printf("Sizeof(Header): %d...\n", sizeof(Header));
  printf("...with %d spare chars not allocated.\n", n);  

  return 0;
}
