#ifndef SPERL_H
#define SPERL_H

#include <stdio.h>

#include "sperl_base.h"

enum {
  SPerl_C_ARRAY_HEADER_LENGTH = sizeof(int64_t) * 2,
  SPerl_C_ARRAY_HEADER_LENGTH_OFFSET = sizeof(int64_t),
};

// Parser information
struct sperl_ {
  // Memory_pool
  SPerl_MEMORY_POOL* memory_pool;
  
  // All array pointers
  SPerl_ARRAY* array_ptrs;
  
  // All hash pointers
  SPerl_ARRAY* hash_ptrs;
  
  // All long string pointers
  SPerl_ARRAY* long_str_ptrs;
  
  // Parser
  SPerl_PARSER* parser;
  
  // Constant pool
  SPerl_CONSTANT_POOL* constant_pool;
  
  // Bytecodes
  SPerl_BYTECODE_ARRAY* bytecode_array;
  
  // Call stack capacity default
  int32_t call_stack_capacity_default;
  
  // Runtime environment
  int32_t call_stack_capacity;
  int64_t* call_stack;
  int64_t call_stack_base;
  int64_t operand_stack_top;
  _Bool abort;
};

SPerl* SPerl_new();
void SPerl_free(SPerl* sperl);

#endif
