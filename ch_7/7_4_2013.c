#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define MAXWORD 100

struct specifiers{
  int star;
  int f_width;
  int o_width;
  char type;
};
struct specifiers specs;

int my_sscanf(char*, char*, ...);
char next_nws(char*, int*);
int valid_conversion(char);
int valid_o_width_c(char);
int valid_o_width(char, char);
int parse_specs(char*, int*, struct specifiers*);
int read_token(char*, char*, int*, int, char, char);
char adj_token_type(char*, struct specifiers);

struct specifiers s_default(void);
void print_specs(struct specifiers);
int read_number(char*, int*);
void move_left(char*);
int otoi(char*, int*);
int htoi(char*, int*);
void reverse(char*);
int octal(char);
int hex(char, int*);
void leave(char*);

int main(){ // 175 - 38 = 137;  617 - 137 = 480

  printf("***************************************************\n");
  printf("* Date: Sep 10, 2013                              *\n");
  printf("*                                                 *\n");
  printf("* Function supports:                              *\n");
  printf("*    - formats: d, i, o, x, c, s, f, e, G         *\n");
  printf("*    - all conversion specifications              *\n");
  printf("*                                                 *\n");
  printf("* Restrictions:                                   *\n");
  printf("*               We assume %% cannot be in the      *\n");
  printf("*               string to be parsed               *\n");
  printf("*                                                 *\n");
  printf("***************************************************\n\n");

  printf("***************************************************************\n");
  printf("*                                                             *\n");
  printf("* Summary of sscanf():                                        *\n");
  printf("*                                                             *\n");
  printf("* (1) Whitespace outside %%<c> ignored, in both fmt & parsed   *\n");
  printf("* (2) In fmt, a sequence %%<Whitespace><c> is an error         *\n");
  printf("* (3) Using %%c reads the next input char found into c_1[0].   *\n");
  printf("*     To print, make valid string by setting c_1[1] = ");
  printf("\'\\0\'    *\n");
  printf("* (4) %%<n><c> only considers the next n chars for <c>; in     *\n");
  printf("*     other words, field width applies to all conversions:    *\n");
  printf("*     e.g., %%4f applied to -1.234 reads -1.2                  *\n");
  printf("* (5) If both field width 13 and, say, h specified, need to   *\n");
  printf("*     be in order %%13hd, not %%h13d                            *\n");
  printf("* (6) However, to skip, order must be %%*3g, not %%3*g        ");
  printf("  *\n");
  printf("* (7) For skipping, a sequence like %%*3ld is legal            *\n");
  printf("* (8) If you read only <n> wide, need to be at next matching  *\n");
  printf("*     character, or next input variable after                 *\n");
  printf("* (9) scanf reads what it can successfully; stops w/o error   *\n");
  printf("*     in most cases when it encounters a no-match (even       *\n");
  printf("*     non-existing char conversion chars produce no error)    *\n");
  printf("*                                                             *\n");
  printf("***************************************************************\n");

  char fmt[MAXWORD];
  char parsed[MAXWORD];

  int rv, i1, i2, i3, i4, i5, i6;
  short s1, s2;
  float f1;
  long double ld1;
  double d1, d2;
  char c1[MAXWORD], c2[MAXWORD];
  char sstr1[MAXWORD];

  printf("\nFirst read-in and p/o is using sscanf(); 2nd my_sscanf()\n\n");
  strcpy(parsed, "/\t5/\n017/6/ -3.1415E+03/R alf/Pimp/0XAf/ ");
  strcpy(fmt, "/%d/  %5o/ %20ho / %Lf/%5c/%4s/%7x/");
  printf("parsed: %s\n", parsed);
  printf("fmt: %s\n", fmt);
  rv = sscanf(parsed, fmt, &i1, &i2, &s1, &ld1, c1, sstr1, &i3);
  c1[5] = '\0';
  printf("rv = %d\n", rv); 
  printf("i1 = %d; i2 = %d; s1 = %hd; f1 = %Lf; c1 = %s; s1 = %s, i3 = %d\n", 
	 i1, i2, s1, ld1, c1, sstr1, i3);
  i1 = i2 = i3 = s1 = ld1 = 0, c1[0] = sstr1[0] = '\0';  
  rv = my_sscanf(parsed, fmt, &i1, &i2, &s1, &ld1, c1, sstr1, &i3);
  c1[5] = '\0';
  printf("rv = %d\n", rv); 
  printf("i1 = %d; i2 = %d; s1 = %hd; f1 = %Lf; c1 = %s; s1 = %s, i3 = %d\n\n", 
	 i1, i2, s1, ld1, c1, sstr1, i3);
  i1 = i2 = i3 = s1 = ld1 = 0, c1[0] = sstr1[0] = '\0';  

  strcpy(parsed, "/-09.16/\n   567/0x89/Ralf B");
  strcpy(fmt, "/   %f/%d/\n%x/%s%c%c/");
  printf("parsed: %s\n", parsed);
  printf("fmt: %s\n", fmt);
  rv = sscanf(parsed, fmt, &f1, &i1, &i2, sstr1, c1, c2);
  c1[1] = c2[1] = '\0';
  printf("rv = %d:\t", rv); 
  printf("d1 = %f; i1 = %d; i2 = %d; s1 = %s, c1 = %s, c2 = %s\n", 
	 f1, i1, i2, sstr1, c1, c2); 
  f1 = i1 = i2 = 0, sstr1[0] = c1[0] = c2[0] = '\0';
  rv = my_sscanf(parsed, fmt, &f1, &i1, &i2, sstr1, c1, c2);
  c1[1] = c2[1] = '\0';
  printf("rv = %d:\t", rv); 
  printf("f1 = %f; i1 = %d; i2 = %d; s1 = %s, c1 = %s, c2 = %s\n\n", 
	 f1, i1, i2, sstr1, c1, c2); 
  f1 = i1 = i2 = 0, sstr1[0] = c1[0] = c2[0] = '\0';

  strcpy(parsed, "/26/26/026/26/0x26/0X26/");
  strcpy(fmt, "/%d/%i/%i/%x/%i/%i/");
  printf("parsed: %s\n", parsed);
  printf("fmt: %s\n", fmt);
  rv = sscanf(parsed, fmt, &i1, &i2, &i3, &i4, &i5, &i6);
  printf("rv = %d:\t", rv); 
  printf("i1 = %d; i2 = %d; i3 = %d; i4 = %d; i5 = %d; i6 = %d\n",
	 i1, i2, i3, i4, i5, i6);
  i1 = i2 = i3 = i4 = i5 = i6 = 0;
  rv = my_sscanf(parsed, fmt, &i1, &i2, &i3, &i4, &i5, &i6);
  printf("rv = %d:\t", rv); 
  printf("i1 = %d; i2 = %d; i3 = %d; i4 = %d; i5 = %d; i6 = %d\n\n",
	 i1, i2, i3, i4, i5, i6);
  i1 = i2 = i3 = i4 = i5 = i6 = 0;

  strcpy(parsed, "/-1234/-1234/-1234/");
  strcpy(fmt, "/%d/%hd/%3hd/");
  printf("parsed: %s\n", parsed);
  printf("fmt: %s\n", fmt);
  rv = sscanf(parsed, fmt, &i1, &s1, &s2);
  printf("rv = %d:\t", rv); 
  printf("i1 = %d; s1 = %hd; s2 = %hd\n", i1, s1, s2);
  i1 = s1 = s2 = 0;
  rv = my_sscanf(parsed, fmt, &i1, &s1, &s2);
  printf("rv = %d:\t", rv); 
  printf("i1 = %d; s1 = %hd; s2 = %hd\n\n", i1, s1, s2);
  i1 = s1 = s2 = 0;

  strcpy(parsed, "/-1.234/-1.234/");
  strcpy(fmt, "/%lf/%4lf/");
  printf("parsed: %s\n", parsed);
  printf("fmt: %s\n", fmt);
  rv = sscanf(parsed, fmt, &d1, &d2);
  printf("rv = %d:\t", rv); 
  printf("f1 = %g; f2 = %g\n", d1, d2);
  d1 = d2 = 0;
  rv = my_sscanf(parsed, fmt, &d1, &d2);
  printf("rv = %d:\t", rv); 
  printf("f1 = %g; f2 = %g\n\n", d1, d2);
  d1 = d2 = 0;

  strcpy(parsed, "/123/456/789/");
  strcpy(fmt, "/%*d/%*3ld/%d/");
  printf("parsed: %s\n", parsed);
  printf("fmt: %s\n", fmt);
  rv = sscanf(parsed, fmt, &i1, &i2, &i3);
  printf("rv = %d:\t", rv); 
  printf("i1 = %d; i2 = %d; i3 = %d\n", i1, i2, i3);
  i1 = i2 = i3 = 0;
  rv = my_sscanf(parsed, fmt, &i1, &i2, &i3);
  printf("rv = %d:\t", rv); 
  printf("i1 = %d; i2 = %d; i3 = %d\n\n", i1, i2, i3);

  return 0;
}

