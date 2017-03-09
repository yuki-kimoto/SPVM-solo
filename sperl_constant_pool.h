#ifndef SPERL_CONSTANT_POOL_H
#define SPERL_CONSTANT_POOL_H

#include "sperl_base.h"

// Array
struct SPerl_constant_pool {
  int64_t* values;
  int64_t length;
  int64_t capacity;
};

// Array function
SPerl_CONSTANT_POOL* SPerl_CONSTANT_POOL_new(SPerl* sperl);
void SPerl_CONSTANT_POOL_extend(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int64_t extend);
void SPerl_CONSTANT_POOL_push_int(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int32_t value);
void SPerl_CONSTANT_POOL_push_long(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, int64_t value);
void SPerl_CONSTANT_POOL_push_float(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, float value);
void SPerl_CONSTANT_POOL_push_double(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, double value);
void SPerl_CONSTANT_POOL_push_string(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_CONSTANT* constant);
void SPerl_CONSTANT_POOL_push_sub(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_SUB* sub);
void SPerl_CONSTANT_POOL_push_package(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_PACKAGE* package);
void SPerl_CONSTANT_POOL_push_field(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool, SPerl_FIELD* field);

#endif
