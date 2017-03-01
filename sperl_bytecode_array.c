#include <string.h>
#include <stdlib.h>

#include "sperl_bytecode_array.h"
#include "sperl_allocator.h"

SPerl_BYTECODE_ARRAY* SPerl_BYTECODE_ARRAY_new(SPerl* sperl) {
  (void)sperl;
  
  SPerl_BYTECODE_ARRAY* bytecodes = SPerl_ALLOCATOR_safe_malloc(1, sizeof(SPerl_BYTECODE_ARRAY));
  bytecodes->capacity = 64;
  bytecodes->length = 0;
  
  uint8_t* values = SPerl_ALLOCATOR_safe_malloc_zero(bytecodes->capacity, sizeof(uint8_t));
  bytecodes->values = values;
  
  return bytecodes;
}

void SPerl_BYTECODE_ARRAY_push(SPerl* sperl, SPerl_BYTECODE_ARRAY* bytecodes, uint8_t value) {
  int32_t length = bytecodes->length;
  int32_t capacity = bytecodes->capacity;
  
  if (length >= capacity) {
    int32_t new_capacity = capacity * 2;
    bytecodes->values = (uint8_t*) SPerl_ALLOCATOR_safe_realloc(bytecodes->values, new_capacity, sizeof(uint8_t));
    memset(bytecodes->values + capacity, 0, (new_capacity - capacity) * sizeof(uint8_t));
    bytecodes->capacity = new_capacity;
  }
  bytecodes->values[length] = value;
  bytecodes->length++;
}

void SPerl_BYTECODE_ARRAY_free(SPerl* sperl, SPerl_BYTECODE_ARRAY* bytecodes) {
  free(bytecodes->values);
  free(bytecodes);
}
