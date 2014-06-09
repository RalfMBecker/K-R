#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFSIZE 64
#define OPEN_MAX 20

// int read(int, char*, int); // declared in <unistd.h>
// int write(int, char*, int); // declared in <unistd.h>
void error(char*, ...);
long filelength(char*);

typedef struct _iobuf{
  int cnt;
  char* ptr;
  char* base;
  int flag;
  int fd;
} myFILE;

extern myFILE _iob[OPEN_MAX];

myFILE* myfopen(char*, char*);
int myfclose(myFILE*);
int myfflush(myFILE*);
int _fillbuf(myFILE*);
int _flushbuf(char, myFILE*);
int myfseek(myFILE*, long, int);

#if defined(stdin)
#undef stdin
#endif
#define stdin (&_iob[0])
#if defined(stdout)
#undef stdout
#endif
#define stdout (&_iob[1])
// do not redefine stderr as it works with fprintf, which we do not redefine

enum _flags{
  _READ = 01,
  _WRITE = 02,
  _UNBUF = 04,
  _EOF = 010,
  _ERR = 020
};

#if defined(feof)
#undef feof
#endif
#define feof(p)      (((p)->flag & _EOF) != 0 )
#if defined(ferror)
#undef ferror
#endif
#define ferror(p)    (((p)->flag & _ERR) != 0 )
#if defined(fileno)
#undef fileno
#endif
#define fileno(p)    ((p)->fd)

#if defined(getc)
#undef getc
#endif
#define getc(p)      (--(p)->cnt >= 0 \
		      ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#if defined(putc)
#undef putc
#endif
#define putc(x, p)   (--(p)->cnt >= 0 \
		      ? *(p)->ptr++ = (x) : _flushbuf((x), p) )

#if defined(getchar)
#undef getchar
#endif
#define getchar()    getc(stdin)
#if defined(putchar)
#undef putchar
#endif
#define putchar(x)   putc(x, stdout)

// defs for alloc.c
typedef long Align;
   
union header{
  struct{
    union header* ptr;  
    unsigned size;
  } s;
  Align x;  
};
   
typedef union header Header;

int mybfree(char*, int);
void* mycalloc(unsigned, unsigned);
void* mymalloc(unsigned);
//static Header* morecore(unsigned);
void myfree(void*);
