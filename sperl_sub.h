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
  char* abs_name;
  int64_t bytecode_base;
  int64_t bytecode_length;
  int64_t operand_stack_max;
  int64_t constant_pool_address;
  int64_t abs_name_constant_pool_address;
  _Bool is_native;
  _Bool is_constant;
};

SPerl_SUB* SPerl_SUB_new(SPerl* sperl);

#endif
