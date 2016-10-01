#ifndef SPERL_ALLOCATOR
#define SPERL_ALLOCATOR

#include "sperl_type.h"

struct SPerl_allocator {
  SPerl_int block_size;
};

SPerl_ALLOCATOR* SPerl_ALLOCATOR_new();

#endif
