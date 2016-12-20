#include <string.h>
#include <stdlib.h>

#include "sperl_constant_pool.h"

SPerl_CONSTANT_POOL* SPerl_CONSTANT_POOL_new() {
  
  SPerl_CONSTANT_POOL* constant_pool = malloc(sizeof(SPerl_CONSTANT_POOL));
  constant_pool->capacity = 64;
  constant_pool->length = 0;
  
  int32_t* values = calloc(constant_pool->capacity, sizeof(int32_t));
  constant_pool->values = values;
  
  return constant_pool;
}

void SPerl_CONSTANT_POOL_push(SPerl_CONSTANT_POOL* constant_pool, int32_t value) {
  int32_t length = constant_pool->length;
  int32_t capacity = constant_pool->capacity;
  
  if (length >= capacity) {
    int32_t new_capacity = capacity * 2;
    constant_pool->values = realloc(constant_pool->values, new_capacity * sizeof(int32_t));
    memset(constant_pool->values + capacity, 0, (new_capacity - capacity) * sizeof(int32_t));
    constant_pool->capacity = new_capacity;
  }
  
  constant_pool->values[length] = value;
  constant_pool->length++;
}

void SPerl_CONSTANT_POOL_free(SPerl_CONSTANT_POOL* constant_pool) {
  free(constant_pool->values);
  free(constant_pool);
}
