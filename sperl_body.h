#include <string.h>

#include "sperl_base.h"

enum {
  SPerl_BODY_C_CODE_CLASS,
  SPerl_BODY_C_CODE_ENUM,
  SPerl_BODY_C_CODE_CORE
};

// Class information
struct SPerl_body {
  SPerl_char code;
  SPerl_char* name;
  uv {
    SPerl_BODY_CLASS* body_class;
    SPerl_BODY_ENUM* body_enum;
    SPerl_BODY_CORE* body_core;
  }
};
