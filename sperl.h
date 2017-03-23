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
  
  // Parser
  SPerl_PARSER* parser;
  
  // Constant pool
  SPerl_CONSTANT_POOL* constant_pool;
  
  // Constant pool package symbol table
  SPerl_HASH* constant_pool_package_symtable;

  // Constant pool field symbol table
  SPerl_HASH* constant_pool_field_symtable;

  // Constant pool subroutine symbol table
  SPerl_HASH* constant_pool_sub_symtable;
  
  // Bytecodes
  SPerl_BYTECODE_ARRAY* bytecode_array;
  
  // Call stack
  SPerl_VALUE_T* call_stack;

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
