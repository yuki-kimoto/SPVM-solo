#include "sperl_opdef.h"
#include "sperl_parser.h"

SPerl_char* const SPerl_OPDEF_C_CODE_NAMES[] = {
  "iadd",
  "ladd",
  "fadd",
  "dadd",
  "isubtract",
  "lsubtract",
  "fsubtract",
  "dsubtract",
  "imultiply",
  "lmultiply",
  "fmultiply",
  "dmultiply",
  "fdivide",
  "ddivide"
};

SPerl_OPDEF* SPerl_OPDEF_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_OPDEF));
}
