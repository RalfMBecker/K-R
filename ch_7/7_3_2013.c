#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

#define MAXWORD 100

// keeping track of format string
struct flags{
  int left_adj;
  int sign; // ignored with o, x/X
  int space; // ignored with o, x/X
  int hash;
  int padding; // ignored with left_adj
               // undocumented: if prec&f_width specified,pads to prec with '0',
               //               but to field width left with ' ', not '0
};

struct specifiers{
  struct flags f;
  int f_width;
  int precision;
};

struct flags f_default(void);
struct specifiers s_default(void);
void print_specs(struct specifiers);

// my_printf core functions
void my_printf(char*, ...);
void print_str(char*);
char parse_specs(char*, int*, struct specifiers*);
int valid_conversion(char);
void use_specs(char*, char*, char, int, struct specifiers);
void pad_left(char*, char*, int, char, int);
void rem_sign(char*, char*);

// re-implementing %f, %e (%E), and %g format
void reverse(char*, char*);
void itoa(int, char*);
void ftoa(double, char*, int, int);
void etoa(double, char*, int, int, int);
void add_zeros(char*, int, int);
void gtoa(double, char*, int);
void otoa(unsigned int, char*);
void xtoa(unsigned int, char*, char);
char ntoh(unsigned int, char);

// general utility
int read_number(char*, int*);
void leave(char*);

int main(){

  printf("***************************************************\n");
  printf("* Date: Aug 07, 2013                              *\n");
  printf("*                                                 *\n");
  printf("* Note: - we truncate at precision                *\n");
  printf("*       - no attempt to match undefined output    *\n");
  printf("*                                                 *\n");
  printf("* Function supports:                              *\n");
  printf("*    - formats: d, i, o, x/X, f, e/E, g/G, c, s   *\n");
  printf("*    - specs: -, +, ' ', 0, #; field width, prec  *\n");
  printf("*             (# only for o, x/X)                 *\n");
  printf("*    - note comments in struct flags about some   *\n");
  printf("*      undocumented behavior                      *\n");
  printf("***************************************************\n\n");

  char str1[MAXWORD];
  char str2[MAXWORD];
  char str3[MAXWORD];
  char str4[MAXWORD];
  char str5[MAXWORD];
  char str6[MAXWORD];
  char str7[MAXWORD];
  char str8[MAXWORD];
  char str9[MAXWORD];
  char str10[MAXWORD];
  char str11[MAXWORD];
  char str12[MAXWORD];

  strcpy(str1, "Test%+- 08.5d End Test\n");
  strcpy(str4, "Test%08.5d End Test\n");
  strcpy(str2, "Test% 10.7g End Test\n");
  strcpy(str8, "Test% 10.7f End Test\n");
  strcpy(str3, "Test%-.10s, also %12.8f and %c End Test\n");
  strcpy(str5, "Printing w/o format specs:%g...\n");
  strcpy(str6, "Printing padded to 15:%15g...\n");
  strcpy(str7, "Printing padded to 15; left-adjusted; signed:%+-15g...\n");
  strcpy(str9, "Octal:%o...% 08.5o...%-7.4o...End\n");
  strcpy(str10, "Hex:%x...% 08.5X...%-7.4x...End\n");
  strcpy(str11, "Octal:%#o...% #08.5o...%#-7.4o...End\n");
  strcpy(str12, "Hex:%#x...%# 08.5X...%-#7.4x...End\n");

  int i1 = 13;
  double d1 = 3.1415;
  char c1 = '^';
  char* svar1 = "*a pretty large string*";
  unsigned int u1 = 54;
  unsigned int u2 = 111;

  printf("Print with no specs; then using fmt str: ");
  printf("first with my_print(), then printf()\n");
  printf("-----------------------------------------");
  printf("------------------------------------\n\n");
  printf("%d\n", i1);
  printf("Format string parsed: %s", str1);
  my_printf(str1, i1);
  printf(str1, i1);
  printf("\n");
  printf("%d\n", i1);
  printf("Format string parsed: %s", str4);
  my_printf(str4, i1);
  printf(str4, i1);
  printf("\n");
  printf("%g\n", d1);
  printf("Format string parsed: %s", str2);
  my_printf(str2, d1);
  printf(str2, d1);
  printf("\n");
  printf("%f\n", d1);
  printf("Format string parsed: %s", str8);
  my_printf(str8, d1);
  printf(str8, d1);
  printf("\n");
  printf("%s\n%f\n%c\n", svar1, d1, c1);
  printf("Format string parsed: %s", str3);
  my_printf(str3, svar1, d1, c1);
  printf(str3, svar1, d1, c1);
  printf("\n");
  printf("%g\n", d1);
  printf("Format string parsed: %s", str5);
  my_printf(str5, d1);
  printf(str5, d1);
  printf("\n");
  printf("%g\n", d1);
  printf("Format string parsed: %s", str6);
  my_printf(str6, d1);
  printf(str6, d1);
  printf("\n");
  printf("%g\n", d1);
  printf("Format string parsed: %s", str7);
  my_printf(str7, d1);
  printf(str7, d1);
  printf("\n");

  printf("(decimal) %u\n", u1);
  printf("(decimal) %u\n", u1);
  printf("(decimal) %u\n", u2);
  printf("Format string parsed: %s", str9);
  my_printf(str9, u1, u1, u2);
  printf(str9, u1, u1, u2);
  printf("\n");
  printf("(decimal) %u\n", u1);
  printf("(decimal) %u\n", u2);
  printf("(decimal) %u\n", u2);
  printf("Format string parsed: %s", str10);
  my_printf(str10, u1, u2, u2);
  printf(str10, u1, u2, u2);
  printf("\n");
  printf("(decimal) %u\n", u1);
  printf("(decimal) %u\n", u1);
  printf("(decimal) %u\n", u2);
  printf("Format string parsed: %s", str11);
  my_printf(str11, u1, u1, u2);
  printf(str11, u1, u1, u2);
  printf("\n");
  printf("(decimal) %u\n", u1);
  printf("(decimal) %u\n", u2);
  printf("(decimal) %u\n", u2);
  printf("Format string parsed: %s", str12);
  my_printf(str12, u1, u2, u2);
  printf(str12, u1, u2, u2);

  return 0;
}

