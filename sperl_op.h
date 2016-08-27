#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl.h"

/* Operation code */
enum SPerl_OP_CODE {
  SPerl_OP_NULL,
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
};

/* Base Operation */
#define SPerl_BASEOP \
  SPerl_OP* op_next; \
  SPerl_OP* op_sibparent; \
  SPerl_OP* (*op_ppaddr)(); \
  U8 op_type; \
  U8 op_flags; \
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

#endif
