#ifndef SPERL_BYTECODES_H
#define SPERL_BYTECODES_H

#include "sperl_base.h"

// Array
struct SPerl_bytecodes {
  SPerl_int length;
  SPerl_int capacity;
  SPerl_char* values;
};

// Array function
SPerl_BYTECODES* SPerl_BYTECODES_new();
void SPerl_BYTECODES_push(SPerl_BYTECODES* bytecodes, SPerl_char value);

#endif
