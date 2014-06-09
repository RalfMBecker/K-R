#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

#define MAXWORD 100

void reverse(char*, char*);
void itoa(int, char*);
void ftoa(double, char*, int, int);
void etoa(double, char*, int, int, int);
void add_zeros(char*, int, int);
void gtoa(double, char*, int);

void leave(char*);

int main(){

  char str1[MAXWORD];
  char str2[MAXWORD];
  char str3[MAXWORD];
  char str4[MAXWORD];
  char str5[MAXWORD];
  char str6[MAXWORD];
  char str7[MAXWORD];

  int prec;
  int i1 = 13;
  int i2 = -109;

  double f1 = 1234567.8901234;
  double f2 = -23.987654;
  double f3 = 3.1;
  double f4 = -0.1234;
  double f5 = 0.0;
  double f6 = -50.0;
  double f7 = -5000000000.0;

  char* str = "%g/%G";
  printf("**************************************************************\n");
  printf("*Notes:                                                      *\n"); 
  printf("*       (1) Printing of \'float\' type not done. It's weird.   *\n");
  printf("*           Use \'double\' instead.                            *\n");
  printf("*       (2) We truncate at last digit of precision.          *\n");
  printf("*           (printf() rounds at that precision)              *\n");
  printf("*       (3) In format %s, it appears that printf() does   *\n", str);
  printf("*           not consider 0 to be a relevant digit.           *\n");
  printf("**************************************************************\n\n");

  printf("******************\n");
  printf("* Testing itoa() *\n");
  printf("******************\n\n");
  itoa(i1, str1);
  itoa(i2, str2);
  printf("\titoa(%d) = %s\n", i1, str1);
  printf("\titoa(%d) = %s\n\n", i2, str2);

  printf("******************\n");
  printf("* Testing ftoa() *\n");
  printf("******************\n\n");
  prec = 10;
  printf("- printing with precision = %d:\n", prec);
  ftoa(f1, str1, prec, 1);
  ftoa(f2, str2, prec, 1);
  ftoa(f3, str3, prec, 1);
  ftoa(f4, str4, prec, 1);
  ftoa(f5, str5, prec, 1);
  ftoa(f6, str6, prec, 1);
  ftoa(f7, str7, prec, 1);
  printf("\tftoa(%.10f) = %s\n", f1, str1);
  printf("\tftoa(%.10f) = %s\n", f2, str2);
  printf("\tftoa(%.10f) = %s\n", f3, str3);
  printf("\tftoa(%.10f) = %s\n", f4, str4);
  printf("\tftoa(%.10f) = %s\n", f5, str5);
  printf("\tftoa(%.10f) = %s\n", f6, str6);
  printf("\tftoa(%.10f) = %s\n", f7, str7);
  prec = 4;
  printf("- printing with precision = %d:\n", prec);
  ftoa(f1, str1, prec, 1);
  ftoa(f2, str2, prec, 1);
  ftoa(f3, str3, prec, 1);
  ftoa(f4, str4, prec, 1);
  ftoa(f5, str5, prec, 1);
  ftoa(f6, str6, prec, 1);
  ftoa(f7, str7, prec, 1);
  printf("\tftoa(%.4f) = %s\n", f1, str1);
  printf("\tftoa(%.4f) = %s\n", f2, str2);
  printf("\tftoa(%.4f) = %s\n", f3, str3);
  printf("\tftoa(%.4f) = %s\n", f4, str4);
  printf("\tftoa(%.4f) = %s\n", f5, str5);
  printf("\tftoa(%.4f) = %s\n", f6, str6);
  printf("\tftoa(%.4f) = %s\n", f7, str7);
  prec = 0;
  printf("- printing with precision = %d:\n", prec);
  ftoa(f1, str1, prec, 1);
  ftoa(f2, str2, prec, 1);
  ftoa(f3, str3, prec, 1);
  ftoa(f4, str4, prec, 1);
  ftoa(f5, str5, prec, 1);
  ftoa(f6, str6, prec, 1);
  ftoa(f7, str7, prec, 1);
  printf("\tftoa(%.0f) = %s\n", f1, str1);
  printf("\tftoa(%.0f) = %s\n", f2, str2);
  printf("\tftoa(%.0f) = %s\n", f3, str3);
  printf("\tftoa(%.0f) = %s\n", f4, str4);
  printf("\tftoa(%.0f) = %s\n", f5, str5);
  printf("\tftoa(%.0f) = %s\n", f6, str6);
  printf("\tftoa(%.0f) = %s\n\n", f7, str7);

  printf("******************\n");
  printf("* Testing etoa() *\n");
  printf("******************\n\n");
  prec = 10;
  printf("- printing with precision = %d; type = \'e\':\n", prec);
  etoa(f1, str1, prec, 'e', 1);
  etoa(f2, str2, prec, 'e', 1);
  etoa(f3, str3, prec, 'e', 1);
  etoa(f4, str4, prec, 'e', 1);
  etoa(f5, str5, prec, 'e', 1);
  etoa(f6, str6, prec, 'e', 1);
  etoa(f7, str7, prec, 'e', 1);
  printf("\tetoa(%.10e) = %s\n", f1, str1);
  printf("\tetoa(%.10e) = %s\n", f2, str2);
  printf("\tetoa(%.10e) = %s\n", f3, str3);
  printf("\tetoa(%.10e) = %s\n", f4, str4);
  printf("\tetoa(%.10e) = %s\n", f5, str5);
  printf("\tetoa(%.10e) = %s\n", f6, str6);
  printf("\tetoa(%.10e) = %s\n", f7, str7);
  prec = 4;
  printf("- printing with precision = %d, type = \'E\':\n", prec);
  etoa(f1, str1, prec, 'E', 1);
  etoa(f2, str2, prec, 'E', 1);
  etoa(f3, str3, prec, 'E', 1);
  etoa(f4, str4, prec, 'E', 1);
  etoa(f5, str5, prec, 'E', 1);
  etoa(f6, str6, prec, 'E', 1);
  etoa(f7, str7, prec, 'E', 1);
  printf("\tetoa(%.4E) = %s\n", f1, str1);
  printf("\tetoa(%.4E) = %s\n", f2, str2);
  printf("\tetoa(%.4E) = %s\n", f3, str3);
  printf("\tetoa(%.4E) = %s\n", f4, str4);
  printf("\tetoa(%.4E) = %s\n", f5, str5);
  printf("\tetoa(%.4E) = %s\n", f6, str6);
  printf("\tetoa(%.4E) = %s\n", f7, str7);
  prec = 0;
  printf("- printing with precision = %d, type = \'E\':\n", prec);
  etoa(f1, str1, prec, 'E', 1);
  etoa(f2, str2, prec, 'E', 1);
  etoa(f3, str3, prec, 'E', 1);
  etoa(f4, str4, prec, 'E', 1);
  etoa(f5, str5, prec, 'E', 1);
  etoa(f6, str6, prec, 'E', 1);
  etoa(f7, str7, prec, 'E', 1);
  printf("\tetoa(%.0E) = %s\n", f1, str1);
  printf("\tetoa(%.0E) = %s\n", f2, str2);
  printf("\tetoa(%.0E) = %s\n", f3, str3);
  printf("\tetoa(%.0E) = %s\n", f4, str4);
  printf("\tetoa(%.0E) = %s\n", f5, str5);
  printf("\tetoa(%.0E) = %s\n", f6, str6);
  printf("\tetoa(%.0E) = %s\n\n", f7, str7);

  printf("******************\n");
  printf("* Testing gtoa() *\n");
  printf("******************\n\n");
  prec = 10;
  printf("- printing with precision = %d:\n", prec);
  gtoa(f1, str1, prec);
  gtoa(f2, str2, prec);
  gtoa(f3, str3, prec);
  gtoa(f4, str4, prec);
  gtoa(f5, str5, prec);
  gtoa(f6, str6, prec);
  gtoa(f7, str7, prec);
  printf("\tgtoa(%.10g) = %s\n", f1, str1);
  printf("\tgtoa(%.10g) = %s\n", f2, str2);
  printf("\tgtoa(%.10g) = %s\n", f3, str3);
  printf("\tgtoa(%.10g) = %s\n", f4, str4);
  printf("\tgtoa(%.10g) = %s\n", f5, str5);
  printf("\tgtoa(%.10g) = %s\n", f6, str6);
  printf("\tgtoa(%.10g) = %s\n", f7, str7);
  prec = 4;
  printf("- printing with precision = %d:\n", prec);
  gtoa(f1, str1, prec);
  gtoa(f2, str2, prec);
  gtoa(f3, str3, prec);
  gtoa(f4, str4, prec);
  gtoa(f5, str5, prec);
  gtoa(f6, str6, prec);
  gtoa(f7, str7, prec);
  printf("\tgtoa(%.4g) = %s\n", f1, str1);
  printf("\tgtoa(%.4g) = %s\n", f2, str2);
  printf("\tgtoa(%.4g) = %s\n", f3, str3);
  printf("\tgtoa(%.4g) = %s\n", f4, str4);
  printf("\tgtoa(%.4g) = %s\n", f5, str5);
  printf("\tgtoa(%.4g) = %s\n", f6, str6);
  printf("\tgtoa(%.4g) = %s\n", f7, str7);
  prec = 0;
  printf("- printing with precision = %d:\n", prec);
  gtoa(f1, str1, prec);
  gtoa(f2, str2, prec);
  gtoa(f3, str3, prec);
  gtoa(f4, str4, prec);
  gtoa(f5, str5, prec);
  gtoa(f6, str6, prec);
  gtoa(f7, str7, prec);
  printf("\tgtoa(%.0g) = %s\n", f1, str1);
  printf("\tgtoa(%.0g) = %s\n", f2, str2);
  printf("\tgtoa(%.0g) = %s\n", f3, str3);
  printf("\tgtoa(%.0g) = %s\n", f4, str4);
  printf("\tgtoa(%.0g) = %s\n", f5, str5);
  printf("\tgtoa(%.0g) = %s\n", f6, str6);
  printf("\tgtoa(%.0g) = %s\n", f7, str7);


  return 0;
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
    int neg, m, digit, isint;
    double power;
    double epsilon = pow(10.0, -prec);
    isint = (floor(f) == f);

    neg = ( (f < 0) );
    if ( (neg) ){
      f = -f;
      *(w++) = '-';
    }
    m = (int) floor(log10(f)); //********* CHANGED ************** 
    if ( (m < 1.0) ) // restart at 0, not, say, -3 if 0.004
      m = 0;

    while ( (f > epsilon) || (m >= 0) ){ // 2nd test for numbers like 50.xxx
      power = pow(10.0f, m);
      digit = (int) floor((f / power)); //********* CHANGED **************
      f -= (digit * power);
      *(w++) = '0' + digit;
      if ( (m == 0) && (f > 0) && !(prec == 0) ) // 2nd test for xxx.0
	*(w++) = '.';
      m--;
    }

    if ( (isint) && !(prec == 0) && (tr_zeros == 1) ){
      *(w++) = '.';
      m = 0;
      while ( (++m <= prec) )
	*(w++) = '0';
    }   

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
    int neg, m, exp, digit, i, j, temp, isint;
    double power;
    double epsilon = pow(10.0, -prec);
    isint = (floor(f) == f);

    neg = ( (f < 0) );
    if ( (neg) ){
      f = -f;
      *(w++) = '-';
    }
    m = (int) floor(log10(f)); //********* CHANGED **************

    // normalizing to x.xxx
    //   if ( (m < 0) ) 
    //     m--; // 0.5 to 5.0: m = 0, & (0.5/10^(-1)), not (0.1/10^0)
    f = f / pow(10.0, m);
    //    exp = (m == 0)?1:m; 
    exp = m; // 1 for mantissa; 1 for exponenet
    m = 0; // restart at 0, not, say, -3 if 0.004

    // mantissa
    while ( (f > epsilon) || (m >=0) ){  
      power = pow(10.0f, m);
      digit = (int) floor((f / power)); //********* CHANGED **************
      f -= (digit * power);
      *(w++) = '0' + digit;
      if ( (m == 0) && (f > 0) && !(prec == 0) )
	*(w++) = '.';
      m--;
    }

    if ( (isint) && !(prec == 0) && (tr_zeros == 1) ){
      *(w++) = '.';
      m = 0;
      while ( (++m <= prec) )
	*(w++) = '0';
    }   

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

void leave(char* word){

  printf("Error: %s", word);
  exit(-1);

}
