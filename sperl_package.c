#include "sperl_package.h"
#include "sperl_parser.h"

SPerl_PACKAGE* SPerl_PACKAGE_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_PACKAGE));
}
