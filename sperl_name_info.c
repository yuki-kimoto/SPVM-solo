#include "sperl_name_info.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_NAME_INFO* SPerl_NAME_INFO_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_NAME_INFO));
}
