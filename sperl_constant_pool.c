#include <string.h>
#include <stdlib.h>

#include "sperl_constant_pool.h"

SPerl_CONSTANT_POOL* SPerl_CONSTANT_POOL_new() {
  
  SPerl_CONSTANT_POOL* constant_pool = malloc(sizeof(SPerl_CONSTANT_POOL));
  constant_pool->capacity = 64;
  
  SPerl_int* values = calloc(constant_pool->capacity, sizeof(SPerl_int));
  constant_pool->values = values;
  
  return constant_pool;
}

void SPerl_CONSTANT_POOL_push(SPerl_CONSTANT_POOL* constant_pool, SPerl_int value) {
  SPerl_int length = constant_pool->length;
  SPerl_int capacity = constant_pool->capacity;
  
  if (length >= capacity) {
    SPerl_int new_capacity = capacity * 2;
    constant_pool->values = realloc(constant_pool->values, new_capacity * sizeof(SPerl_int));
    memset(constant_pool->values + capacity, 0, (new_capacity - capacity) * sizeof(SPerl_int));
    constant_pool->capacity = new_capacity;
  }
  
  constant_pool->values[length] = value;
  constant_pool->length++;
}

void SPerl_CONSTANT_POOL_free(SPerl_CONSTANT_POOL* constant_pool) {
  free(constant_pool->values);
  free(constant_pool);
}
