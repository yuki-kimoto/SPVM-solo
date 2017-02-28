#ifndef SPERL_VIRTUAL_MEMORY_H
#define SPERL_VIRTUAL_MEMORY_H

#include "sperl_base.h"

// Method information
struct SPerl_virtual_memory {
  int64_t current_index;
  intptr_t* indexes;
};

SPerl_VIRTUAL_MEMORY* SPerl_VIRTUAL_MEMORY_new(SPerl* sperl);
SPerl_VIRTUAL_MEMORY* SPerl_VIRTUAL_MEMORY_malloc(SPerl* sperl, SPerl_VIRTUAL_MEMORY* virtual_memory, size_t byte_size);

#endif
