#ifndef SPERL_ASSIGN_H
#define SPERL_ASSIGN_H

#include "sperl_base.h"

enum {
  SPerl_ASSIGN_C_CODE_ADD,
  SPerl_ASSIGN_C_CODE_SUBTRACT,
  SPerl_ASSIGN_C_CODE_MULTIPLY,
  SPerl_ASSIGN_C_CODE_DIVIDE,
  SPerl_ASSIGN_C_CODE_MODULO,
  SPerl_ASSIGN_C_CODE_BIT_XOR,
  SPerl_ASSIGN_C_CODE_OR,
  SPerl_ASSIGN_C_CODE_AND,
};

extern SPerl_char* const SPerl_ASSIGN_C_CODE_NAMES[];

// Field information
struct SPerl_assign {
  SPerl_char code;
};

SPerl_ASSIGN* SPerl_ASSIGN_new(SPerl_PARSER* parser);

#endif
