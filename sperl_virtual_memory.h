#ifndef SPERL_VIRTUAL_MEMORY_H
#define SPERL_VIRTUAL_MEMORY_H

#include "sperl_base.h"

// Method information
struct SPerl_virtual_memory {
  intptr_t* indexes;
};

SPerl_VIRTUAL_MEMORY* SPerl_VIRTUAL_MEMORY_new(SPerl* sperl);

#endif
