#include <string.h>
#include <stdlib.h>

#include "sperl_bytecodes.h"
#include "sperl_bytecode.h"

SPerl_BYTECODES* SPerl_BYTECODES_new() {
  
  SPerl_BYTECODES* bytecodes = malloc(sizeof(SPerl_BYTECODES));
  bytecodes->capacity = 64;
  bytecodes->length = 0;
  
  SPerl_char* values = calloc(bytecodes->capacity, sizeof(SPerl_char));
  bytecodes->values = values;
  
  return bytecodes;
}

void SPerl_BYTECODES_push(SPerl_BYTECODES* bytecodes, SPerl_char* value) {
  SPerl_int length = bytecodes->length;
  SPerl_int capacity = bytecodes->capacity;
  
  if (length >= capacity) {
    SPerl_int new_capacity = capacity * 2;
    bytecodes->values = realloc(bytecodes->values, new_capacity * sizeof(SPerl_char));
    memset(bytecodes->values + capacity, 0, (new_capacity - capacity) * sizeof(SPerl_char));
    bytecodes->capacity = new_capacity;
  }
  
  bytecodes->values[length] = *value;
  bytecodes->length++;
}

void SPerl_BYTECODES_free(SPerl_BYTECODES* bytecodes) {
  free(bytecodes->values);
  free(bytecodes);
}