//**********************************************
// my_printf core functions
//**********************************************

// we use print to print values converted to a string
void my_printf(char* fmt, ...){

  va_list ap;
  char word[MAXWORD], ret[MAXWORD];
  char* w;
  int sign;
  struct specifiers specs;
  int prec;
  char type;
  int pos;

  int ival;
  unsigned int uival;
  double dval;
  char cval;

  va_start(ap, fmt);
  for (pos = 0; fmt[pos]; pos++){
    if ( (fmt[pos] != '%') ){
      putc(fmt[pos], stdout);
      continue;
    }
    else{
      sign = -1; // no number by default
      pos++;
      type = parse_specs(fmt, &pos, &specs);

      switch ( (type) ){
      case 'd': case 'i':
	ival = va_arg(ap, int);
	itoa(ival, word);
	sign = (ival >= 0)?1:-1;
	break;
      case 'o':
	uival = va_arg(ap, unsigned int);
	otoa(uival, word); // note we do not have to set sign <> -1
	break;
      case 'x': case 'X':
	uival = va_arg(ap, unsigned int); //note don't have to set sign <> -1
	xtoa(uival, word, (type == 'x')?'a':'A');
	break;
      case 'f':
	dval = va_arg(ap, double);
	prec = (specs.precision == -1)?6:specs.precision;
	ftoa(dval, word, prec, 1);
	sign = (dval >= 0)?1:-1;
	break;
      case 'e': case 'E':
	dval = va_arg(ap, double);
	prec = (specs.precision == -1)?6:specs.precision;
	etoa(dval, word, prec, type, 1);
	sign = (dval >= 0)?1:-1;
	break;
      case 'g': case 'G':
	dval = va_arg(ap, double);
	prec = (specs.precision == -1)?6:specs.precision;
	gtoa(dval, word, prec);
	sign = (dval >= 0)?1:-1;
	break;
      case 'c':
	cval = (char) va_arg(ap, int);
	word[0] = cval;
	word[1] = '\0';
	break;
      case 's':
	w = va_arg(ap, char*);
	strcpy(word, w);
	break;
      default:
	leave("invalid conversion character\n");
	break; // error check in sub routines; for completeness
      } // end switch

      use_specs(ret, word, type, sign, specs);
      print_str(ret);
    } // end else
  } // end for

  va_end(ap);

}

