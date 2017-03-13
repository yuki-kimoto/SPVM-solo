#ifndef SPERL_ALLOCATOR_PARSER_H
#define SPERL_ALLOCATOR_PARSER_H
#include <stddef.h>
#include "sperl_base.h"

SPerl_ARRAY* SPerl_ALLOCATOR_PARSER_new_array(SPerl* sperl, SPerl_PARSER* parser, int64_t capacity);
SPerl_HASH* SPerl_ALLOCATOR_PARSER_new_hash(SPerl* sperl, SPerl_PARSER* parser, int64_t capacity);
char* SPerl_ALLOCATOR_PARSER_new_string(SPerl* sperl, SPerl_PARSER* parser, int64_t length);
int32_t* SPerl_ALLOCATOR_PARSER_new_int(SPerl* sperl, SPerl_PARSER* parser);
void* SPerl_ALLOCATOR_PARSER_alloc_memory_pool(SPerl* sperl, SPerl_PARSER* parser, int64_t size);

SPerl_VMCODE* SPerl_ALLOCATOR_PARSER_new_vmcode(SPerl* sperl, SPerl_PARSER* parser);


#endif
