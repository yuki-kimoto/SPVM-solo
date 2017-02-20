#include <stdint.h>

#include "sperl.h"

int8_t SPerl_API_get_var_byte(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(int8_t*)&sperl->vars[index];
}

int16_t SPerl_API_get_var_short(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(int16_t*)&sperl->vars[index];
}

int32_t SPerl_API_get_var_int(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(int32_t*)&sperl->vars[index];
}

int64_t SPerl_API_get_var_long(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(int64_t*)&sperl->vars[index];
}

float SPerl_API_get_var_float(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(float*)&sperl->vars[index];
}

double SPerl_API_get_var_double(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(double*)&sperl->vars[index];
}

intptr_t SPerl_API_get_var_address(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(intptr_t*)&sperl->vars[index];
}

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