void print_str(char* w){

  char* p = w;

  for (; *p; p++)
    putc(*p, stdout); 
}

//**********************************************
// Format string management
//**********************************************

struct flags f_default(void){

  struct flags temp;

  temp.left_adj = 0;
  temp.sign = 0;
  temp.space = 0;
  temp.padding = 0;
  temp.hash = 0;

  return temp;
}

struct specifiers s_default(void){

  struct specifiers temp;

  temp.f = f_default();
  temp.f_width = -1;
  temp.precision = -1;

  return temp;
}

// debugging function
void print_specs(struct specifiers s){

  printf("  f.left_adj = %d\n", s.f.left_adj);
  printf("  f.sign = %d\n", s.f.sign);
  printf("  f.space = %d\n", s.f.space);
  printf("  f.padding = %d\n", s.f.padding);
  printf("  f.hash = %d\n", s.f.hash);
  printf("  f_width = %d\n", s.f_width);
  printf("  precision = %d\n", s.precision);
}

// entering pointing to first char after %
// leaving pointing to (valid) conversion char (if not valid, error)
char parse_specs(char* w, int* i, struct specifiers* specs){

  char c;
  *specs = s_default();
  int in_flags = 1;
  int min_f_set = 0;
  int period_set = 0;

  while ( ( (c = *(w + *i)) != '\0') && (!valid_conversion(c)) ){
    if ( (c == '\0') )
      leave("in format string, missing valid conversion character\n");

    if ( ( (isdigit(c)) && ( c != '0') ) || ( c == '.') )
      in_flags = 0;

    // do flags part
    if ( (in_flags) ){
      switch ( (c) ){
      case '-':
	specs->f.left_adj = 1;
	(*i)++;
	break;
      case '+':
	specs->f.sign = 1;
	(*i)++;
	break;
      case ' ':
	specs->f.space = 1;
	(*i)++;
	break;
      case '0':
	specs->f.padding = 1;
	(*i)++;
	break;
      case '#':
	specs->f.hash = 1;
	(*i)++;
	break;
      default:
	in_flags = 0;
	break;
      }
      continue;
    } // end in_flags part

    // if we come here, we're past the flags part, if any
    if ( (isdigit(c)) && (min_f_set == 0) ){
      min_f_set = 1;
      specs->f_width = read_number(w, i);
      (*i)++;
      continue;
    }
    else if ( (c == '.') ){
      min_f_set = period_set = 1;
      (*i)++;
      continue;
    }
    else if ( (isdigit(c)) && (period_set == 1) ){
      specs->precision = read_number(w, i);
      (*i)++;
      continue;
    }
    else{
      print_specs(*specs);
      leave("invalid format string specifiers\n");
    }
  } // end outer while

  return c;
}

int valid_conversion(char c){

  int ret;

  switch( (c) ){
  case 'd':
  case 'i':
  case 'o':  
  case 'x':
  case 'X':
  case 'c':
  case 's':
  case 'f':
  case 'e':
  case 'E':
  case 'g':
  case 'G':
    ret = 1;
    break;
  default:
    ret = 0;
    break;
  }

  return ret;
}

