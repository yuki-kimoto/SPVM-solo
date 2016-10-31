#include <string.h>

#include "sperl_base.h"

enum {
  SPerl_BODY_C_CODE_CLASS,
  SPerl_BODY_C_CODE_ENUM,
  SPerl_BODY_C_CODE_CORE
};

extern SPerl_char* const SPerl_BODY_C_CODE_NAMES[];

// Class information
struct SPerl_body {
  SPerl_char code;
  SPerl_char* name;
  union {
    SPerl_BODY_CLASS* body_class;
    SPerl_BODY_ENUM* body_enum;
    SPerl_BODY_CORE* body_core;
  } uv;
};

SPerl_BODY* SPerl_BODY_new(SPerl_PARSER* parser);
