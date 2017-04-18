#include <stdlib.h>
#include <stdio.h>

#include "sperl.h"
#include "sperl_allocator_runtime.h"
#include "sperl_allocator_util.h"

SPerl_ALLOCATOR_RUNTIME* SPerl_ALLOCATOR_RUNTIME_new(SPerl* sperl) {
  SPerl_ALLOCATOR_RUNTIME* allocator = malloc(sizeof(SPerl_ALLOCATOR_RUNTIME));
  
  // Memory pool - memory pool save short strings and object, except array and hash
  // These datas are created at compile time
  allocator->memory_pool = SPerl_MEMORY_POOL_new(sperl, 0);
  
  return allocator;
}

void* SPerl_ALLOCATOR_RUNTIME_alloc(SPerl* sperl, SPerl_ALLOCATOR_RUNTIME* allocator, int64_t size) {
  (void) sperl;
  
  void* block;
  if (size > 0xFF) {
    block = SPerl_ALLOCATOR_UTIL_safe_malloc_i64(1, size);
  }
  else {
    block = SPerl_MEMORY_POOL_alloc(sperl, allocator->memory_pool, size);
  }
  
  return block;
}

void SPerl_ALLOCATOR_RUNTIME_free(SPerl* sperl, SPerl_ALLOCATOR_RUNTIME* allocator) {
  // Free memory pool */
  SPerl_MEMORY_POOL_free(sperl, allocator->memory_pool);
  
  free(allocator);
}
