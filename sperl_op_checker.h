#ifndef SPERL_OP_CHECKER_H
#define SPERL_OP_CHECKER_H

#include "sperl_base.h"

void SPerl_OP_CHECKER_check(SPerl* sperl);
void SPerl_OP_CHECKER_update_operand_stack_info(SPerl* sperl, SPerl_OP* op, int32_t* operand_stack_max, int32_t* operand_stack_count);

#endif
