#include <stdio.h>



int main(){

  char s[] = "0x00009////1a/1965";
  char fmt[] = "%x////%x/%x";
  int rv, d, m, y;

  sscanf(s, fmt, &d, &m, &y);
  rv = printf("format string: %s\n", fmt);
  printf("string: %s, assigned = %d, d = %x, m = %x, y = %x\n", 
	 s, rv, d, m, y);

  return 0;
}
/*
format: (0) Much less error checking. Assigns non-sensical values, no report
        (1) scan fmt for first %. all chars before must match in s 
            (except skip leading ws in both)
        (2) read in admissable format chars until (3). No ws.       
        (3) end = position in fmt after reading this set of format 
            instructions. sep_next = fmt[end+1], or -1 if fmt[end+1] == '\0'.
        (3) (a) if sep_next = '%', of form '%d%d' or '%%'; if '%%', 
            this is last read in. Parse s to first char not admissible 
            (could be trailing ws).  
        (4) match in s:
            - leading ws ok, but not in or after number
            - get lead-in (eg, 0x) as necessary 
            - read numbers in current base             
            - stop reading at earlier of (a) '\0', (b) ws, (c) inadmissible, 
              (d) sep_next. In (a) - (c), next read in fails; last read. 
        (5) next in fmt: match with string until earlier
            of (a) '\0', and (b) %; skip ws in both. Go to (2).
        (6) until no more % left in fmt.
        (7) if there is a sep_next <=> ws, ws after end of one read-in
            and before sep_next (if present in both fmt and s) is skipped
	(8) implement: 
            between format read ins...
            s: disregard isspace
            fmt: disregard ' ' and '\t'
            in format read ins/getting its value:
            s: stop at first isspace (is not equal nsep, last read in)
            fmt: no spaces allowed at all.

useful vars:

next_sep = '/'
next_percent = j (-1 if no more)

*/ 
