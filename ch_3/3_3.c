#include <stdio.h>
#include <ctype.h>

#define MAXSTRING 500

void expand(const char*, char*);

int main(){

  const char a[] = "Test string for 0-27, H-C, or U-f5-2, or b-f-a "
    " other -c-h-.";
  char b[MAXSTRING];

  printf("%s\n", a);
  expand(a, b);
  printf("%s", b);

  return 0;
}

/* invalid syntax such as a-D is taken as literal input */
/* for simplicity, we expand 12-59 as 123459 */
void expand(const char* s1, char* s2){

  int i, j;
  int ct, ct_prior, next, first, second;

  ct = ct_prior = 0;
  for (i = j = 0; (ct = s1[i]) != '\0'; ++i){
    if (ct == '-'){
      next = i+1;

      /* stray - */
      if ( (!isalnum(ct_prior)) || (!isalnum(s1[next])) ){
	s2[j++] = ct;
	ct_prior = ct;
      }

      /* upper case range? */
      if (isupper(ct_prior)){
	if (!isupper(s1[next])){
	    s2[j++] = ct;
	    ct_prior = ct;
	}
	else{
	  first = s1[i-1];
	  second = s1[next];
	  if (first <= second){
	    while (++first <= second)
	      s2[j++] = first;
	  }
	  else{
	    while (--first >= second)
	      s2[j++] = first;
	    }
	  i++; /* another i counted up as part of loop */
	  ct_prior = s1[i];
	}

      } /* end of upper case */

      /* lower case range? */
      if (islower(ct_prior)){
	if (!islower(s1[next])){
	  s2[j++] = ct;
	  ct_prior = ct;
	}
	else{
	  first = s1[i-1];
	  second = s1[next];
	  if (first <= second){
	    while (++first <= second)
	      s2[j++] = first;
	  }
	  else{
	    while (--first >= second)
	      s2[j++] = first;
	  }
	  i++; /* another i counted up as part of loop */
	  ct_prior = s1[i];
	}
      } /* end of lower case */

      /* digit case range? */
      if (isdigit(ct_prior)){
	if (!isdigit(s1[next])){
	  s2[j++] = ct;
	  ct_prior = ct;
	}
	else{
	  first = s1[i-1];
	  second = s1[next];
	  if (first <= second){
	    while (++first <= second)
	      s2[j++] = first;
	  }
	  else{
	    while (--first >= second)
	      s2[j++] = first;
	  }
	  i++; /* another i counted up as part of loop */
	  ct_prior = s1[i];
	}
      } /* end of digit case */

    } /* end of - check, if clause */

    /* anything but - */
    else{
      s2[j++] = ct;
      ct_prior = ct;
    } 
  }

  s2[j++] = '\n';
  s2[j] = '\0';
}
