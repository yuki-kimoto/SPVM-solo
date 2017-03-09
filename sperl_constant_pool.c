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
#include "sperl_allocator_parser.h"
#include "sperl_allocator_util.h"
#include "sperl_array.h"
#include "sperl_op.h"

SPerl_CONSTANT_POOL* SPerl_CONSTANT_POOL_new(SPerl* sperl) {
  (void)sperl;
  
  SPerl_CONSTANT_POOL* constant_pool = SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_CONSTANT_POOL));
  constant_pool->capacity = 64;
  constant_pool->length = 0;
  
  int64_t* values = SPerl_ALLOCATOR_UTIL_safe_malloc_zero(constant_pool->capacity, sizeof(int64_t));
  constant_pool->values = values;
  
  return constant_pool;
}

void SPerl_CONSTANT_POOL_extend(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int64_t extend) {
  (void)sperl;
  
  int64_t capacity = constant_pool->capacity;
  
  if (constant_pool->length + extend >= capacity) {
    if (capacity > INT32_MAX / 2) {
      SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
    }
    int64_t new_capacity = capacity * 2;
    constant_pool->values = (int64_t*) SPerl_ALLOCATOR_UTIL_safe_realloc(constant_pool->values, new_capacity, sizeof(int64_t));
    memset(constant_pool->values + capacity, 0, (new_capacity - capacity) * sizeof(int64_t));
    constant_pool->capacity = new_capacity;
  }
}

void SPerl_CONSTANT_POOL_push_package(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_PACKAGE* package) {
  (void)sperl;
  
  assert(sizeof(SPerl_CONSTANT_POOL_FIELD) <= sizeof(int64_t));

  // Constant pool package information
  package->constant_pool_address = constant_pool->length;
  

  // Add package information
  int64_t extend_length = (sizeof(SPerl_CONSTANT_POOL_PACKAGE) + (sizeof(int64_t) - 1)) / sizeof(int64_t);
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, extend_length);

  // Constant pool package information
  SPerl_CONSTANT_POOL_PACKAGE* constant_pool_package = (SPerl_CONSTANT_POOL_PACKAGE*)&constant_pool->values[constant_pool->length];
  constant_pool_package->byte_size = package->byte_size;
  
  constant_pool->length += extend_length;
}

void SPerl_CONSTANT_POOL_push_sub(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_SUB* sub) {
  (void)sperl;
  
  // Constant pool sub information
  sub->constant_pool_address = constant_pool->length;
  // Constant pool sub information
  
  // Add sub information
  int64_t extend_length = (sizeof(SPerl_CONSTANT_POOL_SUB) + (sizeof(int64_t) - 1)) / sizeof(int64_t);
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, extend_length);
  
  SPerl_CONSTANT_POOL_SUB* constant_pool_sub = (SPerl_CONSTANT_POOL_SUB*)&constant_pool->values[constant_pool->length];
  constant_pool_sub->native_address = (uintptr_t)sub->native_address;
  constant_pool_sub->bytecode_base = (uint32_t)sub->bytecode_base;
  constant_pool_sub->my_vars_length = (uint16_t)sub->op_my_vars->length;
  constant_pool_sub->operand_stack_max = (uint16_t)sub->operand_stack_max;
  constant_pool_sub->args_length = (uint8_t)sub->op_args->length;
  constant_pool_sub->is_native = (uint8_t)sub->is_native;
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
  
  assert(sizeof(SPerl_CONSTANT_POOL_FIELD) <= sizeof(int64_t));
  
  // Constant pool field information
  field->constant_pool_address = constant_pool->length;
  
  // Add field information
  int64_t extend_length = (sizeof(SPerl_CONSTANT_POOL_FIELD) + (sizeof(int64_t) - 1)) / sizeof(int64_t);
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, extend_length);

  // Constant pool field information
  SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool->values[constant_pool->length];
  constant_pool_field->package_byte_offset = field->package_byte_offset;

  constant_pool->length += extend_length;
}

void SPerl_CONSTANT_POOL_push_int(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_CONSTANT* constant) {
  (void)sperl;
  
  warn("AAAAAAA %p", constant);
  
  int32_t value = constant->uv.int_value;
  
  if (value >= -32768 && value <= 32767) {
    constant->constant_pool_address = -1;
    return;
  }
  
  constant->constant_pool_address = constant_pool->length;
  
  // Add int value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 1);
  *(int32_t*)&constant_pool->values[constant_pool->length] = value;
  constant_pool->length++;
}

void SPerl_CONSTANT_POOL_push_long(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_CONSTANT* constant) {
  (void)sperl;
  
  int32_t value = constant->uv.long_value;
  
  if (value >= -32768 && value <= 32767) {
    constant->constant_pool_address = -1;
    return;
  }
  
  constant->constant_pool_address = constant_pool->length;
  
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

void SPerl_CONSTANT_POOL_push_string(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_CONSTANT* constant) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  const char* string = constant->uv.string_value;
  int64_t string_length = strlen(string);
  int32_t* address_ptr = SPerl_HASH_search(sperl, parser->string_literal_symtable, string, strlen(string));
  
  // Already exists
  if (address_ptr) {
    constant->constant_pool_address = *address_ptr;
  }
  else {
    
    constant->constant_pool_address = constant_pool->length;
    
    // Add string length
    SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 1);
    constant_pool->values[constant_pool->length] = string_length;
    constant_pool->length++;
    
    // Calculate constant pool size
    int64_t constant_pool_size;
    if (string_length % sizeof(int64_t) == 0) {
      constant_pool_size = string_length / sizeof(int64_t);
    }
    else {
      constant_pool_size = (string_length / sizeof(int64_t)) + 1;
    }
    
    SPerl_CONSTANT_POOL_extend(sperl, constant_pool, constant_pool_size);
    memcpy(&constant_pool->values[constant_pool->length], string, string_length);
    constant_pool->length += constant_pool_size;
    
    int32_t* new_address_ptr = SPerl_ALLOCATOR_PARSER_new_int(sperl, sperl->parser);
    *new_address_ptr = constant->constant_pool_address;

    SPerl_HASH_insert(sperl, parser->string_literal_symtable, string, string_length, new_address_ptr);
  }
}

void SPerl_CONSTANT_POOL_free(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool) {
  (void)sperl;
  
  free(constant_pool->values);
  free(constant_pool);
}
