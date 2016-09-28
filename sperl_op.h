#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl_type.h"
#include "sperl_parser.h"

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
  SPerl_OP_FIELD,
  SPerl_OP_ASSIGN,
  SPerl_OP_CALLSUB,
  SPerl_OP_SUBARG,
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
  SPerl_OP_GRAMMER,
  SPerl_OP_DESCTYPE
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

/* Binary operation */
struct SPerl_op {
  SPerl_char type;
  SPerl_char flags;
  SPerl_char private;
  SPerl_boolean moresib;
  SPerl_OP* first;
  SPerl_OP* last;
  SPerl_OP* sibparent;
  union {
    SPerl_boolean boolean_value;
    SPerl_char char_value;
    SPerl_byte byte_value;
    SPerl_short short_value;
    SPerl_int int_value;
    SPerl_int long_value;
    float float_value;
    SPerl_double double_value;
    SPerl_char* string_value;
    void* ptr_value;
  } uv;
};

SPerl_OP* SPerl_OP_sibling(SPerl_OP* o);
void SPerl_OP_moresib_set(SPerl_OP* o, SPerl_OP* sib);
void SPerl_OP_lastsib_set(SPerl_OP* o, SPerl_OP* parent);
void SPerl_OP_maybesib_set(SPerl_OP* o, SPerl_OP* sib, SPerl_OP* parent);

SPerl_OP* SPerl_OP_newOP(SPerl_char type, SPerl_OP *first, SPerl_OP *last);
SPerl_OP* SPerl_OP_newOP_flag(SPerl_char type, SPerl_OP *first, SPerl_OP *last, SPerl_char flags, SPerl_char private);
SPerl_OP* SPerl_OP_sibling_splice(SPerl_OP* parent, SPerl_OP* start, SPerl_int del_count, SPerl_OP *insert);
SPerl_OP* SPerl_OP_append_elem(SPerl_OP* first, SPerl_OP* last);

SPerl_OP* SPerl_OP_newOP_SUB(SPerl_yy_parser* parser, SPerl_OP* op_subname, SPerl_OP* op_optsubargs, SPerl_OP* op_desctype, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_newOP_PACKAGE(SPerl_yy_parser* parser, SPerl_OP* op_pkgname, SPerl_OP* op_block, SPerl_OP* op_descripters);
SPerl_OP* SPerl_OP_newOP_LIST();
SPerl_OP* SPerl_OP_newOP_CONST(SPerl_yy_parser* parser, SPerl_OP* op);
SPerl_OP* SPerl_OP_newOP_HAS(SPerl_yy_parser* parser, SPerl_OP* op_field_name, SPerl_OP* op_desctype);

SPerl_char SPerl_OP_create_desc_flags(SPerl_OP* op_descripters);

void SPerl_dump_ast(SPerl_OP* op, SPerl_int depth);

SPerl_ARGUMENT_INFO* SPerl_OP_create_argument_info(SPerl_OP* op_subarg);

#endif
