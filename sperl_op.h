#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl.h"
#include <stdint.h>

/* Operation code */
enum SPerl_OP_CODE {
  SPerl_OP_NULL,
  SPerl_OP_CONST_BOOLEAN,
  SPerl_OP_CONST_BYTE,
  SPerl_OP_CONST_SHORT,
  SPerl_OP_CONST_INT,
  SPerl_OP_CONST_LONG,
  SPerl_OP_CONST_FLOAT,
  SPerl_OP_CONST_DOUBLE,
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
  SPerl_OP_PREINC,
  SPerl_OP_POSTINC,
  SPerl_OP_PREDEC,
  SPerl_OP_POSTDEC,
  SPerl_OP_COMPLEMENT,
  SPerl_OP_NEGATE,
  SPerl_OP_LIST,
  SPerl_OP_PUSHMARK,
  SPerl_OP_AELEM,
  SPerl_OP_ATTR,
  SPerl_OP_ASSIGN,
  SPerl_OP_FUNC,
  SPerl_OP_SUBARG,
  SPerl_OP_ANONSUB,
  SPerl_OP_BLOCK,
  SPerl_OP_MY,
  SPerl_OP_OUR,
  SPerl_OP_HAS,
  SPerl_OP_COND,
  SPerl_OP_USE,
  SPerl_OP_RETURN,
  SPerl_OP_LAST,
  SPerl_OP_NEXT,
  SPerl_OP_PACKAGE,
  SPerl_OP_LOOP,
  SPerl_OP_SUB,
  SPerl_OP_VAR,
  SPerl_OP_EQ,
  SPerl_OP_NE,
  SPerl_OP_STATEMENT,
  SPerl_OP_GRAMMER
};

extern int8_t* const SPerl_op_name[];

/*
boolean	1bit
byte	8bit
short	16bit
int	32bit
float	32bit
long	64bit
double	64bit
*/

/* Binary operation */
struct SPerl_op;
typedef struct SPerl_op SPerl_OP;
struct SPerl_op {
  int8_t op_type;
  int8_t op_flags;
  int8_t op_private;
  int8_t op_moresib;
  SPerl_OP* op_first;
  SPerl_OP* op_last;
  SPerl_OP* op_sibparent;
  union {
    int8_t* pv;
    int32_t iv;
    double nv;
  } uv;
};

SPerl_OP* SPerl_OpSIBLING(SPerl_OP* o);
void SPerl_OpMORESIB_set(SPerl_OP* o, SPerl_OP* sib);
void SPerl_OpLASTSIB_set(SPerl_OP* o, SPerl_OP* parent);
void SPerl_OpMAYBESIB_set(SPerl_OP* o, SPerl_OP* sib, SPerl_OP* parent);

SPerl_OP* SPerl_newOP(int8_t type, SPerl_OP *first, SPerl_OP *last);
SPerl_OP* SPerl_newOP_flag(int8_t type, SPerl_OP *first, SPerl_OP *last, int8_t flags, int8_t private);
SPerl_OP* SPerl_op_sibling_splice(SPerl_OP* parent, SPerl_OP* start, int32_t del_count, SPerl_OP *insert);
SPerl_OP* SPerl_op_append_elem(SPerl_OP* first, SPerl_OP* last);

void SPerl_dump_ast(SPerl_OP* op, int32_t depth);

#endif
