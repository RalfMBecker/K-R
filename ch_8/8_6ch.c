#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include "syscalls.h"
#include "dirent_old.h"

int main(int argc, char** argv){

  printf("\n\n***************************************\n");
  printf("aborted (reasons: see myreadd() in dirent_old.c)\n");
  printf("redone as 'myls' in this same directory\n");
  printf("***************************************\n\n\n");
  printf(" DIR, Dirent, stat exist on system in /usr/include/bits and\n");
  printf("/usr/include/sys header files such as dir.h, dirent.h, stat.h.\n");
  printf("The files in /sys pull in those in /bits.\n");
  printf("opendir, closedir, and readdir exist too and work with the above\n");
  printf("structures. Note also that header files named dirent.h and,\n");
  printf("probably, syscalls.h exist as header files in the system too.\n\n");

  if (argc == 1)
    fsize(".");
  else
    while (--argc > 0)
      fsize(*++argv);

  return 0;
}
