#include <stdlib.h>

#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_memory_pool.h"
#include "sperl_allocator.h"

void* SPerl_ALLOCATOR_alloc_memory_pool(SPerl_PARSER* parser, int32_t size) {
  return SPerl_MEMORY_POOL_alloc(parser->memory_pool, size);
}

SPerl_ARRAY* SPerl_ALLOCATOR_new_array(SPerl_PARSER* parser, int32_t capacity) {
  SPerl_ARRAY* array = SPerl_ARRAY_new(capacity);
  
  SPerl_ARRAY_push(parser->array_ptrs, array);
  
  return array;
}

SPerl_HASH* SPerl_ALLOCATOR_new_hash(SPerl_PARSER* parser, int32_t capacity) {
  SPerl_HASH* hash = SPerl_HASH_new(capacity);
  
  SPerl_ARRAY_push(parser->hash_ptrs, hash);
  
  return hash;
}

int32_t* SPerl_ALLOCATOR_new_int(SPerl_PARSER* parser) {
  int32_t* value = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(int32_t));
  
  return value;
}

uint8_t* SPerl_ALLOCATOR_new_string(SPerl_PARSER* parser, int32_t length) {
  uint8_t* str = malloc(length + 1);
  
  if (length < 40) {
    str = SPerl_MEMORY_POOL_alloc(parser->memory_pool, 40);
  }
  else {
    str = malloc(length + 1);
    SPerl_ARRAY_push(parser->long_str_ptrs, str);
  }
  
  return str;
}
