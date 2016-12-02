#include "sperl_enumeration_value.h"
#include "sperl_parser.h"

SPerl_ENUMERATION_VALUE* SPerl_ENUMERATION_VALUE_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_ENUMERATION_VALUE));
}
