#include "sperl_enumeration.h"
#include "sperl_parser.h"

SPerl_ENUMERATION* SPerl_ENUMERATION_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_ENUMERATION));
}
