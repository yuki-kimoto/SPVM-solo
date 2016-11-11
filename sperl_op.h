#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl_base.h"




/* Operation code */
enum {                          // [GROUP]
  SPerl_OP_C_CODE_NULL,         // UNKNOWN
  SPerl_OP_C_CODE_CONST_VALUE,  // UNKNOWN
  SPerl_OP_C_CODE_LT,           // RELOP
  SPerl_OP_C_CODE_LE,           // RELOP
  SPerl_OP_C_CODE_GT,           // RELOP
  SPerl_OP_C_CODE_GE,           // RELOP
  SPerl_OP_C_CODE_ADD,          // BINOP
  SPerl_OP_C_CODE_SUBTRACT,     // BINOP
  SPerl_OP_C_CODE_MULTIPLY,     // BINOP
  SPerl_OP_C_CODE_DIVIDE,       // BINOP
  SPerl_OP_C_CODE_AND,          // RELOP
  SPerl_OP_C_CODE_OR,           // RELOP
  SPerl_OP_C_CODE_BIT_AND,      // BINOP
  SPerl_OP_C_CODE_BIT_OR,       // BINOP
  SPerl_OP_C_CODE_MODULO,       // BINOP
  SPerl_OP_C_CODE_BIT_XOR,      // BINOP
  SPerl_OP_C_CODE_NOT,          // UNIOP
  SPerl_OP_C_CODE_LEFT_SHIFT,   // SHIFTOP
  SPerl_OP_C_CODE_RIGHT_SHIFT,  // SHIFTOP
  SPerl_OP_C_CODE_INC,          // UNIOP
  SPerl_OP_C_CODE_DEC,          // UNIOP
  SPerl_OP_C_CODE_BIT_NOT,      // UNIOP
  SPerl_OP_C_CODE_PREINC,       // UNIOP
  SPerl_OP_C_CODE_POSTINC,      // UNIOP
  SPerl_OP_C_CODE_PREDEC,       // UNIOP
  SPerl_OP_C_CODE_POSTDEC,      // UNIOP
  SPerl_OP_C_CODE_COMPLEMENT,   // UNIOP
  SPerl_OP_C_CODE_NEGATE,       // UNIOP
  SPerl_OP_C_CODE_LIST,         // UNKNOWN
  SPerl_OP_C_CODE_PUSHMARK,     // UNKNOWN
  SPerl_OP_C_CODE_AELEM,        // UNKNOWN
  SPerl_OP_C_CODE_FIELD,        // UNKNOWN
  SPerl_OP_C_CODE_ASSIGN,       // UNKNOWN
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
  SPerl_OP_C_CODE_EQ,           // RELOP
  SPerl_OP_C_CODE_NE,           // RELOP
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
  SPerl_OP_C_CODE_PLUS,         // UNIOP
  SPerl_OP_C_CODE_ASSIGN_ADD,      // ASSIGNOP
  SPerl_OP_C_CODE_ASSIGN_SUBTRACT, // ASSIGNOP
  SPerl_OP_C_CODE_ASSIGN_MULTIPLY, // ASSIGNOP
  SPerl_OP_C_CODE_ASSIGN_DIVIDE,   // ASSIGNOP
  SPerl_OP_C_CODE_ASSIGN_MODULO,   // ASSIGNOP
  SPerl_OP_C_CODE_ASSIGN_BIT_XOR,  // ASSIGNOP
  SPerl_OP_C_CODE_ASSIGN_OR,       // ASSIGNOP
  SPerl_OP_C_CODE_ASSIGN_AND,      // ASSIGNOP
  SPerl_OP_C_CODE_D2F, // UNIOP
  SPerl_OP_C_CODE_D2I, // UNIOP
  SPerl_OP_C_CODE_D2L, // UNIOP
  SPerl_OP_C_CODE_F2D, // UNIOP
  SPerl_OP_C_CODE_F2I, // UNIOP
  SPerl_OP_C_CODE_F2L, // UNIOP
  SPerl_OP_C_CODE_I2B, // UNIOP
  SPerl_OP_C_CODE_I2C, // UNIOP
  SPerl_OP_C_CODE_I2D, // UNIOP
  SPerl_OP_C_CODE_I2F, // UNIOP
  SPerl_OP_C_CODE_I2L, // UNIOP
  SPerl_OP_C_CODE_I2S, // UNIOP
  SPerl_OP_C_CODE_L2D, // UNIOP
  SPerl_OP_C_CODE_L2F, // UNIOP
  SPerl_OP_C_CODE_L2I,  // UNIOP
  SPerl_OP_C_CODE_CONVERTTYPE // UNKNOWN
};

