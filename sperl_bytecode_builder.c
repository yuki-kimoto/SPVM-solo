#include <string.h>

#include "sperl_bytecode_builder.h"
#include "sperl_bytecode.h"
#include "sperl_bytecodes.h"
#include "sperl_parser.h"
#include "sperl_constant.h"
#include "sperl_op.h"
#include "sperl_resolved_type.h"
#include "sperl_array.h"
#include "sperl_sub.h"
#include "sperl_var.h"
#include "sperl_my_var.h"
#include "sperl_allocator.h"
#include "sperl_package.h"
#include "sperl_name_info.h"
#include "sperl_hash.h"
#include "sperl_field.h"
#include "sperl_switch_info.h"

void SPerl_BYTECODE_BUILDER_build_bytecodes(SPerl_PARSER* parser) {
  for (int32_t i = 0; i < parser->op_packages->length; i++) {
    SPerl_OP* op_package = SPerl_ARRAY_fetch(parser->op_packages, i);
    SPerl_PACKAGE* package = op_package->uv.package;
    
    for (int32_t k = 0; k < package->op_subs->length; k++) {
      
      SPerl_OP* op_sub = SPerl_ARRAY_fetch(package->op_subs, k);
      SPerl_SUB* sub = op_sub->uv.sub;
      
      // VM codes
      SPerl_BYTECODES* bytecodes = sub->bytecodes;
      
      // Run OPs
      SPerl_OP* op_base = op_sub;
      SPerl_OP* op_cur = op_base;
      _Bool finish = 0;
      
      SPerl_ARRAY* if_condition_bytecode_pos_stack = SPerl_ALLOCATOR_new_array(parser, 0);
      SPerl_ARRAY* loop_condition_bytecode_pos_stack = SPerl_ALLOCATOR_new_array(parser, 0);
      SPerl_ARRAY* last_bytecode_pos_stack = SPerl_ALLOCATOR_new_array(parser, 0);
      
      // Current switch
      SPerl_OP* cur_op_switch_info = NULL;
      
      // Current case addresses
      int32_t cur_switch_address = -1;
      
      int32_t cur_default_address = -1;
      SPerl_ARRAY* cur_case_addresses;
      
      while (op_cur) {
        // [START]Preorder traversal position
        
        switch (op_cur->code) {
          case SPerl_OP_C_CODE_SWITCH: {
            cur_op_switch_info = op_cur;
            cur_case_addresses = SPerl_ALLOCATOR_new_array(parser, 0);
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
              case SPerl_OP_C_CODE_DEFAULT: {
                
                cur_default_address = bytecodes->length - 1;
                
                break;
              }
              case SPerl_OP_C_CODE_CASE: {
                
                int32_t* address_ptr = SPerl_ALLOCATOR_new_int(parser);
                *address_ptr = bytecodes->length - 1;
                
                SPerl_ARRAY_push(cur_case_addresses, address_ptr);
                
                break;
              }
              case SPerl_OP_C_CODE_SWITCH_CONDITION: {
                
                SPerl_SWITCH_INFO* switch_info = cur_op_switch_info->uv.switch_info;
                
                if (switch_info->code == SPerl_SWITCH_INFO_C_CODE_TABLESWITCH) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_TABLESWITCH);
                  
                  cur_switch_address = bytecodes->length - 1;
                  
                  int32_t length = switch_info->op_cases->length;
                  int32_t max = switch_info->max;
                  int32_t min = switch_info->min;
                  int32_t padding = 3 - ((bytecodes->length - 1) % 4);
                  
                  for (int32_t j = 0; j < padding; j++) {
                    SPerl_BYTECODES_push(bytecodes, 0);
                  }
                  
                  // Default
                  for (int32_t j = 0; j < 4; j++) {
                    SPerl_BYTECODES_push(bytecodes, 0);
                  }
                  
                  // Low
                  for (int32_t j = 0; j < 4; j++) {
                    SPerl_BYTECODES_push(bytecodes, (min >> (24 - (8 * j))) & 0xFF);
                  }
                  
                  // High
                  for (int32_t j = 0; j < 4; j++) {
                    SPerl_BYTECODES_push(bytecodes, (max >> (24 - (8 * j))) & 0xFF);
                  }
                  
                  // Addresses
                  for (int32_t j = 0; j < (max - min + 1) * 4; j++) {
                    SPerl_BYTECODES_push(bytecodes, 0);
                  }
                }
                else if (switch_info->code == SPerl_SWITCH_INFO_C_CODE_LOOKUPSWITCH) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOOKUPSWITCH);
                  
                  cur_switch_address = bytecodes->length - 1;
                  
                  int32_t length = switch_info->op_cases->length;
                  int32_t max = switch_info->max;
                  int32_t min = switch_info->min;
                  int32_t padding = (int)(length / 4);
                  
                  int32_t bytecode_length
                    = 1 + padding + 4 + 4 + (4 * length);
                  
                  for (int32_t i = 0; i < bytecode_length; i++) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NOP);
                  }
                }
                
                break;
              }
              case SPerl_OP_C_CODE_SWITCH: {
                
                SPerl_SWITCH_INFO* switch_info = cur_op_switch_info->uv.switch_info;
                
                if (switch_info->code == SPerl_SWITCH_INFO_C_CODE_TABLESWITCH) {
                  int32_t padding = 3 - (cur_switch_address % 4);

                  // Default offset
                  int32_t default_offset = cur_default_address - cur_switch_address;
                  bytecodes->values[cur_switch_address + padding + 1] = (default_offset >> 24) & 0xFF;
                  bytecodes->values[cur_switch_address + padding + 2] = (default_offset >> 16) & 0xFF;
                  bytecodes->values[cur_switch_address + padding + 3] = (default_offset >> 8) & 0xFF;
                  bytecodes->values[cur_switch_address + padding + 4] = default_offset & 0xFF;
                }
                else if (switch_info->code == SPerl_SWITCH_INFO_C_CODE_LOOKUPSWITCH) {
                  
                }
                
                cur_op_switch_info = NULL;
                cur_default_address = -1;
                cur_case_addresses = NULL;
                
                break;
              }
              case SPerl_OP_C_CODE_FIELD: {
                
                if (!op_cur->lvalue) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_GETFIELD);

                  SPerl_NAME_INFO* name_info = op_cur->uv.name_info;
                  uint8_t* field_abs_name = name_info->abs_name;
                  SPerl_FIELD* field = SPerl_HASH_search(parser->field_abs_name_symtable, field_abs_name, strlen(field_abs_name));
                  int32_t id = field->id;
                  
                  SPerl_BYTECODES_push(bytecodes, (id >> 8) & 0xFF);
                  SPerl_BYTECODES_push(bytecodes, id & 0xFF);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_CALL_SUB: {
                
                // Call subroutine
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_INVOKESTATIC);
                SPerl_NAME_INFO* name_info = op_cur->uv.name_info;
                uint8_t* sub_abs_name = name_info->abs_name;
                SPerl_SUB* sub = SPerl_HASH_search(parser->sub_abs_name_symtable, sub_abs_name, strlen(sub_abs_name));
                int32_t id = sub->id;
                
                SPerl_BYTECODES_push(bytecodes, (id >> 8) & 0xFF);
                SPerl_BYTECODES_push(bytecodes, id & 0xFF);
                
                break;
              }
              case SPerl_OP_C_CODE_DIE: {
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ATHROW);
                break;
              }
              case SPerl_OP_C_CODE_LAST: {
                // Add goto
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_GOTO);
                
                int32_t* pos_ptr = SPerl_ALLOCATOR_new_int(parser);
                *pos_ptr = bytecodes->length - 1;
                SPerl_ARRAY_push(last_bytecode_pos_stack, pos_ptr);
                
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NOP);
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NOP);
                break;
              }
              case SPerl_OP_C_CODE_NEXT: {
                int32_t* pos_ptr = SPerl_ARRAY_fetch(loop_condition_bytecode_pos_stack, loop_condition_bytecode_pos_stack->length - 1);
                
                // Add "goto"
                SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_GOTO);

                // Jump offset
                int32_t jump_offset = *pos_ptr - (bytecodes->length - 1);

                SPerl_BYTECODES_push(bytecodes, (jump_offset >> 8) & 0xFF);
                SPerl_BYTECODES_push(bytecodes, jump_offset & 0xFF);
                
                break;
              }
              case SPerl_OP_C_CODE_BLOCK: {
                if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_IF) {
                  int32_t* pos_ptr = SPerl_ARRAY_pop(if_condition_bytecode_pos_stack);
                  
                  // Jump offset
                  int32_t jump_offset = bytecodes->length - *pos_ptr;
                  
                  // Set jump offset
                  bytecodes->values[*pos_ptr + 1] = (jump_offset >> 8) & 0xFF;
                  bytecodes->values[*pos_ptr + 2] = jump_offset & 0xFF;
                }
                else if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_LOOP) {
                  int32_t* pos_ptr = SPerl_ARRAY_pop(loop_condition_bytecode_pos_stack);
                  
                  // Add "goto" to end of block
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_GOTO);
                  
                  // Goto offset
                  int32_t block_end_goto_offset = *pos_ptr - (bytecodes->length - 1);
                  SPerl_BYTECODES_push(bytecodes, (block_end_goto_offset >> 8) & 0xFF);
                  SPerl_BYTECODES_push(bytecodes, block_end_goto_offset & 0xFF);
                  
                  // If offset
                  int32_t if_offset = bytecodes->length - *pos_ptr;
                  
                  // Set condition jump position
                  bytecodes->values[*pos_ptr + 1] = (if_offset >> 8) & 0xFF;
                  bytecodes->values[*pos_ptr + 2] = if_offset & 0xFF;
                  
                  // Set last position
                  int32_t* last_pos_ptr;
                  while (last_pos_ptr = SPerl_ARRAY_pop(last_bytecode_pos_stack)) {
                    // Last offset
                    int32_t last_offset = bytecodes->length - *last_pos_ptr;
                    
                    bytecodes->values[*last_pos_ptr + 1] = (last_offset >> 8) & 0xFF;
                    bytecodes->values[*last_pos_ptr + 2] = last_offset & 0xFF;
                  }
                }
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
                    if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPEQ);
                    }
                    else if (resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                      
                      if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                        SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                      }
                      else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                        SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                      }
                      else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
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
                    if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPNE);
                    }
                    else if (resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                      
                      if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                        SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                      }
                      else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                        SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                      }
                      else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
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
                  if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPGT);
                  }
                  else if (resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                    
                    if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCMPG);
                    }
                    
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFGT);
                  }
                }
                else if (op_condition_target->code == SPerl_OP_C_CODE_GE) {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target->first);
                  if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPGE);
                  }
                  else if (resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                    
                    if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCMPG);
                    }
                    
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFGE);
                  }
                }
                else if (op_condition_target->code == SPerl_OP_C_CODE_LT) {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target->first);
                  if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPLT);
                  }
                  else if (resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                    
                    if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCMPG);
                    }
                    
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFLT);
                  }
                }
                else if (op_condition_target->code == SPerl_OP_C_CODE_LE) {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target->first);
                  if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IF_ICMPLE);
                  }
                  else if (resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                    
                    if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCMP);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCMPG);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCMPG);
                    }
                    
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFLE);
                  }
                }
                else if (op_condition_target) {
                  
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_condition_target);
                  
                  if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFNE);
                  }
                  else if (resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                    
                    if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_L2I);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_F2I);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_D2I);
                    }
                    
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFNE);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IFNONNULL);
                  }
                }
                
                int32_t* pos_ptr = SPerl_ALLOCATOR_new_int(parser);
                *pos_ptr = bytecodes->length - 1;
                
                if (op_cur->flag & SPerl_OP_C_FLAG_CONDITION_IF) {
                  SPerl_ARRAY_push(if_condition_bytecode_pos_stack, pos_ptr);
                }
                else if (op_cur->flag & SPerl_OP_C_FLAG_CONDITION_LOOP) {
                  SPerl_ARRAY_push(loop_condition_bytecode_pos_stack, pos_ptr);
                }
                
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
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                
                if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISHL);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSHL);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_RIGHT_SHIFT: {
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                
                if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISHR);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSHR);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED: {
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                
                if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IUSHR);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LUSHR);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_NEW_TYPE: {
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                
                if (SPerl_RESOLVED_TYPE_is_core_type_array(parser, op_cur_resolved_type)) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_NEWARRAY);
                }
                else if (SPerl_RESOLVED_TYPE_is_multi_array(parser, op_cur_resolved_type)) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_MULTIANEWARRAY);
                }
                else if (SPerl_RESOLVED_TYPE_is_array(parser, op_cur_resolved_type)) {
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
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                  SPerl_BYTECODES_push(bytecodes, my_var->pos);
                  SPerl_BYTECODES_push(bytecodes, 1);
                }
                
                // TODO
                
                break;
              }
              case SPerl_OP_C_CODE_POST_INC: {
                SPerl_VAR* var = op_cur->first->uv.var;
                SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                  SPerl_BYTECODES_push(bytecodes, my_var->pos);
                  SPerl_BYTECODES_push(bytecodes, 1);
                }

                break;
              }
              case SPerl_OP_C_CODE_PRE_DEC: {
                SPerl_VAR* var = op_cur->first->uv.var;
                SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                  SPerl_BYTECODES_push(bytecodes, my_var->pos);
                  SPerl_BYTECODES_push(bytecodes, -1);
                }
                
                // TODO
                
                break;
              }
              case SPerl_OP_C_CODE_POST_DEC: {
                SPerl_VAR* var = op_cur->first->uv.var;
                SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;

                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IINC);
                  SPerl_BYTECODES_push(bytecodes, my_var->pos);
                  SPerl_BYTECODES_push(bytecodes, -1);
                }

                break;
              }
              case SPerl_OP_C_CODE_BIT_XOR: {
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IXOR);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LXOR);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_BIT_OR: {
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IOR);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LOR);
                }
                              
                break;
              }
              case SPerl_OP_C_CODE_BIT_AND: {
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                
                if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IAND);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LAND);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_ARRAY_ELEM: {
                
                if (op_cur->lvalue) {
                  break;
                }
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                
                if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BOOLEAN
                  || op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE)
                {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_CALOAD);
                }
                else if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_SALOAD);
                }
                else if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IALOAD);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LALOAD);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FALOAD);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DALOAD);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_AALOAD);
                }              
                break;
              }
              case SPerl_OP_C_CODE_ASSIGN: {
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                
                if (op_cur->first->code == SPerl_OP_C_CODE_VAR) {
                  SPerl_OP* op_var = op_cur->first;
                  int32_t my_var_pos = op_var->uv.var->op_my_var->uv.my_var->pos;
                  
                  if (my_var_pos > 0xFF) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_WIDE);
                  }
                  
                  _Bool has_operand = 0;
                  if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                    if (my_var_pos == 0) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISTORE_0);
                    }
                    else if (my_var_pos == 1) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISTORE_1);
                    }
                    else if (my_var_pos == 2) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISTORE_2);
                    }
                    else if (my_var_pos == 3) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISTORE_3);
                    }
                    else {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISTORE);
                      has_operand = 1;
                    }
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    if (my_var_pos == 0) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSTORE_0);
                    }
                    else if (my_var_pos == 1) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSTORE_1);
                    }
                    else if (my_var_pos == 2) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSTORE_2);
                    }
                    else if (my_var_pos == 3) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSTORE_3);
                    }
                    else {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSTORE);
                      has_operand = 1;
                    }
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    if (my_var_pos == 0) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FSTORE_0);
                    }
                    else if (my_var_pos == 1) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FSTORE_1);
                    }
                    else if (my_var_pos == 2) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FSTORE_2);
                    }
                    else if (my_var_pos == 3) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FSTORE_3);
                    }
                    else {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FSTORE);
                      has_operand = 1;
                    }
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    if (my_var_pos == 0) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DSTORE_0);
                    }
                    else if (my_var_pos == 1) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DSTORE_1);
                    }
                    else if (my_var_pos == 2) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DSTORE_2);
                    }
                    else if (my_var_pos == 3) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DSTORE_3);
                    }
                    else {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DSTORE);
                      has_operand = 1;
                    }
                  }
                  else {
                    if (my_var_pos == 0) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ASTORE_0);
                    }
                    else if (my_var_pos == 1) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ASTORE_1);
                    }
                    else if (my_var_pos == 2) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ASTORE_2);
                    }
                    else if (my_var_pos == 3) {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ASTORE_3);
                    }
                    else {
                      SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ASTORE);
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
                  
                  if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IASTORE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LASTORE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FASTORE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DASTORE);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_AASTORE);
                  }
                }
                else if (op_cur->first->code == SPerl_OP_C_CODE_FIELD) {
                  SPerl_OP* op_field = op_cur->first;
                  
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_PUTFIELD);
                  
                  // Call subroutine
                  SPerl_NAME_INFO* name_info = op_cur->first->uv.name_info;
                  uint8_t* field_abs_name = name_info->abs_name;
                  SPerl_FIELD* field = SPerl_HASH_search(parser->field_abs_name_symtable, field_abs_name, strlen(field_abs_name));
                  int32_t id = field->id;
                  
                  SPerl_BYTECODES_push(bytecodes, (id >> 8) & 0xFF);
                  SPerl_BYTECODES_push(bytecodes, id & 0xFF);
                }
                
                break;
              }
              
              case SPerl_OP_C_CODE_RETURN: {
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (!op_cur_resolved_type) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_RETURN);
                }
                else if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IRETURN);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LRETURN);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FRETURN);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DRETURN);
                }
                else {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ARETURN);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_NEGATE: {
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_INEG);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LNEG);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FNEG);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DNEG);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_CONVERT: {
                
                SPerl_OP* op_term = op_cur->first;
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                SPerl_RESOLVED_TYPE* resolved_type_src = SPerl_OP_get_resolved_type(parser, op_term);
                if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  if (resolved_type_src->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_L2I);
                  }
                  else if (resolved_type_src->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_F2I);
                  }
                  else if (resolved_type_src->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_D2I);
                  }
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  if (resolved_type_src->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_I2L);
                  }
                  else if (resolved_type_src->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_F2L);
                  }
                  else if (resolved_type_src->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_D2L);
                  }
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  if (resolved_type_src->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_I2F);
                  }
                  else if (resolved_type_src->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_L2F);
                  }
                  else if (resolved_type_src->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_D2F);
                  }
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  if (resolved_type_src->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_I2D);
                  }
                  else if (resolved_type_src->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_L2D);
                  }
                  else if (resolved_type_src->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_F2D);
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
                
                if (op_first->code != SPerl_OP_C_CODE_ASSIGN && op_first->code != SPerl_OP_C_CODE_RETURN && !op_first->lvalue) {
                  if (first_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG || first_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_POP2);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_POP);
                  }
                }
                
                break;
              }
              case SPerl_OP_C_CODE_ADD: {
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IADD);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LADD);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FADD);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DADD);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_SUBTRACT: {
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ISUB);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LSUB);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FSUB);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DSUB);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_MULTIPLY: {
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IMUL);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LMUL);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FMUL);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DMUL);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_DIVIDE: {
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IDIV);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LDIV);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FDIV);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DDIV);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_REMAINDER: {
                
                SPerl_RESOLVED_TYPE* op_cur_resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                if (op_cur_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_IREM);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LREM);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FREM);
                }
                else if (op_cur_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DREM);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_VAR: {
                SPerl_VAR* var = op_cur->uv.var;
                
                if (op_cur->lvalue) {
                  break;
                }
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(parser, op_cur);
                
                int32_t my_var_pos = var->op_my_var->uv.my_var->pos;
                
                if (my_var_pos > 0xFF) {
                  SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_WIDE);
                }
                
                _Bool has_operand = 0;
                if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ILOAD_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ILOAD_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ILOAD_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ILOAD_3);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ILOAD);
                    has_operand = 1;
                  }
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LLOAD_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LLOAD_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LLOAD_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LLOAD_3);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LLOAD);
                    has_operand = 1;
                  }
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FLOAD_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FLOAD_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FLOAD_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FLOAD_3);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FLOAD);
                    has_operand = 1;
                  }
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DLOAD_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DLOAD_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DLOAD_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DLOAD_3);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DLOAD);
                    has_operand = 1;
                  }
                }
                else {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ALOAD_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ALOAD_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ALOAD_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ALOAD_3);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ALOAD);
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
                
                if (op_cur->flag == SPerl_OP_C_FLAG_CONSTANT_CASE) {
                  break;
                }
                
                _Bool bytecode_set = 0;
                if (constant->code == SPerl_CONSTANT_C_CODE_BOOLEAN) {
                  if (constant->uv.int_value == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ICONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ICONST_1);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPerl_CONSTANT_C_CODE_INT) {
                  if (constant->uv.int_value == -1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ICONST_M1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ICONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ICONST_1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ICONST_2);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 3) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ICONST_3);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 4) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ICONST_4);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 5) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_ICONST_5);
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
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LCONST_1);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPerl_CONSTANT_C_CODE_FLOAT) {
                  if (constant->uv.float_value == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.float_value == 1) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCONST_1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.float_value == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_FCONST_1);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPerl_CONSTANT_C_CODE_DOUBLE) {
                  if (constant->uv.double_value == 0) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.double_value == 2) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_DCONST_1);
                    bytecode_set = 1;
                  }
                }
                
                if (!bytecode_set) {
                  if (constant->code == SPerl_CONSTANT_C_CODE_LONG || constant->code == SPerl_CONSTANT_C_CODE_DOUBLE) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LDC2_W);
                    SPerl_BYTECODES_push(bytecodes, (constant->pool_pos >> 8) & 0xFF);
                    SPerl_BYTECODES_push(bytecodes, constant->pool_pos & 0xFF);
                  }
                  else if (constant->pool_pos > 0xFF) {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LDC_W);
                    SPerl_BYTECODES_push(bytecodes, (constant->pool_pos >> 8) & 0xFF);
                    SPerl_BYTECODES_push(bytecodes, constant->pool_pos & 0xFF);
                  }
                  else {
                    SPerl_BYTECODES_push(bytecodes, SPerl_BYTECODE_C_CODE_LDC);
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
}
