#include "sperl_var.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_VAR* SPerl_VAR_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_VAR));
}
