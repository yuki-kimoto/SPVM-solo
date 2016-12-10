#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl_base.h"











































/* Operation code */
enum {                          // [GROUP]
  SPerl_OP_C_CODE_IF,           // UNKNOWN
  SPerl_OP_C_CODE_ELSIF,        // UNKNOWN
  SPerl_OP_C_CODE_ELSE,         // UNKNOWN
  SPerl_OP_C_CODE_FOR,          // UNKNOWN
  SPerl_OP_C_CODE_WHILE,        // UNKNOWN
  SPerl_OP_C_CODE_NULL,         // UNKNOWN
  SPerl_OP_C_CODE_LIST,         // UNKNOWN
  SPerl_OP_C_CODE_PUSHMARK,     // UNKNOWN
  SPerl_OP_C_CODE_GRAMMER,      // UNKNOWN
  SPerl_OP_C_CODE_WORD,         // UNKNOWN
  SPerl_OP_C_CODE_DECL_PACKAGE,      // UNKNOWN
  SPerl_OP_C_CODE_DECL_MY_VAR,   // UNKNOWN
  SPerl_OP_C_CODE_DECL_FIELD,   // UNKNOWN
  SPerl_OP_C_CODE_DECL_SUB,          // UNKNOWN
  SPerl_OP_C_CODE_DECL_ENUM,
  SPerl_OP_C_CODE_DECL_DESCRIPTER,   // UNKNOWN
  SPerl_OP_C_CODE_DECL_ANON_SUB,      // UNKNOWN
  SPerl_OP_C_CODE_DECL_ENUMERATION_VALUE,    // UNKNOWN
  SPerl_OP_C_CODE_BLOCK,        // UNKNOWN
  SPerl_OP_C_CODE_ENUM_BLOCK,    // UNKNOWN
  SPerl_OP_C_CODE_CLASS_BLOCK,   // UNKNOWN
  SPerl_OP_C_CODE_TYPE,         // UNKNOWN
  SPerl_OP_C_CODE_DESCRIPTER_ENUM,         // UNKNOWN
  SPerl_OP_C_CODE_CONSTANT,     // CONST
  SPerl_OP_C_CODE_PRE_INC,       // INTDEC
  SPerl_OP_C_CODE_POST_INC,      // INTDEC
  SPerl_OP_C_CODE_PRE_DEC,       // INTDEC
  SPerl_OP_C_CODE_POST_DEC,      // INTDEC
  SPerl_OP_C_CODE_BIT_NOT,       // UNOP
  SPerl_OP_C_CODE_COMPLEMENT,   // UNOP
  SPerl_OP_C_CODE_NEGATE,       // UNOP
  SPerl_OP_C_CODE_PLUS,         // UNOP
  SPerl_OP_C_CODE_EQ,           // BINOP
  SPerl_OP_C_CODE_NE,           // BINOP
  SPerl_OP_C_CODE_LT,           // BINOP
  SPerl_OP_C_CODE_LE,           // BINOP
  SPerl_OP_C_CODE_GT,           // BINOP
  SPerl_OP_C_CODE_GE,           // BINOP
  SPerl_OP_C_CODE_ADD,          // BINOP
  SPerl_OP_C_CODE_SUBTRACT,     // BINOP
  SPerl_OP_C_CODE_MULTIPLY,     // BINOP
  SPerl_OP_C_CODE_DIVIDE,       // BINOP
  SPerl_OP_C_CODE_BIT_AND,       // BINOP
  SPerl_OP_C_CODE_BIT_OR,        // BINOP
  SPerl_OP_C_CODE_REMAINDER,       // BINOP
  SPerl_OP_C_CODE_BIT_XOR,       // BINOP
  SPerl_OP_C_CODE_LEFT_SHIFT,    // BINOP
  SPerl_OP_C_CODE_RIGHT_SHIFT,   // BINOP
  SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED,   // BINOP
  SPerl_OP_C_CODE_AND,          // LOGICALOP
  SPerl_OP_C_CODE_OR,           // LOGICALOP
  SPerl_OP_C_CODE_NOT,          // LOGICALOP
  SPerl_OP_C_CODE_ARRAY_ELEM,        // UNKNOWN
  SPerl_OP_C_CODE_FIELD,        // UNKNOWN
  SPerl_OP_C_CODE_ASSIGN,       // ASSIGN
  SPerl_OP_C_CODE_CALL_SUB,      // UNKNOWN
  SPerl_OP_C_CODE_USE,          // UNKNOWN
  SPerl_OP_C_CODE_RETURN,       // UNKNOWN
  SPerl_OP_C_CODE_LAST,         // UNKNOWN
  SPerl_OP_C_CODE_NEXT,         // UNKNOWN
  SPerl_OP_C_CODE_LOOP,         // UNKNOWN
  SPerl_OP_C_CODE_VAR,          // UNKNOWN
  SPerl_OP_C_CODE_GET_ENUMERATION_VALUE, // UNKNOWN
  SPerl_OP_C_CODE_CONVERT,  // UNKNOWN
  SPerl_OP_C_CODE_POP,          // UNKNOWN
  SPerl_OP_C_CODE_NEW_ARRAY,
  SPerl_OP_C_CODE_UNDEF,
  SPerl_OP_C_CODE_NEW,
  SPerl_OP_C_CODE_NEW_TYPE,
  SPerl_OP_C_CODE_NEW_ARRAY_CONSTANT,
  SPerl_OP_C_CODE_TERM_STATEMENT,
  SPerl_OP_C_CODE_ARRAY_LENGTH,
};

