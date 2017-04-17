#include <stdlib.h>
#include <stdio.h>

#include "sperl.h"
#include "sperl_allocator_runtime.h"
#include "sperl_allocator_util.h"

void* SPerl_ALLOCATOR_RUNTIME_alloc(SPerl* sperl, size_t size) {
  (void) sperl;

  return SPerl_ALLOCATOR_UTIL_safe_malloc_i32(size, sizeof(char));
}

SPerl_ALLOCATOR_RUNTIME* SPerl_ALLOCATOR_RUNTIME_new(SPerl* sperl) {
  SPerl_ALLOCATOR_RUNTIME* allocator = malloc(sizeof(SPerl_ALLOCATOR_RUNTIME));
  
  // Memory pool - memory pool save short strings and object, except array and hash
  // These datas are created at compile time
  allocator->memory_pool = SPerl_MEMORY_POOL_new(sperl, 0);
  
  return allocator;
}

void* SPerl_ALLOCATOR_RUNTIME_alloc_memory_pool(SPerl* sperl, SPerl_ALLOCATOR_RUNTIME* allocator, int32_t size) {
  return SPerl_MEMORY_POOL_alloc(sperl, allocator->memory_pool, size);
}

void SPerl_ALLOCATOR_RUNTIME_free(SPerl* sperl, SPerl_ALLOCATOR_RUNTIME* allocator) {
  // Free memory pool */
  SPerl_MEMORY_POOL_free(sperl, allocator->memory_pool);
  
  free(allocator);
}
