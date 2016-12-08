#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"
#include "sperl_yacc.h"
#include "sperl_op.h"
#include "sperl_sub.h"
#include "sperl_parser.h"
#include "sperl_constant.h"
#include "sperl_field.h"
#include "sperl_my_var.h"
#include "sperl_var.h"
#include "sperl_memory_pool.h"
#include "sperl_use.h"
#include "sperl_word.h"
#include "sperl_enumeration_value.h"
#include "sperl_descripter.h"
#include "sperl_type.h"
#include "sperl_type_component_word.h"
#include "sperl_type_component_array.h"
#include "sperl_type_component_sub.h"
#include "sperl_type_part.h"
#include "sperl_body.h"
#include "sperl_body_core.h"
#include "sperl_enumeration.h"
#include "sperl_body_class.h"
#include "sperl_package.h"
#include "sperl_name.h"
#include "sperl_op_info.h"
#include "sperl_name.h"
#include "sperl_op_info.h"
#include "sperl_resolved_type.h"
#include "sperl_constant_pool.h"
#include "sperl_bytecode.h"
#include "sperl_bytecodes.h"








SPerl_char* const SPerl_OP_C_CODE_NAMES[] = {
  "IF",
  "ELSIF",
  "ELSE",
  "FOR",
  "WHILE",
  "NULL",
  "LIST",
  "PUSHMARK",
  "GRAMMER",
  "WORD",
  "DECL_PACKAGE",
  "DECL_MY_VAR",
  "DECL_FIELD",
  "DECL_SUB",
  "DECL_ENUM",
  "DECL_DESCRIPTER",
  "DECL_ANON_SUB",
  "DECL_ENUMERATION_VALUE",
  "BLOCK",
  "ENUM_BLOCK",
  "CLASS_BLOCK",
  "TYPE",
  "DESCRIPTER_ENUM",
  "CONSTANT",
  "PRE_INC",
  "POST_INC",
  "PRE_DEC",
  "POST_DEC",
  "BIT_NOT",
  "COMPLEMENT",
  "NEGATE",
  "PLUS",
  "LT",
  "LE",
  "GT",
  "GE",
  "ADD",
  "SUBTRACT",
  "MULTIPLY",
  "DIVIDE",
  "BIT_AND",
  "BIT_OR",
  "REMAINDER",
  "BIT_XOR",
  "EQ",
  "NE",
  "LEFT_SHIFT",
  "RIGHT_SHIFT",
  "RIGHT_SHIFT_UNSIGNED",
  "AND",
  "OR",
  "NOT",
  "ARRAY_ELEM",
  "FIELD",
  "ASSIGN",
  "CALL_SUB",
  "IF_STATEMENT",
  "USE",
  "RETURN",
  "LAST",
  "NEXT",
  "LOOP",
  "VAR",
  "GET_ENUMERATION_VALUE",
  "CONVERT",
  "POP",
  "NEW_ARRAY",
  "UNDEF",
  "NEW",
  "NEW_OBJECT",
  "NEW_ARRAY_OBJECT",
  "TERM_STATEMENT",
};

SPerl_OP* SPerl_OP_build_new_object(SPerl_PARSER* parser, SPerl_OP* op_new, SPerl_OP* op_type) {
  SPerl_OP* op_new_object = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NEW_OBJECT, NULL, NULL);
  SPerl_OP_sibling_splice(parser, op_new_object, NULL, 0, op_type);
  op_new_object->file = op_new->file;
  op_new_object->line = op_new->line;
  
  op_new_object->uv.op_info = SPerl_OP_INFO_new(parser);

  SPerl_ARRAY_push(parser->op_types, op_type);
  
  return op_new_object;
}

