#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_hash.h"
#include "sperl_array.h"
#include "sperl_api.h"

void SPerl_run(SPerl* sperl, const char* package_name) {

  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_ARRAY_push(parser->include_pathes, ".");
  SPerl_ARRAY_push(parser->include_pathes, "lib");

  SPerl_PARSER_parse(sperl, package_name);

  if (parser->error_count > 0) {
    return;
  }
  // Entry point
  const char* start_sub_name = parser->start_sub_name;
  
  // Initialize sperl
  SPerl_API_init_env(sperl);
  
  // Set argument
  SPerl_API_set_var_long(sperl, 0, 2);
  
  // Run
  SPerl_API_call_sub(sperl, start_sub_name);
  
  if (sperl->abort) {
    intptr_t message = SPerl_API_get_ret_address(sperl);
    
    int64_t length = SPerl_API_get_array_length(sperl, message);
    
    int8_t* byte_array_data = SPerl_API_get_byte_array_data(sperl, message);
    
    for (size_t i = 0; i < length; i++) {
      putchar((int)byte_array_data[i]);
    }
    
    printf("\n");
  }
  else {
    // Get return value
    int64_t return_value = SPerl_API_get_ret_long(sperl);
    
    printf("TEST return_value: %d\n", return_value);
  }
}

SPerl* SPerl_new() {
  SPerl* sperl = SPerl_ALLOCATOR_safe_malloc_zero(1, sizeof(SPerl));
  
  // Manipulate memory. This is freed last.
  sperl->array_ptrs = SPerl_ARRAY_new(0);
  sperl->hash_ptrs = SPerl_ARRAY_new(0);
  sperl->long_str_ptrs = SPerl_ARRAY_new(0);
  sperl->memory_pool = SPerl_MEMORY_POOL_new(0);

  // Parser
  sperl->parser = SPerl_PARSER_new(sperl);
  
  // Constant pool
  sperl->constant_pool = SPerl_CONSTANT_POOL_new(sperl);
  
  // Bytecodes
  sperl->bytecode_array = SPerl_BYTECODE_ARRAY_new(sperl, 0);
  
  // Default call stack capacity
  sperl->call_stack_capacity_default = 255;
  
  sperl->call_stack_capacity = -1;
  
  sperl->call_stack_base = 0;
  sperl->operand_stack_top = -1;
  
  return sperl;
}

void SPerl_free(SPerl* sperl) {

  // Free all array pointers
  for (size_t i = 0, len = sperl->array_ptrs->length; i < len; i++) {
    SPerl_ARRAY* array = SPerl_ARRAY_fetch(sperl->array_ptrs, i);
    SPerl_ARRAY_free(array);
  }
  SPerl_ARRAY_free(sperl->array_ptrs);
  
  // Free all hash pointers
  for (size_t i = 0, len = sperl->hash_ptrs->length; i < len; i++) {
    SPerl_HASH* hash = SPerl_ARRAY_fetch(sperl->hash_ptrs, i);
    SPerl_HASH_free(hash);
  }
  SPerl_ARRAY_free(sperl->hash_ptrs);
  
  // Free all string pointers;
  for (size_t i = 0, len = sperl->long_str_ptrs->length; i < len; i++) {
    void* str = SPerl_ARRAY_fetch(sperl->long_str_ptrs, i);
    free(str);
  }
  SPerl_ARRAY_free(sperl->long_str_ptrs);
  
  // Free memory pool */
  SPerl_MEMORY_POOL_free(sperl->memory_pool);
  
  free(sperl);
}
