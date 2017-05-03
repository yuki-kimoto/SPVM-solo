#ifndef SPVM_LIMIT_H
#define SPVM_LIMIT_H

#include "spvm_base.h"

enum {
  /* Fields count limit */
  SPVM_LIMIT_C_FIELDS = 0xFFFF,
  
  /* Lexical variables count limit */
  SPVM_LIMIT_C_MY_VARS = 0xFFFF,

  /* Subroutines count limit */
  SPVM_LIMIT_C_SUBS = INT32_MAX ,
  
  /* should be expressible by the type of SPVM_RESOLVED_TYPE::id
    and greater than 2 * SPVM_RESOLVED_TYPE_C_CORE_LENGTH */
  SPVM_LIMIT_C_TYPES = INT32_MAX , 
  
  /* should be expressible by the type of SPVM_BYTECODE_C_CODE_LOOKUPSWITCH operands */
  SPVM_LIMIT_C_CASES = INT32_MAX,
};

#endif
