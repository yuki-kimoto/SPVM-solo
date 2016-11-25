#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl_base.h"




/* Operation code */
enum {                          // [GROUP]
  SPerl_OP_C_CODE_CONSTBOOLEAN, // CONST
  SPerl_OP_C_CODE_CONSTCHAR,    // CONST
  SPerl_OP_C_CODE_CONSTINT,     // CONST
  SPerl_OP_C_CODE_CONSTLONG,    // CONST
  SPerl_OP_C_CODE_CONSTFLOAT,   // CONST
  SPerl_OP_C_CODE_CONSTDOUBLE,  // CONST
  SPerl_OP_C_CODE_CONSTSTRING,  // CONST
  SPerl_OP_C_CODE_BITNOT,       // UNOP
  SPerl_OP_C_CODE_PREINC,       // INTDEC
  SPerl_OP_C_CODE_POSTINC,      // INTDEC
  SPerl_OP_C_CODE_PREDEC,       // INTDEC
  SPerl_OP_C_CODE_POSTDEC,      // INTDEC
  SPerl_OP_C_CODE_COMPLEMENT,   // UNOP
  SPerl_OP_C_CODE_NEGATE,       // UNOP
  SPerl_OP_C_CODE_PLUS,         // UNOP
  SPerl_OP_C_CODE_D2F,          // UNOP
  SPerl_OP_C_CODE_D2I,          // UNOP
  SPerl_OP_C_CODE_D2L,          // UNOP
  SPerl_OP_C_CODE_F2D,          // UNOP
  SPerl_OP_C_CODE_F2I,          // UNOP
  SPerl_OP_C_CODE_F2L,          // UNOP
  SPerl_OP_C_CODE_I2B,          // UNOP
  SPerl_OP_C_CODE_I2C,          // UNOP
  SPerl_OP_C_CODE_I2D,          // UNOP
  SPerl_OP_C_CODE_I2F,          // UNOP
  SPerl_OP_C_CODE_I2L,          // UNOP
  SPerl_OP_C_CODE_I2S,          // UNOP
  SPerl_OP_C_CODE_L2D,          // UNOP
  SPerl_OP_C_CODE_L2F,          // UNOP
  SPerl_OP_C_CODE_L2I,          // UNOP
  SPerl_OP_C_CODE_NULL,         // UNKNOWN
  SPerl_OP_C_CODE_LT,           // BINOP
  SPerl_OP_C_CODE_LE,           // BINOP
  SPerl_OP_C_CODE_GT,           // BINOP
  SPerl_OP_C_CODE_GE,           // BINOP
  SPerl_OP_C_CODE_ADD,          // BINOP
  SPerl_OP_C_CODE_SUBTRACT,     // BINOP
  SPerl_OP_C_CODE_MULTIPLY,     // BINOP
  SPerl_OP_C_CODE_DIVIDE,       // BINOP
  SPerl_OP_C_CODE_BITAND,       // BINOP
  SPerl_OP_C_CODE_BITOR,        // BINOP
  SPerl_OP_C_CODE_MODULO,       // BINOP
  SPerl_OP_C_CODE_BITXOR,       // BINOP
  SPerl_OP_C_CODE_EQ,           // BINOP
  SPerl_OP_C_CODE_NE,           // BINOP
  SPerl_OP_C_CODE_LEFTSHIFT,    // BINOP
  SPerl_OP_C_CODE_RIGHTSHIFT,   // BINOP
  SPerl_OP_C_CODE_AND,          // LOGICALOP
  SPerl_OP_C_CODE_OR,           // LOGICALOP
  SPerl_OP_C_CODE_NOT,          // LOGICALOP
  SPerl_OP_C_CODE_LIST,         // UNKNOWN
  SPerl_OP_C_CODE_PUSHMARK,     // UNKNOWN
  SPerl_OP_C_CODE_AELEM,        // UNKNOWN
  SPerl_OP_C_CODE_FIELD,        // UNKNOWN
  SPerl_OP_C_CODE_ASSIGN,       // ASSIGN
  SPerl_OP_C_CODE_CALLSUB,      // UNKNOWN
  SPerl_OP_C_CODE_SUBARG,       // UNKNOWN
  SPerl_OP_C_CODE_BLOCK,        // UNKNOWN
  SPerl_OP_C_CODE_MY,           // UNKNOWN
  SPerl_OP_C_CODE_HAS,          // UNKNOWN
  SPerl_OP_C_CODE_COND,         // UNKNOWN
  SPerl_OP_C_CODE_USE,          // UNKNOWN
  SPerl_OP_C_CODE_RETURN,       // UNKNOWN
  SPerl_OP_C_CODE_LAST,         // UNKNOWN
  SPerl_OP_C_CODE_NEXT,         // UNKNOWN
  SPerl_OP_C_CODE_PACKAGE,      // UNKNOWN
  SPerl_OP_C_CODE_LOOP,         // UNKNOWN
  SPerl_OP_C_CODE_SUB,          // UNKNOWN
  SPerl_OP_C_CODE_VAR,          // UNKNOWN
  SPerl_OP_C_CODE_STATEMENT,    // UNKNOWN
  SPerl_OP_C_CODE_GRAMMER,      // UNKNOWN
  SPerl_OP_C_CODE_WORD,         // UNKNOWN
  SPerl_OP_C_CODE_IF,           // UNKNOWN
  SPerl_OP_C_CODE_ELSIF,        // UNKNOWN
  SPerl_OP_C_CODE_ELSE,         // UNKNOWN
  SPerl_OP_C_CODE_FOR,          // UNKNOWN
  SPerl_OP_C_CODE_WHILE,        // UNKNOWN
  SPerl_OP_C_CODE_ENUM,         // UNKNOWN
  SPerl_OP_C_CODE_ENUMBLOCK,    // UNKNOWN
  SPerl_OP_C_CODE_ENUMVALUE,    // UNKNOWN
  SPerl_OP_C_CODE_CLASSBLOCK,   // UNKNOWN
  SPerl_OP_C_CODE_DESCRIPTER,   // UNKNOWN
  SPerl_OP_C_CODE_ANONSUB,      // UNKNOWN
  SPerl_OP_C_CODE_TYPE,         // UNKNOWN
  SPerl_OP_C_CODE_GETENUMVALUE, // UNKNOWN
  SPerl_OP_C_CODE_GETFIELD,     // UNKNOWN
  SPerl_OP_C_CODE_CONVERTTYPE,  // UNKNOWN
  SPerl_OP_C_CODE_POP,          // UNKNOWN
};

