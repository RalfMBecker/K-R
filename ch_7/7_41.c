#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXWORD 100

int mysscanf(char*, char*, ...);
int pfmt(int from, char* fmt, char* pt, int* ps, int* pn);
int pvalue(char* vstr, char* source, int from, 
	   char type, int number, int sep, char* pbase, int len);

int matchprecise(char* fmt, int fromf, int tof, char* s, int* pfroms); 
                  // betw. numbers. out: -1 for failure, 0 else
int fnpercent(char* fmt, int from); // in: fmt string; from. out: where.
int trimlead(char* s, int from, char type, char* pbase, int* num);
int nospace(char);
int onlyspaces(char* fmt, int j, int npercent);
int atohi(char* s);
int atooi(char* s);
int okh(int c);
int oko(int c);
int okc(int c);
int oks(int c);

int main(){

  printf("\n**Does not implement the h and l options.\n");
  printf("**Implements types d, i, o, x, e, f, g, c, s\n");
  printf("**C89 sscanf is very sloppy - many reads with incorrect feedback.\n");
  printf("**This tries to be more consistent and responsive.\n");
  printf("**Note: if 'pcx', say, default will be decimal read in.\n");
  printf("**      Hence, '1a' will read in 1 only;\n"); 
  printf("**      '0x1a' 26 (decimal).\n");
  printf("**i prints like sscanf. Apparently, printout is in decmial.\n\n");

  char s[] =    "/-09.16/Ralf  /567/ /0x89";
  char s1[] =   "0123456789012345678901234567890";
  char s2[] =   "01234 56789 01234 56789 01234567890";
  char fmt[] =  "  /%g/Ralf/%d/   /%x";
  char fmt1[] = "  /%g/%*g/Ralf/%d/   /%x";
  char fmt2[] = "  %5d%5d%5d  %5d   ";
  char fmt3[] = "%d %d %d %d";
  char nums[] = "0123456789012345678901234567890";

  int a, b, c, d, rv;
  double x;

  printf("\nBeginning test of mysscanf on strings:\n\n");
 
  printf("s = %s, fmt = %s\n", s, fmt);
  rv = mysscanf(s, fmt, &x, &b, &c);
  printf("test returned rv = %d, x = %g, b = %d, c = %x\n", rv, x, b, c);
  x = b = c = 0;
  printf("s = %s, fmt1 = %s\n", s, fmt1);
  rv = mysscanf(s, fmt1, &x, &b, &c);
  printf("test returned rv = %d, x = %g, b = %d, c = %x\n", rv, x, b, c);
  a = b = c = d = 0;
  printf("s2 = %s, fmt3 = %s\n", s2, fmt3);
  rv = mysscanf(s2, fmt3, &a, &b, &c, &d);
  printf("test returned rv = %d, a = %d, b = %d, c = %d, d = %d\n", 
	 rv, a, b, c, d);
  a = b = c = d = 0;
  printf("s1 = %s, fmt2 = %s\n", s1, fmt2);
  rv = mysscanf(s1, fmt2, &a, &b, &c, &d);
  printf("test returned rv = %d, a = %d, b = %d, c = %d, d = %d\n\n", 
	 rv, a, b, c, d);

  int th1, th2, th3, to1, to2, to3, ti1, ti2, ti3;
  th1 = th2 = th3 = to1 = to2 = to3 = ti1 = ti2 = ti3 = 0;
  char sh1[] = "/0x9/0X2a/1B";
  char sh2[] = "/08/1y/27";
  char sh3[] = "/01/01/0x-11";
  char so1[] = "/017/008/8";
  char so2[] = "/01/012/12";
  char so3[] = "/19/15/029";
  char si1[] = "/0x9/0X2a/1B";
  char si2[] = "/08/1y/27";
  char si3[] = "/017/008/8";
  char si4[] = "/01/012/12";
  char fmti[] = "/%i/%i/%i";
  char fmto[] = "/%o/%o/%o";
  char fmtx[] = "/%x/%x/%x";

  printf("Format string is: %s\n", fmtx);
  rv = mysscanf(sh1, fmtx, &th1, &th2, &th3);
  printf("Hex. String: %s; successfully converted: "
	 "%d, 1 = %x, 2 = %x, 3 = %x\n" , sh1, rv, th1, th2, th3);
  
  rv = mysscanf(sh2, fmtx, &th1, &th2, &th3);
  printf("Hex. String: %s; successfully converted: "
	 "%d, 1 = %x, 2 = %x, 3 = %x\n" , sh2, rv, th1, th2, th3);
  rv = mysscanf(sh3, fmtx, &th1, &th2, &th3);
  printf("Hex. String: %s; successfully converted: "
	 "%d, 1 = %x, 2 = %x, 3 = %x\n\n" , sh3, rv, th1, th2, th3);

  printf("Format string is: %s\n", fmto);
  to1 = to2 = to3 = 0;
  rv = mysscanf(so1, fmto, &to1, &to2, &to3);
  printf("Octal. String: %s; successfully converted: "
	 "%d, 1 = %o, 2 = %o, 3 = %o\n" , so1, rv, to1, to2, to3);
  to1 = to2 = to3 = 0;
  rv = mysscanf(so2, fmto, &to1, &to2, &to3);
  printf("Octal. String: %s; successfully converted: "
	 "%d, 1 = %o, 2 = %o, 3 = %o\n" , so2, rv, to1, to2, to3);
  to1 = to2 = to3 = 0;
  rv = mysscanf(so3, fmto, &to1, &to2, &to3);
  printf("Octal. String: %s; successfully converted: "
	 "%d, 1 = %o, 2 = %o, 3 = %o\n\n" , so3, rv, to1, to2, to3);

  printf("Format string is: %s\n", fmti);
  to1 = to2 = to3 = 0;
  rv = mysscanf(si1, fmti, &to1, &to2, &to3);
  printf("type i. String: %s; successfully converted: "
	 "%d, 1 = %i, 2 = %i, 3 = %i\n" , si1, rv, to1, to2, to3);
  to1 = to2 = to3 = 0;
  rv = mysscanf(si2, fmti, &to1, &to2, &to3);
  printf("type i. String: %s; successfully converted: "
	 "%d, 1 = %i, 2 = %i, 3 = %i\n" , si2, rv, to1, to2, to3);
  to1 = to2 = to3 = 0;
  rv = mysscanf(si3, fmti, &to1, &to2, &to3);
  printf("type i. String: %s; successfully converted: "
	 "%d, 1 = %i, 2 = %i, 3 = %i\n" , si3, rv, to1, to2, to3);
  to1 = to2 = to3 = 0;
  rv = mysscanf(si4, fmti, &to1, &to2, &to3);
  printf("type i. String: %s; successfully converted: "
	 "%d, 1 = %i, 2 = %i, 3 = %i\n\n" , si4, rv, to1, to2, to3);

  char ss1[] = " trying once";
  char ss2[] = "trying this again";
  char fmts1[] = "%s%s";
  char fmts2[] = "%s%s %s";
  char st1[MAXWORD];
  char st2[MAXWORD];
  char st3[MAXWORD];

  printf("Format string is: %s\n", fmts1);
  strcpy(st1, "");
  strcpy(st2, "");
  rv = mysscanf(ss1, fmts1, st1, st2);
  printf("Type s. String: %s; successfully converted: "
	 "%d, 1 = %s, 2 = %s\n" , ss1, rv, st1, st2);
  printf("Format string is: %s\n", fmts2);
  strcpy(st1, "");
  strcpy(st2, "");
  strcpy(st3, "");
  rv = mysscanf(ss2, fmts2, st1, st2, st3);
  printf("Type s. String: %s; successfully converted: "
	 "%d, 1 = %s, 2 = %s, 3 = %s\n\n" , ss2, rv, st1, st2, st3);

  return 0;
}

