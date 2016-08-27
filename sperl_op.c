#include "sperl.h"
#include "sperl_op.h"
#include <malloc.h>

#define SPerl_OpTYPE_set(o, type) \
  STMT_START {\
    o->op_type = (SPerl_OPCODE)type;\
    /* o->op_ppaddr = PL_ppaddr[type]; */\
  } STMT_END

/* TODO */
void * SPerl_Slab_Alloc(size_t sz)
{
    return malloc(sizeof(char) * sz);
}

