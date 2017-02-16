#include <assert.h>

#include "sperl_field.h"
#include "sperl.h"
#include "sperl_allocator.h"
#include "sperl_resolved_type.h"
#include "sperl_type.h"
#include "sperl_op.h"

SPerl_FIELD* SPerl_FIELD_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_FIELD));
}

int32_t SPerl_FIELD_get_byte_size(SPerl* sperl, SPerl_FIELD* field) {
  SPerl_RESOLVED_TYPE* field_resolved_type = field->op_type->uv.type->resolved_type;
  
  int32_t byte_size;
  if (field_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
    byte_size = 1;
  }
  else if (field_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
    byte_size = 2;
  }
  else if (field_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT || field_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
    byte_size = 4;
  }
  else if (field_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG || field_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
    byte_size = 8;
  }
  else {
    assert(0);
  }
  
  return byte_size;
}
