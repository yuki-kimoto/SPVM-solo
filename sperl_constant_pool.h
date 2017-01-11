#ifndef SPERL_CONSTANT_POOL_H
#define SPERL_CONSTANT_POOL_H

#include "sperl_base.h"

// Array
struct SPerl_constant_pool {
  int32_t length;
  int32_t capacity;
  int32_t* values;
};

// Array function
SPerl_CONSTANT_POOL* SPerl_CONSTANT_POOL_new();
void SPerl_CONSTANT_POOL_extend(SPerl_CONSTANT_POOL* constant_pool, int32_t extend);
void SPerl_CONSTANT_POOL_push_int(SPerl_CONSTANT_POOL* constant_pool, int value);
void SPerl_CONSTANT_POOL_push_long(SPerl_CONSTANT_POOL* constant_pool, int64_t value);
void SPerl_CONSTANT_POOL_push_double(SPerl_CONSTANT_POOL* constant_pool, double value);

#endif
