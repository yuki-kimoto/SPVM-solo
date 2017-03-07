#include "sperl_switch_info.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"
#include "sperl_parser.h"

SPerl_SWITCH_INFO* SPerl_SWITCH_INFO_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser, sizeof(SPerl_SWITCH_INFO));
}