void SPerl_OP_create_bytecodes(SPerl_PARSER* parser) {
  for (SPerl_int i = 0; i < parser->op_subs->length; i++) {
    SPerl_OP* op_sub = SPerl_ARRAY_fetch(parser->op_subs, i);
    SPerl_SUB* sub = op_sub->uv.sub;
    
    // VM codes
    SPerl_BYTECODES* bytecodes = sub->bytecodes;
    
    // Run OPs
    SPerl_OP* op_base = op_sub;
    SPerl_OP* op_cur = op_base;
    SPerl_boolean finish = 0;
    while (op_cur) {
      // [START]Preorder traversal position
      
      // [END]Preorder traversal position
      
      if (op_cur->first) {
        op_cur = op_cur->first;
      }
      else {
        while (1) {
          // [START]Postorder traversal position
          switch (op_cur->code) {
            case SPerl_OP_C_CODE_LEFT_SHIFT: {
              
              if (op_cur->uv.op_info->resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISHL);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSHL);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_RIGHT_SHIFT: {
              
              if (op_cur->uv.op_info->resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISHR);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSHR);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED: {
              
              if (op_cur->uv.op_info->resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IUSHR);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LUSHR);
              }
              
              break;
            }
                                    
            case SPerl_OP_C_CODE_NEW_OBJECT: {
              
              SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEW);
              
              break;
            }
            case SPerl_OP_C_CODE_UNDEF: {
              
              SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ACONST_NULL);
              
              break;
            }
            case SPerl_OP_C_CODE_PRE_INC: {
              SPerl_VAR* var = op_cur->first->uv.var;
              SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
              
              if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, 1);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, 1);
              }
              
              // TODO
              
              break;
            }
            case SPerl_OP_C_CODE_POST_INC: {
              SPerl_VAR* var = op_cur->first->uv.var;
              SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
              
              if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, 1);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, 1);
              }

              break;
            }
            case SPerl_OP_C_CODE_PRE_DEC: {
              SPerl_VAR* var = op_cur->first->uv.var;
              SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
              
              if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, -1);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, -1);
              }
              
              // TODO
              
              break;
            }
            case SPerl_OP_C_CODE_POST_DEC: {
              SPerl_VAR* var = op_cur->first->uv.var;
              SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
              
              if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, -1);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, -1);
              }

              break;
            }
            case SPerl_OP_C_CODE_BIT_XOR: {
              
              if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_XOR_INT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_XOR_LONG);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_BIT_OR: {
              
              if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_OR_INT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_OR_LONG);
              }
                            
              break;
            }
            case SPerl_OP_C_CODE_BIT_AND: {
              
              if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_AND_INT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_AND_LONG);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_ARRAY_ELEM: {
              
              SPerl_OP_INFO* op_info = op_cur->uv.op_info;
              
              if (op_cur->lvalue) {
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = op_info->resolved_type;
              
              if (resolved_type->id == SPerl_BODY_CORE_C_CODE_BOOLEAN
                || resolved_type->id == SPerl_BODY_CORE_C_CODE_CHAR
                || resolved_type->id == SPerl_BODY_CORE_C_CODE_BYTE)
              {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_ARRAY_CHAR);
              }
              else if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_SHORT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_ARRAY_SHORT);
              }
              else if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_ARRAY_INT);
              }
              else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_ARRAY_LONG);
              }
              else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_ARRAY_FLOAT);
              }
              else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_ARRAY_DOUBLE);
              }
              else {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_ARRAY_REF);
              }              
              break;
            }
            case SPerl_OP_C_CODE_ASSIGN: {
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
              
              if (op_cur->first->code == SPerl_OP_C_CODE_VAR) {
                SPerl_OP* op_var = op_cur->first;
                SPerl_int my_var_pos = op_var->uv.var->op_my_var->uv.my_var->pos;
                
                if (my_var_pos > 0xFF) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_WIDE);
                }
                
                SPerl_boolean has_operand = 0;
                if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_INT_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_INT_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_INT_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_INT_3);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_INT);
                    has_operand = 1;
                  }
                }
                else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_LONG_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_LONG_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_LONG_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_LONG_3);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_LONG);
                    has_operand = 1;
                  }
                }
                else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_FLOAT_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_FLOAT_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_FLOAT_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_FLOAT_3);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_FLOAT);
                    has_operand = 1;
                  }
                }
                else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_DOUBLE_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_DOUBLE_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_DOUBLE_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_DOUBLE_3);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_DOUBLE);
                    has_operand = 1;
                  }
                }
                else {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_REF_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_REF_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_REF_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_REF_3);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_REF);
                    has_operand = 1;
                  }
                }
                
                if (has_operand) {
                  if (my_var_pos > 0xFF) {
                    SPerl_BYTECODES_push(bytecodes, (my_var_pos >> 8) & 0xFF);
                    SPerl_BYTECODES_push(bytecodes, my_var_pos);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, my_var_pos);
                  }
                }
              }
              else if (op_cur->first->code == SPerl_OP_C_CODE_ARRAY_ELEM) {
                SPerl_OP* op_op_info = op_cur->first;
                
                if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_ARRAY_INT);
                }
                else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_ARRAY_LONG);
                }
                else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_ARRAY_FLOAT);
                }
                else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_ARRAY_DOUBLE);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_STORE_ARRAY_REF);
                }
              }
              
              break;
            }
            
            case SPerl_OP_C_CODE_RETURN: {
              
              if (!op_cur->uv.op_info->resolved_type) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_RETURN);
              }
              else if (op_cur->uv.op_info->resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_RETURN_INT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_RETURN_LONG);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FETURN_FLOAT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_RETURN_DOUBLE);
              }
              else {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_RETURN_REF);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_NEGATE: {
              
              if (op_cur->uv.op_info->resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEGATE_INT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEGATE_LONG);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEGATE_FLOAT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEGATE_DOUBLE);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_CONVERT: {
              
              SPerl_OP* op_term = op_cur->first;
              SPerl_RESOLVED_TYPE* resolved_type_src = SPerl_OP_get_resolved_type(parser, op_term);
              if (op_cur->uv.op_info->resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                if (resolved_type_src->id == SPerl_BODY_CORE_C_CODE_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_LONG_TO_INT);
                }
                else if (resolved_type_src->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_FLOAT_TO_INT);
                }
                else if (resolved_type_src->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_INT);
                }
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                if (resolved_type_src->id <= SPerl_BODY_CORE_C_CODE_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_INT_TO_LONG);
                }
                else if (resolved_type_src->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_FLOAT_TO_LONG);
                }
                else if (resolved_type_src->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_DDOUBLE_TO_LONG);
                }
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                if (resolved_type_src->id <= SPerl_BODY_CORE_C_CODE_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_INT_TO_FLOAT);
                }
                else if (resolved_type_src->id == SPerl_BODY_CORE_C_CODE_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_LONG_TO_FLOAT);
                }
                else if (resolved_type_src->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_FLOAT);
                }
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                if (resolved_type_src->id <= SPerl_BODY_CORE_C_CODE_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_INT_TO_DOUBLE);
                }
                else if (resolved_type_src->id == SPerl_BODY_CORE_C_CODE_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_LONG_TO_DOUBLE);
                }
                else if (resolved_type_src->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_FLOAT_TO_DOUBLE);
                }
              }
              
              break;
            }
            case SPerl_OP_C_CODE_TERM_STATEMENT: {
              
              SPerl_OP* op_first = op_cur->first;
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_first);
              if (!first_resolved_type) {
                break;
              }
              
              if (op_first->code != SPerl_OP_C_CODE_ASSIGN && !op_first->lvalue) {
                if (first_resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG || first_resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_POP2);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_POP);
                }
              }
              
              break;
            }
            case SPerl_OP_C_CODE_ADD: {
              
              if (op_cur->uv.op_info->resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ADD_INT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ADD_LONG);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ADD_FLOAT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ADD_DOUBLE);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_SUBTRACT: {
              
              if (op_cur->uv.op_info->resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_SUBTRACT_INT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_SUBTRACT_LONG);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_SUBTRACT_FLOAT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_SUBTRACT_DOUBLE);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_MULTIPLY: {
              
              if (op_cur->uv.op_info->resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_MULTIPLY_INT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_MULTIPLY_LONG);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_MULTIPLY_FLOAT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_MULTIPLY_DOUBLE);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_DIVIDE: {
              
              if (op_cur->uv.op_info->resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DIVIDE_INT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DIVIDE_LONG);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DIVIDE_FLOAT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DIVIDE_DOUBLE);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_REMAINDER: {
              
              if (op_cur->uv.op_info->resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_REMAINDER_INT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_REMAINDER_LONG);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_REMAINDER_FLOAT);
              }
              else if (op_cur->uv.op_info->resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_REMAINDER_DOUBLE);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_VAR: {
              SPerl_VAR* var = op_cur->uv.var;
              
              if (op_cur->lvalue) {
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
              
              SPerl_int my_var_pos = var->op_my_var->uv.my_var->pos;
              
              if (my_var_pos > 0xFF) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_WIDE);
              }
              
              SPerl_boolean has_operand = 0;
              if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                if (my_var_pos == 0) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_INT_0);
                }
                else if (my_var_pos == 1) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_INT_1);
                }
                else if (my_var_pos == 2) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_INT_2);
                }
                else if (my_var_pos == 3) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_INT_3);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_INT);
                  has_operand = 1;
                }
              }
              else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                if (my_var_pos == 0) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_LONG_0);
                }
                else if (my_var_pos == 1) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_LONG_1);
                }
                else if (my_var_pos == 2) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_LONG_2);
                }
                else if (my_var_pos == 3) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_LONG_3);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_LONG);
                  has_operand = 1;
                }
              }
              else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                if (my_var_pos == 0) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_FLOAT_0);
                }
                else if (my_var_pos == 1) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_FLOAT_1);
                }
                else if (my_var_pos == 2) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_FLOAT_2);
                }
                else if (my_var_pos == 3) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_FLOAT_3);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_FLOAT);
                  has_operand = 1;
                }
              }
              else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                if (my_var_pos == 0) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_DOUBLE_0);
                }
                else if (my_var_pos == 1) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_DOUBLE_1);
                }
                else if (my_var_pos == 2) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_DOUBLE_2);
                }
                else if (my_var_pos == 3) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_DOUBLE_3);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_DOUBLE);
                  has_operand = 1;
                }
              }
              else {
                if (my_var_pos == 0) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_REF_0);
                }
                else if (my_var_pos == 1) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_REF_1);
                }
                else if (my_var_pos == 2) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_REF_2);
                }
                else if (my_var_pos == 3) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_REF_3);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_REF);
                  has_operand = 1;
                }
              }
              
              if (has_operand) {
                if (my_var_pos > 0xFF) {
                  SPerl_BYTECODES_push(bytecodes, (my_var_pos >> 8) & 0xFF);
                  SPerl_BYTECODES_push(bytecodes, my_var_pos);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, my_var_pos);
                }
              }
              
              break;
            }
            case SPerl_OP_C_CODE_CONSTANT: {
              SPerl_CONSTANT* constant = op_cur->uv.constant;
              
              SPerl_boolean bytecode_set = 0;
              if (constant->code == SPerl_CONSTANT_C_CODE_BOOLEAN) {
                if (constant->uv.int_value == 0) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_INT_0);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 1) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_INT_1);
                  bytecode_set = 1;
                }
              }
              else if (constant->code == SPerl_CONSTANT_C_CODE_INT) {
                if (constant->uv.int_value == -1) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_INT_M1);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 0) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_INT_0);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 1) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_INT_1);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 2) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_INT_2);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 3) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_INT_3);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 4) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_INT_4);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 5) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_INT_5);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value >= -128 && constant->uv.int_value <= 127) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIPUSH);
                  SPerl_BYTECODES_push(bytecodes, constant->uv.int_value & 0xFF);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value >= -32768 && constant->uv.int_value <= 32767) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_SIPUSH);
                  SPerl_BYTECODES_push(bytecodes, (constant->uv.int_value >> 8) & 0xFF);
                  SPerl_BYTECODES_push(bytecodes, constant->uv.int_value & 0xFF);
                  bytecode_set = 1;
                }
              }
              else if (constant->code == SPerl_CONSTANT_C_CODE_LONG) {
                if (constant->uv.long_value == 0) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_LONG_0);
                  bytecode_set = 1;
                }
                else if (constant->uv.long_value == 1) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_LONG_1);
                  bytecode_set = 1;
                }
              }
              else if (constant->code == SPerl_CONSTANT_C_CODE_FLOAT) {
                if (constant->uv.float_value == 0) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_FLOAT_0);
                  bytecode_set = 1;
                }
                else if (constant->uv.float_value == 1) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_FLOAT_1);
                  bytecode_set = 1;
                }
                else if (constant->uv.float_value == 2) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_FLOAT_1);
                  bytecode_set = 1;
                }
              }
              else if (constant->code == SPerl_CONSTANT_C_CODE_DOUBLE) {
                if (constant->uv.double_value == 0) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_DOUBLE_0);
                  bytecode_set = 1;
                }
                else if (constant->uv.double_value == 2) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONSTANT_DOUBLE_1);
                  bytecode_set = 1;
                }
              }
              
              if (!bytecode_set) {
                if (constant->code == SPerl_CONSTANT_C_CODE_LONG || constant->code == SPerl_CONSTANT_C_CODE_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_CONSTANT2_W);
                  SPerl_BYTECODES_push(bytecodes, (constant->pool_pos >> 8) & 0xFF);
                  SPerl_BYTECODES_push(bytecodes, constant->pool_pos & 0xFF);
                }
                else if (constant->pool_pos > 0xFF) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_CONSTANT_W);
                  SPerl_BYTECODES_push(bytecodes, (constant->pool_pos >> 8) & 0xFF);
                  SPerl_BYTECODES_push(bytecodes, constant->pool_pos & 0xFF);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOAD_CONSTANT);
                  SPerl_BYTECODES_push(bytecodes, constant->pool_pos &0xFF);
                }
              }
              
              break;
            }
          }
          
          // [END]Postorder traversal position
          
          if (op_cur == op_base) {
            finish = 1;
            break;
          }
          
          // Next sibling
          if (op_cur->moresib) {
            op_cur = SPerl_OP_sibling(parser, op_cur);
            break;
          }
          // Next is parent
          else {
            op_cur = op_cur->sibparent;
          }
        }
        if (finish) {
          break;
        }
      }
    }
  }
}

SPerl_RESOLVED_TYPE* SPerl_OP_get_resolved_type(SPerl_PARSER* parser, SPerl_OP* op) {
  SPerl_RESOLVED_TYPE*  resolved_type;
  
  switch (op->code) {
    case SPerl_OP_C_CODE_UNDEF : {
      resolved_type = NULL;
      break;
    }
    case SPerl_OP_C_CODE_DECL_ANON_SUB: {
      SPerl_SUB* sub = op->uv.sub;
      resolved_type = sub->op_return_type->uv.type->resolved_type;
      break;
    }
    case SPerl_OP_C_CODE_CONSTANT: {
      SPerl_CONSTANT* constant = op->uv.constant;
      resolved_type = constant->resolved_type;
      break;
    }
    case SPerl_OP_C_CODE_VAR: {
      SPerl_VAR* var = op->uv.var;
      resolved_type = var->op_my_var->uv.my_var->op_type->uv.type->resolved_type;
      break;
    }
    case SPerl_OP_C_CODE_CALL_SUB: {
      SPerl_NAME* name = op->uv.name;
      SPerl_char* complete_name = name->complete_name;
      SPerl_SUB* sub = SPerl_HASH_search(parser->sub_complete_name_symtable, complete_name, strlen(complete_name));
      resolved_type = sub->op_return_type->uv.type->resolved_type;
      break;
    }
    case SPerl_OP_C_CODE_GET_ENUMERATION_VALUE: {
      SPerl_NAME* name = op->uv.name;
      SPerl_char* complete_name = name->complete_name;
      SPerl_ENUMERATION_VALUE* enumeration_value = SPerl_HASH_search(parser->enum_complete_name_symtable, complete_name, strlen(complete_name));
      resolved_type = enumeration_value->op_constant->uv.constant->resolved_type;
      break;
    }
    case SPerl_OP_C_CODE_FIELD: {
      SPerl_NAME* name = op->uv.name;
      SPerl_char* complete_name = name->complete_name;
      SPerl_FIELD* field = SPerl_HASH_search(parser->field_complete_name_symtable, complete_name, strlen(complete_name));
      resolved_type = field->op_type->uv.type->resolved_type;
      break;
    }
    default:
    {
      SPerl_OP_INFO* op_info = op->uv.op_info;
      resolved_type = op_info->resolved_type;
    }
  }
  
  return resolved_type;
}

