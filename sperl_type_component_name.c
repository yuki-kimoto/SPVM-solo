#include "sperl_type_component_name.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"
#include "sperl_parser.h"

SPerl_TYPE_COMPONENT_NAME* SPerl_TYPE_COMPONENT_NAME_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser->allocator, sizeof(SPerl_TYPE_COMPONENT_NAME));
}
