/* program as requested, except does not handle escape sequences */

#include <stdio.h>
#include <string.h>

#define MAXLINE 200
#define MAXROW 200

enum states { OUT=0, IN };
/* external should be placed into main(). Kept here for easier view. */
int flag_quotes_double = OUT;
int quotes_double_x, quotes_double_y;
int flag_quotes_single = OUT;
int flag_comment = OUT;
int comment_x, comment_y;
int curly_in; /* all brackets checks only match, no consistency inside */
int curly_x[MAXLINE]; /* max number of such errors tracked */
int curly_y[MAXLINE];
int round_in;
int round_x[MAXLINE]; 
int round_y[MAXLINE];
int square_in;
int square_x[MAXLINE];
int square_y[MAXLINE];

int my_getline(char*, int);
void truncate(char*);
void copy_tf(char*, char*, int, int);
int is_escape(char*, int);

int main(){

  char line[MAXLINE];
  char escape_check[10];
  char ct, ct_prior, ct_temp;
  int row, column, i;
  int escape_length, len, len_esc, end;

  curly_in = round_in = square_in = -1;
  row = -1;
  ct = ct_prior = ct_temp = 0;
  flag_comment = flag_quotes_double = flag_quotes_single = OUT;
  while ( (++row < MAXROW) && my_getline(line, MAXLINE) > 0){
    truncate(line); /* Note: now every line terminates with '\n' '\0' */
    printf("%s", line);
    column = 0;
    while ( (ct = line[column]) != EOF && ct != '\n'){
      switch(ct){

      case '\"':
	if ( flag_comment == IN ){
	  ++column;
	  break;
	}
	flag_quotes_double = ++flag_quotes_double%2;
	quotes_double_x = row;
	quotes_double_y = column;
	++column;
	printf("double = %d\n", flag_quotes_double);
	break;

      case '\'': /* flag only used to check escape sequences */
	end = column+1;
	ct_temp = line[end];

	if ( flag_comment == IN ){
	  ++column;
	  break;
	}
	/* as we check escape sequences separately (\ before ');and a closing */
	/* '\'' only after an escape sequence, and as further in this case. */
	else if ( (flag_quotes_double == IN) || (ct_temp =='\n') ){
	  printf("Error: Stray '\'' in row:%d, column:%d\n", row+1, column);
	  ++column;
	  break;
	}

	if (ct_temp == '\"'){
	  printf("Error: Stray \' in row:%d, column:%d\n", row+1, column);
	  ++column;
	  break;
	}
	else if (ct_temp == '\\'){
	  flag_quotes_single = IN;
	  ++column;
	  printf("single = %d\n", flag_quotes_single);
	  break;
	}
	else{ 
	  end = column+2;
	  if (line[end] != '\''){
	    printf("Error: check \' in row:%d, column:%d\n", row+1, column);
	    ++column;
	  }
	  else{ /* identified proper char const with 1 letter, say */
	    column += 3;
	    printf("Proper char const(2) found at row:%d, column:%d\n", 
		   row+1, column);
	  }
	  break;
	}

      case('*'):
	if ( (ct_prior != '/') || (flag_comment == IN) || 
	     (flag_quotes_double == IN) ){
	  ++column;
	  break;
	}
	flag_comment = IN;
	comment_x = row;
	comment_y = column;
	++column;
	printf("comment = %d\n", flag_comment);
	break;

	case('/'):
	     if ( (ct_prior != '*') || (flag_quotes_double == IN) ||
		  (flag_quotes_single == IN) ){
	       ++column;
	       break;
	     }
	     else if (flag_comment == OUT){
	       printf("Error: */ without opening comment row:%d, column:%d\n", 
		      row+1, column);
	       column++;
	       break;
	     }
	     else{
	       flag_comment = OUT;
	       column++;
	       printf("comment = %d\n", flag_comment);
	       break;
	     }

      case '\\': /* note that escape sequences need only to be checked */
	/* within char constants, which cannot stretch across lines */
	if ( flag_comment == IN ){
	       ++column;
	       break;
	}
	else if ( (flag_quotes_single == OUT) && (flag_quotes_double == OUT) ){
	  printf("Error: \\ where it shouldn't be in row:%d, column:%d\n",
		 row+1, column);
	  ++column;
	  break;
	}
	
	len = strlen(line);
	if (len - column > 4){ /* note truncate() puts '/n' at end */
	  len_esc = 4;
	}
	else if (len - column > 2){
	  len_esc = 2;
	}
	else{
	  printf("Error: Stray \\ in row:%d, column:%d\n", row+1, column);
	  ++column;
	  break;
	}
	end = column+len_esc-1;
	copy_tf(escape_check, line, column, end);      

	escape_length = is_escape(escape_check, len_esc);
	printf("Escape length of:%d at row:%d, column:%d\n", 
	       escape_length, row+1, column);
	if (escape_length == -1){
	  ++column;
	  break;
	}
	else{
	  end = column+escape_length;
	  ct_temp = line[end];
	  if ( flag_quotes_double == IN ){
	    column = end;
	    break;
	  }
	  else{ /* if we come here, flag_quotes_single is IN */

	    if ( (ct_prior != '\'') || (ct_temp != '\'') ){
	      printf("Error: escape sequence outside character constant "
		     "after row:%d, column:%d\n", row+1, column);
	      column = end;
	      break;
	    }
	    else{
	      printf("Char constant at row:%d, column%d\n", row+1, column);
	      ++end;
	      column = end;
	      break;
	    }
	  }
	}

      case '{':
	if ( (flag_quotes_double == IN) || (flag_quotes_single == IN) ||
	     (flag_comment == IN) ){
	       ++column;
	       break;
	}
	++curly_in;
	curly_x[curly_in] = row;
	curly_y[curly_in] = column;
	++column;
	printf("curly: %d\n", curly_in);
	break;

      case '}':
	if ( (flag_quotes_double == IN) || (flag_comment == IN) ){
	  ++column;
	  break;
	}
	else if (curly_in < 0){
	  printf("Error: } without opening { in row:%d, column:%d\n", 
		      row+1, column);
	  break;
	}
	--curly_in;
	++column;
	printf("curly: %d\n", curly_in);
	break;

      case '(':
	if ( (flag_quotes_double == IN) || (flag_comment == IN) ){
	       ++column;
	       break;
	}
	++round_in;
	round_x[round_in] = row;
	round_y[round_in] = column;
	++column;
	printf("round: %d\n", round_in);
	break;
      
      case ')':
	if ( (flag_quotes_double == IN) || (flag_comment == IN) ){
	  ++column;
	  break;
	}
	else if (round_in < 0){
	  printf("Error: ) without opening ( in row:%d, column:%d\n", 
		      row+1, column);
	  ++column;
	  break;
	}
	--round_in;
	++column;
	printf("round: %d\n", round_in);
	break;

      case '[':
	if ( (flag_quotes_double == IN) || (flag_comment == IN) ){
	       ++column;
	       break;
	}
	++square_in;
	square_x[square_in] = row;
	square_y[square_in] = column;
	++column;
	printf("square: %d\n", square_in);
	break;
      
      case ']':
	if ( (flag_quotes_double == IN) || (flag_comment == IN) ){
	  ++column;
	  break;
	}
	else if (square_in < 0){
	  printf("Error: ] without opening [ in row:%d, column:%d\n", 
		      row+1, column);
	  ++column;
	  break;
	}
	--square_in;
	++column;
	printf("square: %d\n", square_in);
	break;
	
      default:
	++column;
	break;
      }
      ct_prior = ct;
    }

  }

  /* error reporting evident at end only following */
  if (flag_quotes_double)
    printf("Error: Unmatched \" in row:%d, column:%d\n", 
	   quotes_double_x+1, quotes_double_y);
  if (flag_comment)
    printf("Error: Unmatched /* in row:%d, column:%d\n",
	   comment_x+1, comment_y);
  if (curly_in >= 0){
    if (curly_in >= MAXLINE)
      printf("Over %d opening { with no matching }. Get a grip.\n", MAXLINE);
    else{
      for (i=0; i <= curly_in; ++i)
	printf("Error: { with no matching } in row:%d, column:%d\n",
	       curly_x[i], curly_y[i]);
    }
  }
  if (round_in >= 0){
    if (round_in >= MAXLINE)
      printf("Over %d opening ( with no matching ). Get a grip.\n", MAXLINE);
    else{
      for (i=0; i <= round_in; ++i)
	printf("Error: ( with no matching ) in row:%d, column:%d\n",
	       round_x[i], round_y[i]);
    }
  }
  if (square_in >= 0){
    if (square_in >= MAXLINE)
      printf("Over %d opening [ with no matching ]. Get a grip.\n", MAXLINE);
    else{
      for (i=0; i <= square_in; ++i)
	printf("Error: [ with no matching ] in row:%d, column:%d\n",
	       square_x[i], square_y[i]);
    }
  }

  return 0;

  }

