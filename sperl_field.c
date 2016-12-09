#include "sperl_field.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_FIELD* SPerl_FIELD_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_FIELD));
}
