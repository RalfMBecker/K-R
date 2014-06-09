#include <unistd.h> // recommended for sbrk
#include "alloc.h"

#define NALLOC 1024

static Header* morecore(unsigned);
static Header base;
static Header *freep = NULL;

// p: pointer into n consecutive chars to be added to free list
// note that only Header-sized blocks can be added, so might have 
// up to size(Header)-1 elements e at end spare.
// returns -1 on error, number e of unallocated spares else
int mybfree(char* p, int n){

  if ( p == NULL || n <= 0 )
    return -1;

  int blocks, spare;
  Header* ps;

  blocks = n/sizeof(Header);
  spare = n%sizeof(Header);
  ps = (Header*) p;
  ps->s.size = blocks;
  my_free( (void*) ps ); // this adjusts freep 

  return spare;
}

void* mycalloc(unsigned times, unsigned nbytes){

  Header *p, *prevp;
  unsigned nunits;
  int nullout, i;
  char* pnullout;
  //  char c = 0;

  nunits = (times*(nbytes+sizeof(Header))-1)/sizeof(Header) + 1;
  nullout = nunits*sizeof(Header);

  if ( (prevp = freep) == NULL){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }

  for ( p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if ( p->s.size >= nunits){
      if ( p->s.size == nunits )
	prevp->s.ptr = p->s.ptr;
      else{
	p->s.size -= nunits;
	p+= p->s.size;
	p->s.size = nunits;
      }
      freep = prevp;
      pnullout = (char*) freep;
      for (i = 0; i < nullout; i++)
	pnullout[i] = 0;
      return (void*) (p+1);
    }
    if (p == freep)
      if ( ( p = morecore(nunits) ) == NULL )
	return NULL;
  }

}

void* mymalloc(unsigned nbytes){

  Header *p, *prevp;
  unsigned nunits;

  nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;

  if ( (prevp = freep) == NULL){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }

  for ( p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if ( p->s.size >= nunits){
      if ( p->s.size == nunits )
	prevp->s.ptr = p->s.ptr;
      else{
	p->s.size -= nunits;
	p+= p->s.size;
	p->s.size = nunits;
      }
      freep = prevp;
      return (void*) (p+1);
    }
    if (p == freep)
      if ( ( p = morecore(nunits) ) == NULL )
	return NULL;
  }

}

static Header* morecore(unsigned nu){

  char *cp; 
  //  char* sbrk(int);
  Header* up;

  if (nu < NALLOC)
    nu = NALLOC;
  cp = sbrk( nu * sizeof(Header) );
  if ( cp == (char*) -1 )  // return void* pointer these days
    return NULL;
  up = (Header*) cp;
  up->s.size = nu;
  my_free( (void*) up ); // the +1 in KR is unnecessary as voided in free()
  return freep;
}

// note that the program has a loophole we correct.
// however, as sbrk appears to allocate in increasing memory order,
// the loophole would not matter
// code review Sep 12, 2013: nope. This was based on a mis-reading of KR's
//                           code - see documentation in book.
//                           However, the adjusted code does not hurt.
void my_free(void* ap){

  Header *bp, *p;

  bp = (Header*) ap; // changed from KR in conjunction with change in morecore
  // determine relative location of new block
  for ( p = freep; !( bp > p && bp < p->s.ptr); p = p->s.ptr)
    if ( p >= p->s.ptr && ( bp > p || bp < p->s.ptr) )
      break;

  /*
  // added by me to close loophole described in book (closing case (3))
  if ( bp < p ){
    if ( bp + bp->s.size == p ){
      bp->s.size += p->s.size;
      bp->s.ptr = p->s.ptr;
    }
    else
      bp->s.ptr = p;
    while ( p->s.ptr != freep)
      p = p->s.ptr;
    p->s.ptr = bp;
    freep = bp;
    return;
  }
  // end code added by me
  */

  if ( bp + bp->s.size == p->s.ptr ){
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  }
  else
    bp->s.ptr = p->s.ptr;
  if ( p + p->s.size == bp){
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  }
  else
    p->s.ptr = bp;
  freep = p;

}
