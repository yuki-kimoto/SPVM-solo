#ifndef SPERL_H
#define SPERL_H

#include <stdio.h>

#include "sperl_base.h"

enum {
  SPerl_C_ARRAY_HEADER_BYTE_SIZE = sizeof(int64_t) * 2,
  SPerl_C_ARRAY_HEADER_REF_COUNT_BYTE_OFFSET = sizeof(int64_t) * 0,
  SPerl_C_ARRAY_HEADER_LENGTH_BYTE_OFFSET = sizeof(int64_t) * 1,
};

enum {
  SPerl_C_OBJECT_HEADER_BYTE_SIZE = sizeof(int64_t),
  SPerl_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET = 0,
};

// Parser information
struct sperl_ {
  
  // Memory_pool - This is compile time memory pool. This memory pool save short string and object except array, hash
  SPerl_MEMORY_POOL* ct_memory_pool;
  
  // Compile time arrays
  SPerl_ARRAY* ct_arrays;
  
  // Compile time hashes
  SPerl_ARRAY* ct_hashes;
  
  // Compile time long strings
  SPerl_ARRAY* ct_long_strings;
  
  // Parser
  SPerl_PARSER* parser;
  
  // Constant pool
  SPerl_CONSTANT_POOL* constant_pool;
  
  // Bytecodes
  SPerl_BYTECODE_ARRAY* bytecode_array;
  
  // Call stack
  SPerl_CALL_STACK_T* call_stack;

  // Call stack capacity default
  int32_t call_stack_capacity_default;
  
  // Call stack capacity
  int32_t call_stack_capacity;
  
  // Operand stack base
  int32_t call_stack_base;
  
  // Operand stack top
  int32_t operand_stack_top;

  // Alignment size
  int32_t alignment;
  
  // Exception
  _Bool abort;
};

SPerl* SPerl_new();
void SPerl_free(SPerl* sperl);

#endif