// sign: sign of numbers; -1 for nan
void use_specs(char* ret,char* w,char type, int sign, struct specifiers specs){

  int n;
  char c;
  char temp[MAXWORD];
  int is_hexoct = -1; // notational convenience: checked together usually

  strcpy(ret, w); // in case of no specifiers
  strcpy(temp, w); // there is probably a smart way to not need temp
  if ( (type == 'o') || (type == 'x') || (type == 'X') )
    is_hexoct = 1;

  if ( (specs.f.sign == 1) && (sign != -1) && (is_hexoct == -1) ){
    if ( (w[0] != '+') && (w[0] != '-') ){
      if ( (sign > 0) )
	strcpy(temp, "+");
      else
	strcpy(temp, "-");
      strcat(temp, w);
    }
  }

  if ( (specs.f.space == 1) && (sign != -1) && (is_hexoct == -1) ){
    if ( (temp[0] != '+') && (temp[0] != '-') ){
      strcpy(temp, " ");
      strcat(temp, w);
      if ( (specs.f_width > 0) )
	specs.f_width--;
    }
  }

  // Note: precision integration for doubles is in char-to-string functions
  if ( (specs.precision != -1) ){

    if ( (type == 's') ){
      if ( (specs.precision < (strlen(ret)-1) ) )
	temp[specs.precision] = '\0';
    } // end precision string %s

    if ( (type == 'd') || (type == 'i') || ( is_hexoct == 1) ){
      n = specs.precision - strlen(temp);
      if ( (specs.f.sign == 1) )
	n++; // doesn't seem to count towards precision
      if ( (n > 0) ){
	pad_left(ret, temp, specs.f.sign, '0', n);
	strcpy(temp, ret); // keep temp current
      }
    } // end precision integer %d
  } // end precision

  if ( (specs.f.hash == 1) && (is_hexoct == 1) ){
    if ( (type == 'o') ){
      if ( ( specs.precision == -1) ||( (specs.precision - strlen(temp)) < 0) ){
	strcpy(ret, "0"); // leading '0' counted as part of padding
	strcat(ret, temp);
      }
    }
    else if ( (type == 'x') ){
      strcpy(ret, "0x");
      strcat(ret, temp);
    }
    else{
      strcpy(ret, "0X");
      strcat(ret, temp);
    }

    strcpy(temp, ret); // keep temp current
  } // end hash

  if ( (specs.f_width != -1) ){
    n = specs.f_width - strlen(temp);
    if ( (n > 0) ){
      if ( (specs.f.padding == 1) && (specs.f.left_adj != 1) &&
	   (specs.precision == -1) )
	c = '0';
      else
	c = ' ';
      if ( (specs.f.left_adj != 1) ){
	pad_left(ret, temp, specs.f.sign, c, n);
	strcpy(temp, ret); // keep temp current
      }
      else{
	n = strlen(temp);
	while ( (n < specs.f_width) )
	  temp[n++] = c;
	temp[n] = '\0';
      }
    } // end (n > 0)
  } // end field width

  strcpy(ret, temp);

}

void pad_left(char* ret, char* w, int sign, char pad_c, int n){

  char c;
  char* p = ret;
  char adj[MAXWORD];

  if ( (sign == 1) )
    rem_sign(adj, w);
  else
    strcpy(adj, w);

  if ( ( (c = w[0]) == '+') || (c == '-') || (c == ' ') )
    ret[0] = c;
  else{
    ret[0] = pad_c;
    n--;
  }

  while ( (n-- > 0) )
    *++p = pad_c;
  *++p = '\0';

  strcat(ret, adj);

}

//**********************************************
// Type-to-Char* translation functions
//**********************************************

