#ifndef SPERL_CONSTANT_POOL_SUB_H
#define SPERL_CONSTANT_POOL_SUB_H

#include "sperl_base.h"

// Array
struct SPerl_constant_pool_sub {
  void* native_address;
  int64_t bytecode_base;
  int64_t abs_name_constant_pool_address;
  int64_t operand_stack_max;
  int64_t my_vars_length;
  int64_t args_length;
  _Bool is_native;
  _Bool has_return_value;
};

#endif