int mysscanf(char* source, char* fmt, ...){

  char* pcval;
  int* pival;
  float* pfval;
  double* pdval;

  va_list ap;
  int i, j, c, to, rv, npercent;
  int converted, suppress, isuppress, number, lasttoread;
  char sep, type, base;
  char vstring[MAXWORD];

  va_start(ap, fmt);
  i = j = 0; // i for source, j for fmt
  converted = suppress = isuppress = number = to = lasttoread = 0;
  type = base = '0';
  vstring[0] = '\0';

  for ( ; source[i] != '\0' && ( c = fmt[j] ) != '\0'; j++){

    npercent = fnpercent(fmt, j);
    if (npercent == -1)
      return converted;
    to = (npercent != j)?npercent-1:to;

    //    printf("tested from - fmt: %d, - s: %d; to (in fmt): %d\n", j, i, to);

    isuppress = i;
    if (  npercent != j && !onlyspaces(fmt, j, npercent) ){
      rv = matchprecise(fmt, j, to, source, &i);
      if ( rv == -1){
	printf("Error: value string does not match format string.\n");
	return converted;
      }
    }
    j = npercent+1;

    //printf("success? - %d, matched to - fmt: %d, - s: %d\n", rv, to, i);

    j = pfmt(j, fmt, &type, &suppress, &number); 

    if ( j == -1){
      printf("Error: invalid format string.\n");
      return converted;
    }

    // ***************next line wrong****************
    if ( (c = fmt[j+1]) == '\0' ){ 
      lasttoread = 1;
      sep = '\0';
    }
    else if (c == '%')
      sep = '\0';
    else
      sep = c;

    //    testing[0] = type;
    //  printf("type = %s, suppress = %d, number = %d\n", testing, suppress, number);

    if (suppress){ // might have to check more if any skip in s
      if (lasttoread)
	return converted;
      i = isuppress;
      suppress = 0;
      number = 0;
      type = -1;
      continue;
    } 

    i = pvalue(vstring, source, i, type, number, sep, &base, MAXWORD);
    if ( i == -1){
      printf("Error: invalid value in source string.\n");
      return converted;
    }

    //    printf("value string = %s\n", vstring);

    switch (type){
    case 'd':
      pival = va_arg(ap, int*);
      *pival = atoi(vstring);
      break;
    case 'i':
      pival = va_arg(ap, int*);
      if (base == 'o')
	*pival = atooi(vstring);	  
      else if (base == 'h')
	*pival = atohi(vstring);	  
      else
	*pival = atoi(vstring);
      break;
    case 'o':
      pival = va_arg(ap, int*);
      if (base == 'o')
	*pival = atooi(vstring);
      else
	*pival = atoi(vstring);
      break;
    case 'x':
      pival = va_arg(ap, int*);
      if (base == 'h')
	*pival = atohi(vstring);
      else
	*pival = atoi(vstring);
      break;
    case 'e': case 'f':
      pfval = va_arg(ap, float*);
      *pfval = atof(vstring);
      break;
    case 'g':
      pdval = va_arg(ap, double*);
      *pdval = (double) atof(vstring);
      break;
    case 'c':
      pival = va_arg(ap, int*);
      *pival = (char) vstring[0];
      break;
    case 's':
      pcval = va_arg(ap, char*);
      strcpy(pcval, vstring);
      break;
    default:
      printf("Error: conversion for type '%c' failed.\n", type);
      return -1;
    } // end switch over final assignment of value

    // reset for safety
    type = -1;
    number = 0;
    vstring[0] = '\0';
    converted++;
    if (lasttoread)
      return converted;
    
  } // end outmost for loop  

  va_end(ap);
  return converted;

}