//**********************************************
// core functions
//**********************************************

int my_sscanf(char* parse, char* fmt, ...){

  int rv = 0;
  int pos_p, pos_f, max, eofstr, def_read;
  pos_p = pos_f = 0;
  int* pp = &pos_p;
  int* pf = &pos_f;
  char c, type;
  struct specifiers specs;
  char token[MAXWORD];

  va_list ap;
  int* pival;
  short* psival;
  long* plival;
  float* pfval;
  double* pdval;
  long double* pldval;
  char* strvalue;

  int i_temp;
  int* pi_temp = &i_temp;

  va_start(ap, fmt);
  while ( ( (c = next_nws(fmt, pf)) != '\0') ){

    if ( (c == '%') ){
      (*pf)++;
      if ( (parse_specs(fmt, pf, &specs) == 0) )
	return rv;

      c = next_nws(fmt, pf); // now c = what delineates input field?
      def_read = (specs.type == 'c')?1:MAXWORD;
      max = (specs.f_width > 0)?specs.f_width:def_read;
      eofstr = read_token(token, parse, pp, max, c, specs.type);
      if ( (specs.star == 1) ){ // skip starred input, AFTER processing token
	if ( (eofstr == 0) )
	  return rv;
	else
	  continue;
      }

      // wrap i to d, o, or x, respectively; and strip leading 0, 0x/0X
      type = adj_token_type(token, specs);

      switch ( (type) ){
      case 'd': 
	if ( (specs.o_width == 'h') ){
	  psival = (short*) va_arg(ap, int*); 
	  *psival = (short) atoi(token);
	}
	else if ( (specs.o_width == 'l') ){
	  plival = va_arg(ap, long*);
	  *plival = (long) atoi(token); 
	}
	else{
	  pival = va_arg(ap, int*);
	  *pival = atoi(token); 
	}
	break;
      case 'o':
	pival = va_arg(ap, int*);
	if ( (otoi(token, pi_temp) == -1) )
	  return rv;
	else
	  *pival = *pi_temp;
	break;
      case 'x':
	pival = va_arg(ap, int*);
	if ( (htoi(token, pi_temp) == -1) )
	  return rv;
	else
	  *pival = *pi_temp;
	break;
      case 'c': case 's':
	strvalue = va_arg(ap, char*);
	strcpy(strvalue, token);
	break;
      case 'f': case 'e': case 'g':
	if ( (specs.o_width == 'l') ){ 
	  pdval = va_arg(ap, double*);
	  *pdval = atof(token);
	}
	else if ( (specs.o_width == 'L') ){ 
	  pldval = va_arg(ap, long double*);
	  *pldval = (long double) atof(token);
	}
	else{ 
	  pfval = (float*) va_arg(ap, double*);
	  *pfval = (float) atof(token);
	}
	break;
      default: // sscanf() does not report error if we get here
	break;
      } // end switch

      rv++;
      if ( (eofstr == 0) )
	return rv;
      continue;
    }

    while ( ( (c = next_nws(fmt, pf)) == next_nws(parse, pp) ) ){
      if ( (c == '\0') || (c == '%') ) // by assumption, not allowed to find %
	return rv;
      (*pf)++, (*pp)++;
    }

    if ( (c != '%') && (c != '\0') )
      return rv; // when they stop matching, must be end of string or %

  } // end outer while loop

  return rv;
}

