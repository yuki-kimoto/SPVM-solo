#include "sperl_name.h"
#include "sperl_parser.h"

SPerl_uchar* const SPerl_NAME_C_CODE_NAMES[] = {
  "sub",
  "field",
  "enum"
};

SPerl_NAME* SPerl_NAME_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_NAME));
}
