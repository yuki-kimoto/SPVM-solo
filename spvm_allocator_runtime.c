#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "spvm.h"
#include "spvm_allocator_runtime.h"
#include "spvm_allocator_util.h"
#include "spvm_memory_pool.h"
#include "spvm_array.h"
#include "spvm_api.h"
#include "spvm_data_header_object.h"

SPVM_ALLOCATOR_RUNTIME* SPVM_ALLOCATOR_RUNTIME_new(SPVM* spvm) {
  SPVM_ALLOCATOR_RUNTIME* allocator = SPVM_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPVM_ALLOCATOR_RUNTIME));
  
  // Memory pool
  allocator->memory_pool = SPVM_MEMORY_POOL_new(spvm, 0);
  
  // Free lists
  allocator->freelists = SPVM_ALLOCATOR_UTIL_safe_malloc_i32(16, sizeof(SPVM_ARRAY));
  
  // Initialize free list
  for (int32_t i = 0; i < 16; i++) {
    allocator->freelists[i] = SPVM_ARRAY_new(spvm, 0);
  }
  
  return allocator;
}

int32_t SPVM_ALLOCATOR_RUNTIME_get_freelist_index(SPVM* spvm, SPVM_ALLOCATOR_RUNTIME* allocator, int64_t size) {
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

void* SPVM_ALLOCATOR_RUNTIME_alloc(SPVM* spvm, SPVM_ALLOCATOR_RUNTIME* allocator, int64_t size) {
  (void) spvm;
  
  assert(size > 0);
  
  void* block;
  if (size > 0xFFFF) {
    block = SPVM_ALLOCATOR_UTIL_safe_malloc_i64(1, size);
  }
  else {
    int32_t index = SPVM_ALLOCATOR_RUNTIME_get_freelist_index(spvm, allocator, size);
    
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

void SPVM_ALLOCATOR_RUNTIME_free_address(SPVM* spvm, SPVM_ALLOCATOR_RUNTIME* allocator, void* address) {
  if (address == NULL) {
    return;
  }
  else {
    // Object byte size
    int32_t byte_size = ((SPVM_DATA_HEADER_OBJECT*)address)->byte_size;
    
    assert(byte_size > 0);
    
    // Freelist index
    int32_t freelist_index = SPVM_ALLOCATOR_RUNTIME_get_freelist_index(spvm, allocator, byte_size);
    
    // Push free address
    SPVM_ARRAY_push(spvm, allocator->freelists[freelist_index], address);
  }
}

void SPVM_ALLOCATOR_RUNTIME_free(SPVM* spvm, SPVM_ALLOCATOR_RUNTIME* allocator) {
  // Free memory pool */
  SPVM_MEMORY_POOL_free(spvm, allocator->memory_pool);
  
  free(allocator->freelists);
  
  free(allocator);
}
