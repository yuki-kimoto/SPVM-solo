#ifndef SPERL_ARRAY_H
#define SPERL_ARRAY_H

#include "sperl_type.h"

// Array
struct SPerl_array {
  SPerl_int length;
  SPerl_int capacity;
  void** values;
};

// Array function
SPerl_ARRAY* SPerl_ARRAY_new(SPerl_int capacity);
void SPerl_ARRAY_push(SPerl_ARRAY* array, void* value);
void* SPerl_ARRAY_fetch(SPerl_ARRAY* array, SPerl_int index);

#endif
