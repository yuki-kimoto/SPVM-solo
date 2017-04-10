#ifndef SPERL_ALLOCATOR_RUNTIME_H
#define SPERL_ALLOCATOR_RUNTIME_H
#include <stddef.h>
#include "sperl_base.h"

void* SPerl_ALLOCATOR_RUNTIME_alloc(SPerl* sperl, int64_t size);

#endif
