#include <string.h>
#include <stdlib.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl* sperl, size_t capacity) {
  
  SPerl_ARRAY* array = SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  void** values = SPerl_ALLOCATOR_UTIL_safe_malloc(array->capacity, sizeof(void*));
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_push(SPerl* sperl, SPerl_ARRAY* array, const void* value) {

  size_t length = array->length;
  size_t capacity = array->capacity;
  
  if (length >= capacity) {
    if (capacity > SIZE_MAX / 2) {
      SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
    }
    size_t new_capacity = capacity * 2;
    array->values = (void**) SPerl_ALLOCATOR_UTIL_safe_realloc(array->values, new_capacity, sizeof(void*));
    array->capacity = new_capacity;
  }
  
  /* Casting away a const qualification, I know what I'm doing. */
  array->values[length] = (void*) value;
  array->length++;
}

void* SPerl_ARRAY_fetch(SPerl* sperl, SPerl_ARRAY* array, size_t index) {
  (void)sperl;
  
  if (array == NULL || index >= array->length) {
    return NULL;
  }
  else {
    return array->values[index];
  }
}

void* SPerl_ARRAY_pop(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  if (array->length == 0) {
    return NULL;
  }

  array->length--;
  
  return array->values[array->length];
}

void SPerl_ARRAY_free(SPerl* sperl, SPerl_ARRAY* array) {
  free(array->values);
  free(array);
}
