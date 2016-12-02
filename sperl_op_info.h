#ifndef SPERL_OP_INFO_H
#define SPERL_OP_INFO_H

#include "sperl_base.h"




enum {
  SPerl_OP_INFO_C_CODE_NULL,
  SPerl_OP_INFO_C_CODE_D2F,          
  SPerl_OP_INFO_C_CODE_D2I,          
  SPerl_OP_INFO_C_CODE_D2L,          
  SPerl_OP_INFO_C_CODE_F2D,          
  SPerl_OP_INFO_C_CODE_F2I,          
  SPerl_OP_INFO_C_CODE_F2L,          
  SPerl_OP_INFO_C_CODE_I2C,          
  SPerl_OP_INFO_C_CODE_I2UC,          
  SPerl_OP_INFO_C_CODE_I2D,          
  SPerl_OP_INFO_C_CODE_I2F,          
  SPerl_OP_INFO_C_CODE_I2L,          
  SPerl_OP_INFO_C_CODE_I2S,          
  SPerl_OP_INFO_C_CODE_L2D,          
  SPerl_OP_INFO_C_CODE_L2F,          
  SPerl_OP_INFO_C_CODE_L2I,          
  SPerl_OP_INFO_C_CODE_IADD,         
  SPerl_OP_INFO_C_CODE_LADD,         
  SPerl_OP_INFO_C_CODE_FADD,         
  SPerl_OP_INFO_C_CODE_DADD,         
  SPerl_OP_INFO_C_CODE_ISUBTRACT,    
  SPerl_OP_INFO_C_CODE_LSUBTRACT,    
  SPerl_OP_INFO_C_CODE_FSUBTRACT,    
  SPerl_OP_INFO_C_CODE_DSUBTRACT,    
  SPerl_OP_INFO_C_CODE_IMULTIPLY,    
  SPerl_OP_INFO_C_CODE_LMULTIPLY,    
  SPerl_OP_INFO_C_CODE_FMULTIPLY,    
  SPerl_OP_INFO_C_CODE_DMULTIPLY,    
  SPerl_OP_INFO_C_CODE_IDIVIDE,      
  SPerl_OP_INFO_C_CODE_LDIVIDE,      
  SPerl_OP_INFO_C_CODE_FDIVIDE,      
  SPerl_OP_INFO_C_CODE_DDIVIDE,      
  SPerl_OP_INFO_C_CODE_INEGATE,      
  SPerl_OP_INFO_C_CODE_LNEGATE,      
  SPerl_OP_INFO_C_CODE_FNEGATE,      
  SPerl_OP_INFO_C_CODE_DNEGATE,      
  SPerl_OP_INFO_C_CODE_IREMAINDER,   
  SPerl_OP_INFO_C_CODE_LREMAINDER,   
  SPerl_OP_INFO_C_CODE_FREMAINDER,   
  SPerl_OP_INFO_C_CODE_DREMAINDER,   
  SPerl_OP_INFO_C_CODE_IPLUS,        
  SPerl_OP_INFO_C_CODE_LPLUS,        
  SPerl_OP_INFO_C_CODE_FPLUS,        
  SPerl_OP_INFO_C_CODE_DPLUS,        
  SPerl_OP_INFO_C_CODE_IRETURN,      
  SPerl_OP_INFO_C_CODE_LRETURN,      
  SPerl_OP_INFO_C_CODE_FRETURN,      
  SPerl_OP_INFO_C_CODE_DRETURN,      
  SPerl_OP_INFO_C_CODE_ARETURN,      
  SPerl_OP_INFO_C_CODE_RETURN,       
  SPerl_OP_INFO_C_CODE_IAND,         
  SPerl_OP_INFO_C_CODE_LAND,         
  SPerl_OP_INFO_C_CODE_IOR,          
  SPerl_OP_INFO_C_CODE_LOR,          
  SPerl_OP_INFO_C_CODE_IXOR,         
  SPerl_OP_INFO_C_CODE_LXOR,         
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
