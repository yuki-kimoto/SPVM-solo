#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl_base.h"

/* Operation code */
enum {
  SPerl_OP_C_CODE_NULL,
  SPerl_OP_C_CODE_CONST_VALUE,
  SPerl_OP_C_CODE_LT,
  SPerl_OP_C_CODE_LE,
  SPerl_OP_C_CODE_GT,
  SPerl_OP_C_CODE_GE,
  SPerl_OP_C_CODE_ADD,
  SPerl_OP_C_CODE_SUBTRACT,
  SPerl_OP_C_CODE_MULTIPLY,
  SPerl_OP_C_CODE_DIVIDE,
  SPerl_OP_C_CODE_AND,
  SPerl_OP_C_CODE_OR,
  SPerl_OP_C_CODE_BIT_AND,
  SPerl_OP_C_CODE_BIT_OR,
  SPerl_OP_C_CODE_MODULO,
  SPerl_OP_C_CODE_BIT_XOR,
  SPerl_OP_C_CODE_NOT,
  SPerl_OP_C_CODE_LEFT_SHIFT,
  SPerl_OP_C_CODE_RIGHT_SHIFT,
  SPerl_OP_C_CODE_INC,
  SPerl_OP_C_CODE_DEC,
  SPerl_OP_C_CODE_BIT_NOT,
  SPerl_OP_C_CODE_PREINC,
  SPerl_OP_C_CODE_POSTINC,
  SPerl_OP_C_CODE_PREDEC,
  SPerl_OP_C_CODE_POSTDEC,
  SPerl_OP_C_CODE_COMPLEMENT,
  SPerl_OP_C_CODE_NEGATE,
  SPerl_OP_C_CODE_LIST,
  SPerl_OP_C_CODE_PUSHMARK,
  SPerl_OP_C_CODE_AELEM,
  SPerl_OP_C_CODE_FIELD,
  SPerl_OP_C_CODE_ASSIGN,
  SPerl_OP_C_CODE_CALLSUB,
  SPerl_OP_C_CODE_SUBARG,
  SPerl_OP_C_CODE_BLOCK,
  SPerl_OP_C_CODE_MY,
  SPerl_OP_C_CODE_HAS,
  SPerl_OP_C_CODE_COND,
  SPerl_OP_C_CODE_USE,
  SPerl_OP_C_CODE_RETURN,
  SPerl_OP_C_CODE_LAST,
  SPerl_OP_C_CODE_NEXT,
  SPerl_OP_C_CODE_PACKAGE,
  SPerl_OP_C_CODE_LOOP,
  SPerl_OP_C_CODE_SUB,
  SPerl_OP_C_CODE_VAR,
  SPerl_OP_C_CODE_EQ,
  SPerl_OP_C_CODE_NE,
  SPerl_OP_C_CODE_STATEMENT,
  SPerl_OP_C_CODE_GRAMMER,
  SPerl_OP_C_CODE_WORD,
  SPerl_OP_C_CODE_IF,
  SPerl_OP_C_CODE_ELSIF,
  SPerl_OP_C_CODE_ELSE,
  SPerl_OP_C_CODE_FOR,
  SPerl_OP_C_CODE_WHILE,
  SPerl_OP_C_CODE_ENUM,
  SPerl_OP_C_CODE_ENUMBLOCK,
  SPerl_OP_C_CODE_ENUMVALUE,
  SPerl_OP_C_CODE_CLASSBLOCK,
  SPerl_OP_C_CODE_DESCRIPTER,
  SPerl_OP_C_CODE_ANONSUB,
  SPerl_OP_C_CODE_TYPE,
  SPerl_OP_C_CODE_GETENUMVALUE,
  SPerl_OP_C_CODE_GETFIELD
};

extern SPerl_char* const SPerl_OP_C_CODE_NAMES[];

/* Binary operation */
struct SPerl_op {
  SPerl_char code;
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

void SPerl_OP_resolve_type(SPerl_PARSER* parser, SPerl_TYPE* type);
void SPerl_OP_check(SPerl_PARSER* parser);

SPerl_OP* SPerl_OP_build_wordtype(SPerl_PARSER* parser, SPerl_OP* op_wordtype);
SPerl_OP* SPerl_OP_build_arraytype(SPerl_PARSER* parser, SPerl_OP* op_simpletype);
SPerl_OP* SPerl_OP_build_subtype(SPerl_PARSER* parser, SPerl_OP* op_argument_types, SPerl_OP* op_return_type);

SPerl_OP* SPerl_OP_build_getfield(SPerl_PARSER* parser, SPerl_OP* op_packagename, SPerl_OP* op_fieldname);
SPerl_OP* SPerl_OP_build_getenumvalue(SPerl_PARSER* parser, SPerl_OP* op_packagename, SPerl_OP* op_enumname);
SPerl_OP* SPerl_OP_build_package(SPerl_PARSER* parser, SPerl_OP* op_package, SPerl_OP* op_packagename, SPerl_OP* op_typedef, SPerl_OP* op_descripters, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_declsub(SPerl_PARSER* parser, SPerl_OP* op_sub, SPerl_OP* op_subname, SPerl_OP* op_subargs, SPerl_OP* op_descripters, SPerl_OP* type, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_CONSTVALUE(SPerl_PARSER* parser, SPerl_OP* op_const);
SPerl_OP* SPerl_OP_build_declhas(SPerl_PARSER* parser, SPerl_OP* op_has, SPerl_OP* op_field_name, SPerl_OP* op_descripters, SPerl_OP* type);
SPerl_OP* SPerl_OP_build_declmy(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_descripters, SPerl_OP* op_type);
SPerl_OP* SPerl_OP_build_grammer(SPerl_PARSER* parser, SPerl_OP* op_packages);
SPerl_OP* SPerl_OP_build_decluse(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_var, SPerl_OP* op_desctype);
SPerl_OP* SPerl_OP_build_callsub(SPerl_PARSER* parser, SPerl_OP* op_invocant, SPerl_OP* op_subname, SPerl_OP* op_terms, SPerl_boolean anon);
void SPerl_OP_build_const_pool(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_newOP_LIST(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_newOP_NULL(SPerl_PARSER* parser);

SPerl_ARRAY* SPerl_OP_create_descripters(SPerl_PARSER* parser, SPerl_OP* op_descripters);
SPerl_char* SPerl_OP_create_sub_complete_name(SPerl_PARSER* parser, SPerl_char* package_name, SPerl_char* sub_name, SPerl_int argument_count);

SPerl_OP* SPerl_OP_newOP(SPerl_PARSER* parser, SPerl_char type, SPerl_OP *first, SPerl_OP *last);
SPerl_OP* SPerl_OP_newOP_flag(SPerl_PARSER* parser, SPerl_char type, SPerl_OP *first, SPerl_OP *last, SPerl_char flags, SPerl_char private);
SPerl_OP* SPerl_OP_sibling_splice(SPerl_PARSER* parser, SPerl_OP* parent, SPerl_OP* start, SPerl_int del_count, SPerl_OP *insert);
SPerl_OP* SPerl_OP_append_elem(SPerl_PARSER* parser, SPerl_OP* first, SPerl_OP* last);

SPerl_OP* SPerl_OP_sibling(SPerl_PARSER* parser, SPerl_OP* o);
void SPerl_OP_moresib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* sib);
void SPerl_OP_lastsib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* parent);
void SPerl_OP_maybesib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* sib, SPerl_OP* parent);

#endif
