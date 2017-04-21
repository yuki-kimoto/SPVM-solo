#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "sperl.h"
#include "sperl_allocator_runtime.h"
#include "sperl_allocator_util.h"
#include "sperl_memory_pool.h"
#include "sperl_array.h"

SPerl_ALLOCATOR_RUNTIME* SPerl_ALLOCATOR_RUNTIME_new(SPerl* sperl) {
  SPerl_ALLOCATOR_RUNTIME* allocator = SPerl_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPerl_ALLOCATOR_RUNTIME));
  
  // Memory pool
  allocator->memory_pool = SPerl_MEMORY_POOL_new(sperl, 0);
  
  // Free lists
  allocator->freelists = SPerl_ALLOCATOR_UTIL_safe_malloc_i32(16, sizeof(SPerl_ARRAY));
  
  // Initialize free list
  for (int32_t i = 0; i < 16; i++) {
    allocator->freelists[i] = SPerl_ARRAY_new(sperl, 0);
  }
  
  return allocator;
}

int32_t SPerl_ALLOCATOR_RUNTIME_get_freelist_index(SPerl* sperl, SPerl_ALLOCATOR_RUNTIME* allocator, int64_t size) {
  (void)sperl;
  (void)allocator;
  
  assert(size > 0);
  
  // To 2 ^ n
  // This algorizm is from http://ideone.com/EStSRd
  int64_t N = size;
  int64_t _N1 = N-1;
  int64_t _N2 = _N1 | (_N1 >>  1);
  int64_t _N3 = _N2 | (_N2 >>  2);
  int64_t _N4 = _N3 | (_N3 >>  4);
  int64_t _N5 = _N4 | (_N4 >>  8);
  int64_t _N6 = _N5 | (_N5 >> 16);
  int64_t _N7 = _N6 | (_N6 >> 32);
  int64_t Value = _N7 + 1;
  
  // Category
  int64_t div_size = Value;
  int32_t index = 0;
  while (1) {
    div_size = div_size / 2;
    if (div_size == 0) {
      break;
    }
    else {
      index++;
    }
  }
  
  return index;
}

void* SPerl_ALLOCATOR_RUNTIME_alloc(SPerl* sperl, SPerl_ALLOCATOR_RUNTIME* allocator, int64_t size) {
  (void) sperl;
  
  assert(size > 0);
  
  void* block;
  if (size > 0xFFFF) {
    block = SPerl_ALLOCATOR_UTIL_safe_malloc_i64(1, size);
  }
  else {
    int32_t index = SPerl_ALLOCATOR_RUNTIME_get_freelist_index(sperl, allocator, size);
    
    // Allocate size
    int32_t allocate_size = (int32_t)pow(2, index);
    
    block = SPerl_MEMORY_POOL_alloc(sperl, allocator->memory_pool, allocate_size);
  }
  
  return block;
}

void SPerl_ALLOCATOR_RUNTIME_free(SPerl* sperl, SPerl_ALLOCATOR_RUNTIME* allocator) {
  // Free memory pool */
  SPerl_MEMORY_POOL_free(sperl, allocator->memory_pool);
  
  free(allocator->freelists);
  
  free(allocator);
}
