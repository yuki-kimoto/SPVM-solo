#ifndef SPERL_RESOLVED_TYPE_H
#define SPERL_RESOLVED_TYPE_H

#include "sperl_base.h"

enum {
  SPerl_RESOLVED_TYPE_C_CORE_LENGTH = 7
};

enum {
  SPerl_RESOLVED_TYPE_C_ID_BOOLEAN,
  SPerl_RESOLVED_TYPE_C_ID_BYTE,
  SPerl_RESOLVED_TYPE_C_ID_SHORT,
  SPerl_RESOLVED_TYPE_C_ID_INT,
  SPerl_RESOLVED_TYPE_C_ID_LONG,
  SPerl_RESOLVED_TYPE_C_ID_FLOAT,
  SPerl_RESOLVED_TYPE_C_ID_DOUBLE,
};

extern const char* const SPerl_RESOLVED_TYPE_C_CORE_NAMES[];
extern int32_t const SPerl_RESOLVED_TYPE_C_CORE_SIZES[];

extern const char* const SPerl_RESOLVED_TYPE_C_CORE_ARRAY_NAMES[];

struct SPerl_resolved_type {
  SPerl_ARRAY* part_names;
  const char* name;
  int32_t code;
  int32_t name_length;
  int32_t id;
};

SPerl_RESOLVED_TYPE* SPerl_RESOLVED_TYPE_new(SPerl* sperl);
_Bool SPerl_RESOLVED_TYPE_is_array(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_contain_sub(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_integral(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_core_type_array(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_core_type(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
int32_t SPerl_RESOLVED_TYPE_get_array_dimention(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_calculatable_type(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);

#endif
