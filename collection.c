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

void SPerl_array_push(SPerl_ARRAY* array, uintptr_t* element) {
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

void* SPerl_array_fetch(SPerl_ARRAY* array, int32_t index) {
  return (array->elements)[index];
}

int64_t SPerL_hash_func(uint8_t* str, int64_t len) {
  uint8_t* str_tmp = str;
  int64_t hash = 5381;
  while (len--) {
    /* hash * 33 + c */
    hash = ((hash << 5) + hash) + *str_tmp++;
  }
  return hash;
}
