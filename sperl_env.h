#ifndef SPERL_ENV_H
#define SPERL_ENV_H

#include "sperl.h"

struct SPerl_env {
  int64_t* vars;
  int64_t* ret;
};

SPerl_ENV* SPerl_ENV_new(SPerl* sperl);
int64_t SPerl_ENV_get_array_length(SPerl_ENV* env, intptr_t array);
int8_t* SPerl_ENV_get_byte_array_data(SPerl_ENV* env, intptr_t array);
int16_t* SPerl_ENV_get_short_array_data(SPerl_ENV* env, intptr_t array);
int32_t* SPerl_ENV_get_int_array_data(SPerl_ENV* env, intptr_t array);
int64_t* SPerl_ENV_get_long_array_data(SPerl_ENV* env, intptr_t array);
float* SPerl_ENV_get_float_array_data(SPerl_ENV* env, intptr_t array);
double* SPerl_ENV_get_double_array_data(SPerl_ENV* env, intptr_t array);

#endif
