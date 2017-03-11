#ifndef SPERL_CONSTANT_POOL_SUB_H
#define SPERL_CONSTANT_POOL_SUB_H

#include "sperl_base.h"

// Array
struct SPerl_constant_pool_sub {
  void* native_address;
  int32_t bytecode_base;
  int32_t abs_name_constant_pool_address;
  int32_t operand_stack_max;
  int32_t my_vars_length;
  int32_t args_length;
  int8_t is_native;
  int8_t has_return_value;
};

#endif
