#include "sperl_package.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"
#include "sperl_constant_pool.h"
#include "sperl_parser.h"

SPerl_PACKAGE* SPerl_PACKAGE_new(SPerl* sperl) {
  SPerl_PACKAGE* package = SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser->allocator, sizeof(SPerl_PACKAGE));
  
  package->op_subs = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, sperl->parser->allocator, 0);
  package->op_fields = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, sperl->parser->allocator, 0);
  
  return package;
}
