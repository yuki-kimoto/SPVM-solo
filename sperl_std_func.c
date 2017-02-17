#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "sperl_std_func.h"
#include "sperl_env.h"

void SPerl_STD_FUNC_printb(SPerl_ENV* env) {
  
  int8_t value = *(int8_t*)&env->vars[0];
  
  printf("TEST: %" PRId8 "\n", value);
}

void SPerl_STD_FUNC_prints(SPerl_ENV* env) {
  
  int16_t value = *(int16_t*)&env->vars[0];
  
  printf("TEST: %" PRId16 "\n", value);
}

void SPerl_STD_FUNC_printi(SPerl_ENV* env) {
  
  int32_t value = *(int32_t*)&env->vars[0];
  
  printf("TEST: %" PRId32 "\n", value);
}

void SPerl_STD_FUNC_printl(SPerl_ENV* env) {
  
  int64_t value = env->vars[0];
  
  printf("TEST: %" PRId64 "\n", value);
}

void SPerl_STD_FUNC_printf(SPerl_ENV* env) {
  
  float value = *(float*)&env->vars[0];
  
  printf("TEST: %f\n", value);
}

void SPerl_STD_FUNC_printd(SPerl_ENV* env) {
  
  double value = *(double*)&env->vars[0];
  
  printf("TEST: %f\n", value);
}
