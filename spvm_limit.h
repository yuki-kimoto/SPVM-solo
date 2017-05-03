#ifndef SPVM_LIMIT_H
#define SPVM_LIMIT_H

#include "spvm_base.h"

enum {
  /* should be expressible by the type of SPVM_BYTECODE_C_CODE_LOOKUPSWITCH operands */
  SPVM_LIMIT_C_CASES             = INT32_MAX,
  
  /* should be expressible by the type of SPVM_FIELD::id */
  SPVM_LIMIT_C_FIELDS            = UINT16_MAX,
  
  /* should be expressible by the type of SPVM_SUB::id */
  SPVM_LIMIT_C_SUBROUTINES       = INT32_MAX ,
  
  /* should be expressible by the type of block_base (spvm_op_checker.c) */
  SPVM_LIMIT_C_MY_VARS = 0xFFFF,
  
  /* should be expressible by the type of SPVM_RESOLVED_TYPE::id
    and greater than 2 * SPVM_RESOLVED_TYPE_C_CORE_LENGTH */
  SPVM_LIMIT_C_TYPES             = INT32_MAX , 
};

#endif
