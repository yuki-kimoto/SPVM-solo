#ifndef SPERL_MEMORY_POOL
#define SPERL_MEMORY_POOL

#include "sperl_base.h"

struct SPerl_memory_pool {
  int64_t page_byte_size;
  int64_t pages_length;
  int64_t current_offset;
  int8_t** pages;
  int64_t current_page;
};

SPerl_MEMORY_POOL* SPerl_MEMORY_POOL_new(SPerl* sperl, int64_t page_byte_size);
void* SPerl_MEMORY_POOL_alloc(SPerl* sperl, SPerl_MEMORY_POOL* memory_pool, int64_t block_size);
void SPerl_MEMORY_POOL_free(SPerl* sperl, SPerl_MEMORY_POOL* memory_pool);

#endif
