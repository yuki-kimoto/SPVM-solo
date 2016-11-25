#include "sperl_type_component_sub.h"
#include "sperl_parser.h"

SPerl_TYPE_COMPONENT_SUB* SPerl_TYPE_COMPONENT_SUB_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_TYPE_COMPONENT_SUB));
}