void SPerl_OP_check_ops(SPerl_PARSER* parser) {
  for (SPerl_int i = 0; i < parser->op_subs->length; i++) {
    
    SPerl_OP* op_sub = SPerl_ARRAY_fetch(parser->op_subs, i);
    SPerl_SUB* sub = op_sub->uv.sub;
    
    // my var informations
    SPerl_int next_my_var_pos = 0;
    SPerl_ARRAY* op_my_vars = SPerl_PARSER_new_array(parser, 0);
    SPerl_HASH* my_var_symtable = SPerl_PARSER_new_hash(parser, 0);
    
    // my variable stack
    SPerl_ARRAY* op_my_var_stack = SPerl_PARSER_new_array(parser, 0);
    
    // block base position stack
    SPerl_ARRAY* block_base_stack = SPerl_PARSER_new_array(parser, 0);
    SPerl_int block_base = 0;
    SPerl_boolean block_start = 0;
    
    // Run OPs
    SPerl_OP* op_base = op_sub;
    SPerl_OP* op_cur = op_base;
    SPerl_boolean finish = 0;
    while (op_cur) {
      // [START]Preorder traversal position
      
      switch (op_cur->code) {
        // Start scope
        case SPerl_OP_C_CODE_BLOCK: {
          if (block_start) {
            SPerl_int* block_base_ptr = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_int));
            *block_base_ptr = op_my_var_stack->length;
            SPerl_ARRAY_push(block_base_stack, block_base_ptr);
            block_base = *block_base_ptr;
          }
          else {
            block_start = 1;
          }
          break;
        }
        case SPerl_OP_C_CODE_ASSIGN: {
          op_cur->first->lvalue = 1;
          break;
        }
      }
      
      // [END]Preorder traversal position
      if (op_cur->first) {
        op_cur = op_cur->first;
      }
      else {
        while (1) {
         // [START]Postorder traversal position
          switch (op_cur->code) {
            case SPerl_OP_C_CODE_LEFT_SHIFT: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_integral(parser, first_resolved_type)) {
                SPerl_yyerror_format(parser, "<< operator left value must be integral at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              if (last_resolved_type->id > SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_yyerror_format(parser, "<< operator right value must be int at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              op_cur->uv.op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_RIGHT_SHIFT: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_integral(parser, first_resolved_type)) {
                SPerl_yyerror_format(parser, ">> operator left value must be integral at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              if (last_resolved_type->id > SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_yyerror_format(parser, ">> operator right value must be int at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              op_cur->uv.op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_integral(parser, first_resolved_type)) {
                SPerl_yyerror_format(parser, ">>> operator left value must be integral at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              if (last_resolved_type->id > SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_yyerror_format(parser, ">>> operator right value must be int at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              op_cur->uv.op_info->resolved_type = resolved_type;
              
              break;
            }
            /*
            case SPerl_OP_C_CODE_NEW_ARRAY_OBJECT: {
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
              
              op_cur->uv.op_info->resolved_type = resolved_type;
              
              if (SPerl_RESOLVED_TYPE_contain_sub(parser, resolved_type)) {
                SPerl_yyerror_format(parser,
                  "new operator can receive sub type %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              if (SPerl_RESOLVED_TYPE_is_array(parser, resolved_type)) {
                SPerl_yyerror_format(parser,
                  "new operator can receive array type %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              break;
            }
            */
            case SPerl_OP_C_CODE_NEW_OBJECT: {
              SPerl_OP* op_type = op_cur->first;
              SPerl_RESOLVED_TYPE* resolved_type = op_type->uv.type->resolved_type;
              
              if (SPerl_RESOLVED_TYPE_contain_sub(parser, resolved_type)) {
                SPerl_yyerror_format(parser,
                  "new operator can receive sub type %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              if (SPerl_RESOLVED_TYPE_is_array(parser, resolved_type)) {
                SPerl_yyerror_format(parser,
                  "new operator can receive array type %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              op_cur->uv.op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_BIT_XOR: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Can receive only core type
              if (first_resolved_type->id >= SPerl_BODY_CORE_C_CODE_FLOAT || last_resolved_type->id >= SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_yyerror_format(parser,
                  "& operator can receive only boolean, char, char, short, int, long type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Insert type converting op
              SPerl_OP_insert_op_convert_type(parser, op_cur);
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_OP_INFO* op_info = op_cur->uv.op_info;
              op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_BIT_OR: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Can receive only core type
              if (first_resolved_type->id >= SPerl_BODY_CORE_C_CODE_FLOAT || last_resolved_type->id >= SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_yyerror_format(parser,
                  "& operator can receive only boolean, char, char, short, int, long type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Insert type converting op
              SPerl_OP_insert_op_convert_type(parser, op_cur);
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_OP_INFO* op_info = op_cur->uv.op_info;
              op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_BIT_AND: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Can receive only core type
              if (first_resolved_type->id >= SPerl_BODY_CORE_C_CODE_FLOAT || last_resolved_type->id >= SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_yyerror_format(parser,
                  "& operator can receive only boolean, char, char, short, int, long type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Insert type converting op
              SPerl_OP_insert_op_convert_type(parser, op_cur);
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_OP_INFO* op_info = op_cur->uv.op_info;
              op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_ARRAY_ELEM: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // First value must be array
              SPerl_boolean first_resolved_type_is_array = SPerl_RESOLVED_TYPE_is_array(parser, first_resolved_type);
              if (!first_resolved_type_is_array) {
                SPerl_yyerror_format(parser, "left value must be array at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Last value must be integer
              if (last_resolved_type->id != SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_yyerror_format(parser, "array index must be integer at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, first_resolved_type->name, strlen(first_resolved_type->name) - 2);
              
              SPerl_OP_INFO* op_info = op_cur->uv.op_info;
              op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_ASSIGN: {
              
              op_cur->lvalue = 1;
              
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Convert type
              if (SPerl_TYPE_is_core_type(parser, first_resolved_type->id) && SPerl_TYPE_is_core_type(parser, last_resolved_type->id)) {
                SPerl_OP_insert_op_convert_type(parser, op_cur);
              }
              
              first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              if (first_resolved_type->id != last_resolved_type->id) {
                SPerl_yyerror_format(parser, "Invalid type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = first_resolved_type;
              SPerl_OP_INFO* op_info = op_cur->uv.op_info;
              
              op_info->resolved_type = resolved_type;
              
              // Insert var op
              if (op_cur->last->code == SPerl_OP_C_CODE_ASSIGN) {
                SPerl_OP* op_var = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_VAR, NULL, NULL);
                op_var->uv.var = op_cur->last->first->uv.var;
                
                SPerl_OP* op_last_old = op_cur->last;
                
                op_last_old->sibparent = op_var;
                
                op_var->first = op_last_old;
                op_var->sibparent = op_cur;
                
                op_cur->last = op_var;
                
                op_cur->first->sibparent = op_var;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_RETURN: {
              if (op_cur->first) {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
                
                // Can receive only core type
                if (first_resolved_type->id != sub->op_return_type->uv.type->resolved_type->id) {
                  SPerl_yyerror_format(parser, "Invalid return type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
                SPerl_OP_INFO* op_info = op_cur->uv.op_info;
                op_info->resolved_type = resolved_type;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_NEGATE: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              
              // Can receive only core type
              if (!SPerl_TYPE_is_core_type(parser, first_resolved_type->id)) {
                SPerl_yyerror_format(parser, "- operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_OP_INFO* op_info = op_cur->uv.op_info;
              op_info->resolved_type = resolved_type;
              
              break;              
            }
            case SPerl_OP_C_CODE_PLUS: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              
              // Can receive only core type
              if (!SPerl_TYPE_is_core_type(parser, first_resolved_type->id)) {
                SPerl_yyerror_format(parser, "+ operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_OP_INFO* op_info = op_cur->uv.op_info;
              op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_ADD: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_TYPE_is_core_type(parser, first_resolved_type->id) || !SPerl_TYPE_is_core_type(parser, last_resolved_type->id)) {
                SPerl_yyerror_format(parser, "+ operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              // Insert type converting op
              SPerl_OP_insert_op_convert_type(parser, op_cur);
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              op_cur->uv.op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_SUBTRACT: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_TYPE_is_core_type(parser, first_resolved_type->id) || !SPerl_TYPE_is_core_type(parser, last_resolved_type->id)) {
                SPerl_yyerror_format(parser, "- operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              // Insert type converting op
              SPerl_OP_insert_op_convert_type(parser, op_cur);
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              op_cur->uv.op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_MULTIPLY: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_TYPE_is_core_type(parser, first_resolved_type->id) || !SPerl_TYPE_is_core_type(parser, last_resolved_type->id)) {
                SPerl_yyerror_format(parser, "* operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              // Insert type converting op
              SPerl_OP_insert_op_convert_type(parser, op_cur);
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              op_cur->uv.op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_DIVIDE: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_TYPE_is_core_type(parser, first_resolved_type->id) || !SPerl_TYPE_is_core_type(parser, last_resolved_type->id)) {
                SPerl_yyerror_format(parser, "/ operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              // Insert type converting op
              SPerl_OP_insert_op_convert_type(parser, op_cur);
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              op_cur->uv.op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_REMAINDER: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_TYPE_is_core_type(parser, first_resolved_type->id) || !SPerl_TYPE_is_core_type(parser, last_resolved_type->id)) {
                SPerl_yyerror_format(parser, "% operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              // Insert type converting op
              SPerl_OP_insert_op_convert_type(parser, op_cur);
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur->first);
              op_cur->uv.op_info->resolved_type = resolved_type;
              
              break;
            }
            case SPerl_OP_C_CODE_PRE_INC:
            case SPerl_OP_C_CODE_POST_INC:
            case SPerl_OP_C_CODE_PRE_DEC:
            case SPerl_OP_C_CODE_POST_DEC: {
              SPerl_OP* first = op_cur->first;
              if (first->code != SPerl_OP_C_CODE_VAR) {
                SPerl_yyerror_format(parser, "invalid lvalue in increment at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, first);
              
              // Only int or long
              if (first_resolved_type->id != SPerl_BODY_CORE_C_CODE_INT &&  first_resolved_type->id != SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_yyerror_format(parser, "must be int or long in increment at %s line %d\n", op_cur->file, op_cur->line);
              }
              op_cur->uv.op_info->resolved_type = first_resolved_type;
              break;
            }
            case SPerl_OP_C_CODE_CONSTANT: {
              SPerl_ARRAY_push(sub->op_constants, op_cur);
              break;
            }
            // End of scope
            case SPerl_OP_C_CODE_BLOCK: {
              SPerl_int* block_base_ptr = SPerl_ARRAY_pop(block_base_stack);
              if (block_base_ptr) {
                SPerl_int block_base = *block_base_ptr;
                for (SPerl_int j = 0; j < op_my_var_stack->length - block_base; j++) {
                  SPerl_ARRAY_pop(op_my_var_stack);
                }
              }
              SPerl_int* before_block_base_ptr = SPerl_ARRAY_fetch(block_base_stack, block_base_stack->length - 1);
              if (before_block_base_ptr) {
                block_base = *before_block_base_ptr;
              }
              else {
                block_base = 0;
              }
              
              break;
            }
            // Add my var
            case SPerl_OP_C_CODE_VAR: {
              SPerl_VAR* var = op_cur->uv.var;
              
              if (op_cur->first && op_cur->first->code == SPerl_OP_C_CODE_DECL_MY_VAR) {
                op_cur->lvalue = 1;
              }
              
              // Serach same name variable
              SPerl_OP* op_my_var = NULL;
              for (SPerl_int i = op_my_var_stack->length - 1 ; i >= 0; i--) {
                SPerl_OP* op_my_var_tmp = SPerl_ARRAY_fetch(op_my_var_stack, i);
                SPerl_MY_VAR* my_var_tmp = op_my_var_tmp->uv.my_var;
                if (strcmp(var->op_name->uv.word->value, my_var_tmp->op_name->uv.word->value) == 0) {
                  op_my_var = op_my_var_tmp;
                  break;
                }
              }
              
              if (op_my_var) {
                // Add my var information to var
                var->op_my_var = op_my_var;
              }
              else {
                // Error
                SPerl_yyerror_format(parser, "\"my %s\" undeclared at %s line %d\n", var->op_name->uv.word->value, op_cur->file, op_cur->line);
                parser->fatal_error = 1;
                return;
              }
              break;
            }
            case SPerl_OP_C_CODE_DECL_MY_VAR: {
              SPerl_MY_VAR* my_var = op_cur->uv.my_var;
              
              // Serach same name variable
              SPerl_int found = 0;
              
              for (SPerl_int i = op_my_var_stack->length - 1 ; i >= block_base; i--) {
                SPerl_OP* op_bef_my_var = SPerl_ARRAY_fetch(op_my_var_stack, i);
                SPerl_MY_VAR* bef_my_var = op_bef_my_var->uv.my_var;
                if (strcmp(my_var->op_name->uv.word->value, bef_my_var->op_name->uv.word->value) == 0) {
                  found = 1;
                  break;
                }
              }
              
              if (found) {
                SPerl_yyerror_format(parser, "redeclaration of my \"%s\" at %s line %d\n", my_var->op_name->uv.word->value, op_cur->file, op_cur->line);
              }
              else {
                // Add my var information
                my_var->pos = next_my_var_pos++;
                SPerl_ARRAY_push(op_my_vars, op_cur);
                my_var->op_sub = op_sub;
                
                SPerl_ARRAY_push(op_my_var_stack, op_cur);
              }
              break;
            }

            case SPerl_OP_C_CODE_CALL_SUB: {
              // Check sub name
              SPerl_NAME* name = op_cur->uv.name;
              if (!name->anon) {
                SPerl_OP_check_sub_name(parser, op_cur);
                if (parser->fatal_error) {
                  return;
                }
              }
              break;
            }
            case SPerl_OP_C_CODE_FIELD: {
              // Check field name
              SPerl_NAME* name = op_cur->uv.name;
              SPerl_OP_check_field_name(parser, name);
              if (parser->fatal_error) {
                return;
              }
              break;
            }
            case SPerl_OP_C_CODE_CONVERT: {
              SPerl_OP* op_term = op_cur->first;
              SPerl_RESOLVED_TYPE* resolved_type_src = SPerl_OP_get_resolved_type(parser, op_term);
              
              SPerl_OP* op_type_dist = op_cur->last;
              SPerl_TYPE* type_dist = op_type_dist->uv.type;
              SPerl_RESOLVED_TYPE* resolved_type_dist = type_dist->resolved_type;
              
              // Can receive only core type
              if (!SPerl_TYPE_is_core_type(parser, resolved_type_src->id) || !SPerl_TYPE_is_core_type(parser, resolved_type_dist->id)) {
                SPerl_yyerror_format(parser, "can't convert type %s to %s at %s line %d\n",
                  resolved_type_src->name, resolved_type_dist->name, op_cur->file, op_cur->line);
              }
              
              // Resolve convert_type op
              if (resolved_type_dist->id <= SPerl_BODY_CORE_C_CODE_INT) {
                op_cur->uv.op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "int", strlen("int"));
              }
              else if (resolved_type_dist->id <= SPerl_BODY_CORE_C_CODE_LONG) {
                op_cur->uv.op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "long", strlen("long"));
              }
              else if (resolved_type_dist->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                op_cur->uv.op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "float", strlen("float"));
              }
              else if (resolved_type_dist->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                op_cur->uv.op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "double", strlen("double"));
              }
            }
            break;
          }
          
          // [END]Postorder traversal position
          
          if (op_cur == op_base) {
            finish = 1;
            break;
          }
          
          // Next sibling
          if (op_cur->moresib) {
            op_cur = SPerl_OP_sibling(parser, op_cur);
            break;
          }
          // Next is parent
          else {
            op_cur = op_cur->sibparent;
          }
        }
        if (finish) {
          break;
        }
      }
    }
    // Set my var information
    sub->op_my_vars = op_my_vars;
  }
}

void SPerl_OP_insert_op_convert_type(SPerl_PARSER* parser, SPerl_OP* op) {

  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op->first);
  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op->last);
  
  SPerl_int first_type_id = first_resolved_type->id;
  SPerl_int last_type_id = last_resolved_type->id;
  
  // last int
  if (first_type_id <= SPerl_BODY_CORE_C_CODE_INT) {
    first_type_id = SPerl_BODY_CORE_C_CODE_INT;
  }
  
  // last int
  if (last_type_id <= SPerl_BODY_CORE_C_CODE_INT) {
    last_type_id = SPerl_BODY_CORE_C_CODE_INT;
  }
  
  if (first_type_id != last_type_id) {
    SPerl_boolean replace_first = 0;
    
    SPerl_OP* type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_CONVERT, NULL, NULL);
    SPerl_OP_INFO* op_info = SPerl_OP_INFO_new(parser);
    type_convert_op->uv.op_info = op_info;
    
    if (first_type_id == SPerl_BODY_CORE_C_CODE_INT) {
      if (last_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "long", strlen("long"));
        replace_first = 1;
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "float", strlen("float"));
        replace_first = 1;
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "double", strlen("double"));
        replace_first = 1;
      }
    }
    else if (first_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
      if (last_type_id == SPerl_BODY_CORE_C_CODE_INT) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "long", strlen("long"));
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "float", strlen("float"));
        replace_first = 1;
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "double", strlen("double"));
        replace_first = 1;
      }
    }
    else if (first_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
      if (last_type_id == SPerl_BODY_CORE_C_CODE_INT) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "float", strlen("float"));
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "long", strlen("long"));
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "double", strlen("double"));
        replace_first = 1;
      }
    }
    else if (first_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
      if (last_type_id == SPerl_BODY_CORE_C_CODE_INT) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "double", strlen("double"));
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "long", strlen("long"));
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        op_info->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "float", strlen("float"));
      }
    }
    
    if (replace_first) {
      type_convert_op->sibparent = op->first->sibparent;
      type_convert_op->moresib = op->first->moresib;
      type_convert_op->first = op->first;
      op->first->sibparent = type_convert_op;
      op->first->moresib = 0;
      op->first = type_convert_op;
    }
    else {
      type_convert_op->sibparent = op->last->sibparent;
      type_convert_op->moresib = op->last->moresib;
      type_convert_op->first = op->last;
      op->last->sibparent = type_convert_op;
      op->last->moresib = 0;
      op->last = type_convert_op;
      
      op->first->sibparent = type_convert_op;
    }
  }
}

void SPerl_OP_resolve_types(SPerl_PARSER* parser) {
  SPerl_ARRAY* op_types = parser->op_types;

  for (SPerl_int i = 0; i < op_types->length; i++) {
    SPerl_OP* op_type = SPerl_ARRAY_fetch(op_types, i);
    SPerl_TYPE_build_parts(parser, op_type->uv.type);
    SPerl_TYPE_build_name(parser, op_type->uv.type);
  }
  
  for (SPerl_int i = 0; i < op_types->length; i++) {
    SPerl_OP* op_type = SPerl_ARRAY_fetch(op_types, i);
    SPerl_OP_resolve_type(parser, op_type->uv.type);
  }
}

void SPerl_OP_check(SPerl_PARSER* parser) {
  
  // Check descripters(Not used)
  SPerl_OP_check_descripters(parser);
  if (parser->fatal_error) {
    return;
  }
  
  // Resolve types
  SPerl_OP_resolve_types(parser);
  if (parser->fatal_error) {
    return;
  }
  
  // Check types
  SPerl_OP_check_ops(parser);
  if (parser->fatal_error) {
    return;
  }
}

void SPerl_OP_check_descripters(SPerl_PARSER* parser) {
  // Check bodys
  SPerl_ARRAY* bodys = parser->bodys;
  SPerl_HASH* body_symtable = parser->body_symtable;
  for (SPerl_int i = 0; i < bodys->length; i++) {
    // Body
    SPerl_BODY* body = SPerl_ARRAY_fetch(bodys, i);
    
    // Check class
    if (body->code == SPerl_BODY_C_CODE_CLASS) {
      SPerl_BODY_CLASS* body_class = body->uv.body_class;
      
      // Check descripter
      SPerl_ARRAY* op_descripters = body_class->op_descripters;
      for (SPerl_int j = 0; j < op_descripters->length; j++) {
        SPerl_OP* op_descripter = SPerl_ARRAY_fetch(op_descripters, j);
        SPerl_DESCRIPTER* descripter = op_descripter->uv.descripter;
       if (descripter->code != SPerl_DESCRIPTER_C_CODE_VALUE)
        {
          SPerl_yyerror_format(parser, "unknown descripter of package \"%s\" at %s line %d\n",
            SPerl_DESCRIPTER_CODE_NAMES[descripter->code], op_descripter->file, op_descripter->line);
        }
      }
      
      
      // Check field
      SPerl_ARRAY* op_fields = body_class->op_fields;
      for (SPerl_int j = 0; j < op_fields->length; j++) {
        SPerl_OP* op_field = SPerl_ARRAY_fetch(op_fields, j);
        SPerl_FIELD* field = op_field->uv.field;

        // Check field descripters(Not used)
        SPerl_ARRAY* op_descripters = field->op_descripters;
        for (SPerl_int k = 0; k < op_descripters->length; k++) {
          SPerl_OP* op_descripter = SPerl_ARRAY_fetch(op_descripters, k);
          SPerl_DESCRIPTER* descripter = op_descripter->uv.descripter;
          if (descripter->code != SPerl_DESCRIPTER_C_CODE_CONST)
          {
            SPerl_yyerror_format(parser, "unknown descripter of has \"%s\" at %s line %d\n",
              SPerl_DESCRIPTER_CODE_NAMES[descripter->code], op_descripter->file, op_descripter->line);
          }
        }
      }
    }
  }

  // Check subs
  SPerl_ARRAY* op_subs = parser->op_subs;
  for (SPerl_int i = 0; i < op_subs->length; i++) {
    SPerl_OP* op_sub = SPerl_ARRAY_fetch(op_subs, i);
    SPerl_SUB* sub = op_sub->uv.sub;
    
    // Check sub descripters(Not used)
    SPerl_ARRAY* op_descripters = sub->op_descripters;
    SPerl_int k;
    for (SPerl_int j = 0; j < op_descripters->length; j++) {
      SPerl_OP* op_descripter = SPerl_ARRAY_fetch(op_descripters, j);
      SPerl_DESCRIPTER* descripter = op_descripter->uv.descripter;
      if (descripter->code != SPerl_DESCRIPTER_C_CODE_STATIC)
      {
        SPerl_yyerror_format(parser, "unknown descripter of sub \"%s\" at %s line %d\n",
          SPerl_DESCRIPTER_CODE_NAMES[descripter->code], op_descripter->file, op_descripter->line);
      }
    }
    
    // Check my var information
    SPerl_ARRAY* op_my_vars = sub->op_my_vars;
    for (SPerl_int j = 0; j < op_my_vars->length; j++) {
      SPerl_OP* op_my_var = SPerl_ARRAY_fetch(op_my_vars, k);
      SPerl_MY_VAR* my_var = op_my_var->uv.my_var;
      
      // Check my_var descripters(Not used)
      SPerl_ARRAY* op_descripters = my_var->op_descripters;
      for (SPerl_int l = 0; l < op_descripters->length; l++) {
        SPerl_OP* op_descripter = SPerl_ARRAY_fetch(op_descripters, l);
        SPerl_DESCRIPTER* descripter = op_descripter->uv.descripter;
        if (descripter->code != SPerl_DESCRIPTER_C_CODE_CONST)
        {
          SPerl_yyerror_format(parser, "unknown descripter of my \"%s\" at %s line %d\n",
            SPerl_DESCRIPTER_CODE_NAMES[descripter->code], op_descripter->file, op_descripter->line);
        }
      }
    }
  }
}

void SPerl_OP_check_sub_name(SPerl_PARSER* parser, SPerl_OP* op_name) {
  SPerl_NAME* name = op_name->uv.name;
  
  SPerl_char* sub_abs_name;
  SPerl_OP* op;
  if (name->op_var) {
    SPerl_char* package_name = name->op_var->uv.var->op_my_var->uv.my_var->op_sub->uv.sub->op_package->uv.package->op_name->uv.word->value;
    SPerl_char* base_name = name->op_name->uv.word->value;
    sub_abs_name = SPerl_OP_create_abs_name(parser, package_name, base_name);
  }
  else if (name->op_name) {
    sub_abs_name = name->op_name->uv.word->value;
  }
  
  SPerl_int argument_count = name->argument_count;
  SPerl_char* sub_complete_name = SPerl_OP_create_sub_complete_name(parser, sub_abs_name, argument_count);
  
  name->complete_name = sub_complete_name;
  
  SPerl_SUB* found_sub= SPerl_HASH_search(
    parser->sub_complete_name_symtable,
    sub_complete_name,
    strlen(sub_complete_name)
  );
  if (!found_sub) {
    SPerl_yyerror_format(parser, "unknown sub \"%s\" at %s line %d\n",
      sub_complete_name, op_name->file, op_name->line);
  }
}

void SPerl_OP_check_field_name(SPerl_PARSER* parser, SPerl_NAME* name) {
  SPerl_char* package_name = name->op_var->uv.var->op_my_var->uv.my_var->op_sub->uv.sub->op_package->uv.package->op_name->uv.word->value;
  SPerl_OP* op_name = name->op_name;
  SPerl_WORD* base_name_word = op_name->uv.word;
  SPerl_char* base_name = base_name_word->value;
  
  SPerl_int complete_name_length = strlen(package_name) + 2 + strlen(base_name);
  
  SPerl_char* complete_name = SPerl_OP_create_complete_name(parser, package_name, base_name);
  name->complete_name = complete_name;
  
  SPerl_FIELD* found_field= SPerl_HASH_search(
    parser->field_complete_name_symtable,
    complete_name,
    strlen(complete_name)
  );
  
  if (!found_field) {
    SPerl_yyerror_format(parser, "unknown field \"%s\" at %s line %d\n",
      complete_name, op_name->file, op_name->line);
  }
}

void SPerl_OP_check_enum_name(SPerl_PARSER* parser, SPerl_NAME* name) {
  SPerl_OP* op_name = name->op_name;
  SPerl_WORD* abs_name_word = op_name->uv.word;
  SPerl_char* abs_name = abs_name_word->value;
  SPerl_char* complete_name = abs_name;
  name->complete_name = complete_name;
  
  SPerl_ENUMERATION_VALUE* found_enum= SPerl_HASH_search(
    parser->enum_complete_name_symtable,
    complete_name,
    strlen(complete_name)
  );
  
  if (!found_enum) {
    SPerl_yyerror_format(parser, "unknown enum \"%s\" at %s line %d\n",
      complete_name, op_name->file, op_name->line);
  }
}

SPerl_OP* SPerl_OP_build_get_enumeration_value(SPerl_PARSER* parser, SPerl_OP* op_enumname) {
  SPerl_OP* op_get_enumeration_value = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_GET_ENUMERATION_VALUE, op_enumname, NULL);
  
  SPerl_NAME* name = SPerl_NAME_new(parser);
  name->code = SPerl_NAME_C_CODE_ENUM;
  name->op_name = op_enumname;
  
  op_get_enumeration_value->uv.name = name;
  
  return op_get_enumeration_value;
}

SPerl_OP* SPerl_OP_build_field(SPerl_PARSER* parser, SPerl_OP* op_var, SPerl_OP* op_fieldname) {
  SPerl_OP* op_field = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_FIELD, NULL, NULL);
  SPerl_OP_sibling_splice(parser, op_field, NULL, 0, op_var);
  SPerl_OP_sibling_splice(parser, op_field, op_var, 0, op_fieldname);
  
  SPerl_NAME* name = SPerl_NAME_new(parser);
  name->code = SPerl_NAME_C_CODE_FIELD;
  name->op_var = op_var;
  name->op_name = op_fieldname;
  
  op_field->uv.name = name;
  
  return op_field;
}

SPerl_OP* SPerl_OP_build_new_array_constant(SPerl_PARSER* parser, SPerl_OP* op_opt_terms) {
  SPerl_OP* op_new_array = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NEW_ARRAY, NULL, NULL);
  SPerl_OP_sibling_splice(parser, op_new_array, NULL, 0, op_opt_terms);
  
  op_new_array->uv.op_info = SPerl_OP_INFO_new(parser);
  
  return op_new_array;
}

SPerl_OP* SPerl_OP_build_convert_type(SPerl_PARSER* parser, SPerl_OP* op_type, SPerl_OP* op_term) {
  
  SPerl_OP* op_convert_type = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_CONVERT, op_term, op_type);
  op_convert_type->file = op_type->file;
  op_convert_type->line = op_type->line;
  
  SPerl_OP_INFO* op_info = SPerl_OP_INFO_new(parser);
  op_convert_type->uv.op_info = op_info;
  
  return op_convert_type;
}

SPerl_OP* SPerl_OP_build_grammer(SPerl_PARSER* parser, SPerl_OP* op_packages) {
  SPerl_OP* op_grammer = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_GRAMMER, op_packages, NULL);
  parser->op_grammer = op_grammer;

  // Resovle types, check types, descripters, and names.
  SPerl_OP_check(parser);
  if (parser->fatal_error) {
    return NULL;
  }
  
  // Build constant pool
  SPerl_OP_build_const_pool(parser);
  
  // Create bytecodes
  SPerl_OP_create_bytecodes(parser);
  
  return op_grammer;
}

// Resolve type and index type
void SPerl_OP_resolve_type(SPerl_PARSER* parser, SPerl_TYPE* type) {
  SPerl_HASH* package_symtable = parser->package_symtable;
  
  if (type->resolved) {
    return;
  }
  else {
    SPerl_int resolved_type_name_length = 0;
    SPerl_ARRAY* resolved_type_part_names = SPerl_PARSER_new_array(parser, 0);
    
    SPerl_ARRAY* parts = type->parts;
    for (SPerl_int i = 0; i < parts->length; i++) {
      SPerl_TYPE_PART* part = SPerl_ARRAY_fetch(parts, i);
      if (part->code == SPerl_TYPE_PART_C_CODE_SUB) {
        resolved_type_name_length += 3;
        SPerl_ARRAY_push(resolved_type_part_names, "sub");
      }
      else if (part->code == SPerl_TYPE_PART_C_CODE_BYTE) {
        resolved_type_name_length++;
        SPerl_ARRAY_push(resolved_type_part_names, part->uv.char_name);
      }
      else {
        SPerl_WORD* part_name_word = part->uv.op_name->uv.word;
        SPerl_char* part_name = part_name_word->value;
        
        SPerl_TYPE* found_type = SPerl_HASH_search(package_symtable, part_name, strlen(part_name));
        if (found_type) {
          SPerl_boolean is_self
            = type->code == SPerl_TYPE_C_CODE_WORD && found_type->code == SPerl_TYPE_C_CODE_WORD
            && strcmp(type->uv.type_component_word->op_name->uv.word->value, found_type->uv.type_component_word->op_name->uv.word->value) == 0;
          
          if (is_self) {
            resolved_type_name_length += strlen(found_type->uv.type_component_word->op_name->uv.word->value);
            SPerl_char* found_part_name = found_type->uv.type_component_word->op_name->uv.word->value;
            SPerl_ARRAY_push(resolved_type_part_names, found_part_name);
          }
          else {
            SPerl_OP_resolve_type(parser, found_type);
            resolved_type_name_length += found_type->resolved_type->name_length;
            for (SPerl_int j = 0; j < found_type->resolved_type->part_names->length; j++) {
              SPerl_char* found_part_name = SPerl_ARRAY_fetch(found_type->resolved_type->part_names, j);
              SPerl_ARRAY_push(resolved_type_part_names, found_part_name);
            }
          }
        }
        else {
          SPerl_yyerror_format(parser, "unknown package \"%s\" at %s line %d\n", part_name, part_name_word->op->file, part_name_word->op->line);
        }
      }
    }
    SPerl_char* resolved_type_name = SPerl_PARSER_new_string(parser, resolved_type_name_length);
    SPerl_int cur_pos = 0;
    for (SPerl_int i = 0; i < resolved_type_part_names->length; i++) {
      SPerl_char* resolved_type_part_name = SPerl_ARRAY_fetch(resolved_type_part_names, i);
      SPerl_int resolved_type_part_name_length = strlen(resolved_type_part_name);
      memcpy(resolved_type_name + cur_pos, resolved_type_part_name, resolved_type_part_name_length);
      cur_pos += resolved_type_part_name_length;
    }
    
    // Create resolved type id
    SPerl_RESOLVED_TYPE* found_resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, resolved_type_name, strlen(resolved_type_name));
    if (found_resolved_type) {
      type->resolved_type = found_resolved_type;
    }
    else {
      SPerl_RESOLVED_TYPE* resolved_type = SPerl_RESOLVED_TYPE_new(parser);
      resolved_type->id = parser->resolved_types->length;
      resolved_type->name = resolved_type_name;
      resolved_type->part_names = resolved_type_part_names;
      SPerl_ARRAY_push(parser->resolved_types, resolved_type);
      SPerl_HASH_insert(parser->resolved_type_symtable, resolved_type_name, strlen(resolved_type_name), resolved_type);
      type->resolved_type = resolved_type;
    }
    type->resolved = 1;
  }
}

void SPerl_OP_build_const_pool(SPerl_PARSER* parser) {
  
  // Subroutines
  SPerl_ARRAY* op_subs = parser->op_subs;
  
  for (SPerl_int i = 0; i < op_subs->length; i++) {
    // Subroutine
    SPerl_OP* op_sub = SPerl_ARRAY_fetch(op_subs, i);
    SPerl_SUB* sub = op_sub->uv.sub;
    
    // Set constant informations
    SPerl_ARRAY* op_constants = sub->op_constants;
    
    // Create constant pool
    SPerl_int const_pool_pos = 0;
    for (SPerl_int j = 0; j < op_constants->length; j++) {
      SPerl_OP* op_constant = SPerl_ARRAY_fetch(op_constants, j);
      SPerl_CONSTANT* constant = op_constant->uv.constant;
      
      SPerl_int value1;
      SPerl_int value2;

      constant->pool_pos = sub->constant_pool->length;
      switch (constant->code) {
        case SPerl_CONSTANT_C_CODE_BOOLEAN:
        case SPerl_CONSTANT_C_CODE_BYTE:
        case SPerl_CONSTANT_C_CODE_INT:
          SPerl_CONSTANT_POOL_push(sub->constant_pool, constant->uv.int_value);
          break;
        case SPerl_CONSTANT_C_CODE_LONG:
          memcpy(&value1, &constant->uv.long_value, 4);
          memcpy(&value2, ((SPerl_int*)&constant->uv.long_value) + 1, 4);
          
          SPerl_CONSTANT_POOL_push(sub->constant_pool, value1);
          SPerl_CONSTANT_POOL_push(sub->constant_pool, value2);
          break;
        case SPerl_CONSTANT_C_CODE_FLOAT:
          memcpy(&value1, &constant->uv.float_value, 4);
          SPerl_CONSTANT_POOL_push(sub->constant_pool, value1);
          break;
        case SPerl_CONSTANT_C_CODE_DOUBLE:
          memcpy(&value1, &constant->uv.double_value, 4);
          memcpy(&value2, ((SPerl_int*)&constant->uv.double_value) + 1, 4);
          
          SPerl_CONSTANT_POOL_push(sub->constant_pool, value1);
          SPerl_CONSTANT_POOL_push(sub->constant_pool, value2);
          break;
      }
    }
  }
}

SPerl_char* SPerl_OP_create_sub_complete_name(SPerl_PARSER* parser, SPerl_char* sub_abs_name, SPerl_int argument_count) {
  // Method complete name - package_name->sub_name(arg1...arg2);
  SPerl_int length = strlen(sub_abs_name) + 2;
  if (argument_count == 1) {
    length += 4;
  }
  else if (argument_count > 1) {
    SPerl_char argument_count_str[6];
    sprintf(argument_count_str, "%d", argument_count);
    argument_count_str[5] = '\0';
    length += 4 + 3 + 3 + strlen(argument_count_str);
  }
  
  SPerl_char* sub_complete_name = SPerl_PARSER_new_string(parser, length);
  
  if (argument_count == 0) {
    sprintf(sub_complete_name, "%s()", sub_abs_name);
  }
  else if (argument_count == 1) {
    sprintf(sub_complete_name, "%s(arg1)", sub_abs_name);
  }
  else if (argument_count > 1) {
    sprintf(sub_complete_name, "%s(arg1...arg%d)", sub_abs_name, argument_count);
  }
  
  return sub_complete_name;
}

SPerl_char* SPerl_OP_create_complete_name(SPerl_PARSER* parser, SPerl_char* package_name, SPerl_char* base_name) {
  SPerl_int length = strlen(package_name) + 2 + strlen(base_name);
  
  SPerl_char* complete_name = SPerl_PARSER_new_string(parser, length);
  
  sprintf(complete_name, "%s::%s", package_name, base_name);
  
  return complete_name;
}

SPerl_char* SPerl_OP_create_abs_name(SPerl_PARSER* parser, SPerl_char* package_name, SPerl_char* base_name) {
  SPerl_int length = strlen(package_name) + 2 + strlen(base_name);
  
  SPerl_char* abs_name = SPerl_PARSER_new_string(parser, length);
  
  sprintf(abs_name, "%s::%s", package_name, base_name);
  
  return abs_name;
}

SPerl_OP* SPerl_OP_build_decl_package(SPerl_PARSER* parser, SPerl_OP* op_package, SPerl_OP* op_package_name, SPerl_OP* op_type, SPerl_OP* op_descripters, SPerl_OP* op_block) {
  SPerl_int i;
  
  SPerl_OP_sibling_splice(parser, op_package, NULL, 0, op_package_name);
  SPerl_OP_sibling_splice(parser, op_package, op_package_name, 0, op_type);
  SPerl_OP_sibling_splice(parser, op_package, op_type, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_package, op_descripters, 0, op_block);
  
  SPerl_WORD* package_name_word = op_package_name->uv.word;
  SPerl_char* package_name = package_name_word->value;
  SPerl_HASH* package_symtable = parser->package_symtable;
  
  // Redeclaration package error
  SPerl_TYPE* found_package = SPerl_HASH_search(package_symtable, package_name, strlen(package_name));
  if (found_package) {
    SPerl_yyerror_format(parser, "redeclaration of package \"%s\" at %s line %d\n", package_name, op_package->file, op_package->line);
  }
  else {
    // Package
    SPerl_PACKAGE* package = SPerl_PACKAGE_new(parser);
    package->op_name = op_package_name;
    
    // Type
    SPerl_TYPE* type;
    
    // Class type
    if (op_type->code == SPerl_OP_C_CODE_NULL) {
      
      // Body
      SPerl_BODY* body = SPerl_BODY_new(parser);
      body->op_name = op_package_name;
      
      // Class type
      
      // Type(type is same as package name)
      type = SPerl_TYPE_new(parser);
      type->code = SPerl_TYPE_C_CODE_WORD;
      SPerl_TYPE_COMPONENT_WORD* type_component_word = SPerl_TYPE_COMPONENT_WORD_new(parser);
      type_component_word->op_name = op_package_name;
      type->uv.type_component_word = type_component_word;
      
      // Type OP
      op_type = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, NULL, NULL);
      op_type->uv.type = type;
      
      // Add type
      package->op_type = op_type;
      SPerl_ARRAY_push(parser->op_types, op_type);
      
      body->code = SPerl_BODY_C_CODE_CLASS;
      
      SPerl_BODY_CLASS* body_class = SPerl_BODY_CLASS_new(parser);
      body_class->op_block = op_block;
      body_class->op_descripters = SPerl_OP_create_op_descripters_array(parser, op_descripters);
      body_class->code = SPerl_BODY_CLASS_C_CODE_NORMAL;
      
      // Class is value class
      for (SPerl_int i = 0; i < body_class->op_descripters->length; i++) {
        SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(body_class->op_descripters, i);
        if (descripter->code == SPerl_DESCRIPTER_C_CODE_VALUE) {
          body_class->is_value_class = 1;
          break;
        }
      }
      
      // Search use and field
      SPerl_ARRAY* op_fields = SPerl_PARSER_new_array(parser, 0);
      SPerl_HASH* field_symtable = SPerl_PARSER_new_hash(parser, 0);
      SPerl_ARRAY* op_uses = SPerl_PARSER_new_array(parser, 0);
      SPerl_HASH* use_symtable = SPerl_PARSER_new_hash(parser, 0);
      
      // Collect field and use information
      SPerl_OP* op_decls = op_block->first;
      SPerl_OP* op_decl = op_decls->first;
      while (op_decl = SPerl_OP_sibling(parser, op_decl)) {
        // Use
        if (op_decl->code == SPerl_OP_C_CODE_USE) {
          SPerl_OP* op_use = op_decl;
          SPerl_USE* use = op_use->uv.use;
          SPerl_char* use_type_name = use->op_package_name->uv.word->value;
          SPerl_USE* found_use
            = SPerl_HASH_search(use_symtable, use_type_name, strlen(use_type_name));
          
          if (found_use) {
            SPerl_yyerror_format(parser, "redeclaration of use \"%s\" at %s line %d\n", use_type_name, op_use->file, op_use->line);
          }
          else {
            SPerl_ARRAY_push(parser->op_use_stack, op_use);
            SPerl_ARRAY_push(op_uses, op_use);
            
            SPerl_HASH_insert(use_symtable, use_type_name, strlen(use_type_name), use);
          }
        }
        // Field
        else if (op_decl->code == SPerl_OP_C_CODE_DECL_FIELD) {
          SPerl_OP* op_has = op_decl;
          SPerl_FIELD* field = op_has->uv.field;
          SPerl_char* field_name = field->op_name->uv.word->value;
          SPerl_FIELD* found_field
            = SPerl_HASH_search(field_symtable, field_name, strlen(field_name));
          if (found_field) {
            SPerl_yyerror_format(parser, "redeclaration of has \"%s\" at %s line %d\n", field_name, op_has->file, op_has->line);
          }
          else {
            // Value class only have core type field
            if (body_class->is_value_class) {
              SPerl_boolean is_core_type = SPerl_TYPE_is_core_type_name(parser, field->op_type->uv.type);
              if (!is_core_type) {
                SPerl_yyerror_format(parser, "value class has only core type field at %s line %d\n", op_has->file, op_has->line);
              }
            }
            
            SPerl_ARRAY_push(op_fields, op_has);
            SPerl_HASH_insert(field_symtable, field_name, strlen(field_name), field);
            
            // Field complete name
            SPerl_char* field_complete_name = SPerl_OP_create_complete_name(parser, package_name, field_name);
            SPerl_HASH_insert(parser->field_complete_name_symtable, field_complete_name, strlen(field_complete_name), field);
          }
        }
      }
      
      // Method information
      SPerl_HASH* sub_complete_name_symtable = parser->sub_complete_name_symtable;
      SPerl_int i = parser->op_subs->length - 1;
      while (1) {
        if (i < 0) {
          break;
        }
        SPerl_OP* op_sub = SPerl_ARRAY_fetch(parser->op_subs, i);
        SPerl_SUB* sub = op_sub->uv.sub;
        if (sub->op_package) {
          break;
        }
        
        if (!sub->anon) {
          SPerl_OP* op_sub_name = sub->op_name;
          SPerl_char* sub_name = op_sub_name->uv.word->value;
          SPerl_char* sub_abs_name = SPerl_OP_create_abs_name(parser, package_name, sub_name);
          SPerl_char* sub_complete_name = SPerl_OP_create_sub_complete_name(parser, sub_abs_name, sub->argument_count);
          
          SPerl_SUB* found_sub = NULL;
          found_sub = SPerl_HASH_search(sub_complete_name_symtable, sub_complete_name, strlen(sub_complete_name));
          
          if (found_sub) {
            SPerl_yyerror_format(parser, "redeclaration of sub \"%s\" at %s line %d\n", sub_complete_name, op_sub->file, op_sub->line);
          }
          // Unknown sub
          else {
            SPerl_HASH_insert(sub_complete_name_symtable, sub_complete_name, strlen(sub_complete_name), sub);
          }
          i--;
        }
        sub->op_package = op_package;
      }

      // Set body
      body_class->op_fields = op_fields;
      body_class->field_symtable = field_symtable;
      body_class->op_uses = op_uses;
      body_class->use_symtable = use_symtable;
      body->uv.body_class = body_class;
      SPerl_ARRAY_push(parser->bodys, body);
      SPerl_HASH_insert(parser->body_symtable, body->op_name->uv.word->value, strlen(body->op_name->uv.word->value), body);
    }
    // Typedef
    else {
      package->op_type = op_type;
      type = op_type->uv.type;
      SPerl_ARRAY_push(parser->op_types, op_type);
    }
    
    // Add package
    op_package->uv.package = package;
    SPerl_ARRAY_push(parser->op_packages, op_package);
    SPerl_HASH_insert(parser->package_symtable, package_name, strlen(package_name), type);
  }
  
  return op_package;
}

