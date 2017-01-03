#ifndef SPERL_ALLOCATOR_H
#define SPERL_ALLOCATOR_H

#include "sperl_base.h"

SPerl_ARRAY* SPerl_ALLOCATOR_new_array(SPerl* sperl, int32_t capacity);
SPerl_HASH* SPerl_ALLOCATOR_new_hash(SPerl* sperl, int32_t capacity);
char* SPerl_ALLOCATOR_new_string(SPerl* sperl, int32_t length);
int32_t* SPerl_ALLOCATOR_new_int(SPerl* sperl);
SPerl_VMCODE* SPerl_ALLOCATOR_new_vmcode(SPerl* sperl);

void* SPerl_ALLOCATOR_alloc_memory_pool(SPerl* sperl, int32_t size);

#endif
