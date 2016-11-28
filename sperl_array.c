#include <string.h>
#include <stdlib.h>

#include "sperl_array.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl_int capacity) {
  
  SPerl_ARRAY* array = malloc(sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  void** values = calloc(array->capacity, sizeof(void*));
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_push(SPerl_ARRAY* array, void* value) {
  SPerl_int length = array->length;
  SPerl_int capacity = array->capacity;
  
  if (length >= capacity) {
    SPerl_int new_capacity = capacity * 2;
    array->values = realloc(array->values, new_capacity * sizeof(void*));
    memset(array->values + capacity, 0, (new_capacity - capacity) * sizeof(void*));
    array->capacity = new_capacity;
  }
  
  array->values[length] = value;
  array->length++;
}

void* SPerl_ARRAY_fetch(SPerl_ARRAY* array, SPerl_int index) {
  if (array == NULL || index < 0 || index >= array->length) {
    return NULL;
  }
  else {
    return array->values[index];
  }
}

void* SPerl_ARRAY_pop(SPerl_ARRAY* array) {
  SPerl_int length = array->length;
  
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