SPerl_OP* SPerl_OP_build_decl_use(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_package_name, SPerl_OP* op_packagealias) {
  SPerl_OP_sibling_splice(parser, op_use, NULL, 0, op_package_name);
  SPerl_OP_sibling_splice(parser, op_use, op_package_name, 0, op_packagealias);
  
  SPerl_USE* use = SPerl_USE_new(parser);
  use->op_package_name = op_package_name;
  op_use->uv.use = use;
  
  return op_use;
}

SPerl_OP* SPerl_OP_build_decl_my(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_descripters, SPerl_OP* op_type) {
  
  SPerl_OP_sibling_splice(parser, op_my, NULL, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_my, op_descripters, 0, op_type);
  
  SPerl_OP_sibling_splice(parser, op_var, NULL, 0, op_my);
  
  // Create my var information
  SPerl_MY_VAR* my_var = SPerl_MY_VAR_new(parser);
  SPerl_VAR* var = op_var->uv.var;
  
  // Name OP
  SPerl_OP* op_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_WORD, NULL, NULL);
  op_name->file = op_var->file;
  op_name->line = op_var->line;
  op_name->uv.word = var->op_name->uv.word;
  my_var->op_name = op_name;
  
  // descripters
  my_var->op_descripters = SPerl_OP_create_op_descripters_array(parser, op_descripters);
  
  // type
  my_var->op_type = op_type;
  
  // Add my_var information to op
  op_my->uv.my_var = my_var;
  
  return op_var;
}

