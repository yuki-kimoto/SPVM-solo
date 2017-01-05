#ifndef SPERL_BYTECODE_ARRAY_H
#define SPERL_BYTECODE_ARRAY_H

#include "sperl_base.h"

// Array
struct SPerl_bytecode_array {
  int32_t length;
  int32_t capacity;
  uint8_t* values;
};

// Array function
SPerl_BYTECODE_ARRAY* SPerl_BYTECODE_ARRAY_new();
void SPerl_BYTECODE_ARRAY_push(SPerl_BYTECODE_ARRAY* bytecodes, uint8_t value);

#endif
