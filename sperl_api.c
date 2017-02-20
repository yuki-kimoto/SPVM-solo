#include <stdint.h>

#include "sperl.h"

int8_t SPerl_API_get_ret_byte(SPerl* sperl) {
  (void)sperl;
  
  return *(int8_t*)sperl->ret;
}

int16_t SPerl_API_get_ret_short(SPerl* sperl) {
  (void)sperl;
  
  return *(int16_t*)sperl->ret;
}

int32_t SPerl_API_get_ret_int(SPerl* sperl) {
  (void)sperl;
  
  return *(int32_t*)sperl->ret;
}

int64_t SPerl_API_get_ret_long(SPerl* sperl) {
  (void)sperl;
  
  return *(int64_t*)sperl->ret;
}

float SPerl_API_get_ret_float(SPerl* sperl) {
  (void)sperl;
  
  return *(float*)sperl->ret;
}

double SPerl_API_get_ret_double(SPerl* sperl) {
  (void)sperl;
  
  return *(double*)sperl->ret;
}

intptr_t SPerl_API_get_ret_address(SPerl* sperl) {
  (void)sperl;
  
  return *(intptr_t*)sperl->ret;
}

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

void SPerl_API_set_var_byte(SPerl* sperl, int64_t index, int8_t value) {
  (void)sperl;
  
  *(int8_t*)&sperl->vars[index] = value;
}

void SPerl_API_set_var_short(SPerl* sperl, int64_t index, int16_t value) {
  (void)sperl;
  
  *(int16_t*)&sperl->vars[index] = value;
}

void SPerl_API_set_var_int(SPerl* sperl, int64_t index, int32_t value) {
  (void)sperl;
  
  *(int32_t*)&sperl->vars[index] = value;
}

void SPerl_API_set_var_long(SPerl* sperl, int64_t index, int64_t value) {
  (void)sperl;
  
  *(int64_t*)&sperl->vars[index] = value;
}

void SPerl_API_set_var_float(SPerl* sperl, int64_t index, float value) {
  (void)sperl;
  
  *(float*)&sperl->vars[index] = value;
}

void SPerl_API_set_var_double(SPerl* sperl, int64_t index, double value) {
  (void)sperl;
  
  *(double*)&sperl->vars[index] = value;
}

void SPerl_API_set_var_address(SPerl* sperl, int64_t index, intptr_t value) {
  (void)sperl;
  
  *(intptr_t*)&sperl->vars[index] = value;
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
