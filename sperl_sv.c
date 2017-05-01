#include <assert.h>

#include "sperl.h"
#include "sperl_sv.h"
#include "sperl_parser.h"
#include "sperl_allocator_parser.h"

SPerl_SV* SPerl_SV_new(SPerl* sperl) {
  (void)sperl;
  
  return SPerl_ALLOCATOR_UTIL_safe_malloc_i32(1, sizeof(SPerl_SV));
}