enum {
  SPerl_OP_C_GROUP_UNKNOWN,
  SPerl_OP_C_GROUP_CONST,
  SPerl_OP_C_GROUP_UNOP,
  SPerl_OP_C_GROUP_BINOP,
  SPerl_OP_C_GROUP_LOGICALOP,
  SPerl_OP_C_GROUP_ASSIGNOP,
  SPerl_OP_C_GROUP_INCDEC,
};
extern SPerl_char* const SPerl_OP_C_CODE_NAMES[];

/* Binary operation */
struct SPerl_op {
  SPerl_int code;
  SPerl_int group;
  SPerl_boolean moresib;
  SPerl_OP* first;
  SPerl_OP* last;
  SPerl_OP* sibparent;
  SPerl_char* file;
  SPerl_int line;
  SPerl_int iv;
  union {
    SPerl_MY_VAR* my_var;
    SPerl_VAR* var;
    SPerl_OP_INFO* op_info;
    SPerl_SUB* sub;
    SPerl_CONSTANT* constant;
    SPerl_NAME* name;
    SPerl_TYPE* type;
    SPerl_WORD* word;
    SPerl_USE* use;
    SPerl_FIELD* field;
    SPerl_DESCRIPTER* descripter;
    SPerl_PACKAGE* package;
  } uv;
};

void SPerl_OP_create_vmcode(SPerl_PARSER* parser);
void SPerl_OP_resolve_type(SPerl_PARSER* parser, SPerl_TYPE* type);
void SPerl_OP_check(SPerl_PARSER* parser);
void SPerl_OP_check_descripters(SPerl_PARSER* parser);
void SPerl_OP_check_sub_name(SPerl_PARSER* parser, SPerl_OP* name);
void SPerl_OP_check_field_name(SPerl_PARSER* parser, SPerl_NAME* name);
void SPerl_OP_check_enum_name(SPerl_PARSER* parser, SPerl_NAME* name);

void SPerl_OP_check_ops(SPerl_PARSER* parser);
SPerl_RESOLVED_TYPE* SPerl_OP_get_resolved_type(SPerl_PARSER* parser, SPerl_OP* op);
SPerl_OP* SPerl_OP_build_call_op(SPerl_PARSER* parser, SPerl_OP* op_call_op, SPerl_OP* op_first, SPerl_OP* op_last);
void SPerl_OP_insert_type_convert_op(SPerl_PARSER* parser, SPerl_OP* op, SPerl_int first_type_id, SPerl_int last_type_id);

