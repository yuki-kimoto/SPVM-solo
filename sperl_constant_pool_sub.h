#ifndef SPERL_CONSTANT_POOL_SUB_H
#define SPERL_CONSTANT_POOL_SUB_H

#include "sperl_base.h"

// Array
struct SPerl_constant_pool_sub {
  uintptr_t native_address;
  uint32_t bytecode_base;
  uint16_t operand_stack_max;
  uint16_t my_vars_length;
  uint8_t args_length;
  uint8_t is_native;
  uint8_t has_return_value;
};

#endif