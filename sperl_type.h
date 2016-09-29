#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include <stdint.h>

typedef _Bool SPerl_boolean;
typedef uint8_t SPerl_char;
typedef int8_t SPerl_byte;
typedef int16_t SPerl_short;
typedef int32_t SPerl_int;
typedef int64_t SPerl_long;
typedef float SPerl_float;
typedef double SPerl_double;

/* sperl_var_info.h */
struct SPerl_var_info;
typedef struct SPerl_var_info SPerl_VAR_INFO;

/* sperl_my_var_info.h */
struct SPerl_my_var_info;
typedef struct SPerl_my_var_info SPerl_MY_VAR_INFO;

/* sperl_const_info.h */
struct SPerl_const_info;
typedef struct SPerl_const_info SPerl_CONST_INFO;

/* sperl_op.h */
struct SPerl_op;
typedef struct SPerl_op SPerl_OP;

/* sperl_argument_info.h */
struct SPerl_argument_info;
typedef struct SPerl_argument_info SPerl_ARGUMENT_INFO;

/* sperl_field_info.h */
struct SPerl_field_info;
typedef struct SPerl_field_info SPerl_FIELD_INFO;

/* sperl_method_info.h */
struct SPerl_method_info;
typedef struct SPerl_method_info SPerl_METHOD_INFO;

/* sperl_class_info.h */
struct SPerl_class_info;
typedef struct SPerl_class_info SPerl_CLASS_INFO;

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

union SPerl_yystype_;
typedef union SPerl_yystype_ SPerl_yystype;

#endif
