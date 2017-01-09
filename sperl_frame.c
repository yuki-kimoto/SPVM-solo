#include "sperl_frame.h"
#include "sperl.h"
#include "sperl_allocator.h"

SPerl_FRAME* SPerl_FRAME_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_FRAME));
}
