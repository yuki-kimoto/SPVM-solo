#include "sperl_descriptor.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"
#include "sperl_parser.h"

const char* const SPerl_DESCRIPTOR_C_CODE_NAMES[] = {
  "native",
  "const",
};

SPerl_DESCRIPTOR* SPerl_DESCRIPTOR_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser->allocator, sizeof(SPerl_DESCRIPTOR));
}
