#include <stdio.h>

int main(){

  int c;
  int blanks, tabs, newlines;

  blanks = tabs = newlines = 0;

  while ((c=getchar()) != EOF){
    if (c == ' ')
      ++blanks;
    else if (c == '\t')
      ++tabs;
    else if (c == '\n')
      ++newlines;
  }

  printf("Number of...\n\nBlanks:\t%6d\nTabs:\t%6d\nNewLines:%6d\n", 
	 blanks, tabs, newlines);

  return 0;
}