// on entry, checks character at pos i first
// on exit, i points to character returned
char next_nws(char* str, int* i){

  while ( isspace(str[*i]) )
    (*i)++;

  return str[*i];
}

//**********************************************
// Parsing functions
//**********************************************

// upon entry, points to right after the %
// upon exit, points right after a valid conversion char (if any)
// returns 1 for success, 0 for error
int parse_specs(char* w, int* i, struct specifiers* specs){

  char c;
  *specs = s_default();

  while ( ( (c = *(w + *i)) != '\0') && (valid_conversion(c) == 0) ){
    if ( (c == '\0') )
      return 0; // scanf doesn't seem to report errors

    // scanf expects specifiers in the order in which they are tested below
    // hence, we should never find a later spec set in an earlier test
    if ( (c == '*') && (specs->star == -1) ){
      if ( (specs->f_width != -1) || (specs->o_width != 'z') )
	return 0; // strict order in which specs must appear
      specs->star = 1;
      (*i)++;
      continue;
    }
    else if ( (isdigit(c) && (specs->f_width == -1) ) ){
      if ( (specs->o_width != 'z') )
	return 0; // strict order in which specs must appear
      specs->f_width = read_number(w, i);
      (*i)++;
      continue;
    }
    else if ( (valid_o_width_c(c) == 1) && (specs->o_width == 'z') ){
      specs->o_width = c;
      (*i)++;
      continue;
    }
    else
      return 0;
  } // end outer while

  if ( (valid_conversion(c) == 1) ){
    if ( (specs->o_width != 'z') )
      if ( (valid_o_width(specs->o_width, c) == 0) )
	return 0; // invalid combination
    specs->type = c;
    (*i)++;
  }

  return 1;
}

