#include <assert.h>

#include "sperl.h"
#include "sperl_sv.h"
#include "sperl_parser.h"
#include "sperl_allocator_parser.h"

SPerl_SV* SPerl_SV_new_pvn(SPerl* sperl, const char* pv, size_t length) {
  (void)sperl;
  
  SPerl_SV* sv = SPerl_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPerl_SV));
  
  sv->buffer = SPerl_ALLOCATOR_UTIL_safe_malloc_i32(length + 1, sizeof(int8_t));
  strncpy(sv->buffer, pv, length);
  sv->buffer[length] = '\0';
  
  sv->ref_count = 1;
  sv->length = length;
  
  return sv;
}
