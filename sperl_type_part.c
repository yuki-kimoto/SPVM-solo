#include "sperl_type_part.h"
#include "sperl_parser.h"

SPerl_char* const SPerl_TYPE_PART_C_CODE_NAMES[] = {
  "sub",
  "word",
  "char"
};

SPerl_TYPE_PART* SPerl_TYPE_PART_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_TYPE_PART));
}