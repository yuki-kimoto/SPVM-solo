#include <stdint.h>

#include "sperl.h"

void SPerl_API_set_ret_byte(SPerl* sperl, int8_t value) {
  (void)sperl;
  
  *(int8_t*)&sperl->call_stack[sperl->operand_stack_top] = value;
}

void SPerl_API_set_ret_short(SPerl* sperl, int16_t value) {
  (void)sperl;
  
  *(int16_t*)&sperl->call_stack[sperl->operand_stack_top] = value;
}

void SPerl_API_set_ret_int(SPerl* sperl, int32_t value) {
  (void)sperl;
  
  *(int32_t*)&sperl->call_stack[sperl->operand_stack_top] = value;
}

void SPerl_API_set_ret_long(SPerl* sperl, int64_t value) {
  (void)sperl;
  
  *(int64_t*)&sperl->call_stack[sperl->operand_stack_top] = value;
}

void SPerl_API_set_ret_float(SPerl* sperl, float value) {
  (void)sperl;
  
  *(float*)&sperl->call_stack[sperl->operand_stack_top] = value;
}

void SPerl_API_set_ret_double(SPerl* sperl, double value) {
  (void)sperl;
  
  *(double*)&sperl->call_stack[sperl->operand_stack_top] = value;
}

void SPerl_API_set_ret_address(SPerl* sperl, intptr_t value) {
  (void)sperl;
  
  *(intptr_t*)&sperl->call_stack[sperl->operand_stack_top] = value;
}

int8_t SPerl_API_get_ret_byte(SPerl* sperl) {
  (void)sperl;
  
  return *(int8_t*)&sperl->call_stack[sperl->operand_stack_top];
}

int16_t SPerl_API_get_ret_short(SPerl* sperl) {
  (void)sperl;
  
  return *(int16_t*)&sperl->call_stack[sperl->operand_stack_top];
}

int32_t SPerl_API_get_ret_int(SPerl* sperl) {
  (void)sperl;
  
  return *(int32_t*)&sperl->call_stack[sperl->operand_stack_top];
}

int64_t SPerl_API_get_ret_long(SPerl* sperl) {
  (void)sperl;
  
  return *(int64_t*)&sperl->call_stack[sperl->operand_stack_top];
}

float SPerl_API_get_ret_float(SPerl* sperl) {
  (void)sperl;
  
  return *(float*)&sperl->call_stack[sperl->operand_stack_top];
}

double SPerl_API_get_ret_double(SPerl* sperl) {
  (void)sperl;
  
  return *(double*)&sperl->call_stack[sperl->operand_stack_top];
}

intptr_t SPerl_API_get_ret_address(SPerl* sperl) {
  (void)sperl;
  
  return *(intptr_t*)&sperl->call_stack[sperl->operand_stack_top];
}

int8_t SPerl_API_get_var_byte(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(int8_t*)&sperl->call_stack[sperl->call_stack_base + index];
}

int16_t SPerl_API_get_var_short(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(int16_t*)&sperl->call_stack[sperl->call_stack_base + index];
}

int32_t SPerl_API_get_var_int(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(int32_t*)&sperl->call_stack[sperl->call_stack_base + index];
}

int64_t SPerl_API_get_var_long(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(int64_t*)&sperl->call_stack[sperl->call_stack_base + index];
}

float SPerl_API_get_var_float(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(float*)&sperl->call_stack[sperl->call_stack_base + index];
}

double SPerl_API_get_var_double(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(double*)&sperl->call_stack[sperl->call_stack_base + index];
}

intptr_t SPerl_API_get_var_address(SPerl* sperl, int64_t index) {
  (void)sperl;
  
  return *(intptr_t*)&sperl->call_stack[sperl->call_stack_base + index];
}

void SPerl_API_set_var_byte(SPerl* sperl, int64_t index, int8_t value) {
  (void)sperl;
  
  *(int8_t*)&sperl->call_stack[sperl->call_stack_base + index] = value;
}

void SPerl_API_set_var_short(SPerl* sperl, int64_t index, int16_t value) {
  (void)sperl;
  
  *(int16_t*)&sperl->call_stack[sperl->call_stack_base + index] = value;
}

void SPerl_API_set_var_int(SPerl* sperl, int64_t index, int32_t value) {
  (void)sperl;
  
  *(int32_t*)&sperl->call_stack[sperl->call_stack_base + index] = value;
}

void SPerl_API_set_var_long(SPerl* sperl, int64_t index, int64_t value) {
  (void)sperl;
  
  *(int64_t*)&sperl->call_stack[sperl->call_stack_base + index] = value;
}

void SPerl_API_set_var_float(SPerl* sperl, int64_t index, float value) {
  (void)sperl;
  
  *(float*)&sperl->call_stack[sperl->call_stack_base + index] = value;
}

void SPerl_API_set_var_double(SPerl* sperl, int64_t index, double value) {
  (void)sperl;
  
  *(double*)&sperl->call_stack[sperl->call_stack_base + index] = value;
}

void SPerl_API_set_var_address(SPerl* sperl, int64_t index, intptr_t value) {
  (void)sperl;
  
  *(intptr_t*)&sperl->call_stack[sperl->call_stack_base + index] = value;
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