void reverse(char* w_i, char* w_o){

  int i, j, len;
  len = strlen(w_i);

  for ( i = 0, j = (len -1); i < j; i++, j--){
    w_o[i] = w_i[j];
    w_o[j] = w_i[i];
  }
  if ( (i == j) )
    w_o[i] = w_i[i];

  w_o[len] = '\0';

}

void itoa(int d, char* w){

  int sign;
  char temp[MAXWORD];
  char* p = temp;

  if ( ( (sign = d) < 0) )
    d = -d;

  do{
    *p++ = d%10 + '0';
  } while( ( (d /= 10) > 0) );

  if ( (sign < 0) )
    *p++ = '-';

  *p = '\0';
  reverse(temp, w);

}

// conversion of %f specifier
// NOTE: we truncate at the last digit of precision; printf() rounds
void ftoa(double f, char* w, int prec, int tr_zeros){

  if ( isnan(f) )
    strcpy(w, "nan");
  else if ( isinf(f) )
    strcpy(w, "inf");
  else if ( (f == 0.0) ){
    if ( (prec == 0 ) )
      strcpy(w, "0");
    else{
      *(w++) = '0';
      *(w++) = '.';
      int i = 0;
      while ( (++i <= prec) )
	*(w++) = '0';
      *w = '\0';
    }
  }
  else{
    int neg, m, digit, isint, zero_pad, after_dot;
    double power;
    double epsilon = pow(10.0, -prec);
    isint = (floor(f) == f);
    zero_pad = (tr_zeros)?prec:0;
    after_dot = 0;

    neg = ( (f < 0) );
    if ( (neg) ){
      f = -f;
      *(w++) = '-';
    }

    m = (int) floor(log10(f));
    if ( (m < 1.0) ) // restart at 0, not, say, -3 if 0.004
      m = 0;

    while ( (f > epsilon) || (m >= 0) ){ // 2nd test for numbers like 50.xxx
      power = pow(10.0f, m);
      digit = (int) floor((f / power)); 
      f -= (digit * power);
      *(w++) = '0' + digit;
      if ( (after_dot) )
	zero_pad--;
      if ( (m == 0) && (f > 0) && !(prec == 0) ){ // 2nd test for xxx.0
	*(w++) = '.';
	after_dot = 1;
      }
      m--;
    }

     // add trailing zeros, if any
    if ( (isint) && !(prec == 0) && (tr_zeros == 1) )
      *(w++) = '.';
    else if ( (zero_pad > 0) )
      prec = zero_pad;
    else
      prec = 0;
    while ( (prec-- > 0) )
	*(w++) = '0';

    *w = '\0';
  } // end else (actual processing)
}

// conversion of %e/%E specifier
// NOTE: we truncate at the last digit of precision; printf() rounds
void etoa(double f, char* w, int prec, int type, int tr_zeros){

  if ( isnan(f) )
    strcpy(w, "nan");
  else if ( isinf(f) )
    strcpy(w, "inf");
  else if ( (f == 0.0) ){
    if ( (prec == 0 ) )
      strcpy(w, "0E+00");
    else{
      strcpy(w, "0.");
      add_zeros(w, prec, type);
    }
  }
  else{
    int neg, m, exp, digit, i, j, temp, isint, zero_pad, after_dot;
    double power;
    double epsilon = pow(10.0, -prec);
    isint = (floor(f) == f);
    zero_pad = (tr_zeros)?prec:0;
    after_dot = 0;

    neg = ( (f < 0) );
    if ( (neg) ){
      f = -f;
      *(w++) = '-';
    }
    m = (int) floor(log10(f)); 

    // normalizing to x.xxx
    f = f / pow(10.0, m);
    exp = (m == 0)?1:m; // 1 for mantissa; 1 for exponenet
    m = 0; // restart at 0, not, say, -3 if 0.004

    // mantissa
    while ( (f > epsilon) || (m >= 0) ){
      power = pow(10.0f, m);
      digit = (int) floor((f / power)); 
      f -= (digit * power);
      *(w++) = '0' + digit;
      if ( (after_dot) )
	zero_pad--;
      if ( (m == 0) && (f > 0) && !(prec == 0) ){
	*(w++) = '.';
	after_dot = 1;
      }
      m--;
    }

    // add trailing zeros, if any
    if ( (isint) && !(prec == 0) && (tr_zeros == 1) )
      *(w++) = '.';
    else if ( (zero_pad > 0) )
      prec = zero_pad;
    else
      prec = 0;
    while ( (prec-- > 0) )
	*(w++) = '0';

    // exponent
    *(w++) = type;
    if ( (exp > 0) )
      *(w++) = '+';
    else{
      *(w++) = '-';
      exp = -exp;
    }
    if ( (exp >= 0) && (exp < 10) )
      *(w++) = '0';
    m = 0; // now, used to keep track of how many digits exponent has
    while ( (exp > 0) ){
      *(w++) = '0' + exp%10;
      exp /= 10;
      m++;
    }

    // swap exponent
    w -= m;
    for (i=0, j = m-1; i < j; i++, j--){
      temp = w[i];
      w[i] = w[j];
      w[j] = temp;
    }
    w += m;

    *w = '\0';
  }
}

