#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl* sperl, int32_t capacity) {
  (void)sperl;
  
  assert(capacity >= 0);
  
  SPerl_ARRAY* array = SPerl_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  void** values = SPerl_ALLOCATOR_UTIL_safe_malloc_i32(array->capacity, sizeof(void*));
  
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_maybe_extend(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  assert(array);
  
  int32_t length = array->length;
  int32_t capacity = array->capacity;
  
  if (length >= capacity) {
    int32_t new_capacity = capacity * 2;
    array->values = SPerl_ALLOCATOR_UTIL_safe_realloc_i32(array->values, new_capacity, sizeof(void*));
    array->capacity = new_capacity;
  }
}

void SPerl_ARRAY_free(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  free(array->values);
  free(array);
}

void SPerl_ARRAY_push(SPerl* sperl, SPerl_ARRAY* array, void* value) {
  
  SPerl_ARRAY_maybe_extend(sperl, array);
  
  int32_t length = array->length;
  
  *(void**)&array->values[length] = value;
  array->length++;
}

void* SPerl_ARRAY_fetch(SPerl* sperl, SPerl_ARRAY* array, int32_t index) {
  (void)sperl;
  
  assert(array);
  assert(index >= 0);
  assert(index < array->length);
  
  return *(void**)&array->values[index];
}

void SPerl_ARRAY_store(SPerl* sperl, SPerl_ARRAY* array, int32_t index, void* value) {
  (void)sperl;
  
  assert(array);
  assert(index >= 0);
  assert(index < array->length);
  
  *(void**)&array->values[index] = value;
}

void* SPerl_ARRAY_pop(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  assert(array->length >= 0);
  
  if (array->length == 0) {
    return NULL;
  }
  else {
    array->length--;
    return *(void**)&array->values[array->length];
  }
}
