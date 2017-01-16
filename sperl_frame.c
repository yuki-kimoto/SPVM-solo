#include "sperl.h"
#include "sperl_allocator.h"
#include "sperl_frame.h"

struct SPerl_frame {
  int32_t* openrad_stack;
  int32_t* call_stack;
};

SPerl_FRAME* SPerl_FRAME_new(SPerl* sperl) {
  SPerl_FRAME* frame = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_FRAME));
  
  return frame;
}
