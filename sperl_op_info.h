#ifndef SPERL_OP_INFO_H
#define SPERL_OP_INFO_H

#include "sperl_base.h"




enum {
  SPerl_OP_INFO_C_CODE_NULL,
  SPerl_OP_INFO_C_CODE_D2F,          // UNOP
  SPerl_OP_INFO_C_CODE_D2I,          // UNOP
  SPerl_OP_INFO_C_CODE_D2L,          // UNOP
  SPerl_OP_INFO_C_CODE_F2D,          // UNOP
  SPerl_OP_INFO_C_CODE_F2I,          // UNOP
  SPerl_OP_INFO_C_CODE_F2L,          // UNOP
  SPerl_OP_INFO_C_CODE_I2B,          // UNOP
  SPerl_OP_INFO_C_CODE_I2C,          // UNOP
  SPerl_OP_INFO_C_CODE_I2D,          // UNOP
  SPerl_OP_INFO_C_CODE_I2F,          // UNOP
  SPerl_OP_INFO_C_CODE_I2L,          // UNOP
  SPerl_OP_INFO_C_CODE_I2S,          // UNOP
  SPerl_OP_INFO_C_CODE_L2D,          // UNOP
  SPerl_OP_INFO_C_CODE_L2F,          // UNOP
  SPerl_OP_INFO_C_CODE_L2I,          // UNOP
  SPerl_OP_INFO_C_CODE_IADD,          // UNOP
  SPerl_OP_INFO_C_CODE_LADD,          // UNOP
  SPerl_OP_INFO_C_CODE_FADD,          // UNOP
  SPerl_OP_INFO_C_CODE_DADD,          // UNOP
};

extern SPerl_char* const SPerl_OP_INFO_C_CODE_NAMES[];

struct SPerl_op_info {
  SPerl_int code;
  SPerl_RESOLVED_TYPE* return_resolved_type;
  SPerl_ARRAY* argument_resolved_types;
  SPerl_int argument_count;
  SPerl_char* symbol;
};

SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser);

#endif
