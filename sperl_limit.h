#ifndef SPERL_LIMIT_H
#define SPERL_LIMIT_H

#include "sperl_base.h"

enum {
  /* should be expressible by the type of SPerl_BYTECODE_C_CODE_LOOKUPSWITCH operands */
  SPerl_LIMIT_C_CASES             = INT32_MAX,
  
  /* should be expressible by the type of SPerl_FIELD::id */
  SPerl_LIMIT_C_FIELDS            = UINT16_MAX,
  
  /* should be expressible by the type of SPerl_SUB::id */
  SPerl_LIMIT_C_SUBROUTINES       = INT32_MAX ,
  
  /* should be expressible by the type of block_base (sperl_op_checker.c) */
  SPerl_LIMIT_C_MY_VARS = 0xFFFF,
  
  /* should be expressible by the type of SPerl_RESOLVED_TYPE::id
    and greater than 2 * SPerl_RESOLVED_TYPE_C_CORE_LENGTH */
  SPerl_LIMIT_C_TYPES             = INT32_MAX , 
};

#endif
