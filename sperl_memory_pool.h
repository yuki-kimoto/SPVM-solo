#ifndef SPERL_MEMORY_POOL
#define SPERL_MEMORY_POOL

#include "sperl_base.h"

struct SPerl_memory_pool {
  SPerl_MEMORY_POOL_PAGE* page;
  int32_t base_capacity;
  int32_t page_depth;
  int32_t current_pos;
};

SPerl_MEMORY_POOL* SPerl_MEMORY_POOL_new(SPerl* sperl, int32_t base_capacity);
void* SPerl_MEMORY_POOL_alloc(SPerl* sperl, SPerl_MEMORY_POOL* memory_pool, int32_t block_size);
void SPerl_MEMORY_POOL_free(SPerl* sperl, SPerl_MEMORY_POOL* memory_pool);

#endif