SPerl_OP* SPerl_OP_build_wordtype(SPerl_PARSER* parser, SPerl_OP* op_wordtype);
SPerl_OP* SPerl_OP_build_type_array(SPerl_PARSER* parser, SPerl_OP* op_simpletype);
SPerl_OP* SPerl_OP_build_type_sub(SPerl_PARSER* parser, SPerl_OP* op_argument_types, SPerl_OP* op_return_type);

SPerl_OP* SPerl_OP_build_get_field(SPerl_PARSER* parser, SPerl_OP* op_packagename, SPerl_OP* op_fieldname);
SPerl_OP* SPerl_OP_build_get_enum_value(SPerl_PARSER* parser, SPerl_OP* op_enumname);
SPerl_OP* SPerl_OP_build_package(SPerl_PARSER* parser, SPerl_OP* op_package, SPerl_OP* op_packagename, SPerl_OP* op_typedef, SPerl_OP* op_descripters, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_declsub(SPerl_PARSER* parser, SPerl_OP* op_sub, SPerl_OP* op_subname, SPerl_OP* op_subargs, SPerl_OP* op_descripters, SPerl_OP* type, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_CONSTVALUE(SPerl_PARSER* parser, SPerl_OP* op_const);
SPerl_OP* SPerl_OP_build_declfield(SPerl_PARSER* parser, SPerl_OP* op_has, SPerl_OP* op_field_name, SPerl_OP* op_descripters, SPerl_OP* type);
SPerl_OP* SPerl_OP_build_declmy(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_descripters, SPerl_OP* op_type);
SPerl_OP* SPerl_OP_build_grammer(SPerl_PARSER* parser, SPerl_OP* op_packages);
SPerl_OP* SPerl_OP_build_decluse(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_var, SPerl_OP* op_desctype);
SPerl_OP* SPerl_OP_build_call_sub(SPerl_PARSER* parser, SPerl_OP* op_invocant, SPerl_OP* op_subname, SPerl_OP* op_terms, SPerl_boolean anon);
void SPerl_OP_build_const_pool(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_newOP_LIST(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_newOP_NULL(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_build_convert_type(SPerl_PARSER* parser, SPerl_OP* op_type, SPerl_OP* op_term);
void SPerl_OP_resolve_convert_type(SPerl_PARSER* parser, SPerl_OP* op_convert_type, SPerl_RESOLVED_TYPE* resolved_type_src, SPerl_RESOLVED_TYPE* resolved_type_dist);
void SPerl_OP_replace_code(SPerl_PARSER* parser, SPerl_OP* op, SPerl_int code);

SPerl_char* SPerl_OP_create_sub_complete_name(SPerl_PARSER* parser, SPerl_char* sub_abs_name, SPerl_int argument_count);
SPerl_char* SPerl_OP_create_complete_name(SPerl_PARSER* parser, SPerl_char* package_name, SPerl_char* call_name);
SPerl_char* SPerl_OP_create_abs_name(SPerl_PARSER* parser, SPerl_char* package_name, SPerl_char* base_name);

SPerl_ARRAY* SPerl_OP_create_op_descripters_array(SPerl_PARSER* parser, SPerl_OP* op_descripters);

SPerl_OP* SPerl_OP_newOP(SPerl_PARSER* parser, SPerl_char type, SPerl_OP *first, SPerl_OP *last);
SPerl_OP* SPerl_OP_newOP_flag(SPerl_PARSER* parser, SPerl_int type, SPerl_OP *first, SPerl_OP *last, SPerl_char flags, SPerl_char private);
SPerl_OP* SPerl_OP_sibling_splice(SPerl_PARSER* parser, SPerl_OP* parent, SPerl_OP* start, SPerl_int del_count, SPerl_OP *insert);
SPerl_OP* SPerl_OP_append_elem(SPerl_PARSER* parser, SPerl_OP* first, SPerl_OP* last);
SPerl_int SPerl_OP_get_group(SPerl_PARSER* parser, SPerl_int op_code);

SPerl_OP* SPerl_OP_sibling(SPerl_PARSER* parser, SPerl_OP* o);
void SPerl_OP_moresib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* sib);
void SPerl_OP_lastsib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* parent);
void SPerl_OP_maybesib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* sib, SPerl_OP* parent);

#endif
