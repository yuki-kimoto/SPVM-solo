#include "sperl_enumeration_value.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"

SPerl_ENUMERATION_VALUE* SPerl_ENUMERATION_VALUE_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser, sizeof(SPerl_ENUMERATION_VALUE));
}
