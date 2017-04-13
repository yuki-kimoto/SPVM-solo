#include <stdio.h>
#include <assert.h>

#include "../sperl.h"
#include "../sperl_memory_pool.h"

#define OK(condition) \
  if (condition) {\
    printf("OK\n");\
  }\
  else {\
    printf("Not OK at line %d\n", __LINE__);\
  }

struct test {
  int64_t a;
  double b;
  int64_t c;
};

int main()
{
  SPerl* sperl = SPerl_new();
  
  // new and free
  {
    SPerl_MEMORY_POOL* memory_pool = SPerl_MEMORY_POOL_new(sperl, 0);
    
    int64_t* int_ptr1 = SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int64_t));
    int64_t* int_ptr2 = SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int64_t));
    
    *int_ptr1 = 1;
    *int_ptr2 = 2;
    
    OK((int8_t*)int_ptr1 == memory_pool->pages[0]);
    OK((int8_t*)int_ptr2 == memory_pool->pages[0] + sizeof(int64_t));
    OK(*(int64_t*)memory_pool->pages[0] == 1);
    OK(*(int64_t*)(memory_pool->pages[0] + sizeof(int64_t)) == 2);
    
    SPerl_MEMORY_POOL_free(sperl, memory_pool);
    
  }
  
  // Create next memory node
  {
    SPerl_MEMORY_POOL* memory_pool = SPerl_MEMORY_POOL_new(sperl, sizeof(int64_t) * 2);
    
    int64_t* int_ptr1 = SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int64_t));
    int64_t* int_ptr2 = SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int64_t));
    int64_t* int_ptr3 = SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int64_t));
    int64_t* int_ptr4 = SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int64_t));
    int64_t* int_ptr5 = SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int64_t));
    int64_t* int_ptr6 = SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int64_t));
    int64_t* int_ptr7 = SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int64_t));
    int64_t* int_ptr8 = SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int64_t));
    
    *int_ptr1 = 1;
    *int_ptr2 = 2;
    *int_ptr3 = 3;
    *int_ptr4 = 4;
    *int_ptr5 = 5;
    *int_ptr6 = 6;
    *int_ptr7 = 7;
    *int_ptr8 = 8;
    
    OK((int8_t*)int_ptr1 == memory_pool->pages[0]);
    OK((int8_t*)int_ptr2 == memory_pool->pages[0] + sizeof(int64_t));
    OK((int8_t*)int_ptr3 == memory_pool->pages[1]);
    OK((int8_t*)int_ptr4 == memory_pool->pages[1] + sizeof(int64_t));
    OK((int8_t*)int_ptr5 == memory_pool->pages[2]);
    OK((int8_t*)int_ptr6 == memory_pool->pages[2] + sizeof(int64_t));
    OK((int8_t*)int_ptr7 == memory_pool->pages[3]);
    OK((int8_t*)int_ptr8 == memory_pool->pages[3] + sizeof(int64_t));

    OK(*(int64_t*)memory_pool->pages[0]);
    OK(*(int64_t*)(memory_pool->pages[0] + sizeof(int64_t)));
    OK(*(int64_t*)memory_pool->pages[1]);
    OK(*(int64_t*)(memory_pool->pages[1] + sizeof(int64_t)));
    OK(*(int64_t*)memory_pool->pages[2]);
    OK(*(int64_t*)(memory_pool->pages[2] + sizeof(int64_t)));
    OK(*(int64_t*)memory_pool->pages[3]);
    OK(*(int64_t*)(memory_pool->pages[3] + sizeof(int64_t)));
    
    SPerl_MEMORY_POOL_free(sperl, memory_pool);
  }
  
  return 0;
}
