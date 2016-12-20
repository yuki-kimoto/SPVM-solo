#include "sperl_type_part.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

uint8_t* const SPerl_TYPE_PART_C_CODE_NAMES[] = {
  "sub",
  "word",
  "char"
};

SPerl_TYPE_PART* SPerl_TYPE_PART_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_TYPE_PART));
}
