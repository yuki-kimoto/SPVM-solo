#include <stdlib.h>
#include <string.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_resolved_type.h"
#include "sperl_package.h"
#include "sperl_type_component_name.h"
#include "sperl_type.h"
#include "sperl_op.h"
#include "sperl_memory_pool.h"
#include "sperl_hash.h"
#include "sperl_array.h"
#include "sperl_allocator.h"
#include "sperl_yacc.h"
#include "sperl_array.h"
#include "sperl_use.h"

SPerl* SPerl_new() {
  SPerl* sperl = calloc(1, sizeof(SPerl));
  
  // Manipulate memory. This is freed last.
  sperl->array_ptrs = SPerl_ARRAY_new(0);
  sperl->hash_ptrs = SPerl_ARRAY_new(0);
  sperl->long_str_ptrs = SPerl_ARRAY_new(0);
  sperl->memory_pool = SPerl_MEMORY_POOL_new(0);

  // Parser
  sperl->parser = SPerl_PARSER_new(sperl);
  
  return sperl;
}

void SPerl_free(SPerl* sperl) {

  // Free all array pointers
  for (int32_t i = 0; i < sperl->array_ptrs->length; i++) {
    SPerl_ARRAY* array = SPerl_ARRAY_fetch(sperl->array_ptrs, i);
    SPerl_ARRAY_free(array);
  }
  SPerl_ARRAY_free(sperl->array_ptrs);
  
  // Free all hash pointers
  for (int32_t i = 0; i < sperl->hash_ptrs->length; i++) {
    SPerl_HASH* hash = SPerl_ARRAY_fetch(sperl->hash_ptrs, i);
    SPerl_HASH_free(hash);
  }
  SPerl_ARRAY_free(sperl->hash_ptrs);
  
  // Free all string pointers;
  for (int32_t i = 0; i < sperl->long_str_ptrs->length; i++) {
    void* str = SPerl_ARRAY_fetch(sperl->long_str_ptrs, i);
    free(str);
  }
  SPerl_ARRAY_free(sperl->long_str_ptrs);
  
  // Free memory pool */
  SPerl_MEMORY_POOL_free(sperl->memory_pool);
  
  free(sperl);
}
