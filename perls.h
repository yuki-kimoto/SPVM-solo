#define OP_CONST_INT 1
#define OP_CONST_FLOAT 2
#define OP_CONST_STRING 3
#define OP_LT 4
#define OP_LE 5
#define OP_GT 6
#define OP_GE 7

#define BASEOP int type;

struct op {
  BASEOP
};
typedef struct op OP;

struct svop {
  BASEOP
  union {
    char* pv;
    int iv;
    double nv;
  } uv;
};
typedef struct svop SVOP;

struct unop {
    BASEOP
    OP *	op_first;
};
typedef struct unop UNOP;

struct binop {
    BASEOP
    OP *	op_first;
    OP *	op_last;
};
typedef struct binop BINOP;

struct listop {
    BASEOP
    OP *	op_first;
    OP *	op_last;
};
typedef struct binop LISTOP;

