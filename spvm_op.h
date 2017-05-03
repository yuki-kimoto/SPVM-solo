#ifndef SPVM_OP_H
#define SPVM_OP_H

#include "spvm_base.h"











































/* Operation code */
enum {                          // [GROUP]
  SPVM_OP_C_CODE_IF,           // UNKNOWN
  SPVM_OP_C_CODE_ELSIF,        // UNKNOWN
  SPVM_OP_C_CODE_ELSE,         // UNKNOWN
  SPVM_OP_C_CODE_FOR,          // UNKNOWN
  SPVM_OP_C_CODE_WHILE,        // UNKNOWN
  SPVM_OP_C_CODE_NULL,         // UNKNOWN
  SPVM_OP_C_CODE_LIST,         // UNKNOWN
  SPVM_OP_C_CODE_PUSHMARK,     // UNKNOWN
  SPVM_OP_C_CODE_GRAMMAR,      // UNKNOWN
  SPVM_OP_C_CODE_NAME,         // UNKNOWN
  SPVM_OP_C_CODE_PACKAGE,      // UNKNOWN
  SPVM_OP_C_CODE_MY_VAR,   // UNKNOWN
  SPVM_OP_C_CODE_MY_VAR_INIT,   // UNKNOWN
  SPVM_OP_C_CODE_FIELD,   // UNKNOWN
  SPVM_OP_C_CODE_SUB,          // UNKNOWN
  SPVM_OP_C_CODE_DECL_ENUM,
  SPVM_OP_C_CODE_DECL_DESCRIPTOR,   // UNKNOWN
  SPVM_OP_C_CODE_DECL_ENUMERATION_VALUE,    // UNKNOWN
  SPVM_OP_C_CODE_BLOCK,        // UNKNOWN
  SPVM_OP_C_CODE_ENUM_BLOCK,    // UNKNOWN
  SPVM_OP_C_CODE_CLASS_BLOCK,   // UNKNOWN
  SPVM_OP_C_CODE_TYPE,         // UNKNOWN
  SPVM_OP_C_CODE_DESCRIPTOR_ENUM,         // UNKNOWN
  SPVM_OP_C_CODE_CONSTANT,     // CONST
  SPVM_OP_C_CODE_PRE_INC,       // INTDEC
  SPVM_OP_C_CODE_POST_INC,      // INTDEC
  SPVM_OP_C_CODE_PRE_DEC,       // INTDEC
  SPVM_OP_C_CODE_POST_DEC,      // INTDEC
  SPVM_OP_C_CODE_BIT_NOT,       // UNOP
  SPVM_OP_C_CODE_COMPLEMENT,   // UNOP
  SPVM_OP_C_CODE_NEGATE,       // UNOP
  SPVM_OP_C_CODE_PLUS,         // UNOP
  SPVM_OP_C_CODE_EQ,           // BINOP
  SPVM_OP_C_CODE_NE,           // BINOP
  SPVM_OP_C_CODE_LT,           // BINOP
  SPVM_OP_C_CODE_LE,           // BINOP
  SPVM_OP_C_CODE_GT,           // BINOP
  SPVM_OP_C_CODE_GE,           // BINOP
  SPVM_OP_C_CODE_ADD,          // BINOP
  SPVM_OP_C_CODE_SUBTRACT,     // BINOP
  SPVM_OP_C_CODE_MULTIPLY,     // BINOP
  SPVM_OP_C_CODE_DIVIDE,       // BINOP
  SPVM_OP_C_CODE_BIT_AND,       // BINOP
  SPVM_OP_C_CODE_BIT_OR,        // BINOP
  SPVM_OP_C_CODE_REMAINDER,       // BINOP
  SPVM_OP_C_CODE_BIT_XOR,       // BINOP
  SPVM_OP_C_CODE_LEFT_SHIFT,    // BINOP
  SPVM_OP_C_CODE_RIGHT_SHIFT,   // BINOP
  SPVM_OP_C_CODE_RIGHT_SHIFT_UNSIGNED,   // BINOP
  SPVM_OP_C_CODE_AND,          // LOGICALOP
  SPVM_OP_C_CODE_OR,           // LOGICALOP
  SPVM_OP_C_CODE_NOT,          // LOGICALOP
  SPVM_OP_C_CODE_ARRAY_ELEM,        // UNKNOWN
  SPVM_OP_C_CODE_ASSIGN,       // ASSIGN
  SPVM_OP_C_CODE_CALL_SUB,      // UNKNOWN
  SPVM_OP_C_CODE_CALL_FIELD,        // UNKNOWN
  SPVM_OP_C_CODE_USE,          // UNKNOWN
  SPVM_OP_C_CODE_RETURN,       // UNKNOWN
  SPVM_OP_C_CODE_LAST,         // UNKNOWN
  SPVM_OP_C_CODE_NEXT,         // UNKNOWN
  SPVM_OP_C_CODE_LOOP,         // UNKNOWN
  SPVM_OP_C_CODE_VAR,          // UNKNOWN
  SPVM_OP_C_CODE_CONVERT,  // UNKNOWN
  SPVM_OP_C_CODE_POP,          // UNKNOWN
  SPVM_OP_C_CODE_NEW_ARRAY,
  SPVM_OP_C_CODE_UNDEF,
  SPVM_OP_C_CODE_MALLOC,
  SPVM_OP_C_CODE_NEW_ARRAY_CONSTANT,
  SPVM_OP_C_CODE_ARRAY_LENGTH,
  SPVM_OP_C_CODE_CONDITION,
  SPVM_OP_C_CODE_DIE,
  SPVM_OP_C_CODE_SWITCH,
  SPVM_OP_C_CODE_CASE,
  SPVM_OP_C_CODE_DEFAULT,
  SPVM_OP_C_CODE_SWITCH_CONDITION,
  SPVM_OP_C_CODE_DESCRIPTOR,   // UNKNOWN
  SPVM_OP_C_CODE_VOID,
  SPVM_OP_C_CODE_TRY,
  SPVM_OP_C_CODE_CATCH,
  SPVM_OP_C_CODE_DECREFCOUNT,
  SPVM_OP_C_CODE_INCREFCOUNT,
  SPVM_OP_C_CODE_ARGS_MY_VARS,
  SPVM_OP_C_CODE_BLOCK_END,
  SPVM_OP_C_CODE_RETURN_PROCESS,
  SPVM_OP_C_CODE_BEFORE_RETURN,
  SPVM_OP_C_CODE_DIE_PROCESS,
  SPVM_OP_C_CODE_BEFORE_DIE,
};

