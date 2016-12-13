
#include "sperl_bytecode_builder.h"
#include "sperl_bytecode.h"
#include "sperl_bytecodes.h"
#include "sperl_parser.h"
#include "sperl_body_core.h"
#include "sperl_body.h"
#include "sperl_constant.h"
#include "sperl_op.h"
#include "sperl_resolved_type.h"
#include "sperl_array.h"
#include "sperl_sub.h"
#include "sperl_var.h"
#include "sperl_my_var.h"
#include "sperl_allocator.h"

void SPerl_BYTECODE_BUILDER_build_bytecodes(SPerl_PARSER* parser) {
  for (SPerl_int i = 0; i < parser->op_subs->length; i++) {
    SPerl_OP* op_sub = SPerl_ARRAY_fetch(parser->op_subs, i);
    SPerl_SUB* sub = op_sub->uv.sub;
    
    // VM codes
    SPerl_BYTECODES* bytecodes = sub->bytecodes;
    
    // Run OPs
    SPerl_OP* op_base = op_sub;
    SPerl_OP* op_cur = op_base;
    SPerl_boolean finish = 0;
    
    SPerl_ARRAY* condition_bytecode_pos_stack = SPerl_ALLOCATOR_new_array(parser, 0);
    
    while (op_cur) {
      // [START]Preorder traversal position
      
      switch (op_cur->code) {
      }
      
      // [END]Preorder traversal position
      
      if (op_cur->first) {
        op_cur = op_cur->first;
      }
      else {
        while (1) {
          // [START]Postorder traversal position
          switch (op_cur->code) {
            case SPerl_OP_C_CODE_CONDITION_TRUE_BLOCK_END: {
              SPerl_int* pos_ptr = SPerl_ARRAY_pop(condition_bytecode_pos_stack);
              
              bytecodes->values[*pos_ptr + 1] = (bytecodes->length >> 8) & 0xFF;
              bytecodes->values[*pos_ptr + 2] = bytecodes->length & 0xFF;
              break;
            }
            case SPerl_OP_C_CODE_CONDITION: {
              SPerl_OP* op_condition_target = op_cur->first;
              
              if (op_condition_target->code == SPerl_OP_C_CODE_EQ) {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target->last);
                
                if (!first_resolved_type || !last_resolved_type) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFNULL);
                }
                else {
                  SPerl_RESOLVED_TYPE* resolved_type = first_resolved_type;
                  if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPEQ);
                  }
                  else if (resolved_type->id >= SPerl_BODY_CORE_C_CODE_LONG && resolved_type->id <= SPerl_BODY_CORE_C_CODE_DOUBLE)  {
                    
                    if (resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                    }
                    else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                    }
                    else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCMPG);
                    }
                    
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFEQ);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ACMPEQ);
                  }
                }
              }
              else if (op_condition_target->code == SPerl_OP_C_CODE_NE) {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target->last);
                
                if (!first_resolved_type || !last_resolved_type) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFNONNULL);
                }
                else {

                  SPerl_RESOLVED_TYPE* resolved_type = first_resolved_type;
                  if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPNE);
                  }
                  else if (resolved_type->id >= SPerl_BODY_CORE_C_CODE_LONG && resolved_type->id <= SPerl_BODY_CORE_C_CODE_DOUBLE)  {
                    
                    if (resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                    }
                    else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                    }
                    else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCMPG);
                    }
                    
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFNE);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ACMPNE);
                  }
                }
              }
              else if (op_condition_target->code == SPerl_OP_C_CODE_GT) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target->first);
                if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPGT);
                }
                else if (resolved_type->id >= SPerl_BODY_CORE_C_CODE_LONG && resolved_type->id <= SPerl_BODY_CORE_C_CODE_DOUBLE)  {
                  
                  if (resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                  }
                  else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                  }
                  else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCMPG);
                  }
                  
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFGT);
                }
              }
              else if (op_condition_target->code == SPerl_OP_C_CODE_GE) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target->first);
                if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPGE);
                }
                else if (resolved_type->id >= SPerl_BODY_CORE_C_CODE_LONG && resolved_type->id <= SPerl_BODY_CORE_C_CODE_DOUBLE)  {
                  
                  if (resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                  }
                  else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                  }
                  else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCMPG);
                  }
                  
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFGE);
                }
              }
              else if (op_condition_target->code == SPerl_OP_C_CODE_LT) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target->first);
                if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPLT);
                }
                else if (resolved_type->id >= SPerl_BODY_CORE_C_CODE_LONG && resolved_type->id <= SPerl_BODY_CORE_C_CODE_DOUBLE)  {
                  
                  if (resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                  }
                  else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                  }
                  else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCMPG);
                  }
                  
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFLT);
                }
              }
              else if (op_condition_target->code == SPerl_OP_C_CODE_LE) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target->first);
                if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPLE);
                }
                else if (resolved_type->id >= SPerl_BODY_CORE_C_CODE_LONG && resolved_type->id <= SPerl_BODY_CORE_C_CODE_DOUBLE)  {
                  
                  if (resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                  }
                  else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                  }
                  else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCMPG);
                  }
                  
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFLE);
                }
              }
              else if (op_condition_target) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target);
                
                if (resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFNE);
                }
                else if (resolved_type->id >= SPerl_BODY_CORE_C_CODE_LONG && resolved_type->id <= SPerl_BODY_CORE_C_CODE_DOUBLE)  {
                  
                  if (resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_LONG_TO_INT);
                  }
                  else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_FLOAT_TO_INT);
                  }
                  else if (resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_INT);
                  }
                  
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFNE);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFNONNULL);
                }
              }
              
              SPerl_int* pos_ptr = SPerl_ALLOCATOR_new_int(parser);
              *pos_ptr = bytecodes->length - 1;
              
              SPerl_ARRAY_push(condition_bytecode_pos_stack, pos_ptr);
              
              // Prepare for bytecode position of branch
              SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NOP);
              SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NOP);
              
              break;
            }
            case SPerl_OP_C_CODE_ARRAY_LENGTH : {
              SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ARRAYLENGTH);
              
              break;
            }
            case SPerl_OP_C_CODE_LEFT_SHIFT: {
              
              if (op_cur->uv.resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISHL);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSHL);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_RIGHT_SHIFT: {
              
              if (op_cur->uv.resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISHR);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSHR);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED: {
              
              if (op_cur->uv.resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IUSHR);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LUSHR);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_NEW_TYPE: {
              SPerl_RESOLVED_TYPE* resolved_type = op_cur->uv.resolved_type;
              
              if (SPerl_RESOLVED_TYPE_is_core_type_array(parser, resolved_type)) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEWARRAY);
              }
              else if (SPerl_RESOLVED_TYPE_is_multi_array(parser, resolved_type)) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_MULTIANEWARRAY);
              }
              else if (SPerl_RESOLVED_TYPE_is_array(parser, resolved_type)) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ANEWARRAY);
              }
              else {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEW);
              }
              
              break;
            }

            case SPerl_OP_C_CODE_UNDEF: {
              
              SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ACONST_NULL);
              
              break;
            }
            case SPerl_OP_C_CODE_PRE_INC: {
              SPerl_VAR* var = op_cur->first->uv.var;
              SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
              
              if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, 1);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
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
              
              if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, 1);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, 1);
              }

              break;
            }
            case SPerl_OP_C_CODE_PRE_DEC: {
              SPerl_VAR* var = op_cur->first->uv.var;
              SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
              
              if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, -1);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
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
              
              if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, -1);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LINC);
                SPerl_BYTECODES_push(bytecodes, my_var->pos);
                SPerl_BYTECODES_push(bytecodes, -1);
              }

              break;
            }
            case SPerl_OP_C_CODE_BIT_XOR: {
              
              if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_XOR_INT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_XOR_LONG);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_BIT_OR: {
              
              if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_OR_INT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_OR_LONG);
              }
                            
              break;
            }
            case SPerl_OP_C_CODE_BIT_AND: {
              
              if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_AND_INT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_BIT_AND_LONG);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_ARRAY_ELEM: {
              
              if (op_cur->lvalue) {
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = op_cur->uv.resolved_type;
              
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
              
              if (!op_cur->uv.resolved_type) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_RETURN);
              }
              else if (op_cur->uv.resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_RETURN_INT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_RETURN_LONG);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FETURN_FLOAT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_RETURN_DOUBLE);
              }
              else {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_RETURN_REF);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_NEGATE: {
              
              if (op_cur->uv.resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEGATE_INT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEGATE_LONG);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEGATE_FLOAT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEGATE_DOUBLE);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_CONVERT: {
              
              SPerl_OP* op_term = op_cur->first;
              SPerl_RESOLVED_TYPE* resolved_type_src = SPerl_OP_get_resolved_type(parser, op_term);
              if (op_cur->uv.resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
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
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
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
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
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
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
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
            case SPerl_OP_C_CODE_POP: {
              
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
              
              if (op_cur->uv.resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ADD_INT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ADD_LONG);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ADD_FLOAT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ADD_DOUBLE);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_SUBTRACT: {
              
              if (op_cur->uv.resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_SUBTRACT_INT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_SUBTRACT_LONG);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_SUBTRACT_FLOAT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_SUBTRACT_DOUBLE);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_MULTIPLY: {
              
              if (op_cur->uv.resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_MULTIPLY_INT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_MULTIPLY_LONG);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_MULTIPLY_FLOAT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_MULTIPLY_DOUBLE);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_DIVIDE: {
              
              if (op_cur->uv.resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DIVIDE_INT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DIVIDE_LONG);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DIVIDE_FLOAT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DIVIDE_DOUBLE);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_REMAINDER: {
              
              if (op_cur->uv.resolved_type->id <= SPerl_BODY_CORE_C_CODE_INT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_REMAINDER_INT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_LONG) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_REMAINDER_LONG);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_FLOAT) {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_REMAINDER_FLOAT);
              }
              else if (op_cur->uv.resolved_type->id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
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
