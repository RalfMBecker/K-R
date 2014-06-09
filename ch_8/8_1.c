#include <fcntl.h>
#include <stdio.h>
#include "syscalls.h"

int filecopy(int, int);

int main(int argc, char* argv[]){

  int fd;

  if (argc ==1)
    filecopy(0, 1);
  else{
    while (--argc > 0){
      if ( ( fd = open(*++argv, O_RDONLY, 0) ) == -1 )
	error("cat: can't open %s", *argv);
      else{
	filecopy(fd, 1);
	close(fd);
      }
    }
  }

  return 0;
}


int filecopy(int fd1, int fd2){

  int n;
  char buf[BUFSIZE];

  while ( ( n = read(fd1, buf, sizeof buf) ) > 0 )
    if ( write(fd2, buf, n) != n )
      error("cat: write error on file %s", fd2);

  return 0;
}
