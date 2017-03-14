#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl* sperl, int64_t capacity) {
  
  SPerl_ARRAY* array = SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  intptr_t* values = SPerl_ALLOCATOR_UTIL_safe_malloc(array->capacity, sizeof(intptr_t));
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_push(SPerl* sperl, SPerl_ARRAY* array, intptr_t value) {

  int64_t length = array->length;
  assert(length >= 0);
  
  int64_t capacity = array->capacity;
  
  if (length >= capacity) {
    if (capacity > SIZE_MAX / 2) {
      SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
    }
    int64_t new_capacity = capacity * 2;
    array->values = (intptr_t*) SPerl_ALLOCATOR_UTIL_safe_realloc(array->values, new_capacity, sizeof(intptr_t));
    array->capacity = new_capacity;
  }
  
  /* Casting away a const qualification, I know what I'm doing. */
  array->values[length] = value;
  array->length++;
}

intptr_t SPerl_ARRAY_fetch(SPerl* sperl, SPerl_ARRAY* array, int64_t index) {
  (void)sperl;
  
  assert(index >= 0);
  
  if (array == NULL || index >= array->length) {
    return NULL;
  }
  else {
    return array->values[index];
  }
}

intptr_t SPerl_ARRAY_pop(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  if (array->length == 0) {
    return NULL;
  }

  array->length--;
  
  assert(array->length >= 0);
  
  return array->values[array->length];
}

void SPerl_ARRAY_free(SPerl* sperl, SPerl_ARRAY* array) {
  free(array->values);
  free(array);
}
