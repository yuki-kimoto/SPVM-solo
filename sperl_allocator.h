#ifndef SPERL_ALLOCATOR_H
#define SPERL_ALLOCATOR_H

#include "sperl_base.h"

SPerl_ARRAY* SPerl_ALLOCATOR_new_array(SPerl_PARSER* parser, int32_t capacity);
SPerl_HASH* SPerl_ALLOCATOR_new_hash(SPerl_PARSER* parser, int32_t capacity);
uint8_t* SPerl_ALLOCATOR_new_string(SPerl_PARSER* parser, int32_t length);
int32_t* SPerl_ALLOCATOR_new_int(SPerl_PARSER* parser);
SPerl_VMCODE* SPerl_ALLOCATOR_new_vmcode(SPerl_PARSER* parser);

void* SPerl_ALLOCATOR_alloc_memory_pool(SPerl_PARSER* parser, int32_t size);

#endif
