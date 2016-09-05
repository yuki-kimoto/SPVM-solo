#include "sperl.h"

#include "sperl_op.h"
#include <malloc.h>

SPerl_OP* SPerl_newOP(I32 type, I32 flags, SPerl_OP* first, SPerl_OP* last) {
        
  SPerl_OP *op;

  if (!first) {
    first = (SPerl_OP*)malloc(sizeof(SPerl_OP) * 1 );
    SPerl_OpTYPE_set(first, SPerl_OP_NULL);
  }
  
  op = (SPerl_OP*)malloc(sizeof(SPerl_OP) * 1);
  SPerl_OpTYPE_set(op, type);
  op->op_first = first;
  op->op_flags = (U8)(flags | SPerl_OPf_KIDS);
  op->op_private = (U8)(1 | (flags >> 8));
  
  if (last) {
    op->op_last = last;
    SPerl_OpMORESIB_set(first, last);
    if (op->op_last)
      SPerl_OpLASTSIB_set(op->op_last, (SPerl_OP*)op);
  }
  else {
    SPerl_OpLASTSIB_set(op->op_first, (SPerl_OP*)op);
  }

  return (SPerl_OP *)op;
}
