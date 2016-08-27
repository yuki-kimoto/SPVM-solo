#include "sperl.h"
#include "sperl_op.h"
#include <malloc.h>

void* SPerl_Slab_Alloc(size_t sz) {
  /* TODO */
  return malloc(sizeof(char) * sz);
}
