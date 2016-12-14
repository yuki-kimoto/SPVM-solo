#include "sperl_name.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_NAME* SPerl_NAME_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_NAME));
}
