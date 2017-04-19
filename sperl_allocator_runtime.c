#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "sperl.h"
#include "sperl_allocator_runtime.h"
#include "sperl_allocator_util.h"
#include "sperl_memory_pool.h"
#include "sperl_freelist.h"

SPerl_ALLOCATOR_RUNTIME* SPerl_ALLOCATOR_RUNTIME_new(SPerl* sperl) {
  SPerl_ALLOCATOR_RUNTIME* allocator = SPerl_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPerl_ALLOCATOR_RUNTIME));
  
  // Memory pool
  allocator->memory_pool = SPerl_MEMORY_POOL_new(sperl, 0);
  
  // Free addresses list
  allocator->freelists = SPerl_ALLOCATOR_UTIL_safe_malloc_i32(16, sizeof(SPerl_FREELIST));
  memset(allocator->freelists, 0, 16 * sizeof(SPerl_FREELIST));
  
  return allocator;
}

void* SPerl_ALLOCATOR_RUNTIME_alloc(SPerl* sperl, SPerl_ALLOCATOR_RUNTIME* allocator, int64_t size) {
  (void) sperl;
  
  assert(size > 0);
  
  void* block;
  if (size > 0xFFFF) {
    block = SPerl_ALLOCATOR_UTIL_safe_malloc_i64(1, size);
  }
  else {
    
    // To 2 ^ n
    // This algorizm is from http://ideone.com/EStSRd
    int32_t N = size;
    int32_t _N1 = N-1;
    int32_t _N2 = _N1 | (_N1 >>  1);
    int32_t _N3 = _N2 | (_N2 >>  2);
    int32_t _N4 = _N3 | (_N3 >>  4);
    int32_t _N5 = _N4 | (_N4 >>  8);
    int32_t _N6 = _N5 | (_N5 >> 16);
    int32_t _N7 = _N6 | (_N6 >> 32);
    int32_t Value = _N7 + 1;
    
    // Allocate size
    int32_t allocate_size = Value;
    
    // Category
    int32_t div_size = allocate_size;
    int32_t category = 0;
    while (1) {
      div_size = div_size / 2;
      if (div_size == 0) {
        break;
      }
      else {
        category++;
      }
    }
    
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
