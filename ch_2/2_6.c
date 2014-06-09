#include <stdio.h>
#include <math.h>

#define MAXSTRING 500

void print_2(unsigned, int);
unsigned setbits(unsigned, int, int, unsigned);
unsigned invert(unsigned, int, int);
unsigned rightrot(unsigned, int);

int main(){

  unsigned x, y, z;

  x = 109;
  y = 30;
  printf("x =%u,\ty =%u\n", x, y);
  print_2(x, 4);
  print_2(y, 4);
  
  z = setbits(x, 7, 2, y);
  printf("\nsetbits(x, 7, 2, y) =\n");
  print_2(z, 4);

  z = invert(x, 31, 32);
  printf("\ninvert(x, 31, 32) =\n");
  print_2(z, 4);

  z = rightrot(x, 3);
  printf("\nrightrot(x, 3) = \n");
  print_2(z, 4);

  return 0;
}


unsigned rightrot(unsigned x, int n){

  int bytes, m;

  bytes = sizeof(unsigned);
  m = 8*bytes - 1;

  if ( (n < 0) || (n > m) ){
    printf("Range error in \'rightrot\'.");
    return -1;
  }

  unsigned high, low, mask;

  mask = ~(~0 << n);

  low = x >> n;
  high = setbits(0, 31, n, mask & x);
  
  return low | high;
}

/* made harder by having to be careful when sizeof(unsigned) is hit, or */
/* bit 0 is part of the inversion */
unsigned invert(unsigned x, int p, int n){

  int bytes, m;

  bytes = sizeof(unsigned);
  m = 8*bytes - 1;

  if ( (p < n-1) || (n < 0) ){
    printf("Range error in \'invert\'.");
    return -1;
  }
  else if (n <= 1)
    return x;

  unsigned b, mask, mask_h, mask_l, mask_t, mask_b;
  unsigned low, high, pn, nn;

  mask_t = (p == m)?0:(~0 << p+1);
  mask_b = (p == n-1)?0:(~(~0 << p-n+1));
  mask = ~(mask_t | mask_b); /* mark beginning and end of range */

  b = x & mask; /* pick what will be rotated */

  /* comment July 2013: there should be a much easier way XORing with
     1111111 in the position to be inverted */
  pn = p;
  nn = p-n+1;
  while (pn > nn){
    mask_t = (pn == m)?0:(~0 << pn+1);
    mask_b = ~(~0 << pn); /* works w/o range check for pn=0 */ 
    mask_h = ~(mask_t | mask_b);

    mask_t = (nn == m)?0:(~0 << nn+1);
    mask_b = ~(~0 << nn);
    mask_l = ~(mask_t | mask_b);

    high = b & mask_h;
    low = b & mask_l;
 
    if (high)
      b |= mask_l;
    else
      b &= ~mask_l;
    if (low)
      b |= mask_h;
    else
      b &= ~mask_h;

    --pn, ++nn;
  }

  return (~mask & x) | b;
}

/* to a lesser extent, range check complicates this too */
unsigned setbits(unsigned x, int p, int n, unsigned y){

  int bytes, m;

  bytes = sizeof(unsigned);
  m = 8*bytes - 1;

  if (p < n-1){
    printf("Range error in \'setbits\'.");
    return -1;
  }

  unsigned b, mask, mask_1;
  mask_1 = (p == m)?0:(~0 << p+1); /* pick all to left of beginning */
  mask = ~(~(~0 << p-n+1) | mask_1); /* pick all to right (double ~), or, ~ */
  b = (y << p-n+1) & mask; /*move to position in x, then pick only w. needed*/
  x = x & ~mask; /* blind out to 0 what will be replaced */

  return x | b;

}

void print_2(unsigned x, int bytes){

  char a[MAXSTRING];
  long long d;
  int i, j;
  d = pow( 2, 8*bytes - 1 );

  while (x >= 2*d){ /* x can't be represented in bytes bytes */
    ++bytes;
    d = pow(2 , 8*bytes -1);
  }

  i = 0;
  j = 1;
  while (d >=1){
    if (x>=d){
      a[i++] = 1 + '0';
      x -= d;
    }
    else
      a[i++] = '0';
    d /= 2;
    a[i++] = ' ';
    if ( (j%8 == 0) && j){
      a[i++] = ' ';
      a[i++] = ' ';
    }
    ++j;
  }

  a[i++] = '\n';
  a[i] = '\0';

  printf("%s", a);

} 
