#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void error(char* fmt, ...){

  va_list ap;

  va_start(ap, fmt);
  fprintf(stderr, "error: ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  exit(1);
}
