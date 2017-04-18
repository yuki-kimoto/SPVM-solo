#ifndef SPERL_ALLOCATOR_RUNTIME_H
#define SPERL_ALLOCATOR_RUNTIME_H
#include <stddef.h>
#include "sperl_base.h"

// Parser information
struct SPerl_allocator_runtime {
  // Memory_pool
  SPerl_MEMORY_POOL* memory_pool;
  
  // Free addresses list
  void** free_addresses_list;
};

SPerl_ALLOCATOR_RUNTIME* SPerl_ALLOCATOR_RUNTIME_new(SPerl* sperl);
void* SPerl_ALLOCATOR_RUNTIME_alloc(SPerl* sperl, SPerl_ALLOCATOR_RUNTIME* allocator, int64_t size);
void SPerl_ALLOCATOR_RUNTIME_free(SPerl* sperl, SPerl_ALLOCATOR_RUNTIME* allocator);

#endif