// parse one sub format string from percent to type char
// does not handle option h, l, or L
// so far handles: d, i, o, x, e, f, g, c, s (c untested)
// position in: from on first letter of sub fmt string
// position out: last letter (type) of sub fmt string returned
int pfmt(int from, char* fmt, char* pt, int* ps, int* pn){

  int c, j, i;
  char temp[MAXWORD];

  j = from;
  *ps = *pn = 0;
  *pt = '0';
  i = 0;
  temp[0] = '\0';

  if (nospace( c = fmt[j] ))
    return -1;
  if ( c == '*' ){
    *ps = 1;
    c = fmt[++j];
  }
  if (nospace(c))
    return -1;

  if (isdigit(c)){
    temp[i++] = c;
    while (isdigit( ( c = fmt[++j] ) ) )
      temp[i++] = c;
    temp[i] = '\0';
  }
  if (nospace(c))
    return -1;

  if ( c == '*'){
    if (*ps == 1){
      printf("Error: only one * character allowed in format string.\n");
      ps = 0;
      return -1;
    }
    else{
      *ps = 1;
      c = fmt[++j];
    }
  }
  if (nospace(c))
    return -1;

  // should be at type qualifier now
  switch(c){
  case 'd':
    *pt = 'd';
    break;
  case 'i':
    *pt = 'i';
    break;
  case 'o':
    *pt = 'o';
    break;
  case 'x':
    *pt = 'x';
    break;
  case 'e':
    *pt = 'e';
    break;
  case 'f':
    *pt = 'f';
    break;
  case 'g':
    *pt = 'g';
    break;
  case 'c':
    *pt = 'c';
    break;
  case 's':
    *pt = 's';
    break;
  default:
    *ps = 0;
    printf("Error: no valid type qualifier detected.\n");
    return -1;
    break;  
  } // end of type parsing switch

  *pn = atoi(temp);
  return j;

}

