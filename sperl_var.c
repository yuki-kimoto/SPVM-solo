#include "sperl_var.h"
#include "sperl_parser.h"

SPerl_VAR* SPerl_VAR_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_VAR));
}
