#include <stdio.h>
#include "dirent.h"

int main(int argc, char** argv){

  printf("\n\n***************************************\n");
  printf("version uses online feedback to no longer 'read()' dirs.\n");
  printf("old verion is 8_6ch.c in this same directory\n");
  printf("***************************************\n\n\n");

  if (argc == 1)
    fsize(".");
  else
    while (--argc > 0)
      fsize(*++argv);

  return 0;
}
