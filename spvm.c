#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "spvm.h"
#include "spvm_parser.h"
#include "spvm_hash.h"
#include "spvm_array.h"
#include "spvm_util_allocator.h"
#include "spvm_constant_pool.h"
#include "spvm_bytecode_array.h"
#include "spvm_runtime.h"
#include "spvm_sv.h"
#include "spvm_runtime.h"
#include "spvm_runtime_allocator.h"
#include "spvm_op.h"
#include "spvm_sub.h"

void SPVM_run(SPVM* spvm, const char* package_name) {
  
  spvm->parser = SPVM_PARSER_new(spvm);
  
  SPVM_ARRAY_push(spvm, spvm->parser->include_pathes, ".");
  SPVM_ARRAY_push(spvm, spvm->parser->include_pathes, "lib");
  
  SPVM_PARSER_parse(spvm, package_name);
  
  if (spvm->parser->error_count > 0) {
    return;
  }
  
  // Entry point
  const char* entry_point_sub_name = spvm->parser->entry_point_sub_name;
  
  // Create subroutine runtimeironment
  SPVM_RUNTIME* runtime = spvm->runtime;
  
  SPVM_RUNTIME_init(spvm, runtime);
  
  // Push argument
  SPVM_RUNTIME_API_push_var_long(spvm, runtime, 2);
  
  // Start address
  SPVM_OP* op_sub_start = SPVM_HASH_search(spvm, spvm->parser->op_sub_symtable, entry_point_sub_name, strlen(entry_point_sub_name));
  int32_t sub_constant_pool_address = op_sub_start->uv.sub->constant_pool_address;
  
  // Run
  SPVM_RUNTIME_call_sub(spvm, runtime, sub_constant_pool_address);
  
#ifdef DEBUG
  if (runtime->abort) {
    void* message = SPVM_RUNTIME_API_pop_return_value_address(spvm, runtime);
    
    SPVM_SV* sv_message = SPVM_RUNTIME_API_get_string_sv(spvm, runtime, message);
    
    printf("%s", sv_message->buffer);
    printf("\n");
  }
  else {
    // Get return value
    int64_t return_value = SPVM_RUNTIME_API_pop_return_value_long(spvm, runtime);
    
    printf("TEST return_value: %ld\n", return_value);
  }
#endif
}

SPVM* SPVM_new() {
  SPVM* spvm = SPVM_UTIL_ALLOCATOR_safe_malloc_i32(1, sizeof(SPVM));
  
  // Parser
  spvm->parser = NULL;
  
  // Constant pool
  spvm->constant_pool = SPVM_CONSTANT_POOL_new(spvm);
  
  // Bytecodes
  spvm->bytecode_array = SPVM_BYTECODE_ARRAY_new(spvm);

  // Runtime
  spvm->runtime = SPVM_RUNTIME_new(spvm);
  
  return spvm;
}

void SPVM_free(SPVM* spvm) {
  
  SPVM_PARSER_free(spvm, spvm->parser);
  
  // Free constant pool
  SPVM_CONSTANT_POOL_free(spvm, spvm->constant_pool);
  
  // Free bytecode array
  SPVM_BYTECODE_ARRAY_free(spvm, spvm->bytecode_array);
  
  free(spvm);
}
