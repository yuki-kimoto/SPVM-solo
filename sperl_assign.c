#include "sperl_assign.h"
#include "sperl_parser.h"

SPerl_char* const SPerl_ASSIGN_C_CODE_NAMES[] = {
  "add",
  "subtract",
  "multiply",
  "divide",
  "modulo",
  "bit_xor",
  "or",
  "and"
};

SPerl_ASSIGN* SPerl_ASSIGN_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_ASSIGN));
}