SPerl_OP* SPerl_OP_build_decl_field(SPerl_PARSER* parser, SPerl_OP* op_has, SPerl_OP* op_field_name, SPerl_OP* op_descripters, SPerl_OP* op_type) {
  
  // Build OP
  SPerl_OP_sibling_splice(parser, op_has, NULL, 0, op_field_name);
  SPerl_OP_sibling_splice(parser, op_has, op_field_name, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_has, op_descripters, 0, op_type);
  
  // Create field information
  SPerl_FIELD* field = SPerl_FIELD_new(parser);
  
  // Name
  field->op_name = op_field_name;

  // Descripters
  field->op_descripters = SPerl_OP_create_op_descripters_array(parser, op_descripters);
  
  // Type
  field->op_type = op_type;
  
  // Set field informaiton
  op_has->uv.field = field;
  
  return op_has;
}

SPerl_ARRAY* SPerl_OP_create_op_descripters_array(SPerl_PARSER* parser, SPerl_OP* op_descripters) {
  
  SPerl_ARRAY* op_descripters_array = SPerl_PARSER_new_array(parser, 0);
  
  SPerl_OP* op_descripter = op_descripters->first;
  while (op_descripter = SPerl_OP_sibling(parser, op_descripter)) {
    SPerl_ARRAY_push(op_descripters_array, op_descripter);
  }
  
  return op_descripters_array;
}

