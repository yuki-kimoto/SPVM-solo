#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "sperl.h"
#include "sperl_func.h"
#include "sperl_api.h"
#include "sperl_env.h"

void SPerl_FUNC_std_test_call1(SPerl* sperl, SPerl_ENV* env) {
  (void)sperl;
  (void)env;
  
  int64_t value =  SPerl_API_get_var_long(sperl, env, 0);
  
  int64_t value2 = value * 2;
  
  // Call subroutine
  SPerl_ENV* new_env = SPerl_ENV_new(sperl);
  SPerl_API_push_var_long(sperl, new_env, value2);
  SPerl_API_call_sub(sperl, new_env, "std::test_call2");
  int64_t value3 = SPerl_API_pop_ret_long(sperl, new_env);
  SPerl_ENV_free(sperl, new_env);
  
  SPerl_API_push_ret_long(sperl, env, value3);
}

void SPerl_FUNC_std_test_call2(SPerl* sperl, SPerl_ENV* env) {
  (void)sperl;
  (void)env;
  
  int64_t value =  SPerl_API_get_var_long(sperl, env, 0);
  
  int64_t value2 = value * 3;
  
  SPerl_API_push_ret_long(sperl, env, value2);
}

void SPerl_FUNC_std_suml(SPerl* sperl, SPerl_ENV* env) {
  (void)sperl;
  (void)env;
  
  void* long_array = SPerl_API_get_var_ref(sperl, env, 0);
  
  int64_t length = SPerl_API_get_array_length(sperl, env, long_array);
  
  int64_t* long_array_data = SPerl_API_get_long_array_data(sperl, env, long_array);
  
  int64_t total = 0;
  for (int64_t i = 0; i < length; i++) {
    total += long_array_data[i];
  }
  
  SPerl_API_push_ret_long(sperl, env, total);
}

void SPerl_FUNC_std_println(SPerl* sperl, SPerl_ENV* env) {
  (void)sperl;
  (void)env;
  
  void* byte_array = SPerl_API_get_var_ref(sperl, env, 0);
  
  int64_t length = SPerl_API_get_array_length(sperl, env, byte_array);
  
  int8_t* byte_array_data = SPerl_API_get_byte_array_data(sperl, env, byte_array);
  
  for (int64_t i = 0; i < length; i++) {
    putchar((int)byte_array_data[i]);
  }
  
  printf("\n");
}

void SPerl_FUNC_std_printb(SPerl* sperl, SPerl_ENV* env) {
  (void)sperl;
  (void)env;
  
  int8_t value = SPerl_API_get_var_byte(sperl, env, 0);
  
  printf("TEST: %" PRId8 "\n", value);
}

void SPerl_FUNC_std_prints(SPerl* sperl, SPerl_ENV* env) {
  (void)sperl;
  (void)env;
  
  int16_t value = SPerl_API_get_var_short(sperl, env, 0);
  
  printf("TEST: %" PRId16 "\n", value);
}

void SPerl_FUNC_std_printi(SPerl* sperl, SPerl_ENV* env) {
  (void)sperl;
  (void)env;
  
  int32_t value = SPerl_API_get_var_int(sperl, env, 0);
  
  printf("TEST: %" PRId32 "\n", value);
}

void SPerl_FUNC_std_printl(SPerl* sperl, SPerl_ENV* env) {
  (void)sperl;
  (void)env;
  
  int64_t value = SPerl_API_get_var_long(sperl, env, 0);
  
  printf("TEST: %" PRId64 "\n", value);
}

void SPerl_FUNC_std_printf(SPerl* sperl, SPerl_ENV* env) {
  (void)sperl;
  (void)env;
  
  float value = SPerl_API_get_var_float(sperl, env, 0);
  
  printf("TEST: %f\n", value);
}

void SPerl_FUNC_std_printd(SPerl* sperl, SPerl_ENV* env) {
  (void)sperl;
  (void)env;
  
  double value = SPerl_API_get_var_double(sperl, env, 0);
  
  printf("TEST: %f\n", value);
}
