#include <string.h>
#include "sperl_resolved_type.h"
#include "sperl_parser.h"

SPerl_uchar* const SPerl_RESOLVED_TYPE_C_CODE_NAMES[] = {
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

SPerl_boolean SPerl_RESOLVED_TYPE_is_array(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type) {
  SPerl_int length = strlen(resolved_type->name);
  SPerl_uchar char1 = resolved_type->name[length - 2];
  SPerl_uchar char2 = resolved_type->name[length - 1];
  
  if (char1 == '[' && char2 == ']') {
    return 1;
  }
  else {
    return 0;
  }
}
