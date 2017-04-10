#ifndef SPERL_BYTECODE_ARRAY_H
#define SPERL_BYTECODE_ARRAY_H

#include "sperl_base.h"

// Array
struct SPerl_bytecode_array {
  uint8_t* values;
  size_t length;
  size_t capacity;
};

// Array function
SPerl_BYTECODE_ARRAY* SPerl_BYTECODE_ARRAY_new(SPerl* sperl);
void SPerl_BYTECODE_ARRAY_push(SPerl* sperl, SPerl_BYTECODE_ARRAY* bytecodes, uint8_t value);
void SPerl_BYTECODE_ARRAY_free(SPerl* sperl, SPerl_BYTECODE_ARRAY* bytecodes);

#endif
