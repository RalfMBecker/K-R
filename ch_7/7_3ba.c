#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100

int minprintf(char*, ...);
int pformat(int, char*, char*, int*, int); // parse from % to d(i,x,s,...)

int main(){

  printf("\n**Does not implement the h and l options.\n");
  printf("**Implements types d, i, o, x, X, c, s, f, g, G\n");

  int i, rv;
  double d;
  char c;
  char s[] = "Test string";

  c = 'a';
  i = 29;
  d = 3.14159267;


  rv = minprintf("\ncharacter: %c.\n", c);
  rv = minprintf("integer: %d.\n", i);
  rv = minprintf("integer: %x.\n", i);
  rv = minprintf("integer: %10.10o.\n", i);
  rv = minprintf("double: %g.\n", d);
  rv = minprintf("double: %20.10g.\n", d);
  rv = minprintf("double: %.10g.\n", d);
  rv = minprintf("character string: %s.\n", s);
  rv = minprintf("double = %15.10G., then character string = %s.\n", d, s);

  return 0;
}

// types translated to: d, i, o, x, X, c = 0; g, G = 1; s = 2
int minprintf(char* fmt, ...){

  va_list ap;
  char* sval;
  int ival;
  double dval;

  int i, j, c, type;
  char temp[MAXWORD];
  char word[MAXWORD];
  int numberset; // can set only one nubmer (possibly with .) per %...d

  va_start(ap, fmt);
  i = j = 0;
  type = -1;
  word[0] = '\0';

  // process format string
  for ( (c = fmt[0]) ; i < MAXWORD && ( c = fmt[i] ) != '\0'; i++){
    word[j++] = c;
    if (c != '%')
      continue;
    word[j] = temp[0] = '\0';
    i = pformat(++i, fmt, temp, &type, MAXWORD);
    if (i == -1){
      printf("Error: invalid format string.\n");
      return -1;
    }
    strcat(word, temp);

    // we got enough information to print this part
    switch (type){
    case '0':
      ival = va_arg(ap, int);
      printf(word, ival);
      word[0] = '\0';
      type = -1;
      break;
    case '1':
      dval = va_arg(ap, double);
      printf(word, dval);
      word[0] = '\0';
      type = -1;
      break;
    case '2':
      sval = va_arg(ap, char*);
      printf(word, sval);
      word[0] = '\0';
      type = -1;
      break;
    default:
      break;
    }
    
    j = 0;
    word[0] = '\0';

  } // end c = fmt[0] loop
 
  // handle leftovers after last %...d sequence
  word[j] = '\0';
  printf("%s", word);

  va_end(ap);
  return 0;
}

// called only immediately after a '%' has been recognized
// return -1 for error; new position in string fmt else
// does not implement the h and l options
// implements types d, i, o, x, X, c, s, f, g, G
// types translated to: d, i, o, x, X, c = 0; g, G = 1; s = 2
int pformat(int from, char* format, char* w, int* pt, int lim){

  if (from > strlen(format) )
    return -1;

  int c, i, j;
  char* p = w;
  int numberset; // can set only one number per %...d, possibly with .
  int parsingnumber; // started parsing xx part of xx.xxxx

  i = from;
  j = numberset = parsingnumber = 0;
  p[0] = '\0';
  while ( i < lim && ( ( c = format[i] ) != EOF ) ){
    if (isdigit(c)){ // handles only the xx in xx.yyyyy, say 
      if (numberset){
	printf("Error: only one xx.xxxxx allowed in format string.\n");
	return -1;
      }
      p[j++] = c;
      numberset = 1;
      while ( isdigit( ( c = format[++i] ) ) ) // sloppy. could hit EOF etc
	p[j++] = c; 
      parsingnumber = ( c == '.')?1:0;
      continue;
    } // end isdigit(c) case

    switch (c){
    case 'd': case 'i': case 'o': case 'x': case 'X': case 'c': 
      *pt = 0 + '0';
      p[j++] = c;
      p[j] = '\0';
      return i;
      break;
    case 'g': case 'G':
      *pt = 1 + '0';
      p[j++] = c;
      p[j] = '\0';
      return i;
      break;
    case 's':
      *pt = 2 + '0';
      p[j++] = c;
      p[j] = '\0';
      return i;
      break;
    case '-':
      parsingnumber = 0;
      p[j++] = c;
      i++;
      break; 
    case '.':
      if (numberset && !parsingnumber){ // form xxe followed by .xxxx
	printf("Error: only one number allowed.\n");
	return -1;
      }
      p[j++] = c;
      if ( !isdigit(format[i+1]) ){
	printf("Error: sequence xx.e or .e (e no digit) found.\n");
	return -1;
      }
      else{
	numberset = 1;
	parsingnumber = 0;
	while ( isdigit( ( c = format[++i] ) ) )
	  p[j++] = c;
	break;
      }
    default:
      printf("Error: invalid character in format string.\n");
      return -1;
      break;
    }
  }
}
