#include "sperl_switch_info.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_SWITCH_INFO* SPerl_SWITCH_INFO_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_SWITCH_INFO));
}
