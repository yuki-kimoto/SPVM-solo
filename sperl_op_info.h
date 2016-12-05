#ifndef SPERL_OP_INFO_H
#define SPERL_OP_INFO_H

#include "sperl_base.h"




enum {
  SPerl_OP_INFO_C_CODE_NULL,
  SPerl_OP_INFO_C_CODE_CONVERT_DOUBLE_TO_FLOAT,          
  SPerl_OP_INFO_C_CODE_CONVERT_DOUBLE_TO_INT,          
  SPerl_OP_INFO_C_CODE_CONVERT_DDOUBLE_TO_LONG,          
  SPerl_OP_INFO_C_CODE_CONVERT_FLOAT_TO_DOUBLE,          
  SPerl_OP_INFO_C_CODE_CONVERT_FLOAT_TO_INT,          
  SPerl_OP_INFO_C_CODE_CONVERT_FLOAT_TO_LONG,          
  SPerl_OP_INFO_C_CODE_CONVERT_INT_TO_BYTE,          
  SPerl_OP_INFO_C_CODE_CONVERT_INT_TO_CHAR,          
  SPerl_OP_INFO_C_CODE_CONVERT_INT_TO_DOUBLE,          
  SPerl_OP_INFO_C_CODE_CONVERT_INT_TO_FLOAT,          
  SPerl_OP_INFO_C_CODE_CONVERT_INT_TO_LONG,          
  SPerl_OP_INFO_C_CODE_CONVERT_INT_TO_SHORT,          
  SPerl_OP_INFO_C_CODE_CONVERT_LONG_TO_DOUBLE,          
  SPerl_OP_INFO_C_CODE_CONVERT_LONG_TO_FLOAT,          
  SPerl_OP_INFO_C_CODE_CONVERT_LONG_TO_INT,          
  SPerl_OP_INFO_C_CODE_RETURN_INT,      
  SPerl_OP_INFO_C_CODE_RETURN_LONG,      
  SPerl_OP_INFO_C_CODE_FETURN_FLOAT,      
  SPerl_OP_INFO_C_CODE_RETURN_DOUBLE,      
  SPerl_OP_INFO_C_CODE_RETURN_REF,      
  SPerl_OP_INFO_C_CODE_RETURN,       
  SPerl_OP_INFO_C_CODE_POP,
  SPerl_OP_INFO_C_CODE_POP2,
};

extern SPerl_char* const SPerl_OP_INFO_C_CODE_NAMES[];

struct SPerl_op_info {
  SPerl_int code;
  SPerl_RESOLVED_TYPE* return_resolved_type;
  SPerl_ARRAY* argument_resolved_types;
  SPerl_int argument_count;
  SPerl_boolean lvalue;
};

SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser);

#endif
