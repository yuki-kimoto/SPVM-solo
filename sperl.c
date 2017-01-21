#include <stdlib.h>
#include <string.h>

#include "sperl.h"
#include "sperl_vm.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"
#include "sperl_hash.h"
#include "sperl_array.h"
#include "sperl_allocator.h"
#include "sperl_constant_pool.h"
#include "sperl_bytecode_array.h"
#include "sperl_frame.h"

void SPerl_run(SPerl* sperl, const char* package_name) {

  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_ARRAY_push(parser->include_pathes, ".");
  SPerl_PARSER_parse(sperl, package_name);
  
  // Entry point
  const char* entry_point = parser->entry_point;
  
  // Create VM
  SPerl_VM* vm = SPerl_VM_new(sperl);
  
  // Initialize frame
  SPerl_FRAME* frame = SPerl_VM_init_frame(sperl, vm);
  frame->operand_stack[0] = 4;
  
  // Run
  SPerl_VM_call_sub(sperl, vm, entry_point);
  
  int64_t return_value = frame->operand_stack[0];
  
  printf("TEST return_value: %d\n", return_value);
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
