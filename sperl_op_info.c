#include "sperl_op_info.h"
#include "sperl_parser.h"






SPerl_uchar* const SPerl_OP_INFO_C_CODE_NAMES[] = {
  "NULL",
  "D2F",
  "D2I",
  "D2L",
  "F2D",
  "F2I",
  "F2L",
  "I2C",
  "I2UC",
  "I2D",
  "I2F",
  "I2L",
  "I2S",
  "L2D",
  "L2F",
  "L2I",
  "IADD",
  "LADD",
  "FADD",
  "DADD",
  "ISUBTRACT",
  "LSUBTRACT",
  "FSUBTRACT",
  "DSUBTRACT",
  "IMULTIPLY",
  "LMULTIPLY",
  "FMULTIPLY",
  "DMULTIPLY",
  "IDIVIDE",
  "LDIVIDE",
  "FDIVIDE",
  "DDIVIDE",
  "INEGATE",
  "LNEGATE",
  "FNEGATE",
  "DNEGATE",
  "IREMAINDER",
  "LREMAINDER",
  "FREMAINDER",
  "DREMAINDER",
  "IPLUS",
  "LPLUS",
  "FPLUS",
  "DPLUS",
  "IRETURN",
  "LRETURN",
  "FRETURN",
  "DRETURN",
  "ARETURN",
  "RETURN",
  "IAND",
  "LAND",
  "IOR",
  "LOR",
  "IXOR",
  "LXOR",
  "POP",
  "POP2",
};


SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_OP_INFO));
}
