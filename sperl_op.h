#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl.h"

/* Operation code */
enum SPerl_OP_CODE {
  SPerl_OP_NULL,
  SPerl_OP_STUB,
  SPerl_OP_CONST_INT,
  SPerl_OP_CONST_FLOAT,
  SPerl_OP_CONST_STRING,
  SPerl_OP_LT,
  SPerl_OP_LE,
  SPerl_OP_GT,
  SPerl_OP_GE,
  SPerl_OP_ADD,
  SPerl_OP_SUBTRACT,
  SPerl_OP_MULTIPLY,
  SPerl_OP_DIVIDE,
  SPerl_OP_AND,
  SPerl_OP_OR,
  SPerl_OP_BIT_AND,
  SPerl_OP_BIT_OR,
  SPerl_OP_MODULO,
  SPerl_OP_BIT_XOR,
  SPerl_OP_NOT,
  SPerl_OP_LEFT_SHIFT,
  SPerl_OP_RIGHT_SHIFT,
  SPerl_OP_INC,
  SPerl_OP_DEC,
  SPerl_OP_BIT_NOT,
  SPerl_OP_CONST_BOOL,
  SPerl_OP_PREINC
};

/* Base Operation */
#define SPerl_BASEOP \
  SPerl_OP* op_next; \
  SPerl_OP* op_sibparent; \
  SPerl_OP* (*op_ppaddr)(); \
  U8 op_type; \
  U8 op_flags; \
  U8 op_private; \
  U8 op_moresib;

/* Operation */
struct SPerl_op;
typedef struct SPerl_op SPerl_OP;
struct SPerl_op {
  SPerl_BASEOP
};

/* SV operation */
struct SPerl_svop;
typedef struct SPerl_svop SPerl_SVOP;
struct SPerl_svop {
  SPerl_BASEOP
  union {
    char* pv;
    int iv;
    double nv;
  } uv;
};

/* Unary operation */
struct SPerl_unop;
typedef struct SPerl_unop SPerl_UNOP;
struct SPerl_unop {
  SPerl_BASEOP
  SPerl_OP* op_first;
};

/* Binary operation */
struct SPerl_binop;
typedef struct SPerl_binop SPerl_BINOP;
struct SPerl_binop {
  SPerl_BASEOP
  SPerl_OP * op_first;
  SPerl_OP * op_last;
};

/* List operation */
struct SPerl_listop;
typedef struct SPerl_listop SPerl_LISTOP;
struct SPerl_listop {
  SPerl_BASEOP
  SPerl_OP* op_first;
  SPerl_OP* op_last;
};

/* Expected token */
enum SPerl_OP_EXPECT {
  SPerl_OP_EXPECT_NORMAL,
  SPerl_OP_EXPECT_WORD
};

#define SPerl_OpTYPE_set(o, type) \
  STMT_START {\
    o->op_type = (SPerl_OPCODE)type;\
    /* o->op_ppaddr = PL_ppaddr[type]; */\
  } STMT_END

#define SPerl_NewOp(var, c, type)	\
  (var = (type *) SPerl_Slab_Alloc(c * sizeof(type)))

/* TODO */
#define SPerl_op_lvalue(op, t) op

/* Define bool */
#ifndef HAS_BOOL
# ifdef bool
#  undef bool
# endif
# define bool char
# define HAS_BOOL 1
#endif

/* cast-to-bool.  A simple (bool) cast may not do the right thing: if bool is
 * defined as char for example, then the cast from int is
 * implementation-defined (bool)!!(cbool) in a ternary triggers a bug in xlc on
 * AIX */
#define SPerl_cBOOL(cbool) ((cbool) ? (bool)1 : (bool)0)


#define SPerl_OpHAS_SIBLING(o) (SPerl_cBOOL((o)->op_moresib))
#define SPerl_OpSIBLING(o) (0 + (o)->op_moresib ? (o)->op_sibparent : NULL)
#define SPerl_OpMORESIB_set(o, sib) ((o)->op_moresib = 1, (o)->op_sibparent = (sib))
#define SPerl_OpLASTSIB_set(o, parent) \
   ((o)->op_moresib = 0, (o)->op_sibparent = (parent))
#define SPerl_OpMAYBESIB_set(o, sib, parent) \
   ((o)->op_sibparent = ((o)->op_moresib = SPerl_cBOOL(sib)) ? (sib) : (parent))

#define SPerl_OPf_KIDS 4 /* There is a firstborn child. */

SPerl_OP* SPerl_newOP(I32 type, I32 flags);
SPerl_OP* SPerl_newUNOP(I32 type, I32 flags, SPerl_OP *first);
SPerl_OP* SPerl_newBINOP(I32 type, I32 flags, SPerl_OP *first, SPerl_OP *last);

#endif
