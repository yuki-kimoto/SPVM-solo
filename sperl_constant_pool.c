#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_constant_pool.h"
#include "sperl_constant.h"
#include "sperl_package.h"
#include "sperl_field.h"
#include "sperl_sub.h"
#include "sperl_constant_pool_package.h"
#include "sperl_constant_pool_field.h"
#include "sperl_constant_pool_sub.h"
#include "sperl_allocator_util.h"
#include "sperl_array.h"
#include "sperl_op.h"

SPerl_CONSTANT_POOL* SPerl_CONSTANT_POOL_new(SPerl* sperl) {
  (void)sperl;
  
  SPerl_CONSTANT_POOL* constant_pool = SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_CONSTANT_POOL));
  constant_pool->capacity = 64;
  constant_pool->length = 0;
  
  SPerl_VALUE_SIZE_T* values = SPerl_ALLOCATOR_UTIL_safe_malloc_zero(constant_pool->capacity, sizeof(SPerl_VALUE_SIZE_T));
  constant_pool->values = values;
  
  return constant_pool;
}

int32_t SPerl_CONSTANT_POOL_calculate_extend_length(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, size_t byte_size) {
  
  int32_t length = (byte_size + (sizeof(SPerl_VALUE_SIZE_T) - 1)) / sizeof(SPerl_VALUE_SIZE_T);
  
  return length;
}

void SPerl_CONSTANT_POOL_extend(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int32_t extend) {
  (void)sperl;
  
  int32_t capacity = constant_pool->capacity;
  
  if (constant_pool->length + extend >= capacity) {
    if (capacity > INT32_MAX / 2) {
      SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
    }
    int32_t new_capacity = capacity * 2;
    constant_pool->values = (SPerl_VALUE_SIZE_T*) SPerl_ALLOCATOR_UTIL_safe_realloc(constant_pool->values, new_capacity, sizeof(SPerl_VALUE_SIZE_T));
    memset(constant_pool->values + capacity, 0, (new_capacity - capacity) * sizeof(SPerl_VALUE_SIZE_T));
    constant_pool->capacity = new_capacity;
  }
}

void SPerl_CONSTANT_POOL_push_package(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_PACKAGE* package) {
  (void)sperl;
  
  assert(sizeof(SPerl_CONSTANT_POOL_FIELD) <= sizeof(SPerl_VALUE_SIZE_T));
  
  // Add package information
  int32_t extend_length = SPerl_CONSTANT_POOL_calculate_extend_length(sperl, constant_pool, sizeof(SPerl_CONSTANT_POOL_PACKAGE));
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, extend_length);
  
  // Constant pool package information
  SPerl_CONSTANT_POOL_PACKAGE* constant_pool_package = (SPerl_CONSTANT_POOL_PACKAGE*)&constant_pool->values[constant_pool->length];
  constant_pool_package->byte_size = package->byte_size;
  constant_pool_package->name_constant_pool_address = package->name_constant_pool_address;
  
  constant_pool->length += extend_length;
}

void SPerl_CONSTANT_POOL_push_sub(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_SUB* sub) {
  (void)sperl;
  
  // Add sub information
  int32_t extend_length = SPerl_CONSTANT_POOL_calculate_extend_length(sperl, constant_pool, sizeof(SPerl_CONSTANT_POOL_SUB));
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, extend_length);
  
  SPerl_CONSTANT_POOL_SUB* constant_pool_sub = (SPerl_CONSTANT_POOL_SUB*)&constant_pool->values[constant_pool->length];
  constant_pool_sub->native_address = (uintptr_t)sub->native_address;
  constant_pool_sub->bytecode_base = (uint32_t)sub->bytecode_base;
  constant_pool_sub->my_vars_length = (uint16_t)sub->op_my_vars->length;
  constant_pool_sub->operand_stack_max = (uint16_t)sub->operand_stack_max;
  constant_pool_sub->args_length = (uint8_t)sub->op_args->length;
  constant_pool_sub->is_native = (uint8_t)sub->is_native;
  constant_pool_sub->abs_name_constant_pool_address = sub->abs_name_constant_pool_address;
  if (sub->op_return_type->code != SPerl_OP_C_CODE_VOID) {
    constant_pool_sub->has_return_value = 1;
  }
  else {
    constant_pool_sub->has_return_value = 0;
  }
  
  constant_pool->length += extend_length;
}

void SPerl_CONSTANT_POOL_push_field(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_FIELD* field) {
  (void)sperl;
  
  assert(sizeof(SPerl_CONSTANT_POOL_FIELD) <= sizeof(SPerl_VALUE_SIZE_T));
  
  // Add field information
  int32_t extend_length = SPerl_CONSTANT_POOL_calculate_extend_length(sperl, constant_pool, sizeof(SPerl_CONSTANT_POOL_FIELD));
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, extend_length);

  // Constant pool field information
  SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool->values[constant_pool->length];
  constant_pool_field->package_byte_offset = field->package_byte_offset;

  constant_pool->length += extend_length;
}

void SPerl_CONSTANT_POOL_push_int(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int32_t value) {
  (void)sperl;
  
  // Add int value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 1);
  *(int32_t*)&constant_pool->values[constant_pool->length] = value;
  constant_pool->length++;
}

void SPerl_CONSTANT_POOL_push_long(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int64_t value) {
  (void)sperl;
  
  // Add long value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 1);
  constant_pool->values[constant_pool->length] = value;
  constant_pool->length++;
}

void SPerl_CONSTANT_POOL_push_float(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, float value) {
  (void)sperl;

  // Add float value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 1);
  *(float*)&constant_pool->values[constant_pool->length] = value;
  constant_pool->length++;
}

void SPerl_CONSTANT_POOL_push_double(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, double value) {
  (void)sperl;
  
  // Add double value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 1);
  *(double*)&constant_pool->values[constant_pool->length] = value;
  constant_pool->length++;
}

void SPerl_CONSTANT_POOL_push_string(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, const char* string) {
  
  int32_t string_length = strlen(string);
  
  // Add string length
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 1);
  constant_pool->values[constant_pool->length] = string_length;
  constant_pool->length++;
  
  int32_t extend_length = SPerl_CONSTANT_POOL_calculate_extend_length(sperl, constant_pool, string_length);
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, extend_length);
  memcpy(&constant_pool->values[constant_pool->length], string, string_length);
  constant_pool->length += extend_length;
}

void SPerl_CONSTANT_POOL_free(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool) {
  (void)sperl;
  
  free(constant_pool->values);
  free(constant_pool);
}
