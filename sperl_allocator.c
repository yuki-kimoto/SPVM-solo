#include <stdlib.h>

#include "sperl_allocator.h"
#include "sperl_parser.h"
#include "sperl_resolved_type.h"
#include "sperl_package.h"
#include "sperl_body_core.h"
#include "sperl_body.h"
#include "sperl_type_component_word.h"
#include "sperl_type.h"
#include "sperl_word.h"
#include "sperl_op.h"
#include "sperl_memory_pool.h"
#include "sperl_hash.h"
#include "sperl_array.h"

SPerl_ARRAY* SPerl_ALLOCATOR_new_array(SPerl_PARSER* parser, SPerl_int capacity) {
  SPerl_ARRAY* array = SPerl_ARRAY_new(capacity);
  
  SPerl_ARRAY_push(parser->array_ptrs, array);
  
  return array;
}

SPerl_HASH* SPerl_ALLOCATOR_new_hash(SPerl_PARSER* parser, SPerl_int capacity) {
  SPerl_HASH* hash = SPerl_HASH_new(capacity);
  
  SPerl_ARRAY_push(parser->hash_ptrs, hash);
  
  return hash;
}

SPerl_int* SPerl_ALLOCATOR_new_int(SPerl_PARSER* parser) {
  SPerl_int* value = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_int));
  
  return value;
}

SPerl_char* SPerl_ALLOCATOR_new_string(SPerl_PARSER* parser, SPerl_int length) {
  SPerl_char* str = malloc(length + 1);
  
  if (length < 40) {
    str = SPerl_MEMORY_POOL_alloc(parser->memory_pool, 40);
  }
  else {
    str = malloc(length + 1);
    SPerl_ARRAY_push(parser->long_str_ptrs, str);
  }
  
  return str;
}
