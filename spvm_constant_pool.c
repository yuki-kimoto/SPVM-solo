#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "spvm.h"
#include "spvm_constant_pool.h"
#include "spvm_constant.h"
#include "spvm_package.h"
#include "spvm_field.h"
#include "spvm_sub.h"
#include "spvm_constant_pool_package.h"
#include "spvm_constant_pool_field.h"
#include "spvm_constant_pool_sub.h"
#include "spvm_allocator_util.h"
#include "spvm_array.h"
#include "spvm_op.h"
#include "spvm_value.h"

SPVM_CONSTANT_POOL* SPVM_CONSTANT_POOL_new(SPVM* spvm) {
  (void)spvm;
  
  SPVM_CONSTANT_POOL* constant_pool = SPVM_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPVM_CONSTANT_POOL));
  
  constant_pool->int_capacity = 64;
  
  // index 0 is not used.
  constant_pool->int_length = 1;
  
  int32_t* int_values = SPVM_ALLOCATOR_UTIL_safe_malloc_i32(constant_pool->int_capacity, sizeof(int32_t));
  constant_pool->int_values = int_values;
  
  return constant_pool;
}

int32_t SPVM_CONSTANT_POOL_int_calculate_extend_length(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, int32_t byte_size) {
  (void)spvm;
  (void)constant_pool;
  
  int32_t length = (byte_size + (sizeof(int32_t) - 1)) / sizeof(int32_t);
  
  return length;
}

void SPVM_CONSTANT_POOL_int_extend(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, int32_t extend) {
  (void)spvm;
  
  int32_t int_capacity = constant_pool->int_capacity;
  
  if (constant_pool->int_length + extend >= int_capacity) {
    int32_t new_int_capacity = int_capacity * 2;
    constant_pool->int_values = SPVM_ALLOCATOR_UTIL_safe_realloc_i32(constant_pool->int_values, new_int_capacity, sizeof(int32_t));
    constant_pool->int_capacity = new_int_capacity;
  }
}

void SPVM_CONSTANT_POOL_push_package(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, SPVM_PACKAGE* package) {
  (void)spvm;
  
  // Extend
  int32_t int_extend_length = SPVM_CONSTANT_POOL_int_calculate_extend_length(spvm, constant_pool, sizeof(SPVM_CONSTANT_POOL_PACKAGE));
  SPVM_CONSTANT_POOL_int_extend(spvm, constant_pool, int_extend_length);

  // Constant pool package information
  SPVM_CONSTANT_POOL_PACKAGE int_constant_pool_package;
  int_constant_pool_package.byte_size = package->byte_size;
  int_constant_pool_package.name_constant_pool_address = package->name_constant_pool_address;
  int_constant_pool_package.ref_fields_count = package->ref_fields_count;
  memcpy(&constant_pool->int_values[constant_pool->int_length], &int_constant_pool_package, sizeof(SPVM_CONSTANT_POOL_PACKAGE));
  
  // Add length
  constant_pool->int_length += int_extend_length;
}

void SPVM_CONSTANT_POOL_push_sub(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, SPVM_SUB* sub) {
  (void)spvm;
  
  // Extend
  int32_t int_extend_length = SPVM_CONSTANT_POOL_int_calculate_extend_length(spvm, constant_pool, sizeof(SPVM_CONSTANT_POOL_SUB));
  SPVM_CONSTANT_POOL_int_extend(spvm, constant_pool, int_extend_length);

  // Set subroutine information
  SPVM_CONSTANT_POOL_SUB int_constant_pool_sub;
  int_constant_pool_sub.native_address = sub->native_address;
  int_constant_pool_sub.bytecode_base = sub->bytecode_base;
  int_constant_pool_sub.my_vars_length = sub->op_my_vars->length;
  int_constant_pool_sub.operand_stack_max = sub->operand_stack_max;
  int_constant_pool_sub.args_length = sub->op_args->length;
  int_constant_pool_sub.is_native = sub->is_native;
  int_constant_pool_sub.abs_name_constant_pool_address = sub->abs_name_constant_pool_address;
  int_constant_pool_sub.file_name_constant_pool_address = sub->file_name_constant_pool_address;
  if (sub->op_return_type->code != SPVM_OP_C_CODE_VOID) {
    int_constant_pool_sub.has_return_value = 1;
  }
  else {
    int_constant_pool_sub.has_return_value = 0;
  }
  memcpy(&constant_pool->int_values[constant_pool->int_length], &int_constant_pool_sub, sizeof(SPVM_CONSTANT_POOL_SUB));
  
  // Add length
  constant_pool->int_length += int_extend_length;
}

