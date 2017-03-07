#include "sperl_enumeration.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"

SPerl_ENUMERATION* SPerl_ENUMERATION_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser, sizeof(SPerl_ENUMERATION));
}
