#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_constant_pool.h"
#include "sperl_constant.h"
#include "sperl_hash.h"
#include "sperl_sub.h"
#include "sperl_field.h"
#include "sperl_package.h"
#include "sperl_constant_pool_sub.h"
#include "sperl_constant_pool_package.h"
#include "sperl_constant_pool_field.h"
#include "sperl_allocator.h"
#include "sperl_array.h"
#include "sperl_op.h"

SPerl_CONSTANT_POOL* SPerl_CONSTANT_POOL_new(SPerl* sperl) {
  (void)sperl;
  
  SPerl_CONSTANT_POOL* constant_pool = SPerl_ALLOCATOR_safe_malloc(1, sizeof(SPerl_CONSTANT_POOL));
  constant_pool->capacity = 64;
  constant_pool->length = 0;
  
  int32_t* values = SPerl_ALLOCATOR_safe_malloc_zero(constant_pool->capacity, sizeof(int32_t));
  constant_pool->values = values;
  
  return constant_pool;
}

void SPerl_CONSTANT_POOL_extend(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int32_t extend) {
  (void)sperl;
  
  int32_t capacity = constant_pool->capacity;
  
  if (constant_pool->length + extend >= capacity) {
    if (capacity > INT32_MAX / 2) {
      SPerl_ALLOCATOR_exit_with_malloc_failure();
    }
    int32_t new_capacity = capacity * 2;
    constant_pool->values = (int32_t*) SPerl_ALLOCATOR_safe_realloc(constant_pool->values, new_capacity, sizeof(int32_t));
    memset(constant_pool->values + capacity, 0, (new_capacity - capacity) * sizeof(int32_t));
    constant_pool->capacity = new_capacity;
  }
}

void SPerl_CONSTANT_POOL_push_constant(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_CONSTANT* constant) {
  (void)sperl;
  switch (constant->code) {
    case SPerl_CONSTANT_C_CODE_INT:
      SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, constant->uv.int_value);
      constant->address = constant_pool->length - 1;
      break;
    case SPerl_CONSTANT_C_CODE_LONG:
      SPerl_CONSTANT_POOL_push_long(sperl, constant_pool, constant->uv.long_value);
      constant->address = constant_pool->length - 2;
      break;
    case SPerl_CONSTANT_C_CODE_FLOAT:
      SPerl_CONSTANT_POOL_push_float(sperl, constant_pool, constant->uv.float_value);
      constant->address = constant_pool->length - 1;
      break;
    case SPerl_CONSTANT_C_CODE_DOUBLE:
      SPerl_CONSTANT_POOL_push_double(sperl, constant_pool, constant->uv.double_value);
      constant->address = constant_pool->length - 2;
      break;
    case SPerl_CONSTANT_C_CODE_STRING:
      constant->address = constant_pool->length;
      SPerl_CONSTANT_POOL_push_string(sperl, constant_pool, constant->uv.string_value);
      break;
  }
}

void SPerl_CONSTANT_POOL_push_package(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_PACKAGE* package) {
  (void)sperl;
  
  assert(sizeof(SPerl_CONSTANT_POOL_FIELD) <= sizeof(int32_t));

  // Constant pool package information
  package->constant_pool_address = constant_pool->length;
  
  // Constant pool package information
  SPerl_CONSTANT_POOL_PACKAGE* constant_pool_package = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_CONSTANT_POOL_PACKAGE));
  constant_pool_package->byte_size = package->byte_size;

  // Add package information
  int32_t extend_length = (sizeof(SPerl_CONSTANT_POOL_PACKAGE) + (sizeof(int32_t) - 1)) / sizeof(int32_t);
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, extend_length);
  *(SPerl_CONSTANT_POOL_PACKAGE*)&constant_pool->values[constant_pool->length] = *constant_pool_package;
  constant_pool->length += extend_length;
}