void SPVM_CONSTANT_POOL_push_field(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, SPVM_FIELD* field) {
  (void)spvm;
  
  // Extend
  int32_t int_extend_length = SPVM_CONSTANT_POOL_int_calculate_extend_length(spvm, constant_pool, sizeof(SPVM_CONSTANT_POOL_FIELD));
  SPVM_CONSTANT_POOL_int_extend(spvm, constant_pool, int_extend_length);
  
  // Constant pool field information
  SPVM_CONSTANT_POOL_FIELD int_constant_pool_field;
  int_constant_pool_field.package_byte_offset = field->package_byte_offset;
  memcpy(&constant_pool->int_values[constant_pool->int_length], &int_constant_pool_field, sizeof(SPVM_CONSTANT_POOL_FIELD));
  
  // Add length
  constant_pool->int_length += int_extend_length;
}

void SPVM_CONSTANT_POOL_push_int(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, int32_t value) {
  (void)spvm;
  
  // Add int value
  int32_t int_extend_length = SPVM_CONSTANT_POOL_int_calculate_extend_length(spvm, constant_pool, sizeof(int32_t));
  SPVM_CONSTANT_POOL_int_extend(spvm, constant_pool, int_extend_length);
  memcpy(&constant_pool->int_values[constant_pool->int_length], &value, sizeof(int32_t));
  constant_pool->int_length++;
}

void SPVM_CONSTANT_POOL_push_long(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, int64_t value) {
  (void)spvm;
  
  // Add long value
  int32_t int_extend_length = SPVM_CONSTANT_POOL_int_calculate_extend_length(spvm, constant_pool, sizeof(int64_t));
  SPVM_CONSTANT_POOL_int_extend(spvm, constant_pool, int_extend_length);
  memcpy(&constant_pool->int_values[constant_pool->int_length], &value, sizeof(int64_t));
  constant_pool->int_length++;
}

void SPVM_CONSTANT_POOL_push_float(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, float value) {
  (void)spvm;

  // Add float value
  int32_t int_extend_length = SPVM_CONSTANT_POOL_int_calculate_extend_length(spvm, constant_pool, sizeof(float));
  SPVM_CONSTANT_POOL_int_extend(spvm, constant_pool, int_extend_length);
  memcpy(&constant_pool->int_values[constant_pool->int_length], &value, sizeof(float));
  constant_pool->int_length++;
}

void SPVM_CONSTANT_POOL_push_double(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, double value) {
  (void)spvm;
  
  // Add double value
  int32_t int_extend_length = SPVM_CONSTANT_POOL_int_calculate_extend_length(spvm, constant_pool, sizeof(double));
  SPVM_CONSTANT_POOL_int_extend(spvm, constant_pool, int_extend_length);
  memcpy(&constant_pool->int_values[constant_pool->int_length], &value, sizeof(double));
  constant_pool->int_length++;
}

void SPVM_CONSTANT_POOL_push_string(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool, const char* string) {
  
  // Add string length
  int32_t int_string_length = (int32_t)strlen(string);
  SPVM_CONSTANT_POOL_int_extend(spvm, constant_pool, 1);
  memcpy(&constant_pool->int_values[constant_pool->int_length], &int_string_length, sizeof(int32_t));
  constant_pool->int_length++;
  
  // Add string data
  int32_t int_extend_length = SPVM_CONSTANT_POOL_int_calculate_extend_length(spvm, constant_pool, int_string_length + 1);
  SPVM_CONSTANT_POOL_int_extend(spvm, constant_pool, int_extend_length);
  memcpy(&constant_pool->int_values[constant_pool->int_length], string, int_string_length + 1);
  
  constant_pool->int_length += int_extend_length;
}

void SPVM_CONSTANT_POOL_free(SPVM* spvm, SPVM_CONSTANT_POOL* constant_pool) {
  (void)spvm;
  
  free(constant_pool->int_values);
  free(constant_pool);
}
