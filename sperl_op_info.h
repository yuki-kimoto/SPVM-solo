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
  SPerl_OP_INFO_C_CODE_ISUBTRACT,          // UNOP
  SPerl_OP_INFO_C_CODE_LSUBTRACT,          // UNOP
  SPerl_OP_INFO_C_CODE_FSUBTRACT,          // UNOP
  SPerl_OP_INFO_C_CODE_DSUBTRACT,          // UNOP
  SPerl_OP_INFO_C_CODE_IMULTIPLY,          // UNOP
  SPerl_OP_INFO_C_CODE_LMULTIPLY,          // UNOP
  SPerl_OP_INFO_C_CODE_FMULTIPLY,          // UNOP
  SPerl_OP_INFO_C_CODE_DMULTIPLY,          // UNOP
  SPerl_OP_INFO_C_CODE_IDIVIDE,          // UNOP
  SPerl_OP_INFO_C_CODE_LDIVIDE,          // UNOP
  SPerl_OP_INFO_C_CODE_FDIVIDE,          // UNOP
  SPerl_OP_INFO_C_CODE_DDIVIDE,          // UNOP
  SPerl_OP_INFO_C_CODE_INEGATE,          // UNOP
  SPerl_OP_INFO_C_CODE_LNEGATE,          // UNOP
  SPerl_OP_INFO_C_CODE_FNEGATE,          // UNOP
  SPerl_OP_INFO_C_CODE_DNEGATE,          // UNOP
  SPerl_OP_INFO_C_CODE_IREMAINDER,          // UNOP
  SPerl_OP_INFO_C_CODE_LREMAINDER,          // UNOP
  SPerl_OP_INFO_C_CODE_FREMAINDER,          // UNOP
  SPerl_OP_INFO_C_CODE_DREMAINDER,          // UNOP
  SPerl_OP_INFO_C_CODE_IPLUS,          // UNOP
  SPerl_OP_INFO_C_CODE_LPLUS,          // UNOP
  SPerl_OP_INFO_C_CODE_FPLUS,          // UNOP
  SPerl_OP_INFO_C_CODE_DPLUS,          // UNOP
  SPerl_OP_INFO_C_CODE_IRETURN,          // UNOP
  SPerl_OP_INFO_C_CODE_LRETURN,          // UNOP
  SPerl_OP_INFO_C_CODE_FRETURN,          // UNOP
  SPerl_OP_INFO_C_CODE_DRETURN,          // UNOP
  SPerl_OP_INFO_C_CODE_ARETURN,          // UNOP
  SPerl_OP_INFO_C_CODE_RETURN,          // UNOP
  SPerl_OP_INFO_C_CODE_POP,
  SPerl_OP_INFO_C_CODE_POP2,
};

extern SPerl_uchar* const SPerl_OP_INFO_C_CODE_NAMES[];

struct SPerl_op_info {
  SPerl_int code;
  SPerl_RESOLVED_TYPE* return_resolved_type;
  SPerl_ARRAY* argument_resolved_types;
  SPerl_int argument_count;
};

SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser);

#endif
