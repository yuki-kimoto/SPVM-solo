#include "sperl_body.h"
#include "sperl_parser.h"

SPerl_char* const SPerl_BODY_C_CODE_NAMES[] = {
  "class",
  "enum",
  "core"
};

SPerl_BODY* SPerl_BODY_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_BODY));
}
