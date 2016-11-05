#include "sperl_body_core.h"
#include "sperl_parser.h"

SPerl_char* const SPerl_BODY_CORE_C_CODE_NAMES[] = {
  "boolean",
  "char",
  "byte",
  "short",
  "int",
  "long",
  "float",
  "double"
};

SPerl_int const SPerl_BODY_CORE_C_SIZES[] = {
  1,
  1,
  1,
  2,
  4,
  8,
  4,
  8
};

SPerl_BODY_CORE* SPerl_BODY_CORE_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_BODY_CORE));
}
