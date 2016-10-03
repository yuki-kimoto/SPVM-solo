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

  // Crete next memroy node
  {
    SPerl_ALLOCATOR* allocator = SPerl_ALLOCATOR_new(sizeof(SPerl_int), 1);
    
    SPerl_int* int_ptr1 = (SPerl_int*)SPerl_ALLOCATOR_alloc(allocator);
    SPerl_int* int_ptr2 = (SPerl_int*)SPerl_ALLOCATOR_alloc(allocator);
    SPerl_int* int_ptr3 = (SPerl_int*)SPerl_ALLOCATOR_alloc(allocator);
    SPerl_int* int_ptr4 = (SPerl_int*)SPerl_ALLOCATOR_alloc(allocator);
    SPerl_int* int_ptr5 = (SPerl_int*)SPerl_ALLOCATOR_alloc(allocator);
    SPerl_int* int_ptr6 = (SPerl_int*)SPerl_ALLOCATOR_alloc(allocator);
    SPerl_int* int_ptr7 = (SPerl_int*)SPerl_ALLOCATOR_alloc(allocator);
    SPerl_int* int_ptr8 = (SPerl_int*)SPerl_ALLOCATOR_alloc(allocator);
    
    *int_ptr1 = 1;
    *int_ptr2 = 2;
    *int_ptr3 = 3;
    *int_ptr4 = 4;
    *int_ptr5 = 5;
    *int_ptr6 = 6;
    *int_ptr7 = 7;
    *int_ptr8 = 8;
    
    void* alloc_ptr1 = SPerl_ARRAY_fetch(allocator->memory_nodes, 0);
    void* alloc_ptr2 = SPerl_ARRAY_fetch(allocator->memory_nodes, 1);
    void* alloc_ptr3 = SPerl_ARRAY_fetch(allocator->memory_nodes, 2);
    void* alloc_ptr4 = SPerl_ARRAY_fetch(allocator->memory_nodes, 3);
    
    OK(*int_ptr1 == ((SPerl_int*)alloc_ptr1)[0]);
    OK(*int_ptr2 == ((SPerl_int*)alloc_ptr2)[0]);
    OK(*int_ptr3 == ((SPerl_int*)alloc_ptr2)[1]);
    OK(*int_ptr4 == ((SPerl_int*)alloc_ptr3)[0]);
    OK(*int_ptr5 == ((SPerl_int*)alloc_ptr3)[1]);
    OK(*int_ptr6 == ((SPerl_int*)alloc_ptr3)[2]);
    OK(*int_ptr7 == ((SPerl_int*)alloc_ptr3)[3]);
    OK(*int_ptr8 == ((SPerl_int*)alloc_ptr4)[0]);

    SPerl_ALLOCATOR_free(allocator);
  }
}
