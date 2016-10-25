#ifndef SPERL_CORE_TYPE_H
#define SPERL_CORE_TYPE_H

#include <stdint.h>

typedef _Bool SPerl_boolean;
typedef uint8_t SPerl_char;
typedef int8_t SPerl_byte;
typedef int16_t SPerl_short;
typedef int32_t SPerl_int;
typedef int64_t SPerl_long;
typedef float SPerl_float;
typedef double SPerl_double;

/* sperl_type.h */
enum C_SPerl_type_type;
typedef enum C_SPerl_type_type C_SPerl_TYPE_TYPE;

struct SPerl_type;
typedef struct SPerl_type SPerl_TYPE;

/* sperl_subtype.h */
struct SPerl_subtype;
typedef struct SPerl_subtype SPerl_SUBTYPE;

/* sperl_enum_value.h */
struct SPerl_enum_value;
typedef struct SPerl_enum_value SPerl_ENUM_VALUE;

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

/* sperl_const_value.h */
struct SPerl_const_value;
typedef struct SPerl_const_value SPerl_CONST_VALUE;

/* sperl_op.h */
struct SPerl_op;
typedef struct SPerl_op SPerl_OP;

enum SPerl_OP_type;
typedef enum SPerl_OP_type SPerl_OP_TYPE;

/* sperl_word.h */
struct SPerl_word;
typedef struct SPerl_word SPerl_WORD;

/* sperl_field.h */
struct SPerl_field;
typedef struct SPerl_field SPerl_FIELD;

/* sperl_descripter.h */
enum SPerl_descripter_type;
typedef enum SPerl_descripter_type SPerl_DESCRIPTER_TYPE;
struct SPerl_descripter;
typedef struct SPerl_descripter SPerl_DESCRIPTER;

/* sperl_method.h */
struct SPerl_method;
typedef struct SPerl_method SPerl_METHOD;

/* sperl_class.h */
struct SPerl_class;
typedef struct SPerl_class SPerl_CLASS;

enum SPerl_class_type;
typedef enum SPerl_class_type SPerl_CLASS_TYPE;

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

enum SPerl_op_expect;
typedef enum SPerl_op_expect SPerl_OP_EXPECT;

#endif
