#include <string.h>
#include <stdint.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_bytecode_builder.h"
#include "sperl_bytecode.h"
#include "sperl_bytecode_array.h"
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

void SPerl_BYTECODE_BUILDER_build_bytecode_array(SPerl* sperl) {
  
  // Parser
  SPerl_PARSER* parser = sperl->parser;
  
  // Bytecode
  SPerl_BYTECODE_ARRAY* bytecode_array = sperl->bytecode_array;
  
  for (int32_t sub_pos = 0; sub_pos < parser->op_subs->length; sub_pos++) {
    SPerl_OP* op_sub = SPerl_ARRAY_fetch(parser->op_subs, sub_pos);
    SPerl_SUB* sub = op_sub->uv.sub;
    
    sub->bytecode_base = bytecode_array->length;
    
    // Run OPs
    SPerl_OP* op_base = op_sub;
    SPerl_OP* op_cur = op_base;
    _Bool finish = 0;
    
    // IFXXX Bytecode address(except loop)
    SPerl_ARRAY* if_address_stack = SPerl_ALLOCATOR_new_array(sperl, 0);
    
    // IFXXX Bytecode address for loop
    SPerl_ARRAY* if_loop_address_stack = SPerl_ALLOCATOR_new_array(sperl, 0);
    
    // GOTO Bytecode address for last
    SPerl_ARRAY* goto_last_address_stack = SPerl_ALLOCATOR_new_array(sperl, 0);

    // GOTO Bytecode address for end of if block
    SPerl_ARRAY* goto_if_block_end_address_stack = SPerl_ALLOCATOR_new_array(sperl, 0);
    
    // Current switch
    SPerl_OP* cur_op_switch_info = NULL;
    
    // Current case addresses
    int32_t cur_switch_address = -1;
    
    int32_t cur_default_address = -1;
    SPerl_ARRAY* cur_case_addresses = NULL;
    SPerl_ARRAY* cur_op_cases = NULL;
    
    while (op_cur) {
      // [START]Preorder traversal position
      
      switch (op_cur->code) {
        case SPerl_OP_C_CODE_SWITCH: {
          cur_op_switch_info = op_cur;
          cur_case_addresses = SPerl_ALLOCATOR_new_array(sperl, 0);
          cur_op_cases = SPerl_ALLOCATOR_new_array(sperl, 0);
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
            case SPerl_OP_C_CODE_SWITCH_CONDITION: {
              
              SPerl_SWITCH_INFO* switch_info = cur_op_switch_info->uv.switch_info;
              
              // tableswitch
              if (switch_info->code == SPerl_SWITCH_INFO_C_CODE_TABLESWITCH) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_TABLESWITCH);
                
                cur_switch_address = bytecode_array->length - 1;
                
                int32_t max = switch_info->max;
                int32_t min = switch_info->min;
                
                // Machine address to culculate padding
                cur_switch_address = bytecode_array->length - 1;
                
                // Padding
                int32_t padding = 3 - (cur_switch_address % 4);
                
                for (int32_t i = 0; i < padding; i++) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
                }
                
                // Default
                for (int32_t i = 0; i < 4; i++) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
                }
                
                // Low
                for (int32_t i = 0; i < 4; i++) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, (min >> (24 - (8 * i))) & 0xFF);
                }
                
                // High
                for (int32_t i = 0; i < 4; i++) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, (max >> (24 - (8 * i))) & 0xFF);
                }
                
                // Offsets
                for (int32_t i = 0; i < (max - min + 1) * 4; i++) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
                }
              }
              // lookupswitch
              else if (switch_info->code == SPerl_SWITCH_INFO_C_CODE_LOOKUPSWITCH) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LOOKUPSWITCH);
                
                int32_t length = switch_info->op_cases->length;
                
                // Machine address to culculate padding
                cur_switch_address = bytecode_array->length - 1;
                
                // Padding
                int32_t padding = 3 - (cur_switch_address % 4);
                
                for (int32_t i = 0; i < padding; i++) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
                }
                
                // Default
                for (int32_t i = 0; i < 4; i++) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
                }
                
                // Case count
                for (int32_t i = 0; i < 4; i++) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, (length >> (24 - (8 * i))) & 0xFF);
                }
                
                // Match-Offset pairs
                for (int32_t i = 0; i < length * 8; i++) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
                }
              }
              
              break;
            }
            case SPerl_OP_C_CODE_CASE: {
              
              int32_t* address_ptr = SPerl_ALLOCATOR_new_int(sperl);
              *address_ptr = bytecode_array->length;
              
              SPerl_ARRAY_push(cur_case_addresses, address_ptr);
              SPerl_ARRAY_push(cur_op_cases, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_DEFAULT: {
              
              cur_default_address = bytecode_array->length;
              
              break;
            }
            case SPerl_OP_C_CODE_SWITCH: {
              
              SPerl_SWITCH_INFO* switch_info = cur_op_switch_info->uv.switch_info;
              
              // tableswitch
              if (switch_info->code == SPerl_SWITCH_INFO_C_CODE_TABLESWITCH) {
                int32_t padding = 3 - (cur_switch_address % 4);

                // Default offset
                int32_t default_offset;
                if (cur_default_address == -1) {
                  default_offset = bytecode_array->length - cur_switch_address;
                }
                else {
                  default_offset = cur_default_address - cur_switch_address;
                }
                bytecode_array->values[cur_switch_address + padding + 1] = (default_offset >> 24) & 0xFF;
                bytecode_array->values[cur_switch_address + padding + 2] = (default_offset >> 16) & 0xFF;
                bytecode_array->values[cur_switch_address + padding + 3] = (default_offset >> 8) & 0xFF;
                bytecode_array->values[cur_switch_address + padding + 4] = default_offset & 0xFF;
                
                // min
                int32_t min
                  = (bytecode_array->values[cur_switch_address + padding + 5] << 24)
                  + (bytecode_array->values[cur_switch_address + padding + 6] << 16)
                  + (bytecode_array->values[cur_switch_address + padding + 7] << 8)
                  + bytecode_array->values[cur_switch_address + padding + 8];
                
                // max
                int32_t max
                  = (bytecode_array->values[cur_switch_address + padding + 9] << 24)
                  + (bytecode_array->values[cur_switch_address + padding + 10] << 16)
                  + (bytecode_array->values[cur_switch_address + padding + 11] << 8)
                  + bytecode_array->values[cur_switch_address + padding + 12];
                
                int32_t length = max - min + 1;
                
                int32_t case_pos = 0;
                for (int32_t i = 0; i < length; i++) {
                  SPerl_OP* op_case = SPerl_ARRAY_fetch(cur_op_cases, case_pos);
                  SPerl_OP* op_constant = op_case->first;
                  if (op_constant->uv.constant->uv.int_value - min == i) {
                    // Case
                    int32_t* case_address_ptr = SPerl_ARRAY_fetch(cur_case_addresses, case_pos);
                    int32_t case_offset = *case_address_ptr - cur_switch_address;
                    
                    bytecode_array->values[cur_switch_address + padding + 13 + (4 * i)] = (case_offset >> 24) & 0xFF;
                    bytecode_array->values[cur_switch_address + padding + 14 + (4 * i)] = (case_offset >> 16) & 0xFF;
                    bytecode_array->values[cur_switch_address + padding + 15 + (4 * i)] = (case_offset >> 8) & 0xFF;
                    bytecode_array->values[cur_switch_address + padding + 16 + (4 * i)] = case_offset & 0xFF;
                    
                    case_pos++;
                  }
                  else {
                    // Default
                    bytecode_array->values[cur_switch_address + padding + 13 + (4 * i)] = (default_offset >> 24) & 0xFF;
                    bytecode_array->values[cur_switch_address + padding + 14 + (4 * i)] = (default_offset >> 16) & 0xFF;
                    bytecode_array->values[cur_switch_address + padding + 15 + (4 * i)] = (default_offset >> 8) & 0xFF;
                    bytecode_array->values[cur_switch_address + padding + 16 + (4 * i)] = default_offset & 0xFF;
                  }
                }
              }
              // looupswitch
              else if (switch_info->code == SPerl_SWITCH_INFO_C_CODE_LOOKUPSWITCH) {
                int32_t padding = 3 - (cur_switch_address % 4);

                // Default offset
                int32_t default_offset;
                if (cur_default_address == -1) {
                  default_offset = bytecode_array->length - cur_switch_address;
                }
                else {
                  default_offset = cur_default_address - cur_switch_address;
                }
                bytecode_array->values[cur_switch_address + padding + 1] = (default_offset >> 24) & 0xFF;
                bytecode_array->values[cur_switch_address + padding + 2] = (default_offset >> 16) & 0xFF;
                bytecode_array->values[cur_switch_address + padding + 3] = (default_offset >> 8) & 0xFF;
                bytecode_array->values[cur_switch_address + padding + 4] = default_offset & 0xFF;
                
                int32_t length = switch_info->op_cases->length;
                
                SPerl_ARRAY* ordered_op_cases = SPerl_ALLOCATOR_new_array(sperl, 0);
                for (int32_t i = 0; i < length; i++) {
                  SPerl_OP* op_case = SPerl_ARRAY_fetch(cur_op_cases, i);
                  SPerl_ARRAY_push(ordered_op_cases, op_case);
                }
                SPerl_ARRAY* ordered_case_addresses = SPerl_ALLOCATOR_new_array(sperl, 0);
                for (int32_t i = 0; i < length; i++) {
                  SPerl_OP* case_address = SPerl_ARRAY_fetch(cur_case_addresses, i);
                  SPerl_ARRAY_push(ordered_case_addresses, case_address);
                }
                
                // sort by asc order
                for (int32_t i = 0; i < length; i++) {
                  for (int32_t j = i + 1; j < length; j++) {
                    SPerl_OP* op_case_i = SPerl_ARRAY_fetch(ordered_op_cases, i);
                    SPerl_OP* op_case_j = SPerl_ARRAY_fetch(ordered_op_cases, j);
                    int32_t match_i = op_case_i->first->uv.constant->uv.int_value;
                    int32_t match_j = op_case_j->first->uv.constant->uv.int_value;
                    
                    SPerl_OP* case_address_i = SPerl_ARRAY_fetch(ordered_case_addresses, i);
                    SPerl_OP* case_address_j = SPerl_ARRAY_fetch(ordered_case_addresses, j);

                    
                    if (match_i > match_j) {
                      ordered_op_cases->values[i] = op_case_j;
                      ordered_op_cases->values[j] = op_case_i;
                      
                      ordered_case_addresses->values[i] = case_address_j;
                      ordered_case_addresses->values[j] = case_address_i;
                    }
                  }
                }
                
                for (int32_t i = 0; i < length; i++) {
                  SPerl_OP* op_case = SPerl_ARRAY_fetch(ordered_op_cases, i);
                  SPerl_OP* op_constant = op_case->first;
                  int32_t match = op_constant->uv.constant->uv.int_value;

                  int32_t* case_address_ptr = SPerl_ARRAY_fetch(ordered_case_addresses, i);
                  int32_t case_offset = *case_address_ptr - cur_switch_address;
                  
                  // Match
                  bytecode_array->values[cur_switch_address + padding + 9 + (8 * i)] = (match >> 24) & 0xFF;
                  bytecode_array->values[cur_switch_address + padding + 10 + (8 * i)] = (match >> 16) & 0xFF;
                  bytecode_array->values[cur_switch_address + padding + 11 + (8 * i)] = (match >> 8) & 0xFF;
                  bytecode_array->values[cur_switch_address + padding + 12 + (8 * i)] = match & 0xFF;

                  // Offset
                  bytecode_array->values[cur_switch_address + padding + 13 + (8 * i)] = (case_offset >> 24) & 0xFF;
                  bytecode_array->values[cur_switch_address + padding + 14 + (8 * i)] = (case_offset >> 16) & 0xFF;
                  bytecode_array->values[cur_switch_address + padding + 15 + (8 * i)] = (case_offset >> 8) & 0xFF;
                  bytecode_array->values[cur_switch_address + padding + 16 + (8 * i)] = case_offset & 0xFF;
                }
              }
              
              cur_op_switch_info = NULL;
              cur_default_address = -1;
              cur_case_addresses = NULL;
              
              break;
            }
            case SPerl_OP_C_CODE_FIELD: {
              
              if (!op_cur->lvalue) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_GETFIELD);
                
                SPerl_NAME_INFO* name_info = op_cur->uv.name_info;
                const char* field_name = name_info->resolved_name;
                SPerl_FIELD* field = SPerl_HASH_search(parser->field_name_symtable, field_name, strlen(field_name));
                int32_t id = field->id;
                
                SPerl_BYTECODE_ARRAY_push(bytecode_array, (id >> 8) & 0xFF);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, id & 0xFF);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_CALL_SUB: {
              
              // Call subroutine
              SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_CALLSUB);
              SPerl_NAME_INFO* name_info = op_cur->uv.name_info;
              const char* sub_name = name_info->resolved_name;
              
              SPerl_SUB* sub = SPerl_HASH_search(parser->sub_name_symtable, sub_name, strlen(sub_name));
              
              int32_t id = sub->id;
              
              SPerl_BYTECODE_ARRAY_push(bytecode_array, (id >> 24) & 0xFF);
              SPerl_BYTECODE_ARRAY_push(bytecode_array, (id >> 16) & 0xFF);
              SPerl_BYTECODE_ARRAY_push(bytecode_array, (id >> 8) & 0xFF);
              SPerl_BYTECODE_ARRAY_push(bytecode_array, id & 0xFF);
              
              break;
            }
            case SPerl_OP_C_CODE_DIE: {
              SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ATHROW);
              break;
            }
            case SPerl_OP_C_CODE_LAST: {
              // Add goto
              SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_GOTO);
              
              int32_t* pos_ptr = SPerl_ALLOCATOR_new_int(sperl);
              *pos_ptr = bytecode_array->length - 1;
              SPerl_ARRAY_push(goto_last_address_stack, pos_ptr);
              
              SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
              SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
              break;
            }
            case SPerl_OP_C_CODE_NEXT: {
              int32_t* pos_ptr = SPerl_ARRAY_fetch(if_loop_address_stack, if_loop_address_stack->length - 1);
              
              // Add "goto"
              SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_GOTO);

              // Jump offset
              int32_t jump_offset = *pos_ptr - (bytecode_array->length - 1);

              SPerl_BYTECODE_ARRAY_push(bytecode_array, (jump_offset >> 8) & 0xFF);
              SPerl_BYTECODE_ARRAY_push(bytecode_array, jump_offset & 0xFF);
              
              break;
            }
            case SPerl_OP_C_CODE_BLOCK: {
              if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_IF) {
                
                // Has else block
                if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_HAS_ELSE) {
                  // Prepare to jump to end of else block
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_GOTO);
                  int32_t* pos_ptr = SPerl_ALLOCATOR_new_int(sperl);
                  *pos_ptr = bytecode_array->length - 1;
                  SPerl_ARRAY_push(goto_if_block_end_address_stack, pos_ptr);
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
                }
                else {
                  // Set if jump address
                  int32_t* pos_ptr = SPerl_ARRAY_pop(if_address_stack);
                  
                  // Jump offset
                  int32_t jump_offset = bytecode_array->length - *pos_ptr;
                  
                  // Set jump offset
                  bytecode_array->values[*pos_ptr + 1] = (jump_offset >> 8) & 0xFF;
                  bytecode_array->values[*pos_ptr + 2] = jump_offset & 0xFF;
                }
              }
              else if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_ELSE) {
                
                // Set if jump address
                {
                  int32_t* pos_ptr = SPerl_ARRAY_pop(if_address_stack);
                  
                  // Jump offset
                  int32_t jump_offset = bytecode_array->length - *pos_ptr;
                  
                  // Set jump offset
                  bytecode_array->values[*pos_ptr + 1] = (jump_offset >> 8) & 0xFF;
                  bytecode_array->values[*pos_ptr + 2] = jump_offset & 0xFF;
                }
                
                {
                  int32_t* pos_ptr = SPerl_ARRAY_pop(goto_if_block_end_address_stack);
                  
                  // Jump offset
                  int32_t jump_offset = bytecode_array->length - *pos_ptr;
                  
                  // Set jump offset
                  bytecode_array->values[*pos_ptr + 1] = (jump_offset >> 8) & 0xFF;
                  bytecode_array->values[*pos_ptr + 2] = jump_offset & 0xFF;
                }
              }
              else if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_LOOP) {
                int32_t* pos_ptr = SPerl_ARRAY_pop(if_loop_address_stack);
                
                // Add "goto" to end of block
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_GOTO);
                
                // Goto offset
                int32_t block_end_goto_offset = *pos_ptr - (bytecode_array->length - 1);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, (block_end_goto_offset >> 8) & 0xFF);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, block_end_goto_offset & 0xFF);
                
                // If offset
                int32_t if_offset = bytecode_array->length - *pos_ptr;
                
                // Set condition jump position
                bytecode_array->values[*pos_ptr + 1] = (if_offset >> 8) & 0xFF;
                bytecode_array->values[*pos_ptr + 2] = if_offset & 0xFF;
                
                // Set last position
                int32_t* last_pos_ptr;
                while ((last_pos_ptr = SPerl_ARRAY_pop(goto_last_address_stack))) {
                  // Last offset
                  int32_t last_offset = bytecode_array->length - *last_pos_ptr;
                  
                  bytecode_array->values[*last_pos_ptr + 1] = (last_offset >> 8) & 0xFF;
                  bytecode_array->values[*last_pos_ptr + 2] = last_offset & 0xFF;
                }
              }
              break;
            }
            case SPerl_OP_C_CODE_CONDITION: {
              SPerl_OP* op_condition_target = op_cur->first;
              
              if (op_condition_target->code == SPerl_OP_C_CODE_EQ) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                
                if (!resolved_type) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFNULL);
                }
                else {
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IF_ICMPEQ);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                    
                    if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                      SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LCMP);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                      SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FCMPG);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                      SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DCMPG);
                    }
                    
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFEQ);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IF_ACMPEQ);
                  }
                }
              }
              else if (op_condition_target->code == SPerl_OP_C_CODE_NE) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                
                if (!resolved_type) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFNONNULL);
                }
                else {
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IF_ICMPNE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                    
                    if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                      SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LCMP);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                      SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FCMPG);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                      SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DCMPG);
                    }
                    
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IF_ACMPNE);
                  }
                }
              }
              else if (op_condition_target->code == SPerl_OP_C_CODE_GT) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IF_ICMPGT);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                  
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LCMP);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FCMPL);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DCMPL);
                  }
                  
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFGT);
                }
              }
              else if (op_condition_target->code == SPerl_OP_C_CODE_GE) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IF_ICMPGE);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                  
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LCMP);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FCMPL);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DCMPL);
                  }
                  
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFGE);
                }
              }
              else if (op_condition_target->code == SPerl_OP_C_CODE_LT) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IF_ICMPLT);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                  
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LCMP);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FCMPG);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DCMPG);
                  }
                  
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFLT);
                }
              }
              else if (op_condition_target->code == SPerl_OP_C_CODE_LE) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IF_ICMPLE);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                  
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LCMP);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FCMPG);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DCMPG);
                  }
                  
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFLE);
                }
              }
              else if (op_condition_target) {
                
                if (op_condition_target->code == SPerl_OP_C_CODE_IF) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFEQ);
                }
                else {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target);
                
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFEQ);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT && resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE)  {
                    
                    if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                      SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_L2I);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                      SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_F2I);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                      SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_D2I);
                    }
                    
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFEQ);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IFNONNULL);
                  }
                }
              }
              
              int32_t* pos_ptr = SPerl_ALLOCATOR_new_int(sperl);
              *pos_ptr = bytecode_array->length - 1;
              
              if (op_cur->flag & SPerl_OP_C_FLAG_CONDITION_IF) {
                SPerl_ARRAY_push(if_address_stack, pos_ptr);
              }
              else if (op_cur->flag & SPerl_OP_C_FLAG_CONDITION_LOOP) {
                SPerl_ARRAY_push(if_loop_address_stack, pos_ptr);
              }
              
              // Prepare for bytecode position of branch
              SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
              SPerl_BYTECODE_ARRAY_push(bytecode_array, 0);
              
              break;
            }
            case SPerl_OP_C_CODE_ARRAY_LENGTH : {
              SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ARRAYLENGTH);
              
              break;
            }
            case SPerl_OP_C_CODE_LEFT_SHIFT: {
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ISHL);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LSHL);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_RIGHT_SHIFT: {
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ISHR);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LSHR);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED: {
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IUSHR);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LUSHR);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_NEW_TYPE: {
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              
              if (SPerl_RESOLVED_TYPE_is_core_type_array(sperl, resolved_type)) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_NEWARRAY);
              }
              else if (SPerl_RESOLVED_TYPE_is_multi_array(sperl, resolved_type)) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_MULTIANEWARRAY);
              }
              else if (SPerl_RESOLVED_TYPE_is_array(sperl, resolved_type)) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ANEWARRAY);
              }
              else {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_NEW);
              }
              
              break;
            }

            case SPerl_OP_C_CODE_UNDEF: {
              
              SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ACONST_NULL);
              
              break;
            }
            case SPerl_OP_C_CODE_PRE_INC: {
              SPerl_VAR* var = op_cur->first->uv.var;
              SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, my_var->address);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, 1);
              }
              
              // TODO
              
              break;
            }
            case SPerl_OP_C_CODE_POST_INC: {
              SPerl_VAR* var = op_cur->first->uv.var;
              SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, my_var->address);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, 1);
              }

              break;
            }
            case SPerl_OP_C_CODE_PRE_DEC: {
              SPerl_VAR* var = op_cur->first->uv.var;
              SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, my_var->address);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, -1);
              }
              
              // TODO
              
              break;
            }
            case SPerl_OP_C_CODE_POST_DEC: {
              SPerl_VAR* var = op_cur->first->uv.var;
              SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;

              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IINC);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, my_var->address);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, -1);
              }

              break;
            }
            case SPerl_OP_C_CODE_BIT_XOR: {
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IXOR);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LXOR);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_BIT_OR: {
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IOR);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LOR);
              }
                            
              break;
            }
            case SPerl_OP_C_CODE_BIT_AND: {
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IAND);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LAND);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_ARRAY_ELEM: {
              
              if (op_cur->lvalue) {
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BOOLEAN || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE)
              {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_BALOAD);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_SALOAD);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IALOAD);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LALOAD);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FALOAD);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DALOAD);
              }
              else {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_AALOAD);
              }              
              break;
            }
            case SPerl_OP_C_CODE_ASSIGN: {
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              
              if (op_cur->first->code == SPerl_OP_C_CODE_VAR) {
                SPerl_OP* op_var = op_cur->first;
                int32_t my_var_pos = op_var->uv.var->op_my_var->uv.my_var->address;
                
                if (my_var_pos > 0xFF) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_WIDE);
                }
                
                _Bool has_operand = 0;
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ISTORE_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ISTORE_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ISTORE_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ISTORE_3);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ISTORE);
                    has_operand = 1;
                  }
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LSTORE_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LSTORE_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LSTORE_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LSTORE_3);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LSTORE);
                    has_operand = 1;
                  }
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FSTORE_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FSTORE_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FSTORE_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FSTORE_3);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FSTORE);
                    has_operand = 1;
                  }
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DSTORE_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DSTORE_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DSTORE_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DSTORE_3);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DSTORE);
                    has_operand = 1;
                  }
                }
                else {
                  if (my_var_pos == 0) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ASTORE_0);
                  }
                  else if (my_var_pos == 1) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ASTORE_1);
                  }
                  else if (my_var_pos == 2) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ASTORE_2);
                  }
                  else if (my_var_pos == 3) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ASTORE_3);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ASTORE);
                    has_operand = 1;
                  }
                }
                
                if (has_operand) {
                  if (my_var_pos > 0xFF) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, (my_var_pos >> 8) & 0xFF);
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, my_var_pos);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, my_var_pos);
                  }
                }
              }
              else if (op_cur->first->code == SPerl_OP_C_CODE_ARRAY_ELEM) {
                
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IASTORE);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LASTORE);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FASTORE);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DASTORE);
                }
                else {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_AASTORE);
                }
              }
              else if (op_cur->first->code == SPerl_OP_C_CODE_FIELD) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_PUTFIELD);
                
                // Call subroutine
                SPerl_NAME_INFO* name_info = op_cur->first->uv.name_info;
                const char* field_name = name_info->resolved_name;
                SPerl_FIELD* field = SPerl_HASH_search(parser->field_name_symtable, field_name, strlen(field_name));
                int32_t id = field->id;
                
                SPerl_BYTECODE_ARRAY_push(bytecode_array, (id >> 8) & 0xFF);
                SPerl_BYTECODE_ARRAY_push(bytecode_array, id & 0xFF);
              }
              
              break;
            }
            
            case SPerl_OP_C_CODE_RETURN: {
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (!resolved_type) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_RETURN);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IRETURN);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LRETURN);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FRETURN);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DRETURN);
              }
              else {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ARETURN);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_NEGATE: {
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_INEG);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LNEG);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FNEG);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DNEG);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_CONVERT: {
              
              SPerl_OP* op_term = op_cur->first;
              SPerl_OP* op_type = op_cur->last;
              SPerl_RESOLVED_TYPE* op_type_resolved_type = SPerl_OP_get_resolved_type(sperl, op_type);
              SPerl_RESOLVED_TYPE* op_term_resolved_type = SPerl_OP_get_resolved_type(sperl, op_term);
              if (op_type_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_L2I);
                }
                else if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_F2I);
                }
                else if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_D2I);
                }
              }
              else if (op_type_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_I2L);
                }
                else if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_F2L);
                }
                else if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_D2L);
                }
              }
              else if (op_type_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_I2F);
                }
                else if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_L2F);
                }
                else if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_D2F);
                }
              }
              else if (op_type_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_I2D);
                }
                else if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_L2D);
                }
                else if (op_term_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_F2D);
                }
              }
              
              break;
            }
            case SPerl_OP_C_CODE_POP: {
              
              SPerl_OP* op_first = op_cur->first;
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_first);
              if (!first_resolved_type) {
                break;
              }
              
              if (op_first->code != SPerl_OP_C_CODE_ASSIGN && op_first->code != SPerl_OP_C_CODE_RETURN && !op_first->lvalue) {
                if (first_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG || first_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_POP2);
                }
                else {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_POP);
                }
              }
              
              break;
            }
            case SPerl_OP_C_CODE_ADD: {
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IADD);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LADD);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FADD);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DADD);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_SUBTRACT: {
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ISUB);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LSUB);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FSUB);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DSUB);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_MULTIPLY: {
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IMUL);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LMUL);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FMUL);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DMUL);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_DIVIDE: {
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IDIV);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LDIV);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FDIV);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DDIV);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_REMAINDER: {
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_IREM);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LREM);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FREM);
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DREM);
              }
              
              break;
            }
            case SPerl_OP_C_CODE_VAR: {
              SPerl_VAR* var = op_cur->uv.var;
              
              if (op_cur->lvalue) {
                break;
              }
              
              SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
              
              int32_t my_var_pos = var->op_my_var->uv.my_var->address;
              
              if (my_var_pos > 0xFF) {
                SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_WIDE);
              }
              
              _Bool has_operand = 0;
              if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                if (my_var_pos == 0) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ILOAD_0);
                }
                else if (my_var_pos == 1) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ILOAD_1);
                }
                else if (my_var_pos == 2) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ILOAD_2);
                }
                else if (my_var_pos == 3) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ILOAD_3);
                }
                else {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ILOAD);
                  has_operand = 1;
                }
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                if (my_var_pos == 0) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LLOAD_0);
                }
                else if (my_var_pos == 1) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LLOAD_1);
                }
                else if (my_var_pos == 2) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LLOAD_2);
                }
                else if (my_var_pos == 3) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LLOAD_3);
                }
                else {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LLOAD);
                  has_operand = 1;
                }
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                if (my_var_pos == 0) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FLOAD_0);
                }
                else if (my_var_pos == 1) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FLOAD_1);
                }
                else if (my_var_pos == 2) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FLOAD_2);
                }
                else if (my_var_pos == 3) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FLOAD_3);
                }
                else {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FLOAD);
                  has_operand = 1;
                }
              }
              else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                if (my_var_pos == 0) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DLOAD_0);
                }
                else if (my_var_pos == 1) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DLOAD_1);
                }
                else if (my_var_pos == 2) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DLOAD_2);
                }
                else if (my_var_pos == 3) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DLOAD_3);
                }
                else {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DLOAD);
                  has_operand = 1;
                }
              }
              else {
                if (my_var_pos == 0) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ALOAD_0);
                }
                else if (my_var_pos == 1) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ALOAD_1);
                }
                else if (my_var_pos == 2) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ALOAD_2);
                }
                else if (my_var_pos == 3) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ALOAD_3);
                }
                else {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ALOAD);
                  has_operand = 1;
                }
              }
              
              if (has_operand) {
                if (my_var_pos > 0xFF) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, (my_var_pos >> 8) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, my_var_pos);
                }
                else {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, my_var_pos);
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
              if (constant->code == SPerl_CONSTANT_C_CODE_INT) {
                if (constant->uv.int_value == -1) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_M1);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 0) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_0);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 1) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_1);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 2) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_2);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 3) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_3);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 4) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_4);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value == 5) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_5);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value >= -128 && constant->uv.int_value <= 127) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_BIPUSH);
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, constant->uv.int_value & 0xFF);
                  bytecode_set = 1;
                }
                else if (constant->uv.int_value >= -32768 && constant->uv.int_value <= 32767) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_SIPUSH);
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, (constant->uv.int_value >> 8) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, constant->uv.int_value & 0xFF);
                  bytecode_set = 1;
                }
              }
              else if (constant->code == SPerl_CONSTANT_C_CODE_LONG) {
                if (constant->uv.long_value == 0) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LCONST_0);
                  bytecode_set = 1;
                }
                else if (constant->uv.long_value == 1) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LCONST_1);
                  bytecode_set = 1;
                }
              }
              else if (constant->code == SPerl_CONSTANT_C_CODE_FLOAT) {
                if (constant->uv.float_value == 0) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FCONST_0);
                  bytecode_set = 1;
                }
                else if (constant->uv.float_value == 1) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FCONST_1);
                  bytecode_set = 1;
                }
                else if (constant->uv.float_value == 2) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_FCONST_2);
                  bytecode_set = 1;
                }
              }
              else if (constant->code == SPerl_CONSTANT_C_CODE_DOUBLE) {
                if (constant->uv.double_value == 0) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DCONST_0);
                  bytecode_set = 1;
                }
                else if (constant->uv.double_value == 1) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_DCONST_1);
                  bytecode_set = 1;
                }
              }
              
              if (!bytecode_set) {
                if (constant->code == SPerl_CONSTANT_C_CODE_LONG || constant->code == SPerl_CONSTANT_C_CODE_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LDC2_W);
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, (constant->address >> 8) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(bytecode_array, constant->address & 0xFF);
                }
                else {
                  if (constant->address > 0xFF) {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LDC_W);
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, (constant->address >> 8) & 0xFF);
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, constant->address & 0xFF);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, SPerl_BYTECODE_C_CODE_LDC);
                    SPerl_BYTECODE_ARRAY_push(bytecode_array, constant->address & 0xFF);
                  }
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
            op_cur = SPerl_OP_sibling(sperl, op_cur);
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
    sub->bytecode_length = bytecode_array->length - sub->bytecode_base;
  }
}