// this stops reading at, say, base = hex, sep = 'a', when 'a' hit 
// in source. this would be sophomoric to enter, but appears consistent
// position in: on first character of value (if any)
// position out: return char 1 past the last character of this value s-string 
int pvalue(char* vstr, char* source, int from, 
	   char type, int number, int sep, char* pbase, int len){

  int i, j, c, d, isfloating;
  int (*comp)(int);
  char warning[] = "%x";

  i = from;
  j = 0;
  vstr[j] ='\0';
  number = (number)?number:len;
  isfloating = ( type == 'e' || type == 'f' || type == 'g');

  // skip only leading whitespace
  while ( isspace(c = source[i]) && number-- > 0 )
    i++; 
  if (!number){
    vstr[j] ='\0';
    return i;
  }

  i = trimlead(source, i, type, pbase, &number); // to next element after
  switch(*pbase){
  case 'd':
    comp = isdigit;
    break;
  case 'h':
    comp = isxdigit;
    break;
  case 'o':
    comp = oko;
    break;
  case 'c':
    comp = okc;
    break;
  case 's':
    comp = oks;
    break;
  default:
    printf("Error: conversion to unknown base.\n");
    return -1;
  }
  if (!number){
    vstr[j] ='\0';
    return i;
  }

  if ( (c = source[i]) == '+' || c == '-'){
    if ( type == 'o' || type == 'x' || type == 'i'){
      warning[1] = type;
      printf("Warning: assigning negative value to type %s\n", warning);
    }
    vstr[j++] = c;
    i++;
    number--;
    if (!number){
      vstr[j] ='\0';
      return i;
    }
  }

  while ( (c = source[i] ) != '\0' && c != sep 
	  && !isspace(c)  && comp(c) && number-- > 0){
    vstr[j++] = c;
    i++;
  }
  if (!number){
    vstr[j] ='\0';
    return i;
  }

  if (isfloating){
    if (c == '.'){
      vstr[j++] = c;
      i++;
      number--;
    }
    while ( ( c = source[i] ) != '\0' && c != sep 
	    && !isspace(c) && comp(c) && number-- > 0 ){
      vstr[j++] = c;
      i++;    
    }
    if (!number){
      vstr[j] ='\0';
      return i;
    }
  }

 leaving:
  vstr[j] = '\0';
  return i;
}

int atohi(char* s){

  int i, n, sign, tmp;
  
  for ( i = 0; isspace(s[i]); i++)
    ;
  sign = ( s[i] == '-' )?-1:1;
  if ( s[i] == '+' || s[i] == '-' )
    i++;
  for ( n = 0; okh(s[i]) ; i++){
    if (s[i]>='A' && s[i]<='F')
      tmp = s[i]-'A'+10;
    else if (s[i]>='a' && s[i]<='f')
      tmp = s[i]-'a'+10;
    else
      tmp = s[i]-'0';
    n = 16*n + tmp;
  }

  if (s[i] != '\0'){
    printf("Error: invalid character in hex string.\n");
    return -1;
  }

  return sign * n;
} 

