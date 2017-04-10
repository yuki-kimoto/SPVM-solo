#ifndef SPERL_MEMORY_POOL
#define SPERL_MEMORY_POOL

#include "sperl_base.h"

struct SPerl_memory_pool {
  size_t page_byte_size;
  size_t pages_length;
  size_t current_offset;
  char** pages;
  size_t current_page;
};

SPerl_MEMORY_POOL* SPerl_MEMORY_POOL_new(SPerl* sperl, size_t page_byte_size);
void* SPerl_MEMORY_POOL_alloc(SPerl* sperl, SPerl_MEMORY_POOL* memory_pool, size_t block_size);
void SPerl_MEMORY_POOL_free(SPerl* sperl, SPerl_MEMORY_POOL* memory_pool);

#endif
