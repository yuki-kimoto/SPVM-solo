#ifndef SPERL_OPDEF_H
#define SPERL_OPDEF_H

#include "sperl_base.h"

enum {
  SPerl_OPDEF_C_CODE_IADD,
  SPerl_OPDEF_C_CODE_LADD,
  SPerl_OPDEF_C_CODE_FADD,
  SPerl_OPDEF_C_CODE_DADD,
  SPerl_OPDEF_C_CODE_ISUBTRACT,
  SPerl_OPDEF_C_CODE_LSUBTRACT,
  SPerl_OPDEF_C_CODE_FSUBTRACT,
  SPerl_OPDEF_C_CODE_DSUBTRACT,
  SPerl_OPDEF_C_CODE_IMULTIPLY,
  SPerl_OPDEF_C_CODE_LMULTIPLY,
  SPerl_OPDEF_C_CODE_FMULTIPLY,
  SPerl_OPDEF_C_CODE_DMULTIPLY,
  SPerl_OPDEF_C_CODE_FDIVIDE,
  SPerl_OPDEF_C_CODE_DDIVIDE
};

extern SPerl_char* const SPerl_OPDEF_C_CODE_NAMES[];

// Field information
struct SPerl_opdef {
  SPerl_char code;
  SPerl_TYPE* return_type;
  SPerl_ARRAY* argument_types;
  SPerl_int argument_count;
};

SPerl_OPDEF* SPerl_OPDEF_new(SPerl_PARSER* parser);

#endif
