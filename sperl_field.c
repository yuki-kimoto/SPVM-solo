#include "sperl_field.h"
#include "sperl_parser.h"

SPerl_FIELD* SPerl_FIELD_new(SPerl_PARSER* parser) {
  return (SPerl_FIELD*)SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_FIELD));
}
