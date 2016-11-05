#include "sperl_callsub.h"
#include "sperl_parser.h"

SPerl_CALLSUB* SPerl_CALLSUB_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_CALLSUB));
}
