#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl.h"
#include "sperl_type.h"

/* Operation code */
enum SPerl_OP_CODE {
  SPerl_OP_NULL,
  SPerl_OP_CONST,
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
  SPerl_OP_CALLSUB,
  SPerl_OP_SUBARG,
  SPerl_OP_ANONSUB,
  SPerl_OP_BLOCK,
  SPerl_OP_MY,
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

extern SPerl_char* const SPerl_op_name[];

// OP_CONST private flag
#define SPerl_OPp_CONST_BOOLEAN 1
#define SPerl_OPp_CONST_CHAR 2
#define SPerl_OPp_CONST_BYTE 3
#define SPerl_OPp_CONST_SHORT 4
#define SPerl_OPp_CONST_INT 5
#define SPerl_OPp_CONST_LONG 6
#define SPerl_OPp_CONST_FLOAT 7
#define SPerl_OPp_CONST_DOUBLE 8
#define SPerl_OPp_CONST_STRING 9

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
  SPerl_char op_type;
  SPerl_char op_flags;
  SPerl_char op_private;
  SPerl_boolean op_moresib;
  SPerl_OP* op_first;
  SPerl_OP* op_last;
  SPerl_OP* op_sibparent;
  union {
    SPerl_boolean boolean_value;
    SPerl_char char_value;
    SPerl_byte byte_value;
    SPerl_short short_value;
    SPerl_long int_value;
    SPerl_long long_value;
    float float_value;
    SPerl_double double_value;
    SPerl_char* string_value;
  } uv;
};

SPerl_OP* SPerl_OpSIBLING(SPerl_OP* o);
void SPerl_OpMORESIB_set(SPerl_OP* o, SPerl_OP* sib);
void SPerl_OpLASTSIB_set(SPerl_OP* o, SPerl_OP* parent);
void SPerl_OpMAYBESIB_set(SPerl_OP* o, SPerl_OP* sib, SPerl_OP* parent);

SPerl_OP* SPerl_newOP(SPerl_char type, SPerl_OP *first, SPerl_OP *last);
SPerl_OP* SPerl_newOP_flag(SPerl_char type, SPerl_OP *first, SPerl_OP *last, SPerl_char flags, SPerl_char private);
SPerl_OP* SPerl_op_sibling_splice(SPerl_OP* parent, SPerl_OP* start, SPerl_long del_count, SPerl_OP *insert);
SPerl_OP* SPerl_op_append_elem(SPerl_OP* first, SPerl_OP* last);

void SPerl_dump_ast(SPerl_OP* op, SPerl_long depth);

#endif
