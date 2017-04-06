#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_memory_pool.h"
#include "sperl_memory_pool_page.h"
#include "sperl_allocator_util.h"

SPerl_MEMORY_POOL* SPerl_MEMORY_POOL_new(SPerl* sperl, int64_t base_capacity) {
  (void)sperl;
  
  SPerl_MEMORY_POOL* memory_pool = (SPerl_MEMORY_POOL*) SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_MEMORY_POOL));
  
  if (base_capacity == 0) {
    memory_pool->base_capacity = 0xFF;
  }
  else {
    memory_pool->base_capacity = base_capacity;
  }
  
  SPerl_MEMORY_POOL_PAGE* page = (SPerl_MEMORY_POOL_PAGE*)SPerl_MEMORY_POOL_PAGE_new(sperl);
  page->data = (uint8_t*) SPerl_ALLOCATOR_UTIL_safe_malloc_zero(memory_pool->base_capacity, sizeof(uint8_t));
  memory_pool->page = page;
  memory_pool->page_depth = 1;
  memory_pool->current_pos = 0;
  
  return memory_pool;
}

void* SPerl_MEMORY_POOL_alloc(SPerl* sperl, SPerl_MEMORY_POOL* memory_pool, int64_t byte_size) {
  (void)sperl;
  
  assert(byte_size > 0);
  
  int64_t page_depth = memory_pool->page_depth;
  int64_t current_pos = memory_pool->current_pos;
  int64_t base_capacity = memory_pool->base_capacity;
  
  // Adjust alignment
  int64_t aligned_byte_size = (byte_size - 1) + (8 - ((byte_size - 1) % sperl->alignment));
  
  // Calculate capacity
  int64_t current_capacity = base_capacity * pow(2, page_depth - 1);

  // Create next memory page
  uint8_t* data_ptr;
  if (current_pos + aligned_byte_size > current_capacity) {
    page_depth++;
    current_pos = 0;
    
    int64_t new_capacity = current_capacity * 2;
    SPerl_MEMORY_POOL_PAGE* new_page = (SPerl_MEMORY_POOL_PAGE*)SPerl_MEMORY_POOL_PAGE_new(sperl);
    new_page->data = (uint8_t*) SPerl_ALLOCATOR_UTIL_safe_malloc_zero(new_capacity, sizeof(uint8_t));
    
    new_page->next = memory_pool->page;
    memory_pool->page = new_page;
    memory_pool->page_depth = page_depth;
  }

  data_ptr = memory_pool->page->data + current_pos;
  
  memory_pool->current_pos = current_pos + aligned_byte_size;
  
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
