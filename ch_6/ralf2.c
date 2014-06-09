#include <stdio.h>

#define BOGUS 0

int main(){

  char* sbrk(int);

  // defining some variables to test 6_2.c
  int* p = NULL;
  int var, var3, var2, var1, varc, varb, vara;
  float floater = (float) 5/2; float floater2 = (float) 4;

  char chara, char2, char1, char0;

  char* str = "test";
  char* str1 = "test1";
  char* str2 = "test2";
  char* strb = "testb";

  var = var1 = var2 = var3 = 0;
  vara = varb = varc = 1;
  var = -1;

  char0 = '\74';
  char1 = 'z';
  char2 = '\'';
  chara = '\xaB147'; 

  printf("printing line \"testing escape chars within strings\"\n");
  printf("line testing if #defines are kept track of: %d\n", BOGUS);

  // end

  int i, j, last;

  j = last = 0;
  for (i = 0; i <= 1000; i++){
    j = sbrk(1);
    printf("%d\t%d\t%d\t%d\n", j>last, j == last+1, last, j );
    last = j;
  }
  return 0;
}
