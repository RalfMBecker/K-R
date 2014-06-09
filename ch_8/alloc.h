typedef long Align;
   
union header{
  struct{
    union header* ptr;  
    unsigned size;
  } s;
  Align x;  
};
   
typedef union header Header;

void my_free(void*);
int mybfree(char*, int);
void* mycalloc(unsigned, unsigned);
void* mymalloc(unsigned);
//static Header* morecore(unsigned);
