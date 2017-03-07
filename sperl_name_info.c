#include "sperl_name_info.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"

SPerl_NAME_INFO* SPerl_NAME_INFO_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser, sizeof(SPerl_NAME_INFO));
}
