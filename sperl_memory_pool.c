#include <stdlib.h>
#include <math.h>

#include "sperl_memory_pool.h"
#include "sperl_memory_pool_page.h"

SPerl_MEMORY_POOL* SPerl_MEMORY_POOL_new(SPerl_int base_capacity) {
  SPerl_MEMORY_POOL* memory_pool = (SPerl_MEMORY_POOL*)calloc(1, sizeof(SPerl_MEMORY_POOL));
  if (base_capacity == 0) {
    memory_pool->base_capacity = 4096;
  }
  else {
    SPerl_int rem = base_capacity % 4;
    if (rem != 0) {
      base_capacity = base_capacity - rem + 4;
    }
    memory_pool->base_capacity = base_capacity;
  }
  
  SPerl_MEMORY_POOL_PAGE* page = (SPerl_MEMORY_POOL_PAGE*)SPerl_MEMORY_POOL_PAGE_new();
  page->data = calloc(memory_pool->base_capacity, sizeof(SPerl_uchar));
  memory_pool->page = page;
  memory_pool->page_depth = 1;
  
  return memory_pool;
}

void* SPerl_MEMORY_POOL_alloc(SPerl_MEMORY_POOL* memory_pool, SPerl_int block_size) {
  
  SPerl_int page_depth = memory_pool->page_depth;
  SPerl_int current_pos = memory_pool->current_pos;
  SPerl_int base_capacity = memory_pool->base_capacity;
  
  SPerl_int block_size_rem = block_size % 4;
  if (block_size_rem != 0) {
    block_size = block_size - block_size_rem + 4;
  }
  
  // Calculate capacity
  SPerl_int current_capacity = base_capacity * pow(2, page_depth - 1);

  // Create next memroy page
  SPerl_uchar* data_ptr;
  SPerl_uchar* page;
  if (current_pos + block_size > current_capacity) {
    page_depth++;
    current_pos = 0;
    
    SPerl_int new_capacity = current_capacity * 2;
    SPerl_MEMORY_POOL_PAGE* new_page = (SPerl_MEMORY_POOL_PAGE*)SPerl_MEMORY_POOL_PAGE_new();
    new_page->data = (SPerl_uchar*)calloc(new_capacity, sizeof(SPerl_uchar));
    
    new_page->next = memory_pool->page;
    memory_pool->page = new_page;
    memory_pool->page_depth = page_depth;
  }

  data_ptr = memory_pool->page->data + current_pos;
  
  memory_pool->current_pos = current_pos + block_size;
  
  return data_ptr;
}

void SPerl_MEMORY_POOL_free(SPerl_MEMORY_POOL* memory_pool) {
  
  SPerl_MEMORY_POOL_PAGE* next_page = memory_pool->page;
  
  while (next_page) {
    SPerl_MEMORY_POOL_PAGE* tmp_page = next_page->next;
    SPerl_MEMORY_POOL_PAGE_free(next_page);
    next_page = tmp_page;
  }
  
  free(memory_pool);
}
