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
int quotes_single_x, quotes_single_y;
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

int main(){

  char line[MAXLINE];
  char ct, ct_prior;
  int row, column, i;

  curly_in = round_in = square_in = -1;
  row = -1;
  ct = ct_prior = 0;
  while ( (++row < MAXROW) && my_getline(line, MAXLINE) > 0){
    column = 0;
    while ( (ct = line[column]) != EOF && ct != '\n'){
      switch(ct){

      case '\"':
	if ( (ct_prior == '\\') || (flag_quotes_single == IN) 
	     || (flag_comment == IN) ){
	  ++column;
	  break;
	}
	flag_quotes_double = ++flag_quotes_double%2;
	quotes_double_x = row;
	quotes_double_y = column;
	++column;
	break;

      case '\'':
	/* we do not check if what is within the quotes is ok, just matching */
	if ( (ct_prior == '\\') || (flag_quotes_double == IN) || 
	     (flag_comment == IN) ){
	  ++column;
	  break;
	}
	flag_quotes_single = ++flag_quotes_single%2;
	quotes_single_x = row;
	quotes_single_y = column;
	++column;
	break;

      case('*'):
	if ( (ct_prior != '/') || (flag_comment == IN) || 
	       (flag_quotes_double == IN) || (flag_quotes_single == IN) ){
	  ++column;
	  break;
	}
	flag_comment = IN;
	comment_x = row;
	comment_y = column;
	++column;
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
	       break;
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
	break;

      case '}':
	if ( (flag_quotes_double == IN) || (flag_quotes_single == IN) ||
	     (flag_comment == IN) ){
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
	break;

      case '(':
	if ( (flag_quotes_double == IN) || (flag_quotes_single == IN) ||
	     (flag_comment == IN) ){
	       ++column;
	       break;
	     }
	++round_in;
	round_x[round_in] = row;
	round_y[round_in] = column;
	++column;
	break;
      
      case ')':
	if ( (flag_quotes_double == IN) || (flag_quotes_single == IN) ||
	     (flag_comment == IN) ){
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
	break;

      case '[':
	if ( (flag_quotes_double == IN) || (flag_quotes_single == IN) ||
	     (flag_comment == IN) ){
	       ++column;
	       break;
	     }
	++square_in;
	square_x[square_in] = row;
	square_y[square_in] = column;
	++column;
	break;
      
      case ']':
	if ( (flag_quotes_double == IN) || (flag_quotes_single == IN) ||
	     (flag_comment == IN) ){
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
  if (flag_quotes_single)
    printf("Error: Unmatched \' in row:%d, column:%d\n",
	   quotes_single_x+1, quotes_single_y);
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
