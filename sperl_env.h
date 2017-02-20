#ifndef SPERL_ENV_H
#define SPERL_ENV_H

#include "sperl.h"

struct SPerl_env {
  int64_t* vars;
  int64_t* ret;
  int64_t* call_stack;
  int32_t call_stack_capacity;
  int32_t call_stack_capacity_default;
  _Bool abort;
};

SPerl_ENV* SPerl_ENV_new(SPerl* sperl);
void SPerl_init_env(SPerl* sperl, SPerl_ENV* env);

#endif
