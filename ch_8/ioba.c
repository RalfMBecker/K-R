#include <fcntl.h>
#include <stdlib.h>
#include "syscalls.h"

#define PERMS 0666

myFILE _iob[OPEN_MAX] = {
  { 0, (char*) NULL, (char*) NULL, _READ, 0 },
  { 0, (char*) NULL, (char*) NULL, _WRITE, 1 }, 
  { 0, (char*) NULL, (char*) NULL, _WRITE | _UNBUF, 2 }
};

long filelength(char* name){



  return 0L;
}

int myfseek(myFILE* fp, long offset, int origin){

  int error, len;

  if (lseek(fp->fd, offset, origin) == -1)
    return EOF;

  return 0;
}

myFILE* myfopen(char* name, char* mode){

  int fd;
  myFILE* fp;

  if (*mode != 'r' && *mode != 'w' && *mode != 'a')
    return NULL;
  for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
    if ( ( fp->flag & (_READ | _WRITE) ) == 0 )
      break;
  if (fp >= _iob + OPEN_MAX)
    return NULL;

  if (*mode == 'w')
    fd = creat(name, PERMS);
  else if (*mode == 'a'){
    if ( ( fd = open(name, O_WRONLY, 0) ) == -1 ) // create on first append
      fd = creat(name, PERMS);
    lseek(fd, 0L, 2);
  }
  else
    fd = open(name, O_RDONLY, 0);
  if (fd == -1)
    return NULL;
  fp->fd = fd;
  fp->cnt = 0;
  fp->base = fp-> ptr = NULL;
  fp->flag = (*mode == 'r') ? _READ: _WRITE;

  return fp;
}

int myfclose(myFILE* fp){

  if ( myfflush(fp) == EOF )
    return EOF;
  free((void*) fp->base);

  fp->cnt = 0;
  fp->base = fp->ptr = NULL;
  fp->flag = 0;
  close(fp->fd);

  return 0;
}

int myfflush(myFILE* stream){

  int vflag;
  myFILE *fp, *from, *to;

  if (stream == NULL){  
    fp = &_iob[0];
    myfflush(fp);
    fp = &_iob[1];
    myfflush(fp);
    from = &_iob[3];
    to = _iob + OPEN_MAX-3;
  }
  else{
    from = stream;
    to = from + 1;
  }
  for (fp = from; fp < to; fp++){
    if ( ( vflag = ( fp->flag & ( _WRITE | _READ | _ERR ) ) ) != 
	 _WRITE && vflag != _READ)
      continue;
    else if ( (fp->flag & _WRITE) == _WRITE ){
      if ( _flushbuf(EOF, fp) != 0 ){
	fprintf(stderr, "Could not flush output buffer.\n");
	return EOF;
      }
      continue;
    }
    else if ( (fp->flag & _READ) == _READ){
      fp->cnt += fp->ptr - fp->base; // undefined: we reset buffer.
      fp->ptr = fp->base;            // no error possible, so no check for it
      continue;
    }
    else // should never come here
      continue;
  } // end for loop over streams to flush

  return 0;
}

int _fillbuf(myFILE* fp){

  int bufsize;

  if ( (fp->flag & ( _READ | _EOF | _ERR) ) != _READ )
    return EOF;
  bufsize = ( fp->flag & _UNBUF ) ? 1: BUFSIZE;
  if (fp->base == NULL)
    if ( ( fp->base = (char*) malloc(bufsize) ) == NULL )
      return EOF;
  fp->ptr = fp->base;
  fp->cnt = read(fp->fd, fp->ptr, bufsize);
  if (--fp->cnt < 0){ // count down as we already used one this time
    if (fp->cnt == -1)
      fp->flag |= _EOF;
    else
      fp->flag |= _ERR;
    fp->cnt = 0;
    return EOF;
  }

  return (unsigned char) *fp->ptr++;
}

// code review: the lines between 'from' and 'to' appear to rewrite current
//              content and seem unnecessary
// return EOF for error; c else, except if c == EOF when it returns 0
int _flushbuf(char c, myFILE* fp){

  int bufsize, size;

  if ( (fp->flag & ( _WRITE | _ERR) ) != _WRITE )
    return EOF;
  if (fp->base == NULL){ // start a new buffer
    bufsize = ( fp->flag & _UNBUF ) ? 1: BUFSIZE;
    if ( ( fp->base = (char*) malloc(bufsize) ) == NULL )
      return EOF;
    fp->ptr = fp->base;
    fp->cnt = bufsize;
    if (c != EOF){
      *(fp->ptr++) = c;  // no parantheses needed as evaluated trl
      fp->cnt--;
    }
  }
  else{ // flush
    bufsize = fp->ptr - fp->base; // as fp->ptr points to next free position
    fp->ptr = fp->base;
    fp->cnt = bufsize; 
    // from
    size = write(fp->fd, fp->ptr, bufsize);
    if (size < bufsize){  // -- as we read/write to zero, not 1
      fp->flag |= _ERR;
      fp->cnt = 0;
      return EOF;
    }
    // to
    if ( c != EOF && write(fp->fd, &c, 1) != 1 )
      return EOF;
  }

  return (c != EOF) ? (unsigned char) c : 0;
}
