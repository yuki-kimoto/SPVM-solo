#include "sperl_use.h"
#include "sperl_parser.h"

SPerl_USE* SPerl_USE_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_USE));
}
