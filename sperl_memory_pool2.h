#ifndef SPERL_MEMORY_POOL2
#define SPERL_MEMORY_POOL2

#include "sperl_base.h"

struct SPerl_memory_pool {
  int64_t page_byte_size;
  int64_t pages_length;
  int64_t current_offset;
  int8_t** pages;
  int64_t current_page;
};

SPerl_MEMORY_POOL2* SPerl_MEMORY_POOL2_new(SPerl* sperl, int64_t page_byte_size);
void* SPerl_MEMORY_POOL2_alloc(SPerl* sperl, SPerl_MEMORY_POOL2* memory_pool, int64_t block_size);
void SPerl_MEMORY_POOL2_free(SPerl* sperl, SPerl_MEMORY_POOL2* memory_pool);

#endif
