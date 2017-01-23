#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_constant_pool.h"
#include "sperl_constant.h"
#include "sperl_hash.h"

SPerl_CONSTANT_POOL* SPerl_CONSTANT_POOL_new(SPerl* sperl) {
  (void)sperl;
  
  SPerl_CONSTANT_POOL* constant_pool = malloc(sizeof(SPerl_CONSTANT_POOL));
  constant_pool->capacity = 64;
  constant_pool->length = 0;
  
  int32_t* values = calloc(constant_pool->capacity, sizeof(int32_t));
  constant_pool->values = values;
  
  return constant_pool;
}

void SPerl_CONSTANT_POOL_extend(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int32_t extend) {
  (void)sperl;
  
  int32_t length = constant_pool->length;
  int32_t capacity = constant_pool->capacity;
  
  if (length + extend >= capacity) {
    int32_t new_capacity = capacity * 2;
    constant_pool->values = realloc(constant_pool->values, new_capacity * sizeof(int32_t));
    memset(constant_pool->values + capacity, 0, (new_capacity - capacity) * sizeof(int32_t));
    constant_pool->capacity = new_capacity;
  }
}

void SPerl_CONSTANT_POOL_push_constant(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_CONSTANT* constant) {
  (void)sperl;
  constant->address = constant_pool->length;
  switch (constant->code) {
    case SPerl_CONSTANT_C_CODE_BOOLEAN:
    case SPerl_CONSTANT_C_CODE_SHORT:
    case SPerl_CONSTANT_C_CODE_INT:
      SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, constant->uv.int_value);
      break;
    case SPerl_CONSTANT_C_CODE_LONG:
      SPerl_CONSTANT_POOL_push_long(sperl, constant_pool, constant->uv.long_value);
      break;
    case SPerl_CONSTANT_C_CODE_FLOAT:
      SPerl_CONSTANT_POOL_push_float(sperl, constant_pool, constant->uv.float_value);
      break;
    case SPerl_CONSTANT_C_CODE_DOUBLE:
      SPerl_CONSTANT_POOL_push_double(sperl, constant_pool, constant->uv.double_value);
      break;
    case SPerl_CONSTANT_C_CODE_STRING:
      SPerl_CONSTANT_POOL_push_string(sperl, constant_pool, constant->uv.string_value);
      break;
  }
}

void SPerl_CONSTANT_POOL_push_int(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int32_t value) {
  (void)sperl;
  
  int32_t length = constant_pool->length;
  
  // Add int value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 1);
  constant_pool->values[length] = value;
  constant_pool->length++;
}

void SPerl_CONSTANT_POOL_push_long(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int64_t value) {
  (void)sperl;
  
  int32_t length = constant_pool->length;

  // Adjust alignment
  if (length % 2 != 0) {
    SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, 0);
  }
  assert(constant_pool->length % 2 == 0);

  // Add long value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 2);
  memcpy(&constant_pool->values[length], &value, 8);
  constant_pool->length += 2;
}

void SPerl_CONSTANT_POOL_push_float(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, float value) {
  (void)sperl;
  
  int32_t length = constant_pool->length;
  
  // Add float value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 1);
  *(float*)&constant_pool->values[length] = value;
  constant_pool->length++;
}

void SPerl_CONSTANT_POOL_push_double(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, double value) {
  (void)sperl;
  
  int32_t length = constant_pool->length;

  // Adjust alignment
  if (length % 2 != 0) {
    SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, 0);
  }
  assert(constant_pool->length % 2 == 0);

  // Add double value
  SPerl_CONSTANT_POOL_extend(sperl, constant_pool, 2);
  memcpy(&constant_pool->values[length], &value, 8);
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
    int32_t length = constant_pool->length;
    
    // Create constant utf8
    SPerl_CONSTANT* new_constant_utf8 = SPerl_CONSTANT_new(sperl);
    new_constant_utf8->code = SPerl_CONSTANT_C_CODE_UTF8;
    new_constant_utf8->address = length;
    SPerl_HASH_insert(constant_utf8_symtable, utf8, strlen(utf8), new_constant_utf8);
    
    int32_t utf8_length = strlen(utf8);
    int32_t real_utf8_length = utf8_length + 1;
    
    // Calculate constant pool size
    int32_t constant_pool_size;
    if (real_utf8_length % 4 == 0) {
      constant_pool_size = real_utf8_length / 4;
    }
    else {
      constant_pool_size = (real_utf8_length / 4) + 1;
    }
    
    SPerl_CONSTANT_POOL_extend(sperl, constant_pool, constant_pool_size);
    memcpy(&constant_pool->values[length], utf8, utf8_length + 1);
    constant_pool->length += constant_pool_size;
    
    // Add string
    SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, new_constant_utf8->address);
  }
}

void SPerl_CONSTANT_POOL_free(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool) {
  (void)sperl;
  
  free(constant_pool->values);
  free(constant_pool);
}
