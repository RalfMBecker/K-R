#include <stdio.h>
#include <string.h>

#define MAXLINE 200

const char pattern[] = "ould";
const char numbers[] = "01234567890123456789012345678901234567890"
  "12345678901234567890123456789\n";

int my_getline(char*, int);
int strindex(char*, const char*);
int strrindex(char*, const char*);

int main(){

  char line[MAXLINE];
  int found = 0;
  int found2 = 0;

  while (my_getline(line, MAXLINE) > 0){
    if (strindex(line, pattern) >= 0){
      printf("%s", line);
      found ++;
    }
    if (strrindex(line, pattern) >= 0){
      printf("%s", line);
      found ++;
    }
  }
  return found;
}

int my_getline(char* s, int lim){

  int c, i;

  i = 0;
  while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;
  if (c == '\n')
    s[i++] = c;
  s[i] = '\0';
  return i;

}

int strindex(char* s, const char* t){

  int i, j, k;

  for (i = 0; s[i] != '\0'; i++){
    for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
      ;
    if (k>0 && t[k] == '\0')
      return i;
  }

  return -1;

}

int strrindex(char* s, const char* t){

  int i, j, k;

  for (i = strlen(s)-1; i >= 0; i--){
    for (j = i, k = strlen(t)-1; j>=0 && k >= 0 && s[j] == t[k]; j--, k--)
      ;  
    if (k == -1)
      return j+1;
  }

  return -1;

}
