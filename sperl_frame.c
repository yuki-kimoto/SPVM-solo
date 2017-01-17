#include "sperl.h"
#include "sperl_allocator.h"
#include "sperl_frame.h"

SPerl_FRAME* SPerl_FRAME_new(SPerl* sperl) {
  SPerl_FRAME* frame = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_FRAME));
  
  return frame;
}
