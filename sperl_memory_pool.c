#include <stdlib.h>
#include <math.h>

#include "sperl.h"
#include "sperl_memory_pool.h"
#include "sperl_memory_pool_page.h"
#include "sperl_allocator.h"

SPerl_MEMORY_POOL* SPerl_MEMORY_POOL_new(SPerl* sperl) {
  (void)sperl;
  
  SPerl_MEMORY_POOL* memory_pool = (SPerl_MEMORY_POOL*) SPerl_ALLOCATOR_safe_malloc_zero(sperl, 1, sizeof(SPerl_MEMORY_POOL));
  memory_pool->base_capacity = 0xFF;
  
  SPerl_MEMORY_POOL_PAGE* page = (SPerl_MEMORY_POOL_PAGE*)SPerl_MEMORY_POOL_PAGE_new(sperl);
  page->data = (uint8_t*) SPerl_ALLOCATOR_safe_malloc_zero(sperl, memory_pool->base_capacity, sizeof(uint8_t));
  memory_pool->page = page;
  memory_pool->page_depth = 1;
  
  return memory_pool;
}

void* SPerl_MEMORY_POOL_alloc(SPerl* sperl, SPerl_MEMORY_POOL* memory_pool, int32_t block_size) {
  (void)sperl;
  
  int32_t page_depth = memory_pool->page_depth;
  int32_t current_pos = memory_pool->current_pos;
  int32_t base_capacity = memory_pool->base_capacity;
  
  int32_t block_size_rem = block_size % 4;
  if (block_size_rem != 0) {
    block_size = block_size - block_size_rem + 4;
  }
  
  // Calculate capacity
  int32_t current_capacity = base_capacity * pow(2, page_depth - 1);

  // Create next memory page
  uint8_t* data_ptr;
  if (current_pos + block_size > current_capacity) {
    page_depth++;
    current_pos = 0;
    
    int32_t new_capacity = current_capacity * 2;
    SPerl_MEMORY_POOL_PAGE* new_page = (SPerl_MEMORY_POOL_PAGE*)SPerl_MEMORY_POOL_PAGE_new(sperl);
    new_page->data = (uint8_t*) SPerl_ALLOCATOR_safe_malloc_zero(sperl, new_capacity, sizeof(uint8_t));
    
    new_page->next = memory_pool->page;
    memory_pool->page = new_page;
    memory_pool->page_depth = page_depth;
  }

  data_ptr = memory_pool->page->data + current_pos;
  
  memory_pool->current_pos = current_pos + block_size;
  
  return data_ptr;
}

void SPerl_MEMORY_POOL_free(SPerl* sperl, SPerl_MEMORY_POOL* memory_pool) {
  (void)sperl;
  
  SPerl_MEMORY_POOL_PAGE* next_page = memory_pool->page;
  
  while (next_page) {
    SPerl_MEMORY_POOL_PAGE* tmp_page = next_page->next;
    SPerl_MEMORY_POOL_PAGE_free(sperl, next_page);
    next_page = tmp_page;
  }
  
  free(memory_pool);
}
