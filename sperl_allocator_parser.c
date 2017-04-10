#include <stdlib.h>
#include <stdio.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_memory_pool2.h"
#include "sperl_allocator_util.h"
#include "sperl_allocator_parser.h"
#include "sperl_parser.h"

void* SPerl_ALLOCATOR_PARSER_alloc_memory_pool(SPerl* sperl, SPerl_PARSER* parser, int64_t size) {
  return SPerl_MEMORY_POOL2_alloc(sperl, parser->memory_pool, size);
}

SPerl_ARRAY* SPerl_ALLOCATOR_PARSER_new_array(SPerl* sperl, SPerl_PARSER* parser, int64_t capacity) {
  SPerl_ARRAY* array = SPerl_ARRAY_new(sperl, capacity);
  
  SPerl_ARRAY_push(sperl, parser->arrays, array);
  
  return array;
}

SPerl_HASH* SPerl_ALLOCATOR_PARSER_new_hash(SPerl* sperl, SPerl_PARSER* parser, int64_t capacity) {
  SPerl_HASH* hash = SPerl_HASH_new(sperl, capacity);
  
  SPerl_ARRAY_push(sperl, parser->hashes, hash);
  
  return hash;
}

int32_t* SPerl_ALLOCATOR_PARSER_new_int(SPerl* sperl, SPerl_PARSER* parser) {
  int32_t* value = SPerl_MEMORY_POOL2_alloc(sperl, parser->memory_pool, sizeof(int32_t));
  
  return value;
}

int64_t* SPerl_ALLOCATOR_PARSER_new_long(SPerl* sperl, SPerl_PARSER* parser) {
  int64_t* value = SPerl_MEMORY_POOL2_alloc(sperl, parser->memory_pool, sizeof(int64_t));
  
  return value;
}

char* SPerl_ALLOCATOR_PARSER_new_string(SPerl* sperl, SPerl_PARSER* parser, int64_t length) {
  char* str;
  
  if (length < 0xFF) {
    str = (char*) SPerl_MEMORY_POOL2_alloc(sperl, parser->memory_pool, length + 1);
  }
  else {
    str = (char*)SPerl_ALLOCATOR_UTIL_safe_malloc(length + 1, sizeof(char));
    SPerl_ARRAY_push(sperl, parser->long_strings, str);
  }
  
  return str;
}
