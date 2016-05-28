#define OP_CONST_INT 1
#define OP_CONST_FLOAT 2
#define OP_CONST_STRING 3
#define OP_LT 4
#define OP_LE 5
#define OP_GT 6
#define OP_GE 7
#define OP_ADD 8
#define OP_SUBTRACT 9
#define OP_MULTIPLY 10
#define OP_DIVIDE 11
#define OP_AND 12
#define OP_OR 13
#define OP_BIT_AND 14
#define OP_BIT_OR 15

#define BASEOP int type;

typedef struct yy_parser {
  /* Source data */
  char* linestr;

  /* Current buffer position */
  char* bufptr;
} yy_parser;

void yyerror(yy_parser* parser, const char* s);

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

typedef union
{
  OP* opval;
  int ival;
} YYSTYPE;
#define YYSTYPE_IS_DECLARED
