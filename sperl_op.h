#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl_type.h"

/* Operation code */
enum SPerl_OP_type {
  SPerl_OP_NULL,
  SPerl_OP_CONST_VALUE,
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
  SPerl_OP_DESCTYPE,
  SPerl_OP_WORD,
  SPerl_OP_IF,
  SPerl_OP_ELSIF,
  SPerl_OP_ELSE,
  SPerl_OP_FOR,
  SPerl_OP_WHILE,
  SPerl_OP_ENUM,
  SPerl_OP_ENUMBLOCK,
  SPerl_OP_ENUMVALUE,
  SPerl_OP_CLASSBLOCK
};

extern SPerl_char* const SPerl_OP_names[];

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
    SPerl_int iv;
    void* pv;
  } uv;
  SPerl_char* file;
  SPerl_int line;
};

SPerl_OP* SPerl_OP_sibling(SPerl_PARSER* parser, SPerl_OP* o);
void SPerl_OP_moresib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* sib);
void SPerl_OP_lastsib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* parent);
void SPerl_OP_maybesib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* sib, SPerl_OP* parent);

SPerl_OP* SPerl_OP_newOP(SPerl_PARSER* parser, SPerl_char type, SPerl_OP *first, SPerl_OP *last);
SPerl_OP* SPerl_OP_newOP_flag(SPerl_PARSER* parser, SPerl_char type, SPerl_OP *first, SPerl_OP *last, SPerl_char flags, SPerl_char private);
SPerl_OP* SPerl_OP_sibling_splice(SPerl_PARSER* parser, SPerl_OP* parent, SPerl_OP* start, SPerl_int del_count, SPerl_OP *insert);
SPerl_OP* SPerl_OP_append_elem(SPerl_PARSER* parser, SPerl_OP* first, SPerl_OP* last);

SPerl_OP* SPerl_OP_build_SUB(SPerl_PARSER* parser, SPerl_OP* op_sub, SPerl_OP* op_subname, SPerl_OP* op_optsubargs, SPerl_OP* op_desctype, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_PACKAGE(SPerl_PARSER* parser, SPerl_OP* op_package, SPerl_OP* op_pkgname, SPerl_OP* op_block, SPerl_OP* op_descripters);
SPerl_OP* SPerl_OP_newOP_LIST(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_build_CONST_VALUE(SPerl_PARSER* parser, SPerl_OP* op_const);
SPerl_OP* SPerl_OP_build_HAS(SPerl_PARSER* parser, SPerl_OP* op_has, SPerl_OP* op_field_name, SPerl_OP* op_desctype);
SPerl_OP* SPerl_OP_build_MY(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_desctype);
SPerl_OP* SPerl_OP_build_GRAMMER(SPerl_PARSER* parser, SPerl_OP* op_packages);
SPerl_OP* SPerl_OP_build_USE(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_var, SPerl_OP* op_desctype);
void SPerl_OP_build_const_pool(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_newOP_NULL(SPerl_PARSER* parser);

SPerl_ARRAY* SPerl_OP_create_descripters(SPerl_PARSER* parser, SPerl_OP* op_descripters);

void SPerl_OP_check(SPerl_PARSER* parser);

#endif
