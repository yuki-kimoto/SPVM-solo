#include "sperl_const_value.h"
#include "sperl_parser.h"

SPerl_CONST_VALUE* SPerl_CONST_VALUE_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_CONST_VALUE));
}
