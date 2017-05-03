#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "spvm.h"
#include "spvm_parser.h"
#include "spvm_hash.h"
#include "spvm_array.h"
#include "spvm_api.h"
#include "spvm_allocator_util.h"
#include "spvm_constant_pool.h"
#include "spvm_bytecode_array.h"
#include "spvm_parser.h"
#include "spvm_env.h"

void SPVM_run(SPVM* spvm, const char* package_name) {
  
  spvm->parser = SPVM_PARSER_new(spvm);
  
  SPVM_ARRAY_push(spvm, spvm->parser->include_pathes, ".");
  SPVM_ARRAY_push(spvm, spvm->parser->include_pathes, "lib");
  
  SPVM_PARSER_parse(spvm, package_name);
  
  if (spvm->parser->error_count > 0) {
    return;
  }
  
  // Entry point
  const char* entry_point_sub_name = spvm->entry_point_sub_name;
  
  // Create subroutine environment
  SPVM_ENV* env = SPVM_ENV_new(spvm);
  
  // Push argument
  SPVM_API_push_var_long(spvm, env, 2);
  
  // Run
  SPVM_API_call_sub(spvm, env, entry_point_sub_name);
  
  if (env->abort) {
    void* message = SPVM_API_pop_ret_ref(spvm, env);
    
    int64_t length = SPVM_API_get_array_length(spvm, env, message);
    
    int8_t* byte_array_data = SPVM_API_get_byte_array_data(spvm, env, message);
    
    for (int64_t i = 0; i < length; i++) {
      putchar((int)byte_array_data[i]);
    }
    
    printf("\n");
  }
  else {
    // Get return value
    int64_t return_value = SPVM_API_pop_ret_long(spvm, env);
    
    printf("TEST return_value: %ld\n", return_value);
  }
  SPVM_ENV_free(spvm, env);
}

SPVM* SPVM_new() {
  SPVM* spvm = SPVM_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPVM));
  
  // Parser
  spvm->parser = NULL;
  
  // Entry point sub name
  spvm->entry_point_sub_name = NULL;
  
  // Constant pool
  spvm->constant_pool = SPVM_CONSTANT_POOL_new(spvm);
  
  // Bytecodes
  spvm->bytecode_array = SPVM_BYTECODE_ARRAY_new(spvm);

  // Constant poll subroutine symbol table
  spvm->constant_pool_package_symtable = SPVM_HASH_new(spvm, 0);
  
  // Constant poll subroutine symbol table
  spvm->constant_pool_field_symtable = SPVM_HASH_new(spvm, 0);
  
  // Constant poll subroutine symbol table
  spvm->constant_pool_sub_symtable = SPVM_HASH_new(spvm, 0);
  
  // Runtime memory allocator
  spvm->allocator_runtime = SPVM_ALLOCATOR_RUNTIME_new(spvm);

  return spvm;
}

void SPVM_free(SPVM* spvm) {
  
  SPVM_PARSER_free(spvm, spvm->parser);
  
  // Free constant pool
  SPVM_CONSTANT_POOL_free(spvm, spvm->constant_pool);
  
  // Free bytecode array
  SPVM_BYTECODE_ARRAY_free(spvm, spvm->bytecode_array);
  
  // Free runtime allocator
  SPVM_ALLOCATOR_RUNTIME_free(spvm, spvm->allocator_runtime);
  
  free(spvm);
}
