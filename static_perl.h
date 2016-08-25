#define STATIC_OP_CONST_INT 1
#define STATIC_OP_CONST_FLOAT 2
#define STATIC_OP_CONST_STRING 3
#define STATIC_OP_LT 4
#define STATIC_OP_LE 5
#define STATIC_OP_GT 6
#define STATIC_OP_GE 7
#define STATIC_OP_ADD 8
#define STATIC_OP_SUBTRACT 9
#define STATIC_OP_MULTIPLY 10
#define STATIC_OP_DIVIDE 11
#define STATIC_OP_AND 12
#define STATIC_OP_OR 13
#define STATIC_OP_BIT_AND 14
#define STATIC_OP_BIT_OR 15
#define STATIC_OP_MODULO 16
#define STATIC_OP_BIT_XOR 17
#define STATIC_OP_NOT 18
#define STATIC_OP_LEFT_SHIFT 19
#define STATIC_OP_RIGHT_SHIFT 20
#define STATIC_OP_INC 21
#define STATIC_OP_DEC 22
#define STATIC_OP_BIT_NOT 23
#define STATIC_OP_CONST_BOOL 24

#define BASEOP int type;

typedef struct {
  /* Source data */
  char* linestr;

  /* Current buffer position */
  char* bufptr;
} yy_parser;

void yyerror(yy_parser* parser, const char* s);

typedef struct {
  BASEOP
} OP;

typedef struct {
  BASEOP
  union {
    char* pv;
    int iv;
    double nv;
  } uv;
} SVOP;

typedef struct {
    BASEOP
    OP *	op_first;
} UNOP;

typedef struct {
    BASEOP
    OP *	op_first;
    OP *	op_last;
} BINOP;

typedef struct {
    BASEOP
    OP *	op_first;
    OP *	op_last;
} LISTOP;

typedef union
{
  OP* opval;
  int ival;
} yystype;

#define YYSTYPE yystype
