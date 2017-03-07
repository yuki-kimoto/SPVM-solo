#include <stdlib.h>

#include "sperl_sub.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"
#include "sperl_bytecode_array.h"
#include "sperl_parser.h"

SPerl_SUB* SPerl_SUB_new(SPerl* sperl) {
  SPerl_SUB* sub = SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser, sizeof(SPerl_SUB));
  
  sub->op_args = SPerl_ALLOCATOR_PARSER_new_array(sperl, sperl->parser, 0);
  sub->op_my_vars = SPerl_ALLOCATOR_PARSER_new_array(sperl, sperl->parser, 0);
  
  return sub;
}
