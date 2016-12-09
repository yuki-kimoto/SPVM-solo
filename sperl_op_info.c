#include "sperl_op_info.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_OP_INFO));
}
