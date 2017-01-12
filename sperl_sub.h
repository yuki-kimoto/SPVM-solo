#ifndef SPERL_SUB_H
#define SPERL_SUB_H

#include "sperl_base.h"

// Method information
struct SPerl_sub {
  SPerl_OP* op_package;
  SPerl_OP* op_name;
  SPerl_OP* op_return_type;
  SPerl_ARRAY* op_args;
  int32_t args_size;
  SPerl_ARRAY* op_my_vars;
  int32_t my_vars_size;
  SPerl_OP* op_block;
  _Bool anon;
  int32_t id;
  int32_t bytecode_base;
  int32_t bytecode_length;
  int32_t operand_stack_max;
  int32_t constant_pool_base;
  int32_t constant_pool_length;
};

SPerl_SUB* SPerl_SUB_new(SPerl* sperl);

#endif
