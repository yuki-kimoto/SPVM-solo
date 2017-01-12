#include <string.h>
#include <stdlib.h>

#include "sperl_bytecode_array.h"

SPerl_BYTECODE_ARRAY* SPerl_BYTECODE_ARRAY_new() {
  
  SPerl_BYTECODE_ARRAY* bytecode_array = malloc(sizeof(SPerl_BYTECODE_ARRAY));
  bytecode_array->capacity = 64;
  bytecode_array->length = 0;
  
  uint8_t* values = calloc(bytecode_array->capacity, sizeof(uint8_t));
  bytecode_array->values = values;
  
  return bytecode_array;
}

void SPerl_BYTECODE_ARRAY_extend(SPerl_BYTECODE_ARRAY* bytecode_array, int32_t extend) {
  int32_t length = bytecode_array->length;
  int32_t capacity = bytecode_array->capacity;

  if (length >= capacity) {
    int32_t new_capacity = capacity * 2;
    bytecode_array->values = realloc(bytecode_array->values, new_capacity);
    memset(bytecode_array->values + capacity, 0, new_capacity - capacity);
    bytecode_array->capacity = new_capacity;
  }
}

void SPerl_BYTECODE_ARRAY_push_byte(SPerl_BYTECODE_ARRAY* bytecode_array, uint8_t value) {
  int32_t length = bytecode_array->length;
  int32_t capacity = bytecode_array->capacity;
  
  SPerl_BYTECODE_ARRAY_extend(bytecode_array, 1);
  
  bytecode_array->values[length] = value;
  bytecode_array->length++;
}

void SPerl_BYTECODE_ARRAY_free(SPerl_BYTECODE_ARRAY* bytecode_array) {
  free(bytecode_array->values);
  free(bytecode_array);
}
