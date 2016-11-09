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

SPerl_char* const SPerl_OPDEF_C_RETURN_TYPE_NAMES[] = {
  "int",    // iadd
  "long",   // ladd
  "float",  // fadd
  "double", // dadd
  "int",    // isubtract
  "long",   // lsubtract
  "float",  // fsubtract
  "double", // dsubtract
  "int",    // imultiply
  "long",   // lmultiply
  "float",  // fmultiply
  "double", // dmultiply
  "float",  // fdivide
  "double", // ddivide
};

SPerl_char* const SPerl_OPDEF_C_ARGUMENT_TYPE_NAMES[] = {
  "int",    // iadd
  "long",   // ladd
  "float",  // fadd
  "double", // dadd
  "int",    // isubtract
  "long",   // lsubtract
  "float",  // fsubtract
  "double", // dsubtract
  "int",    // imultiply
  "long",   // lmultiply
  "float",  // fmultiply
  "double", // dmultiply
  "float",  // fdivide
  "double", // ddivide
};

SPerl_int const SPerl_OPDEF_C_ARGUMENT_COUNTS[] = {
  2, // iadd
  2, // ladd
  2, // fadd
  2, // dadd
  2, // isubtract
  2, // lsubtract
  2, // fsubtract
  2, // dsubtract
  2, // imultiply
  2, // lmultiply
  2, // fmultiply
  2, // dmultiply
  2, // fdivide
  2  // ddivide
};

SPerl_OPDEF* SPerl_OPDEF_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_OPDEF));
}
