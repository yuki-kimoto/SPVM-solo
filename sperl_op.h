#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl_core_type.h"

/* Operation code */
enum {
  SPerl_OP_C_NULL,
  SPerl_OP_C_CONST_VALUE,
  SPerl_OP_C_LT,
  SPerl_OP_C_LE,
  SPerl_OP_C_GT,
  SPerl_OP_C_GE,
  SPerl_OP_C_ADD,
  SPerl_OP_C_SUBTRACT,
  SPerl_OP_C_MULTIPLY,
  SPerl_OP_C_DIVIDE,
  SPerl_OP_C_AND,
  SPerl_OP_C_OR,
  SPerl_OP_C_BIT_AND,
  SPerl_OP_C_BIT_OR,
  SPerl_OP_C_MODULO,
  SPerl_OP_C_BIT_XOR,
  SPerl_OP_C_NOT,
  SPerl_OP_C_LEFT_SHIFT,
  SPerl_OP_C_RIGHT_SHIFT,
  SPerl_OP_C_INC,
  SPerl_OP_C_DEC,
  SPerl_OP_C_BIT_NOT,
  SPerl_OP_C_PREINC,
  SPerl_OP_C_POSTINC,
  SPerl_OP_C_PREDEC,
  SPerl_OP_C_POSTDEC,
  SPerl_OP_C_COMPLEMENT,
  SPerl_OP_C_NEGATE,
  SPerl_OP_C_LIST,
  SPerl_OP_C_PUSHMARK,
  SPerl_OP_C_AELEM,
  SPerl_OP_C_FIELD,
  SPerl_OP_C_ASSIGN,
  SPerl_OP_C_CALLSUB,
  SPerl_OP_C_SUBARG,
  SPerl_OP_C_BLOCK,
  SPerl_OP_C_MY,
  SPerl_OP_C_HAS,
  SPerl_OP_C_COND,
  SPerl_OP_C_USE,
  SPerl_OP_C_RETURN,
  SPerl_OP_C_LAST,
  SPerl_OP_C_NEXT,
  SPerl_OP_C_PACKAGE,
  SPerl_OP_C_LOOP,
  SPerl_OP_C_SUB,
  SPerl_OP_C_VAR,
  SPerl_OP_C_EQ,
  SPerl_OP_C_NE,
  SPerl_OP_C_STATEMENT,
  SPerl_OP_C_GRAMMER,
  SPerl_OP_C_DESCTYPE,
  SPerl_OP_C_WORD,
  SPerl_OP_C_IF,
  SPerl_OP_C_ELSIF,
  SPerl_OP_C_ELSE,
  SPerl_OP_C_FOR,
  SPerl_OP_C_WHILE,
  SPerl_OP_C_ENUM,
  SPerl_OP_C_ENUMBLOCK,
  SPerl_OP_C_ENUMVALUE,
  SPerl_OP_C_CLASSBLOCK,
  SPerl_OP_C_DESCRIPTER,
  SPerl_OP_C_ANONSUB,
  SPerl_OP_C_TYPEOP,
  SPerl_OP_C_TYPEDEF
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
