#include "sperl_use.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_USE* SPerl_USE_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_USE));
}
