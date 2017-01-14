#ifndef SPERL_BYTECODE_ARRAY_H
#define SPERL_BYTECODE_ARRAY_H

#include "sperl_base.h"

// Array
struct SPerl_bytecode_array {
  uint8_t* values;
  int32_t length;
  int32_t capacity;
};

// Array function
SPerl_BYTECODE_ARRAY* SPerl_BYTECODE_ARRAY_new();
void SPerl_BYTECODE_ARRAY_push(SPerl_BYTECODE_ARRAY* bytecodes, uint8_t value);

#endif
