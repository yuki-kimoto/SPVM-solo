#include "sperl_package.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"
#include "sperl_constant_pool.h"

SPerl_char* const SPerl_PACKAGE_C_CODE_NAMES[] = {
  "boolean",
  "char",
  "byte",
  "short",
  "int",
  "long",
  "float",
  "double"
};

SPerl_int const SPerl_PACKAGE_C_SIZES[] = {
  1,
  1,
  1,
  2,
  4,
  8,
  4,
  8
};

SPerl_PACKAGE* SPerl_PACKAGE_new(SPerl_PARSER* parser) {
  SPerl_PACKAGE* package = SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_PACKAGE));
  
  package->op_constants = SPerl_ALLOCATOR_new_array(parser, 0);
  package->constant_pool = SPerl_CONSTANT_POOL_new();
  package->op_subs = SPerl_ALLOCATOR_new_array(parser, 0);
  package->op_descripters = SPerl_ALLOCATOR_new_array(parser, 0);
  package->op_fields = SPerl_ALLOCATOR_new_array(parser, 0);
  package->field_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);
  
  return package;
}
