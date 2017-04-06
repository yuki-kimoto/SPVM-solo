#ifndef SPERL_MEMORY_POOL2
#define SPERL_MEMORY_POOL2

#include "sperl_base.h"

struct SPerl_memory_pool {
  SPerl_MEMORY_POOL_PAGE* page;
  int64_t base_capacity;
  int64_t page_depth;
  int64_t current_pos;
};

SPerl_MEMORY_POOL2* SPerl_MEMORY_POOL2_new(SPerl* sperl, int64_t base_capacity);
void* SPerl_MEMORY_POOL2_alloc(SPerl* sperl, SPerl_MEMORY_POOL2* memory_pool, int64_t block_size);
void SPerl_MEMORY_POOL2_free(SPerl* sperl, SPerl_MEMORY_POOL2* memory_pool);

#endif