/* a is called with len = 2 or len = 4 */
int is_escape(char* a, int len){

  int c1, c2, c3;

  switch(a[1]){

  case 'a':
  case 'b':
  case 'f':
  case 'n':
  case 'r':
  case 't':
  case 'v':
  case '\\':
  case '?':
  case '\'':
  case '\"':{
    len = 2;
    break;
  }

  case 'o':{ /* possibly comparing with garbage, but no matter */
    c1 = ( (a[2] < '9') && (a[2] >= '0' ) )?1:0;
    c2 = ( (a[3] < '9') && (a[3] >= '0') )?1:0;
    if (c1 && c2 && (len == 4))
      len = 4;
    else
      len = -1;
    break;
  }
  
  case 'x':{
    c1 = ( (a[2] <= '9') && (a[2] >= '0') )?1:0;
    c3 = ( (a[2] <= 'f') && (a[2] >= 'a') )?1:0;
    c1 = c1 || c3;
    c2 = ( (a[3] <= ('9' - '0')) && (a[3] >= '0') )?1:0;
    c3 = ( (a[3] <= ('f' - 'a')) && (a[3] >= 'a') )?1:0;
    c2 = c1 || c3;

    if (c1 && c2 && (len == 4))
      len = 4;
    else
      len = -1;
    break;
  }

  default:
    len = -1;
  }

  return len;
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

/* copies from to to, beginning at start in from, ending at end in from */
/* no error checking: proper sizes assumed in arguments */
void copy_tf(char* to, char* from, int start, int end){

  if (end < start)
    return;

  int i, j=0;
  
  for (i=start; i <= end; i++)
    to[j++] = from[i];
  if (to[j-1] != '\n')
    to[j++] = '\n';
  to[j] = '\0';
}

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
