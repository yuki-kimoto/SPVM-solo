#include <string.h>
#include <stdlib.h>

#include "sperl_array.h"
#include "sperl_allocator.h"

SPerl_ARRAY* SPerl_ARRAY_new(int32_t capacity) {
  
  SPerl_ARRAY* array = SPerl_ALLOCATOR_safe_malloc(1, sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  void** values = SPerl_ALLOCATOR_safe_malloc_zero(array->capacity, sizeof(void*));
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_push(SPerl_ARRAY* array, void* value) {
  int32_t length = array->length;
  int32_t capacity = array->capacity;
  
  if (length >= capacity) {
    int32_t new_capacity = capacity * 2;
    array->values = (void**) SPerl_ALLOCATOR_safe_realloc(array->values, new_capacity, sizeof(void*));
    memset(array->values + capacity, 0, (new_capacity - capacity) * sizeof(void*));
    array->capacity = new_capacity;
  }
  
  array->values[length] = value;
  array->length++;
}

void* SPerl_ARRAY_fetch(SPerl_ARRAY* array, int32_t index) {
  if (array == NULL || index < 0 || index >= array->length) {
    return NULL;
  }
  else {
    return array->values[index];
  }
}

void* SPerl_ARRAY_pop(SPerl_ARRAY* array) {
  array->length--;
  
  if (array->length < 0) {
    return NULL;
  }
  
  return array->values[array->length];
}

void SPerl_ARRAY_free(SPerl_ARRAY* array) {
  free(array->values);
  free(array);
}
