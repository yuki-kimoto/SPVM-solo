#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl_core_type.h"

/* Operation code */
enum {
  SPerl_OP_C_TYPE_NULL,
  SPerl_OP_C_TYPE_CONST_VALUE,
  SPerl_OP_C_TYPE_LT,
  SPerl_OP_C_TYPE_LE,
  SPerl_OP_C_TYPE_GT,
  SPerl_OP_C_TYPE_GE,
  SPerl_OP_C_TYPE_ADD,
  SPerl_OP_C_TYPE_SUBTRACT,
  SPerl_OP_C_TYPE_MULTIPLY,
  SPerl_OP_C_TYPE_DIVIDE,
  SPerl_OP_C_TYPE_AND,
  SPerl_OP_C_TYPE_OR,
  SPerl_OP_C_TYPE_BIT_AND,
  SPerl_OP_C_TYPE_BIT_OR,
  SPerl_OP_C_TYPE_MODULO,
  SPerl_OP_C_TYPE_BIT_XOR,
  SPerl_OP_C_TYPE_NOT,
  SPerl_OP_C_TYPE_LEFT_SHIFT,
  SPerl_OP_C_TYPE_RIGHT_SHIFT,
  SPerl_OP_C_TYPE_INC,
  SPerl_OP_C_TYPE_DEC,
  SPerl_OP_C_TYPE_BIT_NOT,
  SPerl_OP_C_TYPE_PREINC,
  SPerl_OP_C_TYPE_POSTINC,
  SPerl_OP_C_TYPE_PREDEC,
  SPerl_OP_C_TYPE_POSTDEC,
  SPerl_OP_C_TYPE_COMPLEMENT,
  SPerl_OP_C_TYPE_NEGATE,
  SPerl_OP_C_TYPE_LIST,
  SPerl_OP_C_TYPE_PUSHMARK,
  SPerl_OP_C_TYPE_AELEM,
  SPerl_OP_C_TYPE_FIELD,
  SPerl_OP_C_TYPE_ASSIGN,
  SPerl_OP_C_TYPE_CALLSUB,
  SPerl_OP_C_TYPE_SUBARG,
  SPerl_OP_C_TYPE_BLOCK,
  SPerl_OP_C_TYPE_MY,
  SPerl_OP_C_TYPE_HAS,
  SPerl_OP_C_TYPE_COND,
  SPerl_OP_C_TYPE_USE,
  SPerl_OP_C_TYPE_RETURN,
  SPerl_OP_C_TYPE_LAST,
  SPerl_OP_C_TYPE_NEXT,
  SPerl_OP_C_TYPE_PACKAGE,
  SPerl_OP_C_TYPE_LOOP,
  SPerl_OP_C_TYPE_SUB,
  SPerl_OP_C_TYPE_VAR,
  SPerl_OP_C_TYPE_EQ,
  SPerl_OP_C_TYPE_NE,
  SPerl_OP_C_TYPE_STATEMENT,
  SPerl_OP_C_TYPE_GRAMMER,
  SPerl_OP_C_TYPE_DESCTYPE,
  SPerl_OP_C_TYPE_WORD,
  SPerl_OP_C_TYPE_IF,
  SPerl_OP_C_TYPE_ELSIF,
  SPerl_OP_C_TYPE_ELSE,
  SPerl_OP_C_TYPE_FOR,
  SPerl_OP_C_TYPE_WHILE,
  SPerl_OP_C_TYPE_ENUM,
  SPerl_OP_C_TYPE_ENUMBLOCK,
  SPerl_OP_C_TYPE_ENUMVALUE,
  SPerl_OP_C_TYPE_CLASSBLOCK,
  SPerl_OP_C_TYPE_DESCRIPTER,
  SPerl_OP_C_TYPE_ANONSUB,
  SPerl_OP_C_TYPE_TYPEOP,
  SPerl_OP_C_TYPE_TYPEDEF
};

extern SPerl_char* const SPerl_OP_C_NAMES[];

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


void SPerl_OP_check(SPerl_PARSER* parser);

SPerl_OP* SPerl_OP_build_subtype(SPerl_PARSER* parser, SPerl_OP* op_wordtypes, SPerl_OP* op_wordtype);
SPerl_OP* SPerl_OP_build_wordtype(SPerl_PARSER* parser, SPerl_OP* op_wordtype);

SPerl_OP* SPerl_OP_build_PACKAGE(SPerl_PARSER* parser, SPerl_OP* op_package, SPerl_OP* op_pkgname, SPerl_OP* op_typedef, SPerl_OP* op_descripters, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_SUB(SPerl_PARSER* parser, SPerl_OP* op_sub, SPerl_OP* op_subname, SPerl_OP* op_subargs, SPerl_OP* op_descripters, SPerl_OP* type, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_CONST_VALUE(SPerl_PARSER* parser, SPerl_OP* op_const);
SPerl_OP* SPerl_OP_build_HAS(SPerl_PARSER* parser, SPerl_OP* op_has, SPerl_OP* op_field_name, SPerl_OP* op_descripters, SPerl_OP* type);
SPerl_OP* SPerl_OP_build_MY(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_descripters, SPerl_OP* op_type);
SPerl_OP* SPerl_OP_build_GRAMMER(SPerl_PARSER* parser, SPerl_OP* op_packages);
SPerl_OP* SPerl_OP_build_USE(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_var, SPerl_OP* op_desctype);
void SPerl_OP_build_const_pool(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_newOP_LIST(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_newOP_NULL(SPerl_PARSER* parser);

SPerl_ARRAY* SPerl_OP_create_descripters(SPerl_PARSER* parser, SPerl_OP* op_descripters);

SPerl_OP* SPerl_OP_newOP(SPerl_PARSER* parser, SPerl_char type, SPerl_OP *first, SPerl_OP *last);
SPerl_OP* SPerl_OP_newOP_flag(SPerl_PARSER* parser, SPerl_char type, SPerl_OP *first, SPerl_OP *last, SPerl_char flags, SPerl_char private);
SPerl_OP* SPerl_OP_sibling_splice(SPerl_PARSER* parser, SPerl_OP* parent, SPerl_OP* start, SPerl_int del_count, SPerl_OP *insert);
SPerl_OP* SPerl_OP_append_elem(SPerl_PARSER* parser, SPerl_OP* first, SPerl_OP* last);

SPerl_OP* SPerl_OP_sibling(SPerl_PARSER* parser, SPerl_OP* o);
void SPerl_OP_moresib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* sib);
void SPerl_OP_lastsib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* parent);
void SPerl_OP_maybesib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* sib, SPerl_OP* parent);

#endif
