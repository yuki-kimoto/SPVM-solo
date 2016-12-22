#include "sperl_type_component_name.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_TYPE_COMPONENT_NAME* SPerl_TYPE_COMPONENT_NAME_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_TYPE_COMPONENT_NAME));
}
