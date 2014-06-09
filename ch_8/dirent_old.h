#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>   // for struct direct (== dirent)x
#include <fcntl.h>   // for read, write

#if !defined(NAME_MAX)
#define NAME_MAX   14
#endif

#if !defined(MAX_PATH)
#define MAX_PATH 1024
#endif

#if !defined(DIRSIZ)
#define DIRSIZ 1024
#endif

typedef struct{
  long ino;
  char name[NAME_MAX+1];
} myDirent;

typedef struct{
  int fd;
  myDirent d;
} myDIR;


myDIR* myopendir(char* dirname);
myDirent* myreaddir(myDIR* dfd);
void myclosedir(myDIR* dfd);
void fsize(char*);
//int stat(const char*, struct stat*);
//int fstat(int, struct stat*);
void dirwalk(char*, void (*fcn)(char*));
