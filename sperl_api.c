#include <stdint.h>

#include "sperl.h"
#include "sperl_vm.h"

int64_t SPerl_API_get_array_length(SPerl* sperl, intptr_t array) {
  (void)sperl;
  return *(int64_t*)array;
}

int8_t* SPerl_API_get_byte_array_data(SPerl* sperl, intptr_t array) {
  (void)sperl;
  return array +  SPerl_C_ARRAY_HEADER_LENGTH;
}

int16_t* SPerl_API_get_short_array_data(SPerl* sperl, intptr_t array) {
  (void)sperl;
  return array +  SPerl_C_ARRAY_HEADER_LENGTH;
}

int32_t* SPerl_API_get_int_array_data(SPerl* sperl, intptr_t array) {
  (void)sperl;
  return array +  SPerl_C_ARRAY_HEADER_LENGTH;
}

int64_t* SPerl_API_get_long_array_data(SPerl* sperl, intptr_t array) {
  (void)sperl;
  return array +  SPerl_C_ARRAY_HEADER_LENGTH;
}

float* SPerl_API_get_float_array_data(SPerl* sperl, intptr_t array) {
  (void)sperl;
  return array +  SPerl_C_ARRAY_HEADER_LENGTH;
}

double* SPerl_API_get_double_array_data(SPerl* sperl, intptr_t array) {
  (void)sperl;
  return array +  SPerl_C_ARRAY_HEADER_LENGTH;
}
