#include "sperl_descripter.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

uint8_t* const SPerl_DESCRIPTER_CODE_NAMES[] = {
  "const",
  "static",
  "value",
  "enum"
};

SPerl_DESCRIPTER* SPerl_DESCRIPTER_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_DESCRIPTER));
}