enum {
  SPerl_OP_C_GROUP_UNKNOWN,
  SPerl_OP_C_GROUP_UNIOP,
  SPerl_OP_C_GROUP_BINOP,
  SPerl_OP_C_GROUP_RELOP,
  SPerl_OP_C_GROUP_SHIFTOP,
  SPerl_OP_C_GROUP_ASSIGNOP,
};
extern SPerl_char* const SPerl_OP_C_CODE_NAMES[];

/* Binary operation */
struct SPerl_op {
  SPerl_char code;
  SPerl_char group;
  SPerl_boolean moresib;
  SPerl_OP* first;
  SPerl_OP* last;
  SPerl_OP* sibparent;
  SPerl_char* file;
  SPerl_int line;
  void* info;
};

void SPerl_OP_create_vmcode(SPerl_PARSER* parser);
void SPerl_OP_resolve_type(SPerl_PARSER* parser, SPerl_TYPE* type);
void SPerl_OP_check(SPerl_PARSER* parser);
void SPerl_OP_check_packages(SPerl_PARSER* parser);
void SPerl_OP_check_bodys(SPerl_PARSER* parser);
void SPerl_OP_check_names(SPerl_PARSER* parser);
void SPerl_OP_check_types(SPerl_PARSER* parser);
void SPerl_OP_check_const_values(SPerl_PARSER* parser);
void SPerl_OP_check_opdefs(SPerl_PARSER* parser);
SPerl_int SPerl_OP_get_return_type_id(SPerl_PARSER* parser, SPerl_OP* op);
SPerl_OP* SPerl_OP_build_callop(SPerl_PARSER* parser, SPerl_OP* op_callop, SPerl_OP* op_first, SPerl_OP* op_last);
void SPerl_OP_insert_type_convert_op(SPerl_PARSER* parser, SPerl_OP* op, SPerl_int first_type_id, SPerl_int last_type_id);

SPerl_OP* SPerl_OP_build_wordtype(SPerl_PARSER* parser, SPerl_OP* op_wordtype);
SPerl_OP* SPerl_OP_build_arraytype(SPerl_PARSER* parser, SPerl_OP* op_simpletype);
SPerl_OP* SPerl_OP_build_subtype(SPerl_PARSER* parser, SPerl_OP* op_argument_types, SPerl_OP* op_return_type);

SPerl_OP* SPerl_OP_build_getfield(SPerl_PARSER* parser, SPerl_OP* op_packagename, SPerl_OP* op_fieldname);
SPerl_OP* SPerl_OP_build_getenumvalue(SPerl_PARSER* parser, SPerl_OP* op_enumname);
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
SPerl_OP* SPerl_OP_build_converttype(SPerl_PARSER* parser, SPerl_OP* op_type, SPerl_OP* op_term);

SPerl_char* SPerl_OP_create_sub_complete_name(SPerl_PARSER* parser, SPerl_char* sub_abs_name, SPerl_int argument_count);
SPerl_char* SPerl_OP_create_complete_name(SPerl_PARSER* parser, SPerl_char* package_name, SPerl_char* call_name);
SPerl_char* SPerl_OP_create_abs_name(SPerl_PARSER* parser, SPerl_char* package_name, SPerl_char* base_name);

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
