#ifndef SPERL_MEMORY_POOL
#define SPERL_MEMORY_POOL

#include "sperl_type.h"

struct SPerl_memory_pool {
  SPerl_int base_capacity;
  SPerl_MEMORY_NODE* memory_node;
  SPerl_int node_depth;
  SPerl_int current_pos;
};

SPerl_MEMORY_POOL* SPerl_MEMORY_POOL_new(SPerl_int base_capacity);
void* SPerl_MEMORY_POOL_alloc(SPerl_MEMORY_POOL* memory_pool, SPerl_int block_size);
void SPerl_MEMORY_POOL_free(SPerl_MEMORY_POOL* memory_pool);

#endif
