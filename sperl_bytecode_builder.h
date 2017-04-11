#ifndef SPERL_BYTECODE_BUILDER_H
#define SPERL_BYTECODE_BUILDER_H

#include "sperl_base.h"

void SPerl_BYTECODE_BUILDER_build_bytecode_array(SPerl* sperl);
void SPerl_BYTECODE_BUILDER_push_load_bytecode(SPerl* sperl, SPerl_BYTECODE_ARRAY* bytecode_array, SPerl_OP* op_var);
void SPerl_BYTECODE_BUILDER_push_inc_bytecode(SPerl* sperl, SPerl_BYTECODE_ARRAY* bytecode_array, SPerl_OP* op_inc, int32_t value);

#endif
