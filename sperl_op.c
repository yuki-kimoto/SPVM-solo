#include "sperl.h"
#include "sperl_op.h"
#include <malloc.h>

void* SPerl_Slab_Alloc(size_t sz) {
  /* TODO */
  return malloc(sizeof(char) * sz);
}

SPerl_OP* SPerl_newOP(I32 type, I32 flags) {
  SPerl_OP *o;
  
  SPerl_NewOp(o, 1, SPerl_OP);
  
  SPerl_OpTYPE_set(o, type);
  o->op_flags = (U8)flags;
  o->op_next = o;

  return o;;
}