// returns 1 in general; 0 if '\0' encountered
// upon exit, points right after last char read
int read_token(char* ret, char* w, int* i, int max, char del, char type){

  char c;
  int read = 0;

  while ( ( (c = *(w + *i)) != '\0') && (read < max) && ( c != del) ){
    if ( !(type == 'c') && isspace(c) )
      break;
    ret[read++] = c;
    (*i)++;
  }
  if ( !(type == 'c') ) // that's how scanf does it...
    ret[read] = '\0';

  if ( (c == '\0') )
    return 0;
  else
    return 1;
}

// re-route i as appropriate, and strip leading 0, 0x, and 0X
char adj_token_type(char* w, struct specifiers specs){

  char type = specs.type; // default

  if ( (specs.type == 'i') ){
    if ( (w[0] == '0') ){
      move_left(w);
      if ( (w[0] == 'x') || (w[0] == 'X') ){
	move_left(w);
	type = 'x';
      }
      else
	type = 'o';
    }
    else
      type = 'd';
  }

  else if ( (specs.type == 'o') && (w[0] == '0') )
    move_left(w);

  else if ( (specs.type == 'x') && (w[0] == '0') ){
    move_left(w);
    if ( (w[0] == 'x') || (w[0] == 'X') )
      move_left(w);
  }

  return type;
}


int valid_conversion(char c){

  int ret;

  switch( (c) ){
  case 'd':
  case 'i':
  case 'o':  
  case 'x':
  case 'c':
  case 's':
  case 'f':
  case 'e':
  case 'g':
    ret = 1;
    break;
  default:
    ret = 0;
    break;
  }

  return ret;
}

int valid_o_width_c(char c){

  int ret;

  switch( (c) ){
  case 'h': case 'l': case 'L':
    ret = 1;
    break; 
  default:
    ret = 0;
    break;
  }

  return ret;

}

int valid_o_width(char c, char type){

  int ret;

  switch( (type) ){
  case 'd': case 'i': case 'o': case 'x': 
    if ( (c == 'h') || (c == 'l') )
      ret = 1;
    else
      ret = 0;
    break;
  case 'f': case 'e': case 'g':
    if ( (c == 'l') || (c == 'L') )
      ret = 1;
    else
      ret = 0;
    break;
  default:
    ret = 0;
    break;
  }

  return ret;
}

//**********************************************
// Struct specifiers management
//**********************************************

struct specifiers s_default(void){

  struct specifiers s;

  s.star = -1;
  s.f_width = -1;
  s.o_width = 'z';
  s.type = 'z';

  return s;
}

// debugging function
void print_specs(struct specifiers s){

  printf("\nFound specifications:\n");
  printf("  s.star = %d\n", s.star);
  printf("  s.f_width = %d\n", s.f_width);
  printf("  s.o_width = %c\n", s.o_width);
  printf("  s.type = %c\n\n", s.type);
}

//**********************************************
// General utlity functions
//**********************************************

// after number read, *i points to last digit read
int read_number(char* w, int* i){

  char temp[MAXWORD];
  char c;
  int j = 0;

  while ( (isdigit(c = w[*i])) ){
    temp[j++] = c;
    (*i)++;
  }
  temp[j] = '\0';
  (*i)--;

  return atoi(temp);
}

void move_left(char* w){

  char temp[MAXWORD];
  char* p = temp;
  char* pw = w;

  w++;
  while ( ( (*(p++) = *(w++)) != '\0') )
    ;

  strcpy(pw, temp);
}

int otoi(char* w, int* v){

  int i, power, n;

  i =  power = *v = 0;

  reverse(w);
  while ( (w[i] != '\0') ){
    if ( ( (n = octal(w[i++])) == -1 ) )
      return -1;
    *v += n * pow(8, power++);
  }

  return 0;
}

int htoi(char* w, int* v){

  int i, power, n;
  int* pn = &n;

  i =  power = *v = 0;

  reverse(w);
  while ( (w[i] != '\0') ){
    if ( ( (hex(w[i++], pn)) == -1 ) )
      return -1;
    *v += n * pow(16, power++);
  }

  return 0;
}

void reverse(char* w){

  int i, j;
  char c;

  for (i = 0, j = strlen(w)-1; i<j; i++, j--){
    c = w[i];
    w[i] = w[j];
    w[j] = c;
  }
}

int octal(char c){

  if ( (c -'0' < 0) || (c - '0' > 7) )
    return -1;
  else
    return c - '0';
}

int hex(char c, int* v){

  if ( (c - '0' >= 0) && (c - '0' < 10) ){
    *v = c - '0';
    return 0;
  }
  else if ( (c - 'a' >= 0) && (c - 'a' < 6) ){
    *v = 10 + c - 'a';
    return 0;
  } 
  else if ( (c - 'A' >= 0) && (c - 'A' < 6) ){
    *v =  10 + c - 'A';
    return 0;
  }
  else
    return -1;
}

void leave(char* word){

  printf("Error: %s", word);
  exit(-1);
}
