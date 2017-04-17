#ifndef SPERL_ENV_H
#define SPERL_ENV_H

#include <stdio.h>

#include "sperl_base.h"

struct SPerl_env {
  // Call stack
  intmax_t* call_stack;
  
  // Call stack capacity
  int32_t call_stack_capacity;
  
  // Operand stack base
  int32_t call_stack_base;
  
  // Operand stack top
  int32_t operand_stack_top;
  
  // Exception
  _Bool abort;
};

SPerl_ENV* SPerl_ENV_new(SPerl* sperl);
void SPerl_ENV_free(SPerl* sperl, SPerl_ENV* env);

#endif
