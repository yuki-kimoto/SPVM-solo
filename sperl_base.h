#ifndef SPERL_BASE_H
#define SPERL_BASE_H

#include <stdint.h>

enum {
  SPerl_BASE_C_INT_MIN = -2147483648,
  SPerl_BASE_C_INT_MAX = 2147483647,
};

/* sperl_frame.h */
struct SPerl_heap;
typedef struct SPerl_heap SPerl_HEAP;

/* sperl_frame.h */
struct SPerl_frame;
typedef struct SPerl_frame SPerl_FRAME;

/* sperl_parser.h */
struct SPerl_parser;
typedef struct SPerl_parser SPerl_PARSER;

/* sperl_vm.h */
struct SPerl_vm_stack;
typedef struct SPerl_vm_stack SPerl_VM_STACK;

/* sperl_vm.h */
struct SPerl_vm;
typedef struct SPerl_vm SPerl_VM;

/* sperl_switch.h */
struct SPerl_switch_info;
typedef struct SPerl_switch_info SPerl_SWITCH_INFO;

/* sperl_allocator.h */
struct SPerl_allocator;
typedef struct SPerl_allocator SPerl_ALLOCATOR;

/* sperl_dumper.h */
struct SPerl_dumper;
typedef struct SPerl_dumper SPerl_DUMPER;

/* sperl_bytecode_array.h */
struct SPerl_bytecode_array;
typedef struct SPerl_bytecode_array SPerl_BYTECODE_ARRAY;

/* sperl_assign.h */
struct SPerl_bytecode;
typedef struct SPerl_bytecode SPerl_BYTECODE;

/* sperl_vmcode.h */
struct SPerl_vmcodes;
typedef struct SPerl_vmcodes SPerl_VMCODES;

/* sperl_constant_pool_field.h */
struct SPerl_constant_pool_field;
typedef struct SPerl_constant_pool_field SPerl_CONSTANT_POOL_FIELD;

/* sperl_constant_pool_package.h */
struct SPerl_constant_pool_package;
typedef struct SPerl_constant_pool_package SPerl_CONSTANT_POOL_PACKAGE;

/* sperl_constant_pool_sub.h */
struct SPerl_constant_pool_sub;
typedef struct SPerl_constant_pool_sub SPerl_CONSTANT_POOL_SUB;

/* sperl_constant_pool.h */
struct SPerl_constant_pool;
typedef struct SPerl_constant_pool SPerl_CONSTANT_POOL;

/* sperl_vmcode.h */
struct SPerl_vmcode;
typedef struct SPerl_vmcode SPerl_VMCODE;

/* sperl_resolved_type.h */
struct SPerl_resolved_type;
typedef struct SPerl_resolved_type SPerl_RESOLVED_TYPE;

/* sperl_assign.h */
struct SPerl_assign;
typedef struct SPerl_assign SPerl_ASSIGN;

/* sperl_package.h */
struct SPerl_package;
typedef struct SPerl_package SPerl_PACKAGE;

/* sperl_type.h */
struct SPerl_name_info;
typedef struct SPerl_name_info SPerl_NAME_INFO;

/* sperl_type.h */
struct SPerl_type;
typedef struct SPerl_type SPerl_TYPE;

/* sperl_type_component_array.h */
struct SPerl_type_part;
typedef struct SPerl_type_part SPerl_TYPE_PART;

/* sperl_type_component_array.h */
struct SPerl_type_component_name;
typedef struct SPerl_type_component_name SPerl_TYPE_COMPONENT_NAME;

/* sperl_type_component_array.h */
struct SPerl_type_component_array;
typedef struct SPerl_type_component_array SPerl_TYPE_COMPONENT_ARRAY;

/* sperl_type_component_sub.h */
struct SPerl_type_component_sub;
typedef struct SPerl_type_component_sub SPerl_TYPE_COMPONENT_SUB;

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

/* sperl_field.h */
struct SPerl_field;
typedef struct SPerl_field SPerl_FIELD;

/* sperl_descriptor.h */
struct SPerl_descriptor;
typedef struct SPerl_descriptor SPerl_DESCRIPTOR;

/* sperl_sub.h */
struct SPerl_sub;
typedef struct SPerl_sub SPerl_SUB;

/* sperl_array.h */
struct SPerl_array;
typedef struct SPerl_array SPerl_ARRAY;

/* sperl_hash.h */
struct SPerl_hash_entry;
typedef struct SPerl_hash_entry SPerl_HASH_ENTRY;

struct SPerl_hash;
typedef struct SPerl_hash SPerl_HASH;

/* sperl.h */
struct sperl_;
typedef struct sperl_ SPerl;

union SPerl_yystype;
typedef union SPerl_yystype SPerl_YYSTYPE;

#endif
