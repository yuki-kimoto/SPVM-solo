#ifndef SPERL_SUB_H
#define SPERL_SUB_H

#include "sperl_base.h"

// Method information
struct SPerl_sub {
  SPerl_OP* op_name;
  SPerl_OP* op_return_type;
  SPerl_OP* op_block;
  SPerl_ARRAY* op_args;
  SPerl_ARRAY* op_my_vars;
  void (*native_address)(SPerl* sperl);
  const char* abs_name;
  size_t bytecode_base;
  size_t bytecode_length;
  size_t operand_stack_max;
  size_t constant_pool_address;
  size_t abs_name_constant_pool_address;
  _Bool is_native;
  _Bool is_constant;
};

SPerl_SUB* SPerl_SUB_new(SPerl* sperl);

#endif
