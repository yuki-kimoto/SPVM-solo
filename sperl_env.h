#ifndef SPERL_ENV_H
#define SPERL_ENV_H

#include "sperl.h"

struct SPerl_env {
  int64_t* vars;
  int64_t* operand_stack;
};

#endif
