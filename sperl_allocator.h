#ifndef SPERL_ALLOCATOR
#define SPERL_ALLOCATOR

#include "sperl_type.h"

struct SPerl_allocator {
  SPerl_int base_capacity;
  SPerl_MEMORY_NODE* memory_node;
  SPerl_int node_depth;
  SPerl_int current_pos;
};

SPerl_ALLOCATOR* SPerl_ALLOCATOR_new(SPerl_int base_capacity);
void* SPerl_ALLOCATOR_alloc(SPerl_ALLOCATOR* allocator, SPerl_int block_size);
void SPerl_ALLOCATOR_free(SPerl_ALLOCATOR* allocator);

#endif
