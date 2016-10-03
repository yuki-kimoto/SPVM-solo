#include <stdio.h>

#include "../sperl_allocator.h"
#include "../sperl_array.h"

#define OK(condition) \
  if (condition) {\
    printf("OK\n");\
  }\
  else {\
    printf("Not OK at line %d\n", __LINE__);\
  }

int main(int argc, char *argv[])
{
  // new and free
  {
    SPerl_ALLOCATOR* allocator = SPerl_ALLOCATOR_new(sizeof(SPerl_int), 0);
    
    SPerl_int* int_ptr1 = (SPerl_int*)SPerl_ALLOCATOR_alloc(allocator);
    SPerl_int* int_ptr2 = (SPerl_int*)SPerl_ALLOCATOR_alloc(allocator);
    
    *int_ptr1 = 1;
    *int_ptr2 = 2;
    
    void* alloc_ptr = SPerl_ARRAY_fetch(allocator->memory_nodes, 0);
    
    OK(*int_ptr1 == ((SPerl_int*)alloc_ptr)[0]);
    OK(*int_ptr2 == ((SPerl_int*)alloc_ptr)[1]);
    
    SPerl_ALLOCATOR_free(allocator);
  }
}
