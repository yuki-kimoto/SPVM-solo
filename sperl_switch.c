#include "sperl_switch.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_SWITCH* SPerl_SWITCH_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_SWITCH));
}
