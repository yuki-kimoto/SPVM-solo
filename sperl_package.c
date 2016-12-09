#include "sperl_package.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_PACKAGE* SPerl_PACKAGE_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_PACKAGE));
}
