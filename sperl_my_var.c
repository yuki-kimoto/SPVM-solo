#include "sperl_my_var.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_MY_VAR* SPerl_MY_VAR_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_MY_VAR));
}
