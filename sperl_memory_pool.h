#ifndef SPERL_MEMORY_POOL
#define SPERL_MEMORY_POOL

#include "sperl_base.h"

struct SPerl_memory_pool {
  int32_t page_byte_size;
  int32_t pages_length;
  int32_t current_offset;
  int8_t** pages;
  int32_t current_page;
};

SPerl_MEMORY_POOL* SPerl_MEMORY_POOL_new(SPerl* sperl, int32_t page_byte_size);
void* SPerl_MEMORY_POOL_alloc(SPerl* sperl, SPerl_MEMORY_POOL* memory_pool, int32_t block_size);
void SPerl_MEMORY_POOL_free(SPerl* sperl, SPerl_MEMORY_POOL* memory_pool);

#endif
