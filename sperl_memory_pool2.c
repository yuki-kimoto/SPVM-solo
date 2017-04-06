#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_memory_pool2.h"
#include "sperl_memory_pool_page.h"
#include "sperl_allocator_util.h"

SPerl_MEMORY_POOL2* SPerl_MEMORY_POOL2_new(SPerl* sperl, int64_t page_byte_size) {
  (void)sperl;
  
  SPerl_MEMORY_POOL2* memory_pool = (SPerl_MEMORY_POOL2*) SPerl_ALLOCATOR_UTIL_safe_malloc_zero(1, sizeof(SPerl_MEMORY_POOL2));
  
  if (page_byte_size == 0) {
    memory_pool->page_byte_size = 0xFF;
  }
  else {
    memory_pool->page_byte_size = page_byte_size;
  }
  
  SPerl_MEMORY_POOL_PAGE* page = (SPerl_MEMORY_POOL_PAGE*)SPerl_MEMORY_POOL_PAGE_new(sperl);
  page->data = (uint8_t*) SPerl_ALLOCATOR_UTIL_safe_malloc_zero(memory_pool->page_byte_size, sizeof(uint8_t));
  memory_pool->page = page;
  memory_pool->pages_length = 1;
  
  return memory_pool;
}

void* SPerl_MEMORY_POOL2_alloc(SPerl* sperl, SPerl_MEMORY_POOL2* memory_pool, int64_t byte_size) {
  (void)sperl;
  
  assert(byte_size > 0);
  
  int64_t pages_length = memory_pool->pages_length;
  int64_t current_pos = memory_pool->current_pos;
  int64_t page_byte_size = memory_pool->page_byte_size;
  
  // Adjust alignment
  int64_t aligned_byte_size = (byte_size - 1) + (8 - ((byte_size - 1) % sperl->alignment));
  
  // Calculate capacity
  int64_t current_capacity = page_byte_size * pow(2, pages_length - 1);

  // Create next memory page
  uint8_t* data_ptr;
  if (current_pos + aligned_byte_size > current_capacity) {
    pages_length++;
    current_pos = 0;
    
    int64_t new_capacity = current_capacity * 2;
    SPerl_MEMORY_POOL_PAGE* new_page = (SPerl_MEMORY_POOL_PAGE*)SPerl_MEMORY_POOL_PAGE_new(sperl);
    new_page->data = (uint8_t*) SPerl_ALLOCATOR_UTIL_safe_malloc_zero(new_capacity, sizeof(uint8_t));
    
    new_page->next = memory_pool->page;
    memory_pool->page = new_page;
    memory_pool->pages_length = pages_length;
  }

  data_ptr = memory_pool->page->data + current_pos;
  
  memory_pool->current_pos = current_pos + aligned_byte_size;
  
  return data_ptr;
}

void SPerl_MEMORY_POOL2_free(SPerl* sperl, SPerl_MEMORY_POOL2* memory_pool) {
  (void)sperl;
  
  free(memory_pool);
}
