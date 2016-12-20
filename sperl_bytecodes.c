#include <string.h>
#include <stdlib.h>

#include "sperl_bytecodes.h"

SPerl_BYTECODES* SPerl_BYTECODES_new() {
  
  SPerl_BYTECODES* bytecodes = malloc(sizeof(SPerl_BYTECODES));
  bytecodes->capacity = 64;
  bytecodes->length = 0;
  
  uint8_t* values = calloc(bytecodes->capacity, sizeof(uint8_t));
  bytecodes->values = values;
  
  return bytecodes;
}

void SPerl_BYTECODES_push(SPerl_BYTECODES* bytecodes, uint8_t value) {
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

void SPerl_BYTECODES_free(SPerl_BYTECODES* bytecodes) {
  free(bytecodes->values);
  free(bytecodes);
}
