#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "collection.h"

SPerl_ARRAY* SPerl_new_array(int32_t length) {
  
  SPerl_ARRAY* array = (SPerl_ARRAY*)malloc(sizeof(SPerl_ARRAY));
  array->length = length;
  if (length > 32) {
    array->capacity = length;
  }
  else {
    array->capacity = 32;
  }
  
  int64_t elements_byte_size = sizeof(uintptr_t) * array->capacity;
  void* elements = malloc(elements_byte_size);
  memset(elements, 0, elements_byte_size);
  
  array->elements = elements;
  
  return array;
}

void SPerl_push_array(SPerl_ARRAY* array, uintptr_t* element) {
  int32_t length = array->length;
  int32_t capacity = array->capacity;
  
  length++;
  if (length > capacity) {
    capacity = capacity * 2;
    array->elements = realloc(array->elements, capacity);
    array->capacity = capacity;
  }
  
  (array->elements)[length - 1] = element;
}