extern const char* const SPVM_OP_C_CODE_NAMES[];

enum {
  // Block flag
  SPVM_OP_C_FLAG_BLOCK_IF = 1,
  SPVM_OP_C_FLAG_BLOCK_ELSE = 2,
  SPVM_OP_C_FLAG_BLOCK_LOOP = 4,
  SPVM_OP_C_FLAG_BLOCK_SWITCH = 8,
  SPVM_OP_C_FLAG_BLOCK_HAS_ELSE = 16,
  SPVM_OP_C_FLAG_BLOCK_SUB = 32,
};

enum {
  // Condition flag
  SPVM_OP_C_FLAG_CONDITION_IF = 1,
  SPVM_OP_C_FLAG_CONDITION_LOOP = 2,
};

enum {
  // Case flag
  SPVM_OP_C_FLAG_CONSTANT_CASE = 1,
};

enum {
  // Miscellaneous limits for syntactic contraints
  SPVM_OP_LIMIT_CASES             = INT32_MAX , /* should be expressible by the type of SPVM_BYTECODE_C_CODE_LOOKUPSWITCH operands */
  SPVM_OP_LIMIT_FIELDS            = UINT16_MAX, /* should be expressible by the type of SPVM_FIELD::id */
  SPVM_OP_LIMIT_SUBROUTINES       = INT32_MAX , /* should be expressible by the type of SPVM_SUB::id */
  SPVM_OP_LIMIT_TYPES             = INT32_MAX , /* should be expressible by the type of SPVM_RESOLVED_TYPE::id
                                                    and greater than 2 * SPVM_RESOLVED_TYPE_C_CORE_LENGTH */
};

