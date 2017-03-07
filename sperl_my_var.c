#include "sperl_my_var.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"

SPerl_MY_VAR* SPerl_MY_VAR_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser, sizeof(SPerl_MY_VAR));
}
