#include "sperl_type_component_array.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"

SPerl_TYPE_COMPONENT_ARRAY* SPerl_TYPE_COMPONENT_ARRAY_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser, sizeof(SPerl_TYPE_COMPONENT_ARRAY));
}
