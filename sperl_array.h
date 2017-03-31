#ifndef SPERL_ARRAY_H
#define SPERL_ARRAY_H

#include "sperl_base.h"

struct SPerl_array {
  void** values;
  int64_t length;
  int64_t capacity;
};

SPerl_ARRAY* SPerl_ARRAY_new(SPerl* sperl, int64_t capacity);
void SPerl_ARRAY_free(SPerl* sperl, SPerl_ARRAY* array);
void SPerl_ARRAY_maybe_extend(SPerl* sperl, SPerl_ARRAY* array);

void SPerl_ARRAY_push(SPerl* sperl, SPerl_ARRAY* array, void* value);
void* SPerl_ARRAY_fetch(SPerl* sperl, SPerl_ARRAY* array, int64_t index);
void SPerl_ARRAY_store(SPerl* sperl, SPerl_ARRAY* array, int64_t index, void* value);
void* SPerl_ARRAY_pop(SPerl* sperl, SPerl_ARRAY* array);

#endif
