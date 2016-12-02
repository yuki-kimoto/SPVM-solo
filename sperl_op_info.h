#ifndef SPERL_OP_INFO_H
#define SPERL_OP_INFO_H

#include "sperl_base.h"




enum {
  SPerl_OP_INFO_C_CODE_NULL,
  SPerl_OP_INFO_C_CODE_DOUBLE2FLOAT,          
  SPerl_OP_INFO_C_CODE_DOUBLE2IINT,          
  SPerl_OP_INFO_C_CODE_DDOUBLE2LONG,          
  SPerl_OP_INFO_C_CODE_FLOAT2DDOUBLE,          
  SPerl_OP_INFO_C_CODE_FLOAT2INT,          
  SPerl_OP_INFO_C_CODE_FLOAT2LONG,          
  SPerl_OP_INFO_C_CODE_INT2CHAR,          
  SPerl_OP_INFO_C_CODE_INT2UCHAR,          
  SPerl_OP_INFO_C_CODE_INT2DOUBLE,          
  SPerl_OP_INFO_C_CODE_INT2FLOAT,          
  SPerl_OP_INFO_C_CODE_INT2LONG,          
  SPerl_OP_INFO_C_CODE_INT2SHORT,          
  SPerl_OP_INFO_C_CODE_LONG2DOUBLE,          
  SPerl_OP_INFO_C_CODE_LONG2FLOAT,          
  SPerl_OP_INFO_C_CODE_LONG2INT,          
  SPerl_OP_INFO_C_CODE_ADD_INT,         
  SPerl_OP_INFO_C_CODE_ADD_LONG,         
  SPerl_OP_INFO_C_CODE_ADD_FLOAT,         
  SPerl_OP_INFO_C_CODE_ADD_DOUBLE,         
  SPerl_OP_INFO_C_CODE_SUBTRACT_INT,    
  SPerl_OP_INFO_C_CODE_SUBTRACT_LONG,    
  SPerl_OP_INFO_C_CODE_SUBTRACT_FLOAT,    
  SPerl_OP_INFO_C_CODE_SUBTRACT_DOUBLE,    
  SPerl_OP_INFO_C_CODE_MULTIPLY_INT,    
  SPerl_OP_INFO_C_CODE_MULTIPLY_LONG,    
  SPerl_OP_INFO_C_CODE_MULTIPLY_FLOAT,    
  SPerl_OP_INFO_C_CODE_MULTIPLY_DOUBLE,    
  SPerl_OP_INFO_C_CODE_DIVIDE_INT,      
  SPerl_OP_INFO_C_CODE_DIVIDE_LONG,      
  SPerl_OP_INFO_C_CODE_DIVIDE_FLOAT,      
  SPerl_OP_INFO_C_CODE_DIVIDE_DOUBLE,      
  SPerl_OP_INFO_C_CODE_NEGATE_INT,      
  SPerl_OP_INFO_C_CODE_NEGATE_LONG,      
  SPerl_OP_INFO_C_CODE_NEGATE_FLOAT,      
  SPerl_OP_INFO_C_CODE_NEGATE_DOUBLE,      
  SPerl_OP_INFO_C_CODE_REMAINDER_INT,   
  SPerl_OP_INFO_C_CODE_REMAINDER_LONG,   
  SPerl_OP_INFO_C_CODE_REMAINDER_FLOAT,   
  SPerl_OP_INFO_C_CODE_REMAINDER_DOUBLE,   
  SPerl_OP_INFO_C_CODE_IPLUS,        
  SPerl_OP_INFO_C_CODE_LPLUS,        
  SPerl_OP_INFO_C_CODE_FPLUS,        
  SPerl_OP_INFO_C_CODE_DPLUS,        
  SPerl_OP_INFO_C_CODE_RETURN_INT,      
  SPerl_OP_INFO_C_CODE_RETURN_LONG,      
  SPerl_OP_INFO_C_CODE_FETURN_FLOAT,      
  SPerl_OP_INFO_C_CODE_RETURN_DOUBLE,      
  SPerl_OP_INFO_C_CODE_RETURN_REF,      
  SPerl_OP_INFO_C_CODE_RETURN,       
  SPerl_OP_INFO_C_CODE_BIT_AND_INT,         
  SPerl_OP_INFO_C_CODE_BIT_AND_LONG,         
  SPerl_OP_INFO_C_CODE_BIT_OR_INT,          
  SPerl_OP_INFO_C_CODE_BIT_OR_LONG,          
  SPerl_OP_INFO_C_CODE_BIT_XOR_INT,         
  SPerl_OP_INFO_C_CODE_BIT_XOR_LONG,         
  SPerl_OP_INFO_C_CODE_POP,
  SPerl_OP_INFO_C_CODE_POP2,
};

extern SPerl_uchar* const SPerl_OP_INFO_C_CODE_NAMES[];

struct SPerl_op_info {
  SPerl_int code;
  SPerl_RESOLVED_TYPE* return_resolved_type;
  SPerl_ARRAY* argument_resolved_types;
  SPerl_int argument_count;
  SPerl_boolean lvalue;
};

SPerl_OP_INFO* SPerl_OP_INFO_new(SPerl_PARSER* parser);

#endif
