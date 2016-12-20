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
void SPerl_CONSTANT_POOL_push(SPerl_CONSTANT_POOL* constant_pool, int value);

#endif
