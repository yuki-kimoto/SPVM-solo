#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "sperl_func.h"
#include "sperl_api.h"
#include "sperl.h"

void SPerl_FUNC_std_suml(SPerl* sperl) {
  
  intptr_t long_array = SPerl_API_get_var_address(sperl, 0);
  
  int64_t length = SPerl_API_get_array_length(sperl, long_array);
  
  int64_t* long_array_data = SPerl_API_get_long_array_data(sperl, long_array);
  
  int64_t total = 0;
  for (int64_t i = 0; i < length; i++) {
    total += long_array_data[i];
  }
  
  *(int64_t*)sperl->ret = total;
}

void SPerl_FUNC_std_println(SPerl* sperl) {
  
  intptr_t byte_array = SPerl_API_get_var_address(sperl, 0);
  
  int64_t length = SPerl_API_get_array_length(sperl, byte_array);
  
  int8_t* byte_array_data = SPerl_API_get_byte_array_data(sperl, byte_array);
  
  for (size_t i = 0; i < length; i++) {
    putchar((int)byte_array_data[i]);
  }
  
  printf("\n");
}

void SPerl_FUNC_std_printb(SPerl* sperl) {
  
  int8_t value = SPerl_API_get_var_byte(sperl, 0);
  
  printf("TEST: %" PRId8 "\n", value);
}

void SPerl_FUNC_std_prints(SPerl* sperl) {
  
  int16_t value = SPerl_API_get_var_short(sperl, 0);
  
  printf("TEST: %" PRId16 "\n", value);
}

void SPerl_FUNC_std_printi(SPerl* sperl) {
  
  int32_t value = SPerl_API_get_var_int(sperl, 0);
  
  printf("TEST: %" PRId32 "\n", value);
}

void SPerl_FUNC_std_printl(SPerl* sperl) {
  
  int64_t value = SPerl_API_get_var_long(sperl, 0);
  
  printf("TEST: %" PRId64 "\n", value);
}

void SPerl_FUNC_std_printf(SPerl* sperl) {
  
  float value = SPerl_API_get_var_float(sperl, 0);
  
  printf("TEST: %f\n", value);
}

void SPerl_FUNC_std_printd(SPerl* sperl) {
  
  double value = SPerl_API_get_var_double(sperl, 0);
  
  printf("TEST: %f\n", value);
}