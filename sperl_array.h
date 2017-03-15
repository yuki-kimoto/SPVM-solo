#ifndef SPERL_ARRAY_H
#define SPERL_ARRAY_H

#include "sperl_base.h"

// Array
struct SPerl_array {
  SPerl_VALUE_T* values;
  int64_t length;
  int64_t capacity;
};

// Array function
SPerl_ARRAY* SPerl_ARRAY_new(SPerl* sperl, int64_t capacity);
void SPerl_ARRAY_push_address(SPerl* sperl, SPerl_ARRAY* array, const void* value);
void* SPerl_ARRAY_fetch_address(SPerl* sperl, SPerl_ARRAY* array, int64_t index);
void* SPerl_ARRAY_pop_address(SPerl* sperl, SPerl_ARRAY* array);
void SPerl_ARRAY_free(SPerl* sperl, SPerl_ARRAY* array);
void SPerl_ARRAY_maybe_realloc(SPerl* sperl, SPerl_ARRAY* array);

#endif
