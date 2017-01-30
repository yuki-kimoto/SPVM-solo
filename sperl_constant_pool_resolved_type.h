#ifndef SPERL_CONSTANT_POOL_FIELD_H
#define SPERL_CONSTANT_POOL_FIELD_H

#include "sperl_base.h"

// Array
struct SPerl_constant_pool_sub {
  void (*native_address)(SPerl_VM* VM);
  int32_t args_length;
  int32_t my_vars_length;
  int32_t bytecode_base;
  int32_t operand_stack_max;
  _Bool has_return_value;
  _Bool is_native;
};

#endif
