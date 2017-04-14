#include "sperl_enumeration_value.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"
#include "sperl_parser.h"

SPerl_ENUMERATION_VALUE* SPerl_ENUMERATION_VALUE_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser->allocator, sizeof(SPerl_ENUMERATION_VALUE));
}
