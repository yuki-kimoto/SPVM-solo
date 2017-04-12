#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_hash.h"
#include "sperl_array.h"
#include "sperl_api.h"
#include "sperl_allocator_util.h"
#include "sperl_constant_pool.h"
#include "sperl_bytecode_array.h"

void SPerl_run(SPerl* sperl, const char* package_name) {
  
  sperl->parser = SPerl_PARSER_new(sperl);
  
  SPerl_ARRAY_push(sperl, sperl->parser->include_pathes, ".");
  SPerl_ARRAY_push(sperl, sperl->parser->include_pathes, "lib");
  
  SPerl_PARSER_parse(sperl, package_name);
  
  if (sperl->parser->error_count > 0) {
    return;
  }
  
  // Free parser
  SPerl_PARSER_free(sperl, sperl->parser);
  sperl->parser = NULL;
  
  // Entry point
  const char* entry_point_sub_name = sperl->entry_point_sub_name;
  
  // Initialize environment
  SPerl_API_init_env(sperl);
  
  // Push argument
  SPerl_API_push_var_long(sperl, 2);
  
  // Run
  SPerl_API_call_sub(sperl, entry_point_sub_name);
  
  if (sperl->abort) {
    intptr_t message = SPerl_API_pop_ret(sperl);
    
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
  SPerl* sperl = SPerl_ALLOCATOR_UTIL_safe_malloc_zero(1, sizeof(SPerl));
  
  // Constant pool
  sperl->constant_pool = SPerl_CONSTANT_POOL_new(sperl);
  
  // Bytecodes
  sperl->bytecode_array = SPerl_BYTECODE_ARRAY_new(sperl);

  // Constant poll subroutine symbol table
  sperl->constant_pool_package_symtable = SPerl_HASH_new(sperl, 0);
  
  // Constant poll subroutine symbol table
  sperl->constant_pool_field_symtable = SPerl_HASH_new(sperl, 0);
  
  // Constant poll subroutine symbol table
  sperl->constant_pool_sub_symtable = SPerl_HASH_new(sperl, 0);
  
  // Default call stack capacity
  sperl->call_stack_capacity_default = 255;
  
  sperl->call_stack_capacity = -1;
  
  sperl->call_stack_base = 0;
  sperl->operand_stack_top = -1;
  
  return sperl;
}

void SPerl_free(SPerl* sperl) {
  
  // Free parser
  if (sperl->parser) {
    SPerl_PARSER_free(sperl, sperl->parser);
  }
  
  // Free constant pool
  SPerl_CONSTANT_POOL_free(sperl, sperl->constant_pool);
  
  // Free bytecode array
  SPerl_BYTECODE_ARRAY_free(sperl, sperl->bytecode_array);
  
  // Free call stack
  free(sperl->call_stack);
  
  free(sperl);
}
