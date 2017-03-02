#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_hash.h"
#include "sperl_array.h"
#include "sperl_api.h"
#include "sperl_memory_pool.h"
#include "sperl_allocator.h"
#include "sperl_constant_pool.h"
#include "sperl_bytecode_array.h"

void SPerl_run(SPerl* sperl, const char* package_name) {

  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_ARRAY_push(sperl, parser->include_pathes, ".");
  SPerl_ARRAY_push(sperl, parser->include_pathes, "lib");

  SPerl_PARSER_parse(sperl, package_name);

  if (parser->error_count > 0) {
    return;
  }
  // Entry point
  const char* start_sub_name = parser->start_sub_name;
  
  // Initialize environment
  SPerl_API_init_env(sperl);
  
  // Push argument
  SPerl_API_push_var_long(sperl, 2);
  
  // Run
  SPerl_API_call_sub(sperl, start_sub_name);
  
  if (sperl->abort) {
    intptr_t message = SPerl_API_pop_ret_address(sperl);
    
    int64_t length = SPerl_API_get_array_length(sperl, message);
    
    int8_t* byte_array_data = SPerl_API_get_byte_array_data(sperl, message);
    
    for (int64_t i = 0; i < length; i++) {
      putchar((int)byte_array_data[i]);
    }
    
    printf("\n");
  }
  else {
    // Get return value
    int64_t return_value = SPerl_API_pop_ret_long(sperl);
    
    printf("TEST return_value: %ld\n", return_value);
  }
}

SPerl* SPerl_new() {
  SPerl* sperl = SPerl_ALLOCATOR_safe_malloc_zero(sperl, 1, sizeof(SPerl));
  
  // Memory pool - memory pool save short strings and object data, except array and hash
  // These datas are created at compile time
  sperl->memory_pool = SPerl_MEMORY_POOL_new(sperl, 0);
  
  // Arrays - these arrays are created at compile time
  sperl->arrays = SPerl_ARRAY_new(sperl, 0);
  
  // Hashed - these hashes are created at compile time
  sperl->hashes = SPerl_ARRAY_new(sperl, 0);
  
  // Long strings - these strings are created at compile time
  sperl->long_strings = SPerl_ARRAY_new(sperl, 0);
  
  // Parser
  sperl->parser = SPerl_PARSER_new(sperl);
  
  // Constant pool
  sperl->constant_pool = SPerl_CONSTANT_POOL_new(sperl);
  
  // Bytecodes
  sperl->bytecode_array = SPerl_BYTECODE_ARRAY_new(sperl);
  
  // Default call stack capacity
  sperl->call_stack_capacity_default = 255;
  
  sperl->call_stack_capacity = -1;
  
  sperl->call_stack_base = 0;
  sperl->operand_stack_top = -1;
  
  return sperl;
}

void SPerl_free(SPerl* sperl) {

  // Free all arrays
  for (size_t i = 0, len = sperl->arrays->length; i < len; i++) {
    SPerl_ARRAY* array = SPerl_ARRAY_fetch(sperl, sperl->arrays, i);
    SPerl_ARRAY_free(sperl, array);
  }
  SPerl_ARRAY_free(sperl, sperl->arrays);
  
  // Free all hashes
  for (size_t i = 0, len = sperl->hashes->length; i < len; i++) {
    SPerl_HASH* hash = SPerl_ARRAY_fetch(sperl, sperl->hashes, i);
    SPerl_HASH_free(sperl, hash);
  }
  SPerl_ARRAY_free(sperl, sperl->hashes);
  
  // Free all long strings
  for (size_t i = 0, len = sperl->long_strings->length; i < len; i++) {
    void* str = SPerl_ARRAY_fetch(sperl, sperl->long_strings, i);
    free(str);
  }
  SPerl_ARRAY_free(sperl, sperl->long_strings);
  
  // Free memory pool */
  SPerl_MEMORY_POOL_free(sperl, sperl->memory_pool);
  
  free(sperl);
}
