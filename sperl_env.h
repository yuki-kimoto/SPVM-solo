#ifndef SPERL_ENV_H
#define SPERL_ENV_H

#include "sperl.h"

struct SPerl_env {
  int64_t* vars;
  int64_t* operand_stack;
};

SPerl_ENV* SPerl_ENV_new(SPerl* sperl);
int64_t SPerl_ENV_get_array_length(SPerl_ENV* env, intptr_t array);
int8_t* SPerl_ENV_get_byte_array_data(SPerl_ENV* env, intptr_t array);

#endif
