#ifndef STATIC_OP_H
#define STATIC_OP_H

/* Operation code */
enum STATIC_OP_CODE {
  STATIC_OP_NULL,
  STATIC_OP_CONST_INT,
  STATIC_OP_CONST_FLOAT,
  STATIC_OP_CONST_STRING,
  STATIC_OP_LT,
  STATIC_OP_LE,
  STATIC_OP_GT,
  STATIC_OP_GE,
  STATIC_OP_ADD,
  STATIC_OP_SUBTRACT,
  STATIC_OP_MULTIPLY,
  STATIC_OP_DIVIDE,
  STATIC_OP_AND,
  STATIC_OP_OR,
  STATIC_OP_BIT_AND,
  STATIC_OP_BIT_OR,
  STATIC_OP_MODULO,
  STATIC_OP_BIT_XOR,
  STATIC_OP_NOT,
  STATIC_OP_LEFT_SHIFT,
  STATIC_OP_RIGHT_SHIFT,
  STATIC_OP_INC,
  STATIC_OP_DEC,
  STATIC_OP_BIT_NOT,
  STATIC_OP_CONST_BOOL,
};

/* Token(OP) type */
#define STATIC_BASEOP int type;

/* Token(OP) expect ID */
enum STATIC_OP_EXPECT {
  STATIC_OP_EXPECT_NORMAL,
  STATIC_OP_EXPECT_WORD
};

typedef struct static_op {
  STATIC_BASEOP
} STATIC_OP;

typedef struct {
  STATIC_BASEOP
  union {
    char* pv;
    int iv;
    double nv;
  } uv;
} STATIC_SVOP;

typedef struct {
    STATIC_BASEOP
    STATIC_OP*	op_first;
} STATIC_UNOP;

typedef struct {
    STATIC_BASEOP
    STATIC_OP *	op_first;
    STATIC_OP *	op_last;
} STATIC_BINOP;

typedef struct {
    STATIC_BASEOP
    STATIC_OP*	op_first;
    STATIC_OP*	op_last;
} STATIC_LISTOP;

#endif
