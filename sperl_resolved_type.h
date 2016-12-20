#ifndef SPERL_RESOLVED_TYPE_H
#define SPERL_RESOLVED_TYPE_H

#include "sperl_base.h"

enum {
  SPerl_RESOLVED_TYPE_C_CORE_LENGTH = 8
};

enum {
  SPerl_RESOLVED_TYPE_C_ID_BOOLEAN,
  SPerl_RESOLVED_TYPE_C_ID_CHAR,
  SPerl_RESOLVED_TYPE_C_ID_BYTE,
  SPerl_RESOLVED_TYPE_C_ID_SHORT,
  SPerl_RESOLVED_TYPE_C_ID_INT,
  SPerl_RESOLVED_TYPE_C_ID_LONG,
  SPerl_RESOLVED_TYPE_C_ID_FLOAT,
  SPerl_RESOLVED_TYPE_C_ID_DOUBLE
};

extern uint8_t* const SPerl_RESOLVED_TYPE_C_CORE_NAMES[];
extern int32_t const SPerl_RESOLVED_TYPE_C_CORE_SIZES[];

enum {
  SPerl_RESOLVED_TYPE_C_CODE_CORE,
  SPerl_RESOLVED_TYPE_C_CODE_CLASS,
  SPerl_RESOLVED_TYPE_C_CODE_ARRAY,
  SPerl_RESOLVED_TYPE_C_CODE_SUB,
};

extern uint8_t* const SPerl_RESOLVED_TYPE_C_CODE_NAMES[];

struct SPerl_resolved_type {
  int32_t code;
  SPerl_ARRAY* part_names;
  uint8_t* name;
  int32_t name_length;
  int32_t id;
  SPerl_PACKAGE* package;
};

SPerl_RESOLVED_TYPE* SPerl_RESOLVED_TYPE_new(SPerl_PARSER* parser);
_Bool SPerl_RESOLVED_TYPE_is_array(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_contain_sub(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_integral(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_core_type_array(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_multi_array(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_core_type(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);

#endif
