#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(){

  printf("bits in a char:                 \t %d\n", CHAR_BIT);
  printf("maximum value of char:          \t%d\n", CHAR_MAX);
  printf("minimum value of char:          \t%d\n", CHAR_MIN);
  printf("maximum value of int:           \t%d\n", INT_MAX);
  printf("minimum value of int:           \t%d\n", INT_MIN);
  printf("maximum value of long:          \t%ld\n", LONG_MAX);
  printf("minimum value of long:          \t%ld\n", LONG_MIN);
  printf("maximum value of long long:     \t%lld\n", LLONG_MAX);
  printf("minimum value of long long:     \t%lld\n", LLONG_MIN);
  printf("maximum value of signed char:   \t%d\n", SCHAR_MAX);
  printf("minimum value of signed char:   \t%d\n", SCHAR_MIN);
  printf("maximum value of short:         \t%d\n", SHRT_MAX);
  printf("minimum value of short:         \t%d\n", SHRT_MIN);
  printf("maximum value of unsigned char: \t%d\n", UCHAR_MAX);
  printf("maximum value of unsigned int:  \t%u\n", UINT_MAX);
  printf("maximum value of unsigned long: \t%lu\n", ULONG_MAX);
  printf("maximum value of unsigned short:\t%d\n", USHRT_MAX);

  printf("\n");

  printf("radix of exponent representation:               \t%d\n", FLT_RADIX);
  printf("floating point rounding mode, addition:         \t%d\n", FLT_ROUNDS);
  printf("decimal digits of float precision:              \t%d\n", FLT_DIG);
  printf("smallest float x s.t. 1.0 + x <=> 1.0:          \t%f\n", FLT_EPSILON);
  printf("number of base FLT_RADIX digits in mantissa:    \t%d\n",FLT_MANT_DIG);
  printf("maximum floating point number:                  \t%f\n", FLT_MAX);
  printf("maximum n s.t. FLT_RADIX^n-1 is representable:  \t%d\n", FLT_MAX_EXP);
  printf("minimum normalized floating point number:       \t%f\n", FLT_MIN);
  printf("minimum n s.t. 10^n is a normalized number:     \t%d\n" ,FLT_MIN_EXP);
  printf("decimal digits of double precision:             \t%d\n", DBL_DIG);
  printf("smallest double x s.t. 1.0 + x <=> 1.0:         \t%f\n", DBL_EPSILON);
  printf("number of base FLT_RADIX digits in mantissa:    \t%d\n",DBL_MANT_DIG);
  printf("maximum double floating point number:           \t%f\n", DBL_MAX);
  printf("maximum n s.t. FLOAT_RADIX^n-1 is representable:\t%d\n", DBL_MAX_EXP);
  printf("minimum normalized double floating point number:\t%f\n", DBL_MIN);
  printf("minimum n s.t. 10^n is a normalized number:     \t%d\n", DBL_MIN_EXP);



  return 0;
}