void SPerl_CONSTANT_POOL_push_sub(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_SUB* sub) {
  (void)sperl;
  
  // Adjust alignment
  if ((constant_pool->length % 2) != 0) {
    SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, 0);
  }
  assert((constant_pool->length % 2) == 0);

  // Constant pool sub information
  sub->constant_pool_address = constant_pool->length;
  // Constant pool sub information
  SPerl_CONSTANT_POOL_SUB* constant_pool_sub = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_CONSTANT_POOL_SUB));
  constant_pool_sub->native_address = (uintptr_t)sub->native_address;
  constant_pool_sub->bytecode_base = (uint32_t)sub->bytecode_base;
  constant_pool_sub->my_vars_length = (uint16_t)sub->op_my_vars->length;
  constant_pool_sub->operand_stack_max = (uint16_t)sub->operand_stack_max;
  constant_pool_sub->args_length = (uint8_t)sub->op_args->length;
  constant_pool_sub->is_native = (uint8_t)sub->is_native;
  if (sub->op_return_type->code != SPerl_OP_C_CODE_VOID) {
    constant_pool_sub->has_return_value = 1;
  }
  
  // Add sub information
  int32_t extend_length = (sizeof(SPerl_CONSTANT_POOL_SUB) + (sizeof(int32_t) - 1)) / sizeof(int32_t);
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, extend_length);
  *(SPerl_CONSTANT_POOL_SUB*)&constant_pool->values[constant_pool->length] = *constant_pool_sub;
  constant_pool->length += extend_length;
}

void SPerl_CONSTANT_POOL_push_field(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_FIELD* field) {
  (void)sperl;
  
  assert(sizeof(SPerl_CONSTANT_POOL_FIELD) <= sizeof(int32_t));
  
  // Constant pool field information
  field->constant_pool_address = constant_pool->length;
  
  // Constant pool field information
  SPerl_CONSTANT_POOL_FIELD* constant_pool_field = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_CONSTANT_POOL_FIELD));
  constant_pool_field->package_byte_offset = field->package_byte_offset;
  
  // Add field information
  int32_t extend_length = (sizeof(SPerl_CONSTANT_POOL_FIELD) + (sizeof(int32_t) - 1)) / sizeof(int32_t);
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, extend_length);
  *(SPerl_CONSTANT_POOL_FIELD*)&constant_pool->values[constant_pool->length] = *constant_pool_field;
  constant_pool->length += extend_length;
}

void SPerl_CONSTANT_POOL_push_int(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int32_t value) {
  (void)sperl;
  
  // Add int value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 1);
  constant_pool->values[constant_pool->length] = value;
  constant_pool->length++;
}

void SPerl_CONSTANT_POOL_push_long(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int64_t value) {
  (void)sperl;
  
  // Adjust alignment
  if ((constant_pool->length % 2) != 0) {
    SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, 0);
  }
  assert((constant_pool->length % 2) == 0);

  // Add long value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 2);
  *(int64_t*)&constant_pool->values[constant_pool->length] = value;
  constant_pool->length += 2;
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
  
  // Adjust alignment
  if ((constant_pool->length % 2) != 0) {
    SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, 0);
  }
  assert((constant_pool->length % 2) == 0);

  // Add double value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 2);
  *(double*)&constant_pool->values[constant_pool->length] = value;
  constant_pool->length += 2;
}

void SPerl_CONSTANT_POOL_push_string(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, const char* utf8) {
  
  SPerl_HASH* constant_utf8_symtable = sperl->parser->constant_utf8_symtable;
  
  SPerl_CONSTANT* found_constant_utf8 = SPerl_HASH_search(constant_utf8_symtable, utf8, strlen(utf8));
  
  // Already exists
  if (found_constant_utf8) {
    // Add string
    SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, found_constant_utf8->address);
  }
  else {
    // Create constant utf8
    SPerl_CONSTANT* new_constant_string = SPerl_CONSTANT_new(sperl);
    new_constant_string->code = SPerl_CONSTANT_C_CODE_STRING;
    new_constant_string->address = constant_pool->length;
    SPerl_HASH_insert(constant_utf8_symtable, utf8, strlen(utf8), new_constant_string);
    
    int32_t utf8_length = strlen(utf8);
    int32_t real_utf8_length = utf8_length + 1;
    
    // Calculate constant pool size
    int32_t constant_pool_size;
    if (real_utf8_length % sizeof(int32_t) == 0) {
      constant_pool_size = real_utf8_length / sizeof(int32_t);
    }
    else {
      constant_pool_size = (real_utf8_length / sizeof(int32_t)) + 1;
    }
    
    SPerl_CONSTANT_POOL_extend(sperl, constant_pool, constant_pool_size);
    memcpy(&constant_pool->values[constant_pool->length], utf8, utf8_length + 1);
    constant_pool->length += constant_pool_size;
    
    // Add string
    SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, new_constant_string->address);
  }
}

void SPerl_CONSTANT_POOL_free(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool) {
  (void)sperl;
  
  free(constant_pool->values);
  free(constant_pool);
}
