#include <stdlib.h>

#include "sperl_callsub.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_CALLSUB* SPerl_CALLSUB_new(SPerl_PARSER* parser) {
  return SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_CALLSUB));
}
