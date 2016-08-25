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
#define OP_MODULO 16
#define OP_BIT_XOR 17
#define OP_NOT 18
#define OP_LEFT_SHIFT 19
#define OP_RIGHT_SHIFT 20
#define OP_INC 21
#define OP_DEC 22
#define OP_BIT_NOT 23
#define OP_CONST_BOOL 24

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
