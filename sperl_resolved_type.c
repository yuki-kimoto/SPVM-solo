#include "sperl_resolved_type.h"
#include "sperl_parser.h"

SPerl_char* const SPerl_RESOLVED_TYPE_C_CODE_NAMES[] = {
  "core",
  "class",
  "array",
  "sub"
};

SPerl_RESOLVED_TYPE* SPerl_RESOLVED_TYPE_new(SPerl_PARSER* parser) {
  SPerl_RESOLVED_TYPE* resolved_type = SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_RESOLVED_TYPE));
  
  resolved_type->part_names = SPerl_PARSER_new_array(parser, 0);
  
  return resolved_type;
}