/* Binary operation */
struct SPVM_op {
  SPVM_OP* first;
  SPVM_OP* last;
  SPVM_OP* sibparent;
  const char* file;
  union {
    const char* name;
    SPVM_MY_VAR* my_var;
    SPVM_SUB* sub;
    SPVM_CONSTANT* constant;
    SPVM_NAME_INFO* name_info;
    SPVM_TYPE* type;
    SPVM_VAR* var;
    SPVM_FIELD* field;
    SPVM_PACKAGE* package;
    SPVM_ENUMERATION* enumeration;
    SPVM_SWITCH_INFO* switch_info;
  } uv;
  int32_t code;
  int32_t flag;
  int32_t line;
  _Bool moresib;
  _Bool lvalue;
};

SPVM_OP* SPVM_OP_build_return(SPVM* spvm, SPVM_OP* op_return, SPVM_OP* op_term);
SPVM_OP* SPVM_OP_build_die(SPVM* spvm, SPVM_OP* op_die, SPVM_OP* op_term);

SPVM_OP* SPVM_OP_new_op_constant_int(SPVM* spvm, int32_t value, const char* file, int32_t line);
SPVM_OP* SPVM_OP_new_op_constant_long(SPVM* spvm, int64_t value, const char* file, int32_t line);
SPVM_OP* SPVM_OP_new_op_constant_float(SPVM* spvm, float value, const char* file, int32_t line);
SPVM_OP* SPVM_OP_new_op_constant_double(SPVM* spvm, double value, const char* file, int32_t line);

SPVM_OP* SPVM_OP_new_op_var_from_op_my_var(SPVM* spvm, SPVM_OP* op_my_var);

SPVM_OP* SPVM_OP_get_op_block_from_op_sub(SPVM* spvm, SPVM_OP* op_sub);

void SPVM_OP_convert_to_op_constant_true(SPVM* spvm, SPVM_OP* op);
void SPVM_OP_convert_to_op_constant_false(SPVM* spvm, SPVM_OP* op);

void SPVM_OP_convert_not_to_if(SPVM* spvm, SPVM_OP* op);
void SPVM_OP_convert_and_to_if(SPVM* spvm, SPVM_OP* op);
void SPVM_OP_convert_or_to_if(SPVM* spvm, SPVM_OP* op);

SPVM_OP* SPVM_OP_build_try_catch(SPVM* spvm, SPVM_OP* op_try, SPVM_OP* op_try_block, SPVM_OP* op_catch, SPVM_OP* op_my_var, SPVM_OP* op_catch_block);

SPVM_OP* SPVM_OP_build_switch_statement(SPVM* spvm, SPVM_OP* op_switch, SPVM_OP* op_term, SPVM_OP* op_block);
SPVM_OP* SPVM_OP_build_case_statement(SPVM* spvm, SPVM_OP* op_case, SPVM_OP* op_term);

SPVM_OP* SPVM_OP_build_logical_op(SPVM* spvm, SPVM_OP* op_logical_op, SPVM_OP* op_first, SPVM_OP* op_last);

SPVM_OP* SPVM_OP_build_for_statement(SPVM* spvm, SPVM_OP* op_for, SPVM_OP* op_term_loop_var, SPVM_OP* op_term_condition, SPVM_OP* op_term_next_value, SPVM_OP* op_block);

SPVM_OP* SPVM_OP_build_while_statement(SPVM* spvm, SPVM_OP* op_while, SPVM_OP* op_term_condition, SPVM_OP* op_block);


SPVM_OP* SPVM_OP_build_if_statement(SPVM* spvm, SPVM_OP* op_if, SPVM_OP* op_term, SPVM_OP* op_block, SPVM_OP* op_else_statement);

SPVM_OP* SPVM_OP_build_array_length(SPVM* spvm, SPVM_OP* op_array_length, SPVM_OP* op_term);

SPVM_OP* SPVM_OP_build_malloc_object(SPVM* spvm, SPVM_OP* op_malloc, SPVM_OP* op_type);

SPVM_OP* SPVM_OP_build_array_init(SPVM* spvm, SPVM_OP* op_opt_terms);
void SPVM_OP_resolve_type(SPVM* spvm, SPVM_TYPE* type, int32_t name_length);
void SPVM_OP_check(SPVM* spvm);
void SPVM_OP_resolve_sub_name(SPVM* spvm, SPVM_OP* op_package, SPVM_OP* op_name);
void SPVM_OP_resolve_field_name(SPVM* spvm, SPVM_OP* op_name);

