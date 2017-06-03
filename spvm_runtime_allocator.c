#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "spvm.h"
#include "spvm_runtime_allocator.h"
#include "spvm_runtime_api.h"
#include "spvm_util_allocator.h"
#include "spvm_memory_pool.h"
#include "spvm_array.h"
#include "spvm_runtime.h"
#include "spvm_ref.h"
#include "spvm_ref_string.h"
#include "spvm_ref_array.h"
#include "spvm_ref_object.h"
#include "spvm_constant_pool.h"
#include "spvm_constant_pool_package.h"

SPVM_RUNTIME_ALLOCATOR* SPVM_RUNTIME_ALLOCATOR_new(SPVM* spvm) {
  SPVM_RUNTIME_ALLOCATOR* allocator = SPVM_UTIL_ALLOCATOR_safe_malloc_i32(1, sizeof(SPVM_RUNTIME_ALLOCATOR));
  
  // Memory pool
  allocator->memory_pool = SPVM_MEMORY_POOL_new(spvm, 0);
  
  // Free lists
  allocator->freelists = SPVM_UTIL_ALLOCATOR_safe_malloc_i32(16, sizeof(SPVM_ARRAY));
  
  // Initialize free list
  for (int32_t i = 0; i < 16; i++) {
    allocator->freelists[i] = SPVM_ARRAY_new(spvm, 0);
  }
  
  // use memory pool max reference byte size
  allocator->ref_max_byte_size_use_memory_pool = 0xFFFF;
  
  return allocator;
}

inline int32_t SPVM_RUNTIME_ALLOCATOR_get_freelist_index(SPVM* spvm, SPVM_RUNTIME_ALLOCATOR* allocator, int64_t size) {
  (void)spvm;
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
  int32_t index = -1;
  while (1) {
    div_size = div_size / 2;
    if (div_size == 0) {
      break;
    }
    else {
      index++;
    }
  }
  assert(index >= 0);
  
  return index;
}

inline void* SPVM_RUNTIME_ALLOCATOR_malloc(SPVM* spvm, SPVM_RUNTIME_ALLOCATOR* allocator, int64_t size) {
  (void) spvm;
  
  assert(size > 0);
  
  void* block;
  if (size > allocator->ref_max_byte_size_use_memory_pool) {
    block = SPVM_UTIL_ALLOCATOR_safe_malloc_i64(1, size);
  }
  else {
    int32_t index = SPVM_RUNTIME_ALLOCATOR_get_freelist_index(spvm, allocator, size);
    
    void* free_address = SPVM_ARRAY_pop(spvm, allocator->freelists[index]);
    if (free_address) {
      return free_address;
    }
    else {
      block = SPVM_MEMORY_POOL_alloc(spvm, allocator->memory_pool, size);
    }
  }
  
  return block;
}

inline void SPVM_RUNTIME_ALLOCATOR_free_ref(SPVM* spvm, SPVM_RUNTIME_ALLOCATOR* allocator, SPVM_REF* ref) {
  if (ref == NULL) {
    return;
  }
  else {
    // Byte size
    int64_t byte_size = SPVM_RUNTIME_API_calcurate_ref_byte_size(spvm, spvm->runtime, ref);
    
    assert(byte_size > 0);
    
    if (byte_size > allocator->ref_max_byte_size_use_memory_pool) {
      free(ref);
    }
    else {
      // Freelist index
      int32_t freelist_index = SPVM_RUNTIME_ALLOCATOR_get_freelist_index(spvm, allocator, byte_size);
      
      // Push free address
      SPVM_ARRAY_push(spvm, allocator->freelists[freelist_index], ref);
    }
  }
}

void SPVM_RUNTIME_ALLOCATOR_free(SPVM* spvm, SPVM_RUNTIME_ALLOCATOR* allocator) {
  // Free memory pool */
  SPVM_MEMORY_POOL_free(spvm, allocator->memory_pool);
  
  free(allocator->freelists);
  
  free(allocator);
}
