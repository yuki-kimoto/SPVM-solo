#include "sperl_type.h"
#include <string.h>
#include <stdlib.h>

#include "sperl_array.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl_long capacity) {
  
  SPerl_ARRAY* array = (SPerl_ARRAY*)malloc(sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  SPerl_long total_byte = sizeof(void*) * array->capacity;
  void** values = (void**)malloc(total_byte);
  memset(values, 0, total_byte);
  
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_push(SPerl_ARRAY* array, void* value) {
  SPerl_long length = array->length;
  SPerl_long capacity = array->capacity;
  
  if (length >= capacity) {
    SPerl_long new_capacity = capacity * 2;
    array->values = realloc(array->values, new_capacity);
    memset(array->values + capacity, 0, new_capacity - capacity);
    array->capacity = new_capacity;
  }
  
  array->values[length] = value;
  array->length++;
}

void* SPerl_ARRAY_fetch(SPerl_ARRAY* array, SPerl_long index) {
  if (array == NULL || index < 0 || index >= array->length) {
    return NULL;
  }
  else {
    return array->values[index];
  }
}
