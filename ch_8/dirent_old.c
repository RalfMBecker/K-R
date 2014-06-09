#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dirent_old.h"
#include <unistd.h>  // ****** check later if needed

void fsize(char* name){

  struct stat stbuf;

  if (stat(name, &stbuf) == -1){
    fprintf(stderr, "fsize: can't access %s\n", name);
    return;
  }
  if ( (stbuf.st_mode & S_IFMT) == S_IFDIR)
    dirwalk(name, fsize);
  printf("%8ld %s\n", stbuf.st_size, name);
}

void dirwalk(char* dir, void (*fcn)(char*)){

  char name[MAX_PATH];
  myDirent* dp;
  myDIR* dfd;

  if ( ( dfd = myopendir(dir) ) == NULL ){
    fprintf(stderr, "dirwalk: can't open %s\n", dir);
    return;
  }

  while ( (dp = myreaddir(dfd) ) != NULL ){
    if (strcmp(dp->name, ".") == 0 || strcmp(dp->name, "..") == 0)
      continue; // to avoid circularity
    if (strlen(dir)+strlen(dp->name)+2 > sizeof(name))
      fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->name);
    else{
      sprintf(name, "%s/%s", dir, dp->name);
      (*fcn)(name);
    }
  }
  myclosedir(dfd);  

}

myDIR* myopendir(char* dirname){

  int fd;
  struct stat stbuf;
  myDIR* dp;

  if ( (fd = open(dirname, O_RDONLY, 0) ) == -1 ||
       fstat(fd, &stbuf) == -1 ||
       (stbuf.st_mode & S_IFMT) != S_IFDIR || 
       ( dp = (myDIR*) malloc( sizeof(myDIR)) ) == NULL )
    return NULL;

  printf("stbuf.st_ino = %8ld\tstbuf.st_mode = %o\tstbuf.st_size = %ld\n",
	 stbuf.st_ino, stbuf.st_mode, stbuf.st_size);
  dp->fd = fd;

  return dp;
}

void myclosedir(myDIR* dp){

  if (dp){
    close(dp->fd);
    free(dp);
  }
}

myDirent* myreaddir(myDIR* dp){

  struct direct dirbuf; // stack overflow: archaic. use dirent. also,
                        // no longer 'read()' in directory info; instead
                        // use directly opendir, closedir, readdir.
                        // Note that #define direct dirent on my system.
  static myDirent d;

  // testing what can be read
  int n;
  n = read(dp->fd, &dirbuf, sizeof(struct direct) );
  printf("n = %d " 
"\tdirbuf.d_ino = %ld" 
"\tdirbuf.d_off = %ld\n"
"\tdirbuf.d_reclen = %u"
"\tdirbuf.d_type = %u\n" 
"\tdirbuf.d_name = %s\n",
	 n, dirbuf.d_ino, dirbuf.d_off, dirbuf.d_reclen, 
	 dirbuf.d_type, dirbuf.d_name);

  while (n != -1){
  //  while (read(dp->fd, (char*) &dirbuf, sizeof dirbuf ) == sizeof dirbuf){
    if (dirbuf.d_ino == 0) // slot not in use
      continue;
    d.ino = dirbuf.d_ino;
    strncpy(d.name, dirbuf.d_name, DIRSIZ);
    d.name[DIRSIZ] = '\0'; // ***check: still necessary?
    return &d;
  }
 
  return NULL;
}
