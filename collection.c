#include <stdint.h>
#include <string.h>

#include "collection.h"

SPerl_ARRAY* SPerl_new_array(int64_t length) {
  
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


