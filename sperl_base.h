#ifndef SPERL_BASE_H
#define SPERL_BASE_H

#include <stdint.h>

typedef _Bool SPerl_boolean;
typedef uint8_t SPerl_char;
typedef int8_t SPerl_byte;
typedef int16_t SPerl_short;
typedef int32_t SPerl_int;
typedef int64_t SPerl_long;
typedef float SPerl_float;
typedef double SPerl_double;

/* sperl_bytecode.h */
struct SPerl_dumper;
typedef struct SPerl_dumper SPerl_DUMPER;

/* sperl_bytecode.h */
struct SPerl_bytecodes;
typedef struct SPerl_bytecodes SPerl_BYTECODES;

/* sperl_assign.h */
struct SPerl_bytecode;
typedef struct SPerl_bytecode SPerl_BYTECODE;

/* sperl_vmcode.h */
struct SPerl_vmcodes;
typedef struct SPerl_vmcodes SPerl_VMCODES;

/* sperl_constant_pool.h */
struct SPerl_constant_pool;
typedef struct SPerl_constant_pool SPerl_CONSTANT_POOL;

/* sperl_vmcode.h */
struct SPerl_vmcode;
typedef struct SPerl_vmcode SPerl_VMCODE;

/* sperl_resolved_type.h */
struct SPerl_resolved_type;
typedef struct SPerl_resolved_type SPerl_RESOLVED_TYPE;

/* sperl_op_info.h */
struct SPerl_op_info;
typedef struct SPerl_op_info SPerl_OP_INFO;

/* sperl_assign.h */
struct SPerl_assign;
typedef struct SPerl_assign SPerl_ASSIGN;

/* sperl_package.h */
struct SPerl_package;
typedef struct SPerl_package SPerl_PACKAGE;

/* sperl_type.h */
struct SPerl_name;
typedef struct SPerl_name SPerl_NAME;

/* sperl_type.h */
struct SPerl_type;
typedef struct SPerl_type SPerl_TYPE;

/* sperl_type_component_array.h */
struct SPerl_type_part;
typedef struct SPerl_type_part SPerl_TYPE_PART;

/* sperl_type_component_array.h */
struct SPerl_type_component_word;
typedef struct SPerl_type_component_word SPerl_TYPE_COMPONENT_WORD;

/* sperl_type_component_array.h */
struct SPerl_type_component_array;
typedef struct SPerl_type_component_array SPerl_TYPE_COMPONENT_ARRAY;

/* sperl_type_component_sub.h */
struct SPerl_type_component_sub;
typedef struct SPerl_type_component_sub SPerl_TYPE_COMPONENT_SUB;

/* sperl_body_core.h */
struct SPerl_body_core;
typedef struct SPerl_body_core SPerl_BODY_CORE;

/* sperl_enum.h */
struct SPerl_enumeration;
typedef struct SPerl_enumeration SPerl_ENUMERATION;

/* sperl_enumeration_value.h */
struct SPerl_enumeration_value;
typedef struct SPerl_enumeration_value SPerl_ENUMERATION_VALUE;

/* sperl_use.h */
struct SPerl_use;
typedef struct SPerl_use SPerl_USE;

/* sperl_memory_pool_page.h */
struct SPerl_memory_pool_page;
typedef struct SPerl_memory_pool_page SPerl_MEMORY_POOL_PAGE;

/* sperl_memory_pool.h */
struct SPerl_memory_pool;
typedef struct SPerl_memory_pool SPerl_MEMORY_POOL;

/* sperl_var.h */
struct SPerl_var;
typedef struct SPerl_var SPerl_VAR;

/* sperl_my_var.h */
struct SPerl_my_var;
typedef struct SPerl_my_var SPerl_MY_VAR;

/* sperl_constant.h */
struct SPerl_constant;
typedef struct SPerl_constant SPerl_CONSTANT;

/* sperl_op.h */
struct SPerl_op;
typedef struct SPerl_op SPerl_OP;

/* sperl_word.h */
struct SPerl_word;
typedef struct SPerl_word SPerl_WORD;

/* sperl_field.h */
struct SPerl_field;
typedef struct SPerl_field SPerl_FIELD;

/* sperl_descripter.h */
struct SPerl_descripter;
typedef struct SPerl_descripter SPerl_DESCRIPTER;

/* sperl_sub.h */
struct SPerl_sub;
typedef struct SPerl_sub SPerl_SUB;

/* sperl_body_class.h */
struct SPerl_body;
typedef struct SPerl_body SPerl_BODY;

/* sperl_body_class.h */
struct SPerl_body_class;
typedef struct SPerl_body_class SPerl_BODY_CLASS;

/* sperl_array.h */
struct SPerl_array;
typedef struct SPerl_array SPerl_ARRAY;

/* sperl_hash.h */
struct SPerl_hash_entry;
typedef struct SPerl_hash_entry SPerl_HASH_ENTRY;

struct SPerl_hash;
typedef struct SPerl_hash SPerl_HASH;

/* sperl_parser.h */
struct SPerl_yy_parser_;
typedef struct SPerl_yy_parser_ SPerl_yy_parser;
typedef SPerl_yy_parser SPerl_PARSER;

union SPerl_yystype;
typedef union SPerl_yystype SPerl_YYSTYPE;

#endif
