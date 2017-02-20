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
  int32_t bytecode_base;
  int32_t bytecode_length;
  int32_t operand_stack_max;
  int32_t constant_pool_address;
  _Bool is_native;
  _Bool is_constant;
};

SPerl_SUB* SPerl_SUB_new(SPerl* sperl);

#endif