extern SPerl_char* const SPerl_OP_C_CODE_NAMES[];

/* Binary operation */
struct SPerl_op {
  SPerl_int code;
  SPerl_OP* first;
  SPerl_OP* last;
  SPerl_OP* sibparent;
  SPerl_char* file;
  SPerl_int line;
  SPerl_boolean moresib;
  SPerl_boolean lvalue;
  SPerl_boolean condition;
  union {
    SPerl_OP_INFO* op_info;
    SPerl_MY_VAR* my_var;
    SPerl_SUB* sub;
    SPerl_CONSTANT* constant;
    SPerl_NAME* name;
    SPerl_TYPE* type;
    SPerl_WORD* word;
    SPerl_USE* use;
    SPerl_VAR* var;
    SPerl_FIELD* field;
    SPerl_DESCRIPTER* descripter;
    SPerl_PACKAGE* package;
    SPerl_ENUMERATION* enumeration;
  } uv;
};

SPerl_OP* SPerl_OP_newOP_CONSTANT_true(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_newOP_CONSTANT_false(SPerl_PARSER* parser);

void SPerl_OP_convert_not_to_if(SPerl_PARSER* parser, SPerl_OP* op);
void SPerl_OP_convert_and_to_if(SPerl_PARSER* parser, SPerl_OP* op);
void SPerl_OP_convert_or_to_if(SPerl_PARSER* parser, SPerl_OP* op);

SPerl_OP* SPerl_OP_build_logical_op(SPerl_PARSER* parser, SPerl_OP* op_logical_op, SPerl_OP* op_first, SPerl_OP* op_last);

SPerl_OP* SPerl_OP_build_for_statement(SPerl_PARSER* parser, SPerl_OP* op_for, SPerl_OP* op_term_loop_var, SPerl_OP* op_term_condition, SPerl_OP* op_term_next_value, SPerl_OP* op_block);

SPerl_OP* SPerl_OP_build_while_statement(SPerl_PARSER* parser, SPerl_OP* op_while, SPerl_OP* op_term_condition, SPerl_OP* op_block);


SPerl_OP* SPerl_OP_build_if_statement(SPerl_PARSER* parser, SPerl_OP* op_if, SPerl_OP* op_term, SPerl_OP* op_block, SPerl_OP* op_else_statement);

SPerl_OP* SPerl_OP_build_array_length(SPerl_PARSER* parser, SPerl_OP* op_array_length, SPerl_OP* op_term);

SPerl_OP* SPerl_OP_build_new_object(SPerl_PARSER* parser, SPerl_OP* op_new, SPerl_OP* op_type);

SPerl_OP* SPerl_OP_build_array_init(SPerl_PARSER* parser, SPerl_OP* op_opt_terms);
void SPerl_OP_resolve_type(SPerl_PARSER* parser, SPerl_TYPE* type);
void SPerl_OP_check(SPerl_PARSER* parser);
void SPerl_OP_check_descripters(SPerl_PARSER* parser);
void SPerl_OP_check_sub_name(SPerl_PARSER* parser, SPerl_OP* name);
void SPerl_OP_check_field_name(SPerl_PARSER* parser, SPerl_NAME* name);
void SPerl_OP_check_enum_name(SPerl_PARSER* parser, SPerl_NAME* name);

void SPerl_OP_check_ops(SPerl_PARSER* parser);
SPerl_RESOLVED_TYPE* SPerl_OP_get_resolved_type(SPerl_PARSER* parser, SPerl_OP* op);
SPerl_OP* SPerl_OP_build_call_op(SPerl_PARSER* parser, SPerl_OP* op_call_op, SPerl_OP* op_first, SPerl_OP* op_last);
void SPerl_OP_insert_op_convert_type(SPerl_PARSER* parser, SPerl_OP* op);

SPerl_OP* SPerl_OP_build_type_word(SPerl_PARSER* parser, SPerl_OP* op_type_word);
SPerl_OP* SPerl_OP_build_type_array(SPerl_PARSER* parser, SPerl_OP* op_type, SPerl_OP* op_term);
SPerl_OP* SPerl_OP_build_type_sub(SPerl_PARSER* parser, SPerl_OP* op_argument_types, SPerl_OP* op_return_type);

SPerl_OP* SPerl_OP_build_field(SPerl_PARSER* parser, SPerl_OP* op_packagename, SPerl_OP* op_fieldname);
SPerl_OP* SPerl_OP_build_get_enumeration_value(SPerl_PARSER* parser, SPerl_OP* op_enumname);
SPerl_OP* SPerl_OP_build_decl_package(SPerl_PARSER* parser, SPerl_OP* op_package, SPerl_OP* op_packagename, SPerl_OP* op_typedef, SPerl_OP* op_descripters, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_decl_sub(SPerl_PARSER* parser, SPerl_OP* op_sub, SPerl_OP* op_subname, SPerl_OP* op_subargs, SPerl_OP* op_descripters, SPerl_OP* type, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_CONSTVALUE(SPerl_PARSER* parser, SPerl_OP* op_const);
SPerl_OP* SPerl_OP_build_decl_field(SPerl_PARSER* parser, SPerl_OP* op_has, SPerl_OP* op_field_name, SPerl_OP* op_descripters, SPerl_OP* type);
SPerl_OP* SPerl_OP_build_decl_my(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_descripters, SPerl_OP* op_type);
SPerl_OP* SPerl_OP_build_grammer(SPerl_PARSER* parser, SPerl_OP* op_packages);
SPerl_OP* SPerl_OP_build_decl_use(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_var, SPerl_OP* op_desctype);
SPerl_OP* SPerl_OP_build_call_sub(SPerl_PARSER* parser, SPerl_OP* op_invocant, SPerl_OP* op_subname, SPerl_OP* op_terms, SPerl_boolean anon);
void SPerl_OP_build_const_pool(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_newOP_LIST(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_newOP_NULL(SPerl_PARSER* parser);
SPerl_OP* SPerl_OP_build_convert_type(SPerl_PARSER* parser, SPerl_OP* op_type, SPerl_OP* op_term);
void SPerl_OP_resolve_op_convert_type(SPerl_PARSER* parser, SPerl_OP* op_convert_type);

SPerl_char* SPerl_OP_create_sub_complete_name(SPerl_PARSER* parser, SPerl_char* sub_abs_name, SPerl_int argument_count);
SPerl_char* SPerl_OP_create_complete_name(SPerl_PARSER* parser, SPerl_char* package_name, SPerl_char* call_name);
SPerl_char* SPerl_OP_create_abs_name(SPerl_PARSER* parser, SPerl_char* package_name, SPerl_char* base_name);

SPerl_ARRAY* SPerl_OP_create_op_descripters_array(SPerl_PARSER* parser, SPerl_OP* op_descripters);

SPerl_OP* SPerl_OP_newOP(SPerl_PARSER* parser, SPerl_char type, SPerl_OP *first, SPerl_OP *last);
SPerl_OP* SPerl_OP_newOP_flag(SPerl_PARSER* parser, SPerl_int type, SPerl_OP *first, SPerl_OP *last, SPerl_char flags, SPerl_char private);
SPerl_OP* SPerl_OP_sibling_splice(SPerl_PARSER* parser, SPerl_OP* parent, SPerl_OP* start, SPerl_int del_count, SPerl_OP *insert);
SPerl_OP* SPerl_OP_append_elem(SPerl_PARSER* parser, SPerl_OP* first, SPerl_OP* last);

SPerl_OP* SPerl_OP_sibling(SPerl_PARSER* parser, SPerl_OP* o);
void SPerl_OP_moresib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* sib);
void SPerl_OP_lastsib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* parent);
void SPerl_OP_maybesib_set(SPerl_PARSER* parser, SPerl_OP* o, SPerl_OP* sib, SPerl_OP* parent);

SPerl_OP* SPerl_OP_build_decl_enum(SPerl_PARSER* parser, SPerl_OP* op_enum, SPerl_OP* op_enum_block);

#endif