void add_zeros(char* w, int prec, int type){

  int i = 0;
  char temp[MAXWORD];
  char* p = temp;

  while ( (++i <= prec) )
    *(p++) = '0';
  if ( (type == 'e') )
    *(p++) = 'e';
  else if ( (type == 'E') )
    *(p++) = 'E';
  else
    leave("incorrect type\n");
  *p = '\0';
  strcat(w, temp);
  strcat(w, "+00"); 

}

// conversion of %g/%G specifier
// NOTE: we truncate at the last digit of precision; printf() rounds
// NOTE: it appears that a leading '0' is not considered a 'relevant digit'
void gtoa(double f, char* w, int prec){

  int scientific, m, neg;
  double temp;

  neg = ( (f < 0) )?1:0;
  temp =  (neg)?-f:f;
  m = log10(temp);
  scientific = (m < -4) || ( m >= prec);

  if ( (m >= 0) && !(prec == 0) && !( (f < 1) && (f > -1) ) ){
    if ( !(scientific) )
      prec -= (m+1);
    else
      prec -= 1;
  }

  if ( (f == 0) )
    ftoa(f, w, 0, 0); // not in KR; matches today's output
  else if ( ((f > -1.0) && (f < 1.0)) && (prec == 0) )
    ftoa(f, w, 1, 0); // not in KR; matches today's output
  else if ( (scientific) )
    etoa(f, w, prec, 'e', 0);
  else
    ftoa(f, w, prec, 0);

}

// function assumes d is properly handed on as unsigned int 
void otoa(unsigned int d, char* w){

  char temp[MAXWORD];
  char* p = temp;

  do{
    *(p++) = '0' + d%8;
  } while ( ( (d /= 8) != 0) );

  *p = '\0';
  reverse(temp, w);
}

// function assumes d is properly handed on as unsigned int
// type is 'a' for conversion 'x'; 'A' for 'X' 
void xtoa(unsigned int d, char* w, char type){

  char temp[MAXWORD];
  char* p = temp;

  do{
    *(p++) = ntoh(d%16, type);
  } while ( ( (d /= 16) != 0) );

  *p = '\0';
  reverse(temp, w);
}

char ntoh(unsigned int d, char type){

  if ( (d < 0) || (d > 15) )
    leave("invalid hex character");

  if ( (d < 10) )
    return '0' + d;
  else
    return type + (d - 10); 
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

void rem_sign(char* in, char* out){

  out++;
  while ( ( (*(in++) = *(out)++) != '\0') )
    ;

}

void leave(char* word){

  printf("Error: %s", word);
  exit(-1);

}
// 861
