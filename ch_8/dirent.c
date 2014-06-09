#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "dirent.h"

void fsize(char* name){

  struct stat stbuf;

  if (stat(name, &stbuf) == -1){
    fprintf(stderr, "fsize: can't access %s\n", name);
    return;
  }
  if ( (stbuf.st_mode & S_IFMT) == S_IFDIR){
      printf("\n%s:\n", name);
    dirwalk(name, fsize);
  }
  printf("%20s %8ld %8ld %8ld %8hd %12ld\n", name, stbuf.st_ino, stbuf.st_size, 
	 stbuf.st_blksize, stbuf.st_uid, stbuf.st_atime);
}

void dirwalk(char* dir, void (*fcn)(char*)){

  char name[MAX_PATH];
  struct dirent* dp;
  DIR* dfd;

  if ( ( dfd = opendir(dir) ) == NULL ){
    fprintf(stderr, "dirwalk: can't open %s\n", dir);
    return;
  }

  while ( (dp = readdir(dfd) ) != NULL ){
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){

      continue; // to avoid circularity
    }
    if (strlen(dir)+strlen(dp->d_name)+2 > sizeof(name))
      fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->d_name);
    else{
      sprintf(name, "%s/%s", dir, dp->d_name);
      (*fcn)(name);
    }
  }
  
  closedir(dfd);
}
