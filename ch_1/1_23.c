#include <stdio.h>
#include <string.h>

#define MAXLINE 200
enum states { OUT=0, IN };

int my_getline(char*, int);
void truncate(char*);
void copy_ft(char*, char*, int, int);
int find_pat(char*, char*);
int find_first_after(char*, char, int);
int find_full_helper(char*, char*);

/* program does not handle correctly quoted sequences of /* and */
/* we do not check for syntax errors such as text after closing comment */
int main(){

  char line[MAXLINE];
  char line_out[MAXLINE];
  char c;

  int state, found;

  state = OUT;
  while ( my_getline(line, MAXLINE) > 0){
    if (state == IN){
      if (find_pat(line, "*/") > -1){ /* if not found, multiline comment */
	state == OUT;
	break;
      }
    }
    else{
      if ( (found = find_pat(line, "/*")) > -1){

	copy_ft(line_out, line, 0, found-1);
	truncate(line_out);
	printf("%s", line_out);
	line_out[0] = '\n'; /* needed if line of 1 char only followed by */
	line_out[1] = '\0'; /* comment */

	if ( !(find_pat(line_out, "*/")) ) /* multiline */
	  state = IN;

      }
      else
	printf("%s", line);
    }
  }

  return 0;

}

/* find exact pattern pat in string in */
int find_pat(char* in, char* pat){

  char in_current[MAXLINE];
  int len_in, len_current, len_pat;
  len_pat = strlen(pat);
  len_current = len_in = strlen(in);

  if ( (len_pat == 0) || (len_in == 0) || (len_in < len_pat) )
    return -1;
  copy_ft(in_current, in, 0, len_in);

  int first; /* position relative to currently examined string */
  int into_in; /* position relative to string in */
  int ct, found;

  into_in = 0; /* how deep into in are we? */
  first = -1; /* as examined from one higher up */
  while (len_current > len_pat){

    ct = pat[0];
    first = find_first_after(in_current, ct, first);
    if (first == -1)
      return -1;

    /* note in next line that after first time, we move up i+1 positions */
    /* as arrays are zero based. in first try, move directly. */
    into_in += (into_in == 0)?first:first+1;
    copy_ft(in_current, in, into_in, len_in);
    len_current = strlen(in_current);
    
    /* found */
    if (find_full_helper(in_current, pat))
      return into_in;
    /* if not, examine updated substring from where old try failed */
  }
}

/* only support function. find full pattern pat in string in, but only */
/* after first occurrance of first character in pat in position in[0] */
/* return boolean 1 if found, or -1 if not */
int find_full_helper(char* in, char* pat){

  int len_in, len_pat;
  len_in = strlen(in);
  len_pat = strlen(pat);

  if (len_in < len_pat || len_in == 0 || len_pat == 0)
    return -1;

  int i = 1;
  while ( (in[i] == pat[i]) && i <= len_pat )
    ++i;

  return (i == len_pat)?1:-1;
}

/* find first character c in string in after position pos */
int find_first_after(char* in, char c, int pos){

  pos++;
  if ( (pos >= strlen(in)) ||(strlen(in) == 0) || (pos < 0))
    return -1;

  int len = strlen(in);

  while ( (pos < len) && (in[pos] != c) )
    ++pos;

  return (pos == len)?-1:pos;
}

/* copies from to to, beginning at start in from, ending at end in from */
/* no error checking: proper sizes assumed in arguments */
void copy_ft(char* to, char* from, int start, int end){

  if (end < start)
    return;

  int i, j=0;
  
  for (i=start; i <= end; i++)
    to[j++] = from[i];
  if (to[j-1] != '\n')
    to[j++] = '\n';
  to[j] = '\0';
}

int my_getline(char* s, int lim){

  int c, i;

  for (i=0; i < lim-1 && ( c = getchar()) != EOF && c!= '\n'; i++)
    s[i] = c;
  if ( c == '\n'){
    s[i] = c;
    ++i;
  }
  s[i] = '\0';

  return i;

}

/* truncates empty space at end, adding one '\n' instead */
void truncate(char* a){

  int i, stop;
  stop = strlen(a); /* position of trailing '\0' */

  if (stop-- > 0){
    while (stop >= 0 && ( a[stop] == ' ' || a[stop] == '\t' || a[stop] == '\n'))
      --stop;
  }

  a[++stop] = '\n';
  a[++stop] = '\0';

}