SPerl_OP* SPerl_OP_build_decl_sub(SPerl_PARSER* parser, SPerl_OP* op_sub, SPerl_OP* op_sub_name, SPerl_OP* op_subargs, SPerl_OP* op_descripters, SPerl_OP* op_type, SPerl_OP* op_block) {
  
  // Build OP_SUB
  SPerl_OP_sibling_splice(parser, op_sub, NULL, 0, op_sub_name);
  SPerl_OP_sibling_splice(parser, op_sub, op_sub_name, 0, op_subargs);
  SPerl_OP_sibling_splice(parser, op_sub, op_subargs, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_sub, op_descripters, 0, op_type);
  SPerl_OP_sibling_splice(parser, op_sub, op_type, 0, op_block);
  
  // Create sub infomation
  SPerl_SUB* sub = SPerl_SUB_new(parser);
  if (op_sub_name->code == SPerl_OP_C_CODE_NULL) {
    sub->anon = 1;
  }
  else {
    sub->op_name = op_sub_name;
  }
  
  // subargs
  SPerl_int argument_count = 0;
  SPerl_OP* op_subarg = op_subargs->first;
  while (op_subarg = SPerl_OP_sibling(parser, op_subarg)) {
    // Increment argument count
    argument_count++;
  }
  sub->argument_count = argument_count;

  // descripters
  sub->op_descripters = SPerl_OP_create_op_descripters_array(parser, op_descripters);
  
  // return type
  sub->op_return_type = op_type;
  
  // Save block
  sub->op_block = op_block;
  
  // ID
  sub->id = parser->op_subs->length;
  
  // Add sub information
  SPerl_ARRAY_push(parser->op_subs, op_sub);
  
  op_sub->uv.sub = sub;
  
  return op_sub;
}

