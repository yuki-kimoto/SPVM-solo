#include <string.h>
#include <stdlib.h>

#include "sperl_bytecode_array.h"

SPerl_BYTECODE_ARRAY* SPerl_BYTECODE_ARRAY_new() {
  
  SPerl_BYTECODE_ARRAY* bytecodes = malloc(sizeof(SPerl_BYTECODE_ARRAY));
  bytecodes->capacity = 64;
  bytecodes->length = 0;
  
  uint8_t* values = calloc(bytecodes->capacity, sizeof(uint8_t));
  bytecodes->values = values;
  
  return bytecodes;
}

void SPerl_BYTECODE_ARRAY_push(SPerl_BYTECODE_ARRAY* bytecodes, uint8_t value) {
  int32_t length = bytecodes->length;
  int32_t capacity = bytecodes->capacity;
  
  if (length >= capacity) {
    int32_t new_capacity = capacity * 2;
    bytecodes->values = realloc(bytecodes->values, new_capacity * sizeof(uint8_t));
    memset(bytecodes->values + capacity, 0, (new_capacity - capacity) * sizeof(uint8_t));
    bytecodes->capacity = new_capacity;
  }
  bytecodes->values[length] = value;
  bytecodes->length++;
}

void SPerl_BYTECODE_ARRAY_free(SPerl_BYTECODE_ARRAY* bytecodes) {
  free(bytecodes->values);
  free(bytecodes);
}