int atooi(char* s){

  int i, n, sign, tmp;
  
  for ( i = 0; isspace(s[i]); i++)
    ;
  sign = ( s[i] == '-' )?-1:1;
  if ( s[i] == '+' || s[i] == '-' )
    i++;
  for ( n = 0; oko(s[i]) ; i++){
    tmp = s[i]-'0';
    n = 8*n + tmp;
  }

  if (s[i] != '\0'){
    printf("Error: invalid character in octal string.\n");
    return -1;
  }

  return sign * n;
} 

// discovered alternative isxdigit
int okh(int c){

  return (isdigit(c) || (c >= 'A' && c <= 'F') ||
	  (c >= 'a' && c <= 'f') );
}

int oko(int c){

  return (isdigit(c) && !(c == '9') && !(c == '8') );
}

int okc(int c){

  return 1;
}

int oks(int c){

  return 1;
}

int fnpercent(char* fmt, int from){
 
  int c, i;

  i = from;
  while ( (c = fmt[i]) != '\0' && c != '%')
    i++;

  return (c == '\0')?-1:i;
}

// match s and fmt between fmt[fromf] to fmt[tof]
// s: disregard isspace, fmt: disregard ' ' and '\t'
// fmt string: in = out = first char to be tested
// val string; in = first char to be tested
//             out= 1 past last char to be tested
int matchprecise(char* fmt, int fromf, int tof, char* s, int* pfroms){

  if (fromf > tof){
    printf("Error: unexplained behavior.\n");
    return -1;
  }
  if (tof == fromf){
    return (fmt[fromf] == s[(*pfroms)++])?0:-1;
  }
  int i, j;
  char c, d;

  i = *pfroms;
  j = fromf;
  for ( ; j <= tof;  i++, j++ ){
    while ( ( c = fmt[j] ) == ' ' || c == '\t'){
      if ( c == '\0' ){
	printf("Error: reached end of string 1 before end of comparison.\n");
	return -1;
      }
      j++;
    }
    while ( isspace( d = s[i] ) ){
      if ( d == '\0' ){
	printf("Error: reached end of string 2 before end of comparison.\n");
	return -1;
      }
      i++;
    }
    if ( d != c)
      return -1;
  } // end outer for loop

  *pfroms = i;
  return 0;
}

int nospace(char c){

  if ( isspace(c) ){
    printf("Error: no white space allowed between percent and type char.\n");
    return -1;
  }

  return 0;
}

int onlyspaces(char* fmt, int j, int npercent){

  int c;
  while ( j < npercent && ( c = fmt[j] ) == ' ' || c == '\t')
    j++;
  return j == npercent;
}
 
// amounts to not allowing sequences such as -0x1a (instead use 0x-1a).
// this seems more logical anyway.
// for o, i, x, the default is to assume input in decimal, if not
// manifested otherwise by leading '0' or '0x'/'0X'
int trimlead(char* s, int from, char type, char* pbase, int* num){

  char c;

  switch(type){
  case 'o':
    *pbase = 'd';
    if (s[from] == '0' && s[from+1] != '\0' && *num > 1)
      *pbase = 'o';
    break;
  case 'x':
    *pbase = 'd';
    if ( !(( c = s[from+1] ) == '\0' || s[from+2] == '\0') && 
	 ( s[from] == '0' &&  c == 'x' || c == 'X' ) && *num > 1){
      *pbase = 'h';
      *num -= 2;
      return from+2;
    }
    break;
  case 'i':
    *pbase = 'd';
    if ( !(*num == 1) ){
      if ( !(( c = s[from+1] ) == '\0' || s[from+2] == '\0') && 
	   s[from] == '0' ){
	if ( c == 'x' || c == 'X' ){
	  *pbase = 'h';
	  *num -= 2;
	  return from+2;
	}
	else
	  *pbase = 'o';
      }
    }
    break;
  case 'c':
    *pbase = 'c';
    break;
  case 's':
    *pbase = 's';
    break;
  default:
    *pbase = 'd';
    break;
  }

  return from;
}
