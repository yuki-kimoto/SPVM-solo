#include "sperl_descripter.h"
#include "sperl_parser.h"

SPerl_char* const SPerl_DESCRIPTER_CODE_NAMES[] = {
  "const",
  "static",
  "value",
  "enum"
};

SPerl_DESCRIPTER* SPerl_DESCRIPTER_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_DESCRIPTER));
}
