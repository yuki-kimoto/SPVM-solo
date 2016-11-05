#include "sperl_enum_value.h"
#include "sperl_parser.h"

SPerl_ENUM_VALUE* SPerl_ENUM_VALUE_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_ENUM_VALUE));
}
