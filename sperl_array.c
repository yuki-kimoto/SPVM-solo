#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl* sperl, int64_t capacity) {
  (void)sperl;
  
  assert(capacity >= 0);
  
  SPerl_ARRAY* array = SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  SPerl_VALUE_T* values = SPerl_ALLOCATOR_UTIL_safe_malloc(array->capacity, sizeof(SPerl_VALUE_T));
  
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_maybe_extend(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  assert(array);
  
  int64_t length = array->length;
  int64_t capacity = array->capacity;
  
  if (length >= capacity) {
    int64_t new_capacity = capacity * 2;
    array->values = SPerl_ALLOCATOR_UTIL_safe_realloc(array->values, new_capacity, sizeof(SPerl_VALUE_T));
    array->capacity = new_capacity;
  }
}

void SPerl_ARRAY_free(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  free(array->values);
  free(array);
}

void SPerl_ARRAY_push(SPerl* sperl, SPerl_ARRAY* array, SPerl_VALUE_T value) {
  
  SPerl_ARRAY_maybe_extend(sperl, array);
  
  int64_t length = array->length;
  
  *(SPerl_VALUE_T*)&array->values[length] = value;
  array->length++;
}

SPerl_VALUE_T SPerl_ARRAY_fetch(SPerl* sperl, SPerl_ARRAY* array, int64_t index) {
  (void)sperl;
  
  assert(array);
  assert(index >= 0);
  assert(index < array->length);
  
  return *(SPerl_VALUE_T*)&array->values[index];
}

void SPerl_ARRAY_store(SPerl* sperl, SPerl_ARRAY* array, int64_t index, SPerl_VALUE_T value) {
  (void)sperl;
  
  assert(array);
  assert(index >= 0);
  assert(index < array->length);
  
  *(SPerl_VALUE_T*)&array->values[index] = value;
}

SPerl_VALUE_T SPerl_ARRAY_pop(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  assert(array->length > 0);
  
  array->length--;
  
  return *(SPerl_VALUE_T*)&array->values[array->length];
}

// Implementation
// SUFFIX : long
// TYPE : int64_t
void SPerl_ARRAY_push_long(SPerl* sperl, SPerl_ARRAY* array, int64_t value) {
  
  SPerl_ARRAY_maybe_extend(sperl, array);
  
  int64_t length = array->length;
  
  *(int64_t*)&array->values[length] = value;
  array->length++;
}

int64_t SPerl_ARRAY_fetch_long(SPerl* sperl, SPerl_ARRAY* array, int64_t index) {
  (void)sperl;
  
  assert(array);
  assert(index >= 0);
  assert(index < array->length);
  
  return *(int64_t*)&array->values[index];
}

void SPerl_ARRAY_store_long(SPerl* sperl, SPerl_ARRAY* array, int64_t index, int64_t value) {
  (void)sperl;
  
  assert(array);
  assert(index >= 0);
  assert(index < array->length);
  
  *(int64_t*)&array->values[index] = value;
}

int64_t SPerl_ARRAY_pop_long(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  assert(array->length > 0);
  
  array->length--;
  
  return *(int64_t*)&array->values[array->length];
}
