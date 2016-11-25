#include "sperl_type_component_array.h"
#include "sperl_parser.h"

SPerl_TYPE_COMPONENT_ARRAY* SPerl_TYPE_COMPONENT_ARRAY_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_TYPE_COMPONENT_ARRAY));
}