SPVM_RESOLVED_TYPE* SPVM_OP_get_resolved_type(SPVM* spvm, SPVM_OP* op);
SPVM_OP* SPVM_OP_build_binop(SPVM* spvm, SPVM_OP* op_call_op, SPVM_OP* op_first, SPVM_OP* op_last);

SPVM_OP* SPVM_OP_build_type_name(SPVM* spvm, SPVM_OP* op_type_name);
SPVM_OP* SPVM_OP_build_type_array(SPVM* spvm, SPVM_OP* op_type, SPVM_OP* op_term);

SPVM_OP* SPVM_OP_build_call_field(SPVM* spvm, SPVM_OP* op_name_package, SPVM_OP* op_name_field);
SPVM_OP* SPVM_OP_build_package(SPVM* spvm, SPVM_OP* op_package, SPVM_OP* op_name_package, SPVM_OP* op_block);
SPVM_OP* SPVM_OP_build_sub(SPVM* spvm, SPVM_OP* op_sub, SPVM_OP* op_subname, SPVM_OP* op_subargs, SPVM_OP* op_descriptors, SPVM_OP* type, SPVM_OP* op_block);
SPVM_OP* SPVM_OP_build_CONSTVALUE(SPVM* spvm, SPVM_OP* op_const);
SPVM_OP* SPVM_OP_build_field(SPVM* spvm, SPVM_OP* op_field, SPVM_OP* op_field_base_name, SPVM_OP* type);
SPVM_OP* SPVM_OP_build_my_var(SPVM* spvm, SPVM_OP* op_my, SPVM_OP* op_var, SPVM_OP* op_type, SPVM_OP* op_term);
SPVM_OP* SPVM_OP_build_grammar(SPVM* spvm, SPVM_OP* op_packages);
SPVM_OP* SPVM_OP_build_use(SPVM* spvm, SPVM_OP* op_use, SPVM_OP* op_name_package);
SPVM_OP* SPVM_OP_build_call_sub(SPVM* spvm, SPVM_OP* op_invocant, SPVM_OP* op_subname, SPVM_OP* op_terms);
SPVM_OP* SPVM_OP_new_op_list(SPVM* spvm, const char* file, int32_t line);
SPVM_OP* SPVM_OP_build_convert_type(SPVM* spvm, SPVM_OP* op_type, SPVM_OP* op_term);
void SPVM_OP_resolve_op_convert_type(SPVM* spvm, SPVM_OP* op_convert_type);

const char* SPVM_OP_create_abs_name(SPVM* spvm, const char* package_name, const char* base_name);

SPVM_OP* SPVM_OP_sibling_splice(SPVM* spvm, SPVM_OP* parent, SPVM_OP* start, int32_t del_count, SPVM_OP *insert);

SPVM_OP* SPVM_OP_sibling(SPVM* spvm, SPVM_OP* o);
void SPVM_OP_moresib_set(SPVM* spvm, SPVM_OP* o, SPVM_OP* sib);
void SPVM_OP_lastsib_set(SPVM* spvm, SPVM_OP* o, SPVM_OP* parent);
void SPVM_OP_maybesib_set(SPVM* spvm, SPVM_OP* o, SPVM_OP* sib, SPVM_OP* parent);

SPVM_OP* SPVM_OP_build_enumeration(SPVM* spvm, SPVM_OP* op_enumeration, SPVM_OP* op_enumeration_block);

SPVM_OP* SPVM_OP_new_op(SPVM* spvm, int32_t code, const char* file, int32_t line);

SPVM_OP* SPVM_OP_append_elem(SPVM* spvm, SPVM_OP *first, SPVM_OP *last, const char* file, int32_t line);

SPVM_OP* SPVM_OP_build_unop(SPVM* spvm, SPVM_OP* op_unary, SPVM_OP* op_first);

SPVM_OP* SPVM_OP_build_array_elem(SPVM* spvm, SPVM_OP* op_var, SPVM_OP* op_term);

#endif
