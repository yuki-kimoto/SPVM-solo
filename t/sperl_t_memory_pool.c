#include <stdio.h>

#include "../sperl.h"
#include "../sperl_memory_pool.h"
#include "../sperl_memory_pool_page.h"

#define OK(condition) \
  if (condition) {\
    printf("OK\n");\
  }\
  else {\
    printf("Not OK at line %d\n", __LINE__);\
  }

struct test {
  int32_t a;
  double b;
  int64_t c;
};

int main()
{
  SPerl* sperl = SPerl_new();
  
  // new and free
  {
    SPerl_MEMORY_POOL* memory_pool = SPerl_MEMORY_POOL_new(sperl, 0);
    
    int32_t* int_ptr1 = (int32_t*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int32_t));
    int32_t* int_ptr2 = (int32_t*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int32_t));
    
    *int_ptr1 = 1;
    *int_ptr2 = 2;
    
    OK(*int_ptr1 == ((int32_t*)memory_pool->page->data)[0]);
    OK(*int_ptr2 == ((int32_t*)memory_pool->page->data)[1]);
    
    SPerl_MEMORY_POOL_free(sperl, memory_pool);
  }

  // default values
  {
    SPerl_MEMORY_POOL* memory_pool = SPerl_MEMORY_POOL_new(sperl, sizeof(struct test));
    
    struct test* int_ptr1 = (struct test*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(struct test));
    struct test* int_ptr2 = (struct test*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(struct test));
    struct test* int_ptr3 = (struct test*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(struct test));
    struct test* int_ptr4 = (struct test*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(struct test));
    struct test* int_ptr5 = (struct test*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(struct test));
    struct test* int_ptr6 = (struct test*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(struct test));
    struct test* int_ptr7 = (struct test*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(struct test));
    struct test* int_ptr8 = (struct test*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(struct test));

    OK(int_ptr1->a == 0);
    OK(int_ptr2->a == 0);
    OK(int_ptr3->a == 0);
    OK(int_ptr4->a == 0);
    OK(int_ptr5->a == 0);
    OK(int_ptr6->a == 0);
    OK(int_ptr7->a == 0);
    OK(int_ptr8->a == 0);
    OK(int_ptr1->b == 0);
    OK(int_ptr2->b == 0);
    OK(int_ptr3->b == 0);
    OK(int_ptr4->b == 0);
    OK(int_ptr5->b == 0);
    OK(int_ptr6->b == 0);
    OK(int_ptr7->b == 0);
    OK(int_ptr8->b == 0);
    OK(int_ptr1->b == 0);
    OK(int_ptr2->b == 0);
    OK(int_ptr3->b == 0);
    OK(int_ptr4->b == 0);
    OK(int_ptr5->b == 0);
    OK(int_ptr6->b == 0);
    OK(int_ptr7->b == 0);
    OK(int_ptr8->b == 0);
  }
  
  // Create next memory node
  {
    SPerl_MEMORY_POOL* memory_pool = SPerl_MEMORY_POOL_new(sperl, sizeof(int32_t));
    
    int32_t* int_ptr1 = (int32_t*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int32_t));
    int32_t* int_ptr2 = (int32_t*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int32_t));
    int32_t* int_ptr3 = (int32_t*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int32_t));
    int32_t* int_ptr4 = (int32_t*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int32_t));
    int32_t* int_ptr5 = (int32_t*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int32_t));
    int32_t* int_ptr6 = (int32_t*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int32_t));
    int32_t* int_ptr7 = (int32_t*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int32_t));
    int32_t* int_ptr8 = (int32_t*)SPerl_MEMORY_POOL_alloc(sperl, memory_pool, sizeof(int32_t));

    OK(*int_ptr1 == 0);
    OK(*int_ptr2 == 0);
    OK(*int_ptr3 == 0);
    OK(*int_ptr4 == 0);
    OK(*int_ptr5 == 0);
    OK(*int_ptr6 == 0);
    OK(*int_ptr7 == 0);
    OK(*int_ptr8 == 0);
    
    *int_ptr1 = 1;
    *int_ptr2 = 2;
    *int_ptr3 = 3;
    *int_ptr4 = 4;
    *int_ptr5 = 5;
    *int_ptr6 = 6;
    *int_ptr7 = 7;
    *int_ptr8 = 8;
    
    OK(*int_ptr1 == ((int32_t*)memory_pool->page->next->next->next->data)[0]);
    OK(*int_ptr2 == ((int32_t*)memory_pool->page->next->next->data)[0]);
    OK(*int_ptr3 == ((int32_t*)memory_pool->page->next->next->data)[1]);
    OK(*int_ptr4 == ((int32_t*)memory_pool->page->next->data)[0]);
    OK(*int_ptr5 == ((int32_t*)memory_pool->page->next->data)[1]);
    OK(*int_ptr6 == ((int32_t*)memory_pool->page->next->data)[2]);
    OK(*int_ptr7 == ((int32_t*)memory_pool->page->next->data)[3]);
    OK(*int_ptr8 == ((int32_t*)memory_pool->page->data)[0]);
    
    SPerl_MEMORY_POOL_free(sperl, memory_pool);
  }
  
  return 0;
}
