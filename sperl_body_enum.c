#include "sperl_body_enum.h"
#include "sperl_parser.h"

SPerl_BODY_ENUM* SPerl_BODY_ENUM_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_BODY_ENUM));
}
