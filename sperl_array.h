#ifndef SPERL_ARRAY_H
#define SPERL_ARRAY_H

#include "sperl_base.h"

struct SPerl_array {
  SPerl_VALUE_T* values;
  int64_t length;
  int64_t capacity;
};

SPerl_ARRAY* SPerl_ARRAY_new(SPerl* sperl, int64_t capacity);
void SPerl_ARRAY_free(SPerl* sperl, SPerl_ARRAY* array);
void SPerl_ARRAY_maybe_extend(SPerl* sperl, SPerl_ARRAY* array);

void SPerl_ARRAY_push_address(SPerl* sperl, SPerl_ARRAY* array, void* value);
void* SPerl_ARRAY_fetch_address(SPerl* sperl, SPerl_ARRAY* array, int64_t index);
void* SPerl_ARRAY_pop_address(SPerl* sperl, SPerl_ARRAY* array);
void SPerl_ARRAY_store_address(SPerl* sperl, SPerl_ARRAY* array, int64_t index, void* value);

void SPerl_ARRAY_push_long(SPerl* sperl, SPerl_ARRAY* array, int64_t value);
int64_t SPerl_ARRAY_fetch_long(SPerl* sperl, SPerl_ARRAY* array, int64_t index);
int64_t SPerl_ARRAY_pop_long(SPerl* sperl, SPerl_ARRAY* array);
void SPerl_ARRAY_store_long(SPerl* sperl, SPerl_ARRAY* array, int64_t index, int64_t value);

#endif
