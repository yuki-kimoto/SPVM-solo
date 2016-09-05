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

  return o;;
}

SPerl_OP* SPerl_newBINOP(I32 type, I32 flags, SPerl_OP* first, SPerl_OP* last) {
        
  SPerl_BINOP *binop;

  if (!first) {
    first = SPerl_newOP(SPerl_OP_NULL, 0);
  }
  
  SPerl_NewOp(binop, 1, SPerl_BINOP);
  SPerl_OpTYPE_set(binop, type);
  binop->op_first = first;
  binop->op_flags = (U8)(flags | SPerl_OPf_KIDS);
  binop->op_private = (U8)(1 | (flags >> 8));
  
  if (last) {
    binop->op_last = last;
    SPerl_OpMORESIB_set(first, last);
    if (binop->op_last)
      SPerl_OpLASTSIB_set(binop->op_last, (SPerl_OP*)binop);
  }
  else {
    SPerl_OpLASTSIB_set(binop->op_first, (SPerl_OP*)binop);
  }

  return (SPerl_OP *)binop;
}
