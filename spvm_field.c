#include <assert.h>

#include "spvm_field.h"
#include "spvm.h"
#include "spvm_allocator_parser.h"
#include "spvm_resolved_type.h"
#include "spvm_type.h"
#include "spvm_op.h"
#include "spvm_parser.h"

SPVM_FIELD* SPVM_FIELD_new(SPVM* spvm) {
  (void)spvm;
  
  return SPVM_ALLOCATOR_PARSER_alloc_memory_pool(spvm, spvm->parser->allocator, sizeof(SPVM_FIELD));
}