SPerl_OP* SPerl_OP_build_decl_enum(SPerl_PARSER* parser, SPerl_OP* op_enum, SPerl_OP* op_enum_block) {
  
  // Build OP_SUB
  SPerl_OP_sibling_splice(parser, op_enum, NULL, 0, op_enum_block);
  
  // Enum values
  SPerl_ARRAY* enumeration_values = SPerl_PARSER_new_array(parser, 0);
  
  // Starting value
  SPerl_long start_value = 0;
  SPerl_OP* op_enumvalues = op_enum_block->first;
  SPerl_OP* op_enumvalue = op_enumvalues->first;
  while (op_enumvalue = SPerl_OP_sibling(parser, op_enumvalue)) {
    SPerl_ENUMERATION_VALUE* enumeration_value = SPerl_ENUMERATION_VALUE_new(parser);
    enumeration_value->op_name = op_enumvalue->first;
    if (op_enumvalue->last) {
      enumeration_value->op_constant = op_enumvalue->last;
    }
    
    SPerl_CONSTANT* constant;
    if (enumeration_value->op_constant) {
      SPerl_OP* op_constant = enumeration_value->op_constant;
      start_value = op_constant->uv.constant->uv.int_value + 1;
      constant = op_constant->uv.constant;
    }
    else {
      constant = SPerl_CONSTANT_new(parser);
      constant->code = SPerl_CONSTANT_C_CODE_INT;
      constant->uv.int_value = start_value;
      constant->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "int", strlen("int"));
      SPerl_OP* op_constant = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_CONSTANT, NULL, NULL);
      op_constant->uv.constant = constant;
      
      enumeration_value->op_constant = op_constant;
      start_value++;
    }

    // sub
    SPerl_OP* op_sub = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_DECL_SUB, NULL, NULL);
    op_sub->file = op_enumvalue->file;
    op_sub->line = op_enumvalue->line;
    
    // sub name
    SPerl_WORD* sub_name_word = SPerl_WORD_new(parser);
    sub_name_word->value = enumeration_value->op_name->uv.word->value;
    SPerl_OP* op_sub_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_WORD, NULL, NULL);
    op_sub_name->uv.word = sub_name_word;
    
    // sub args
    SPerl_OP* op_subargs = SPerl_OP_newOP_LIST(parser);
    
    // Descripters
    SPerl_OP* op_descripters = SPerl_OP_newOP_LIST(parser);
    
    // Type
    SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, NULL, NULL);
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->resolved_type = constant->resolved_type;
    op_type->uv.type = type;

    // Constant
    SPerl_OP* op_constant = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_CONSTANT, NULL, NULL);
    op_constant->uv.constant = constant;
    
    // Return
    SPerl_OP* op_return = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_RETURN, op_constant, NULL);
    SPerl_OP_INFO* op_return_op_info = SPerl_OP_INFO_new(parser);
    op_return->uv.op_info = op_return_op_info;
    
    // Statement
    SPerl_OP* op_statements = SPerl_OP_newOP_LIST(parser);
    SPerl_OP_sibling_splice(parser, op_statements, op_statements->first, 0, op_return);
    
    // Block
    SPerl_OP* op_block = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_BLOCK, op_statements, NULL);
    
    SPerl_OP_build_decl_sub(parser, op_sub, op_sub_name, op_subargs, op_descripters, op_type, op_block);
  }
  
  return op_enum;
}

