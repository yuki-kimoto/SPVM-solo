#include "sperl_op_info.h"
#include "sperl_parser.h"

SPerl_char* const SPerl_OP_INFO_C_CODE_NAMES[] = {
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

SPerl_char* const SPerl_OP_INFO_C_RETURN_TYPE_NAMES[] = {
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
  "double"  // ddivide
};

SPerl_char* const SPerl_OP_INFO_C_ARGUMENT_TYPE_NAMES[14][2] = {
  {"int", "int"},       // iadd
  {"long", "long"},     // ladd
  {"float", "float"},   // fadd
  {"double", "double"}, // dadd
  {"int", "int"},       // isubtract
  {"long", "long"},     // lsubtract
  {"float", "float"},   // fsubtract
  {"double", "double"}, // dsubtract
  {"int", "int"},       // imultiply
  {"long", "long"},     // lmultiply
  {"float", "float"},   // fmultiply
  {"double", "double"}, // dmultiply
  {"float", "float"},   // fdivide
  {"double", "double"}  // ddivide
};

SPerl_int const SPerl_OP_INFO_C_ARGUMENT_COUNTS[] = {
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

SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_OP_INFO));
}
