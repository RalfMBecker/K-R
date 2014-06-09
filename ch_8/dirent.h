#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>   // for DIR

#if !defined(MAX_PATH)
#define MAX_PATH 1024
#endif

void fsize(char*);
void dirwalk(char*, void (*fcn)(char*));
