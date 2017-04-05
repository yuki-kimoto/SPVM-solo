#ifndef SPERL_HEAP_H
#define SPERL_HEAP_H

#include "sperl_base.h"

enum {
  SPerl_HEAP_C_MEMORY_POOL_MAX_DATA_SIZE = 1024
};

void* SPerl_HEAP_alloc(SPerl* sperl, int64_t size);

#endif