SPerl_OP* SPerl_OP_build_call_sub(SPerl_PARSER* parser, SPerl_OP* op_invocant, SPerl_OP* op_sub_name, SPerl_OP* op_terms, SPerl_boolean anon) {
  
  // Build OP_SUB
  SPerl_OP* op_call_sub = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_CALL_SUB, NULL, NULL);
  SPerl_OP_sibling_splice(parser, op_call_sub, NULL, 0, op_sub_name);
  SPerl_OP_sibling_splice(parser, op_call_sub, op_sub_name, 0, op_terms);
  
  SPerl_NAME* name = SPerl_NAME_new(parser);
  name->code = SPerl_NAME_C_CODE_SUB;
  
  if (!anon) {
    SPerl_WORD* sub_name_word = op_sub_name->uv.word;
    SPerl_char* sub_name = sub_name_word->value;
    SPerl_OP* op_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_WORD, NULL, NULL);
    
    if (strstr(sub_name, ":")) {
      op_name->uv.word = sub_name_word;
      name->op_name = op_name;
    }
    else {
      if (op_invocant->code == SPerl_OP_C_CODE_VAR) {
        name->op_var = op_invocant;
        name->op_name = op_sub_name;
      }
      else {
        SPerl_WORD* sub_abs_name_word = SPerl_WORD_new(parser);
        SPerl_char* sub_abs_name = SPerl_OP_create_abs_name(parser, "CORE", sub_name);
        sub_abs_name_word->value = sub_abs_name;
        op_name->uv.word = sub_abs_name_word;
        name->op_name = op_name;
      }
    }
  }
  
  // Add invocant to arguments
  if (op_invocant->code == SPerl_OP_C_CODE_VAR) {
    SPerl_OP_sibling_splice(parser, op_terms, op_terms->first, 0, op_invocant);
  }
  
  name->anon = anon;
  
  // Argument count
  SPerl_int argument_count = 0;
  SPerl_OP* op_term = op_terms->first;
  while (op_term = SPerl_OP_sibling(parser, op_term)) {
    argument_count++;
  }
  name->argument_count = argument_count;
  
  op_call_sub->uv.name = name;
  
  return op_call_sub;
}

SPerl_OP* SPerl_OP_build_call_op(SPerl_PARSER* parser, SPerl_OP* op_call_op, SPerl_OP* op_first, SPerl_OP* op_last) {
  
  // Build OP_SUB
  SPerl_OP_sibling_splice(parser, op_call_op, NULL, 0, op_first);
  if (op_last) {
    SPerl_OP_sibling_splice(parser, op_call_op, op_first, 0, op_last);
  }
  
  SPerl_OP_INFO* op_info = SPerl_OP_INFO_new(parser);
  op_call_op->uv.op_info = op_info;
  
  return op_call_op;
}

SPerl_OP* SPerl_OP_build_type_word(SPerl_PARSER* parser, SPerl_OP* op_word) {
  
  // Type component word
  SPerl_TYPE_COMPONENT_WORD* type_component_word = SPerl_TYPE_COMPONENT_WORD_new(parser);
  type_component_word->op_name = op_word;
  
  // 
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_WORD;
  type->uv.type_component_word = type_component_word;

  SPerl_OP* op_type_word = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, op_word, NULL);
  op_type_word->uv.type = type;
  op_type_word->file = op_word->file;
  op_type_word->line = op_word->line;

  SPerl_ARRAY_push(parser->op_types, op_type_word);
  
  return op_type_word;
}

SPerl_OP* SPerl_OP_build_type_array(SPerl_PARSER* parser, SPerl_OP* op_type, SPerl_OP* op_term) {
  
  // Type array
  SPerl_TYPE_COMPONENT_ARRAY* type_component_array = SPerl_TYPE_COMPONENT_ARRAY_new(parser);
  type_component_array->type = op_type->uv.type;
  type_component_array->op_term = op_term;
  
  // Type
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_ARRAY;
  type->uv.type_component_array = type_component_array;
  
  // Type OP
  SPerl_OP* op_type_array = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, op_type, NULL);
  op_type_array->uv.type = type;
  op_type_array->file = op_type->file;
  op_type_array->line = op_type->line;
  
  SPerl_ARRAY_push(parser->op_types, op_type_array);
  
  return op_type_array;
}

SPerl_OP* SPerl_OP_build_type_sub(SPerl_PARSER* parser, SPerl_OP* op_argument_types, SPerl_OP* op_return_type) {
  
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_SUB;
  
  SPerl_char* file = NULL;
  SPerl_int line = -1;
  
  // sub type
  SPerl_TYPE_COMPONENT_SUB* type_component_sub = SPerl_TYPE_COMPONENT_SUB_new(parser);
  SPerl_ARRAY* argument_types = SPerl_PARSER_new_array(parser, 0);
  {
    SPerl_OP* op_argument_type = op_argument_types->first;
    while (op_argument_type = SPerl_OP_sibling(parser, op_argument_type)) {
      if (file == NULL) {
        file = op_argument_type->file;
        line = op_argument_type->line;
      }
      SPerl_ARRAY_push(argument_types, op_argument_type->uv.type);
    }
  }
  type_component_sub->return_type = op_return_type->uv.type;
  if (file == NULL) {
    file = op_return_type->file;
    line = op_return_type->line;
  }
  type_component_sub->argument_types = argument_types;
  
  type->uv.type_component_sub = type_component_sub;

  SPerl_OP* op_type_sub = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, op_argument_types, op_return_type);
  op_type_sub->uv.type = type;
  op_type_sub->file = file;
  op_type_sub->line = line;

  SPerl_ARRAY_push(parser->op_types, op_type_sub);
  
  return op_type_sub;
}

SPerl_OP* SPerl_OP_newOP(SPerl_PARSER* parser, SPerl_char type, SPerl_OP* first, SPerl_OP* last) {
  return SPerl_OP_newOP_flag(parser, type, first, last, 0, 0);
}

SPerl_OP* SPerl_OP_newOP_flag(SPerl_PARSER* parser, SPerl_int code, SPerl_OP* first, SPerl_OP* last, SPerl_char flags, SPerl_char private) {
        
  SPerl_OP *op = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_OP));
  
  memset(op, 0, sizeof(SPerl_OP));
  
  op->code = code;
  op->first = first;
  
  if (last) {
    if (!first) {
      first = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_OP));
      first->code = SPerl_OP_C_CODE_NULL;
    }
    
    op->last = last;
    SPerl_OP_moresib_set(parser, first, last);
    if (op->last)
      SPerl_OP_lastsib_set(parser, op->last, op);
  }
  else if (first) {
    SPerl_OP_lastsib_set(parser, op->first, op);
  }
  
  return op;
}

SPerl_OP* SPerl_OP_append_elem(SPerl_PARSER* parser, SPerl_OP *first, SPerl_OP *last) {
  if (!first)
    return last;

  if (!last)
    return first;
  
  if (first->code == SPerl_OP_C_CODE_LIST) {
    SPerl_OP_sibling_splice(parser, first, first->last, 0, last);
    return first;
  }
  else {
    SPerl_OP* op_list = SPerl_OP_newOP_LIST(parser);
    SPerl_OP_sibling_splice(parser, op_list, op_list->first, 0, first);
    SPerl_OP_sibling_splice(parser, op_list, first, 0, last);
    
    return op_list;
  }
}

SPerl_OP* SPerl_OP_newOP_LIST(SPerl_PARSER* parser) {
  
  SPerl_OP* op_pushmark = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_PUSHMARK, NULL, NULL);
  
  return SPerl_OP_newOP(parser, SPerl_OP_C_CODE_LIST, op_pushmark, NULL);
}

SPerl_OP* SPerl_OP_newOP_NULL(SPerl_PARSER* parser) {
  
  return SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NULL, NULL, NULL);
}

SPerl_OP* SPerl_OP_sibling_splice(SPerl_PARSER* parser, SPerl_OP* parent, SPerl_OP* start, SPerl_int del_count, SPerl_OP* insert) {
  SPerl_OP *first;
  SPerl_OP *rest;
  SPerl_OP *last_del = NULL;
  SPerl_OP *last_ins = NULL;

  if (start)
    first = SPerl_OP_sibling(parser, start);
  else if (!parent)
    goto no_parent;
  else
    first = parent->first;

  if (del_count && first) {
    last_del = first;
    while (--del_count && last_del->moresib)
      last_del = SPerl_OP_sibling(parser, last_del);
    rest = SPerl_OP_sibling(parser, last_del);
    SPerl_OP_lastsib_set(parser, last_del, NULL);
  }
  else
    rest = first;

  if (insert) {
    last_ins = insert;
    while (last_ins->moresib) {
      last_ins = SPerl_OP_sibling(parser, last_ins);
    }
    SPerl_OP_maybesib_set(parser, last_ins, rest, NULL);
  }
  else {
    insert = rest;
  }

  if (start) {
    SPerl_OP_maybesib_set(parser, start, insert, NULL);
  }
  else {
    if (!parent)
      goto no_parent;
    parent->first = insert;
  }
  
  if (!rest) {
    /* update last etc */
    SPerl_OP *lastop;

    if (!parent)
      goto no_parent;

    lastop = last_ins ? last_ins : start ? start : NULL;
    parent->last = lastop;

    if (lastop)
      SPerl_OP_lastsib_set(parser, lastop, parent);
  }
  return last_del ? first : NULL;

  no_parent:
    fprintf(stderr, "panic: op_sibling_splice(): NULL parent");
    exit(1);
}

SPerl_OP* SPerl_OP_sibling(SPerl_PARSER* parser, SPerl_OP* op) {
  return op->moresib ? op->sibparent : NULL;
}

void SPerl_OP_moresib_set(SPerl_PARSER* parser, SPerl_OP* op, SPerl_OP* sib) {
  op->moresib = 1;
  op->sibparent = sib;
}

void SPerl_OP_lastsib_set(SPerl_PARSER* parser, SPerl_OP* op, SPerl_OP* parent) {
  op->moresib = 0;
  op->sibparent = parent;
}

void SPerl_OP_maybesib_set(SPerl_PARSER* parser, SPerl_OP* op, SPerl_OP* sib, SPerl_OP* parent) {
  op->moresib = sib ? 1 : 0;
  op->sibparent = op->moresib ? sib : parent;
}
