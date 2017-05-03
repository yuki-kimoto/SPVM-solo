#include <assert.h>

#include "spvm.h"
#include "spvm_sv.h"
#include "spvm_parser.h"
#include "spvm_allocator_parser.h"

SPVM_SV* SPVM_SV_new_pvn(SPVM* spvm, const char* pv, size_t length) {
  (void)spvm;
  
  SPVM_SV* sv = SPVM_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPVM_SV));
  
  sv->buffer = SPVM_ALLOCATOR_UTIL_safe_malloc_i32(length + 1, sizeof(int8_t));
  strncpy(sv->buffer, pv, length);
  sv->buffer[length] = '\0';
  
  sv->ref_count = 1;
  sv->length = length;
  
  return sv;
}
