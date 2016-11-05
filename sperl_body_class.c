#include "sperl_body_class.h"
#include "sperl_parser.h"

SPerl_BODY_CLASS* SPerl_BODY_CLASS_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_BODY_CLASS));
}
