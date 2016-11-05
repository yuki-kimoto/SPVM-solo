#include "sperl_type_array.h"
#include "sperl_parser.h"

SPerl_TYPE_ARRAY* SPerl_TYPE_ARRAY_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_TYPE_ARRAY));
}
