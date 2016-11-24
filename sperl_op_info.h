#ifndef SPERL_OP_INFO_H
#define SPERL_OP_INFO_H

#include "sperl_base.h"




// 14 count
enum {
  SPerl_OP_INFO_C_CODE_IADD,
  SPerl_OP_INFO_C_CODE_LADD,
  SPerl_OP_INFO_C_CODE_FADD,
  SPerl_OP_INFO_C_CODE_DADD,
  SPerl_OP_INFO_C_CODE_ISUBTRACT,
  SPerl_OP_INFO_C_CODE_LSUBTRACT,
  SPerl_OP_INFO_C_CODE_FSUBTRACT,
  SPerl_OP_INFO_C_CODE_DSUBTRACT,
  SPerl_OP_INFO_C_CODE_IMULTIPLY,
  SPerl_OP_INFO_C_CODE_LMULTIPLY,
  SPerl_OP_INFO_C_CODE_FMULTIPLY,
  SPerl_OP_INFO_C_CODE_DMULTIPLY,
  SPerl_OP_INFO_C_CODE_FDIVIDE,
  SPerl_OP_INFO_C_CODE_DDIVIDE
};

extern SPerl_char* const SPerl_OP_INFO_C_CODE_NAMES[];
extern SPerl_char* const SPerl_OP_INFO_C_RETURN_TYPE_NAMES[];
extern SPerl_char* const SPerl_OP_INFO_C_ARGUMENT_TYPE_NAMES[14][2];
extern SPerl_int const SPerl_OP_INFO_C_ARGUMENT_COUNTS[];

// Field information
struct SPerl_op_info {
  SPerl_int code;
  SPerl_TYPE* return_type;
  SPerl_ARRAY* argument_types;
  SPerl_int argument_count;
  SPerl_char* symbol;
};

SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser);

#endif
