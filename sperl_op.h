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

/* Token(OP) type */
#define SPerl_BASEOP I32 type;

#define BASEOP\
  OP* op_next;\
  OP* op_sibparent;\
  OP* (*op_ppaddr)();\
  U8 op_type;\
  bool op_moresib;\
  U8 op_flags;\
#endif

/* Token(OP) expect ID */
enum SPerl_OP_EXPECT {
  SPerl_OP_EXPECT_NORMAL,
  SPerl_OP_EXPECT_WORD
};

typedef struct SPerl_op {
  SPerl_BASEOP
} SPerl_OP;

typedef struct {
  SPerl_BASEOP
  union {
    char* pv;
    int iv;
    double nv;
  } uv;
} SPerl_SVOP;

typedef struct {
    SPerl_BASEOP
    SPerl_OP*	op_first;
} SPerl_UNOP;

typedef struct {
    SPerl_BASEOP
    SPerl_OP *	op_first;
    SPerl_OP *	op_last;
} SPerl_BINOP;

typedef struct {
    SPerl_BASEOP
    SPerl_OP*	op_first;
    SPerl_OP*	op_last;
} SPerl_LISTOP;

#endif
