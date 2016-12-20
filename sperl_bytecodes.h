#ifndef SPERL_BYTECODES_H
#define SPERL_BYTECODES_H

#include "sperl_base.h"

// Array
struct SPerl_bytecodes {
  int32_t length;
  int32_t capacity;
  uint8_t* values;
};

// Array function
SPerl_BYTECODES* SPerl_BYTECODES_new();
void SPerl_BYTECODES_push(SPerl_BYTECODES* bytecodes, uint8_t value);

#endif
