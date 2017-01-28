#ifndef SPERL_ARRAY_H
#define SPERL_ARRAY_H

#include "sperl_base.h"

// Array
struct SPerl_array {
  void** values;
  int32_t length;
  int32_t capacity;
};

// Array function
SPerl_ARRAY* SPerl_ARRAY_new(int32_t capacity);
void SPerl_ARRAY_push(SPerl_ARRAY* array, const void* value);
void* SPerl_ARRAY_fetch(SPerl_ARRAY* array, int32_t index);
void* SPerl_ARRAY_pop(SPerl_ARRAY* array);
void SPerl_ARRAY_free(SPerl_ARRAY* array);

#endif
