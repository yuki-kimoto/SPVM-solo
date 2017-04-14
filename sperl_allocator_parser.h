#ifndef SPERL_ALLOCATOR_PARSER_H
#define SPERL_ALLOCATOR_PARSER_H
#include <stddef.h>
#include "sperl_base.h"

SPerl_ALLOCATOR_PARSER* SPerl_ALLOCATOR_PARSER_new(SPerl* sperl);

SPerl_ARRAY* SPerl_ALLOCATOR_PARSER_new_array(SPerl* sperl, SPerl_ALLOCATOR_PARSER* allocator, size_t capacity);
SPerl_HASH* SPerl_ALLOCATOR_PARSER_new_hash(SPerl* sperl, SPerl_ALLOCATOR_PARSER* allocator, size_t capacity);
char* SPerl_ALLOCATOR_PARSER_new_string(SPerl* sperl, SPerl_ALLOCATOR_PARSER* allocator, size_t length);
int32_t* SPerl_ALLOCATOR_PARSER_new_int(SPerl* sperl, SPerl_ALLOCATOR_PARSER* allocator);
void* SPerl_ALLOCATOR_PARSER_alloc_memory_pool(SPerl* sperl, SPerl_ALLOCATOR_PARSER* allocator, size_t size);

SPerl_VMCODE* SPerl_ALLOCATOR_PARSER_new_vmcode(SPerl* sperl, SPerl_PARSER* parser);

void SPerl_ALLOCATOR_PARSER_free(SPerl* sperl, SPerl_ALLOCATOR_PARSER* allocator);

// Parser information
struct SPerl_allocator_parser {
  // Memory_pool - This is compile time memory pool. This memory pool save short string and object except array, hash
  SPerl_MEMORY_POOL* memory_pool;
  
  // Compile time arrays
  SPerl_ARRAY* arrays;
  
  // Compile time hashes
  SPerl_ARRAY* hashes;
  
  // Compile time long strings
  SPerl_ARRAY* long_strings;
};

#endif
