#include "sperl_my_var.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"
#include "sperl_parser.h"

SPerl_MY_VAR* SPerl_MY_VAR_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser->allocator, sizeof(SPerl_MY_VAR));
}
