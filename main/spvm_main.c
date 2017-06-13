#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../spvm_parser.h"
#include "../spvm_hash.h"
#include "../spvm_array.h"
#include "../spvm_util_allocator.h"
#include "../spvm_constant_pool.h"
#include "../spvm_bytecode_array.h"
#include "../spvm_runtime.h"
#include "../spvm_runtime_api.h"
#include "../spvm_runtime_allocator.h"
#include "../spvm_op.h"
#include "../spvm_sub.h"
#include "../spvm_data_api.h"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Not script\n");
    exit(1);
  }
  
  // Package name
  const char* package_name = argv[1];
  
  // Create parser
  SPVM_PARSER* parser = SPVM_PARSER_new();
  
  parser->entry_point_package_name = package_name;
  
  SPVM_ARRAY_push(parser->include_pathes, ".");
  SPVM_ARRAY_push(parser->include_pathes, "lib");
  
  SPVM_PARSER_parse(parser);
  
  if (parser->error_count > 0) {
    return;
  }
  
  // Entry point
  const char* entry_point_sub_name = parser->entry_point_sub_name;

  // Create run-time
  SPVM_RUNTIME* runtime = SPVM_RUNTIME_new();

  // Start address
  SPVM_OP* op_sub_start = SPVM_HASH_search(parser->op_sub_symtable, entry_point_sub_name, strlen(entry_point_sub_name));
  int32_t sub_constant_pool_address = op_sub_start->uv.sub->constant_pool_address;
  
  // Copy constant pool to runtime
  runtime->constant_pool = SPVM_UTIL_ALLOCATOR_safe_malloc_i32(parser->constant_pool->length, sizeof(int32_t));
  memcpy(runtime->constant_pool, parser->constant_pool->values, parser->constant_pool->length * sizeof(int32_t));

  // Copy bytecodes to runtime
  runtime->bytecodes = SPVM_UTIL_ALLOCATOR_safe_malloc_i32(parser->bytecode_array->length, sizeof(uint8_t));
  memcpy(runtime->bytecodes, parser->bytecode_array->values, parser->bytecode_array->length * sizeof(uint8_t));
  
  // Free parser
  SPVM_PARSER_free(parser);
  
  // Initialize runtime before push arguments and call subroutine
  SPVM_RUNTIME_init(runtime);
  
  // Push argument
  SPVM_RUNTIME_API_push_var_long(runtime, 2);
  
  // Run
  SPVM_RUNTIME_call_sub(runtime, sub_constant_pool_address);
  
#ifdef DEBUG
  if (runtime->abort) {
    void* message_address = SPVM_RUNTIME_API_pop_return_value_address(runtime);
    int8_t* message = SPVM_DATA_API_get_array_values_byte(message_address);
    
    printf("%s", (char*)message);
    printf("\n");
  }
  else {
    // Get return value
    int64_t return_value = SPVM_RUNTIME_API_pop_return_value_long(runtime);
    
    printf("TEST return_value: %ld\n", return_value);
  }
#endif
  
  return 0;
}
