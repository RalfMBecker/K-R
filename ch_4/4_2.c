#include <stdio.h>
#include <ctype.h>
#include <math.h>

const char numbers[] = "01234567890123456789012345678901234567890"
  "12345678901234567890123456789\n";

double atof(char*);

int main(){

  char a[] = "  -156.6788";
  char b[] = "  +67.98765";
  char c[] = "  +67.987e-2";
  char d[] = "  -67.987E2";
  char e[] = "  -67.987E-Ralf";
  char f[] = "  -67.987ERalf";
  double x;

  x = atof(a);
  printf("a =%s, atof(a) = %.10g\n", a, x);
  x = atof(b);
  printf("b =%s, atof(b) = %.10g\n", b, x);
  x = atof(c);
  printf("c =%s, atof(c) = %.10g\n", c, x);
  x = atof(d);
  printf("d =%s, atof(d) = %.10g\n", d, x);
  x = atof(e);
  printf("e =%s, atof(e) = %.10g\n", e, x);
  x = atof(f);
  printf("f =%s, atof(f) = %.10g\n", f, x);

  return 0;

}

double atof(char* s){

  double val, power;
  int i, sign, val2;

  for (i = 0; isspace(s[i]); i++)
    ;
  sign = (s[i] == '-')?-1:1;
  if ( s[i] == '+' || s[i] == '-')
    i++;
  for ( val = 0.0; isdigit(s[i]); i++)
    val = 10.0 * val + (s[i] - '0');
  if ( s[i] == '.')
    i++;
  for ( power = 1.0; isdigit(s[i]); i++){
    val = 10.0 * val + (s[i] - '0');
    power *= 10.0;
  }

  if ( s[i] == 'e' || s[i] == 'E'){   
    val2 = 0;
    if ( s[++i] == '-' ){
      if(!isdigit(s[++i]))
	printf("Error: stray e or E after number.\n");
      while (isdigit(s[i]))
	val2 = 10.0 * val2 + (s[i++] - '0');
      power *= pow(10, val2);
    }
    /* note the count up while checking if */
    else if (isdigit(s[i])){
      while (isdigit(s[i]))
	val2 = 10.0 * val2 + (s[i++] - '0');
      power /= pow(10, val2);
    } 
     else /* report error, and use number as it was before error */
	printf("Error: stray e or E after number.\n");
  }

  return sign * val / power;
}
  

