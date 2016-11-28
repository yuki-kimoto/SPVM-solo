#include "sperl_op_info.h"
#include "sperl_parser.h"

SPerl_char* const SPerl_OP_INFO_C_CODE_NAMES[] = {
  "NULL",
  "D2F",
  "D2I",
  "D2L",
  "F2D",
  "F2I",
  "F2L",
  "I2B",
  "I2C",
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
};


SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_OP_INFO));
}
