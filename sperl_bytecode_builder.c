#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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
#include "sperl_allocator_parser.h"
#include "sperl_package.h"
#include "sperl_name_info.h"
#include "sperl_hash.h"
#include "sperl_field.h"
#include "sperl_switch_info.h"
#include "sperl_constant_pool.h"
#include "sperl_type.h"
#include "sperl_constant_pool_sub.h"

void SPerl_BYTECODE_BUILDER_push_inc_bytecode(SPerl* sperl, SPerl_BYTECODE_ARRAY* bytecode_array, SPerl_OP* op_inc, int32_t value) {
  
  SPerl_VAR* var = op_inc->first->uv.var;
  SPerl_MY_VAR* my_var = var->op_my_var->uv.my_var;
  
  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_inc);
  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
    if (my_var->address > 0xFF || (value < -128 || value > 127)) {
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_WIDE);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BINC);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (my_var->address >> 8) & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var->address & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (value >> 8) & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, value & 0xFF);
    }
    else {
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BINC);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var->address);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, value);
    }
  }
  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
    if (my_var->address > 0xFF || (value < -128 || value > 127)) {
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_WIDE);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SINC);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (my_var->address >> 8) & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var->address & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (value >> 8) & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, value & 0xFF);
    }
    else {
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SINC);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var->address);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, value);
    }
  }
  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
    if (my_var->address > 0xFF || (value < -128 || value > 127)) {
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_WIDE);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IINC);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (my_var->address >> 8) & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var->address & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (value >> 8) & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, value & 0xFF);
    }
    else {
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IINC);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var->address);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, value);
    }
  }
  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
    if (my_var->address > 0xFF || (value < -128 || value > 127)) {
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_WIDE);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LINC);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (my_var->address >> 8) & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var->address & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (value >> 8) & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, value & 0xFF);
    }
    else {
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LINC);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var->address);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, value);
    }
  }
}

void SPerl_BYTECODE_BUILDER_push_load_bytecode(SPerl* sperl, SPerl_BYTECODE_ARRAY* bytecode_array, SPerl_OP* op_var) {
  
  (void)sperl;
  
  SPerl_VAR* var = op_var->uv.var;
  
  int32_t my_var_address = var->op_my_var->uv.my_var->address;

  if (my_var_address > 0xFF) {
    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_WIDE);
  }
  
  _Bool has_operand = 0;
  if (my_var_address == 0) {
    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LOAD_0);
  }
  else if (my_var_address == 1) {
    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LOAD_1);
  }
  else if (my_var_address == 2) {
    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LOAD_2);
  }
  else if (my_var_address == 3) {
    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LOAD_3);
  }
  else {
    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LOAD);
    has_operand = 1;
  }
  
  if (has_operand) {
    if (my_var_address > 0xFF) {
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (my_var_address >> 8) & 0xFF);
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var_address);
    }
    else {
      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var_address);
    }
  }
}

void SPerl_BYTECODE_BUILDER_build_bytecode_array(SPerl* sperl) {
  
  // Parser
  SPerl_PARSER* parser = sperl->parser;
  
  // Bytecode
  SPerl_BYTECODE_ARRAY* bytecode_array = sperl->bytecode_array;
  
  for (int32_t package_pos = 0; package_pos < parser->op_packages->length; package_pos++) {
    SPerl_OP* op_package = SPerl_ARRAY_fetch(sperl, parser->op_packages, package_pos);
    SPerl_PACKAGE* package = op_package->uv.package;
    
    for (int32_t sub_pos = 0; sub_pos < package->op_subs->length; sub_pos++) {
      SPerl_OP* op_sub = SPerl_ARRAY_fetch(sperl, package->op_subs, sub_pos);
      SPerl_SUB* sub = op_sub->uv.sub;
      
      if (sub->is_constant || sub->is_native) {
        continue;
      }
      
      sub->bytecode_base = bytecode_array->length;
      
      // Run OPs
      SPerl_OP* op_base = SPerl_OP_get_op_block_from_op_sub(sperl, op_sub);
      SPerl_OP* op_cur = op_base;
      _Bool finish = 0;
      
      // IFXXX Bytecode address(except loop)
      SPerl_ARRAY* if_address_stack = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);

      // GOTO Bytecode address for last
      SPerl_ARRAY* goto_last_address_stack = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);

      // GOTO Bytecode address for end of if block
      SPerl_ARRAY* goto_if_block_end_address_stack = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);
      
      // GOTO bytecode address for loop start
      SPerl_ARRAY* goto_loop_start_address_stack = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);
      
      // GOTO exception handler address
      SPerl_ARRAY* goto_exception_handler_stack = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);

      // try stack
      SPerl_ARRAY* try_stack = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);
      
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
          case SPerl_OP_C_CODE_TRY: {
            
            SPerl_ARRAY_push(sperl, try_stack, op_cur);
            
            break;
          }
          case SPerl_OP_C_CODE_SWITCH: {
            cur_op_switch_info = op_cur;
            cur_case_addresses = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);
            cur_op_cases = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);
            break;
          }
          case SPerl_OP_C_CODE_BLOCK: {
            if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_LOOP) {
              // Add goto
              SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_GOTO);
              
              int32_t* address_ptr = SPerl_ALLOCATOR_PARSER_alloc_int(sperl, parser->allocator);
              *address_ptr = bytecode_array->length - 1;
              
              SPerl_ARRAY_push(sperl, goto_loop_start_address_stack, address_ptr);
              
              SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
              SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
            }
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
              case SPerl_OP_C_CODE_CATCH: {
                if (try_stack->length > 0) {
                  SPerl_ARRAY_pop(sperl, try_stack);
                }
                
                int32_t pop_count = goto_exception_handler_stack->length;
                for (int32_t i = 0; i < pop_count; i++) {
                  assert(goto_exception_handler_stack->length > 0);
                  
                  int32_t* address_ptr = SPerl_ARRAY_pop(sperl, goto_exception_handler_stack);
                  int32_t address = *address_ptr;
                  
                  int32_t jump_offset = bytecode_array->length - address;
                  
                  bytecode_array->values[address + 1] = (jump_offset >> 8) & 0xFF;
                  bytecode_array->values[address + 2] = jump_offset & 0xFF;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_SWITCH_CONDITION: {
                
                SPerl_SWITCH_INFO* switch_info = cur_op_switch_info->uv.switch_info;
                
                // tableswitch
                if (switch_info->code == SPerl_SWITCH_INFO_C_CODE_TABLESWITCH) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_TABLESWITCH);
                  
                  cur_switch_address = bytecode_array->length - 1;
                  
                  int64_t max = switch_info->max;
                  int64_t min = switch_info->min;
                  
                  // Machine address to calculate padding
                  cur_switch_address = bytecode_array->length - 1;
                  
                  // Padding
                  int32_t padding = ((int32_t)sizeof(int64_t) - 1) - (cur_switch_address % (int32_t)sizeof(int64_t));
                  
                  for (int32_t i = 0; i < padding; i++) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  }
                  
                  // Default
                  for (int32_t i = 0; i < (int32_t)sizeof(int64_t); i++) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  }
                  
                  // Low
                  for (int32_t i = 0; i < (int32_t)sizeof(int64_t); i++) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  }
                  *(int64_t*)&bytecode_array->values[bytecode_array->length - sizeof(int64_t)] = min;
                  
                  // Low
                  for (int32_t i = 0; i < (int32_t)sizeof(int64_t); i++) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  }
                  *(int64_t*)&bytecode_array->values[bytecode_array->length - sizeof(int64_t)] = max;
                  
                  // Offsets
                  for (int64_t i = 0; i < (max - min + 1) * (int64_t)sizeof(int64_t); i++) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  }
                }
                // lookupswitch
                else if (switch_info->code == SPerl_SWITCH_INFO_C_CODE_LOOKUPSWITCH) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LOOKUPSWITCH);
                  
                  if (switch_info->op_cases->length > SPerl_OP_LIMIT_CASES) {
                    fprintf(stderr, "Invalid AST: too many cases in switch statement\n");
                    exit(1);
                  }
                  int32_t const length = switch_info->op_cases->length;

                  // Machine address to calculate padding
                  cur_switch_address = bytecode_array->length - 1;
                  
                  // Padding
                  int32_t padding = (sizeof(int64_t) - 1) - (cur_switch_address % sizeof(int64_t));
                  
                  for (int32_t i = 0; i < padding; i++) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  }
                  
                  // Default
                  for (int32_t i = 0; i < (int32_t)sizeof(int64_t); i++) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  }
                  
                  // Case count
                  for (int32_t i = 0; i < (int32_t)sizeof(int64_t); i++) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  }
                  *(int64_t*)&bytecode_array->values[bytecode_array->length - (int32_t)sizeof(int64_t)] = length;
                  
                  int32_t size_of_match_offset_pairs = length * (int32_t)sizeof(int64_t) * 2;
                  for (int32_t i = 0; i < size_of_match_offset_pairs; i++) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  }
                }
                
                break;
              }
              case SPerl_OP_C_CODE_CASE: {
                
                int32_t* address_ptr = SPerl_ALLOCATOR_PARSER_alloc_int(sperl, parser->allocator);
                *address_ptr = bytecode_array->length;
                
                SPerl_ARRAY_push(sperl, cur_case_addresses, address_ptr);
                SPerl_ARRAY_push(sperl, cur_op_cases, op_cur);
                
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
                  int32_t padding = (sizeof(int64_t) - 1) - (cur_switch_address % sizeof(int64_t));
                  
                  
                  // Default offset
                  int64_t default_offset;
                  if (cur_default_address == -1) {
                    default_offset = bytecode_array->length - cur_switch_address;
                  }
                  else {
                    default_offset = cur_default_address - cur_switch_address;
                  }
                  bytecode_array->values[cur_switch_address + padding + 1] = *(int64_t*)&default_offset;
                  
                  // min
                  int64_t min = *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) + 1];
                  
                  // max
                  int64_t max = *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) * 2 + 1];
                  
                  int64_t length = max - min + 1;
                  
                  int64_t case_pos = 0;
                  for (int64_t i = 0; i < length; i++) {
                    SPerl_OP* op_case = SPerl_ARRAY_fetch(sperl, cur_op_cases, case_pos);
                    SPerl_OP* op_constant = op_case->first;
                    if (op_constant->uv.constant->uv.long_value - min == i) {
                      // Case
                      int64_t* case_address_ptr = SPerl_ARRAY_fetch(sperl, cur_case_addresses, case_pos);
                      int64_t case_address = *case_address_ptr;
                      int64_t case_offset = case_address - cur_switch_address;
                      
                      *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) * 3 + 1 + (sizeof(int64_t) * i)] = case_offset;
                      
                      case_pos++;
                    }
                    else {
                      // Default
                      *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) * 3 + 1 + (sizeof(int64_t) * i)] = default_offset;
                    }
                  }
                }
                // lookupswitch
                else if (switch_info->code == SPerl_SWITCH_INFO_C_CODE_LOOKUPSWITCH) {
                  int64_t padding = (sizeof(int64_t) - 1) - (cur_switch_address % sizeof(int64_t));

                  // Default offset
                  int64_t default_offset;
                  if (cur_default_address == -1) {
                    default_offset = bytecode_array->length - cur_switch_address;
                  }
                  else {
                    default_offset = cur_default_address - cur_switch_address;
                  }
                  *(int64_t*)&bytecode_array->values[cur_switch_address + padding + 1] = default_offset;
                  
                  // Note: Here it's assumed that the number of cases can be expressed by a int64_t variable.
                  assert(SPerl_OP_LIMIT_CASES <= INT32_MAX);
                  if (switch_info->op_cases->length > SPerl_OP_LIMIT_CASES) {
                    fprintf(stderr, "Invalid AST: too many cases in switch statement\n");
                    exit(1);
                  }
                  int64_t const length = (int64_t) switch_info->op_cases->length;
                  
                  SPerl_ARRAY* ordered_op_cases = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);
                  for (int64_t i = 0; i < length; i++) {
                    SPerl_OP* op_case = SPerl_ARRAY_fetch(sperl, cur_op_cases, i);
                    SPerl_ARRAY_push(sperl, ordered_op_cases, op_case);
                  }
                  SPerl_ARRAY* ordered_case_addresses = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);
                  for (int64_t i = 0; i < length; i++) {
                    int64_t* case_address_ptr = SPerl_ARRAY_fetch(sperl, cur_case_addresses, i);
                    SPerl_ARRAY_push(sperl, ordered_case_addresses, case_address_ptr);
                  }
                  
                  // sort by asc order
                  for (int64_t i = 0; i < length; i++) {
                    for (int64_t j = i + 1; j < length; j++) {
                      SPerl_OP* op_case_i = SPerl_ARRAY_fetch(sperl, ordered_op_cases, i);
                      SPerl_OP* op_case_j = SPerl_ARRAY_fetch(sperl, ordered_op_cases, j);
                      int64_t match_i = op_case_i->first->uv.constant->uv.long_value;
                      int64_t match_j = op_case_j->first->uv.constant->uv.long_value;
                      
                      int64_t* case_address_i = SPerl_ARRAY_fetch(sperl, ordered_case_addresses, i);
                      int64_t* case_address_j = SPerl_ARRAY_fetch(sperl, ordered_case_addresses, j);
                      
                      if (match_i > match_j) {
                        SPerl_ARRAY_store(sperl, ordered_op_cases, i, op_case_j);
                        SPerl_ARRAY_store(sperl, ordered_op_cases, j, op_case_i);
                        
                        SPerl_ARRAY_store(sperl, ordered_case_addresses, i, case_address_j);
                        SPerl_ARRAY_store(sperl, ordered_case_addresses, j, case_address_i);
                      }
                    }
                  }
                  
                  for (int64_t i = 0; i < length; i++) {
                    SPerl_OP* op_case = SPerl_ARRAY_fetch(sperl, ordered_op_cases, i);
                    SPerl_OP* op_constant = op_case->first;
                    int64_t match = op_constant->uv.constant->uv.long_value;

                    int64_t* case_address_ptr = SPerl_ARRAY_fetch(sperl, ordered_case_addresses, i);
                    int64_t case_address = *case_address_ptr;
                    int64_t case_offset = case_address - cur_switch_address;
                    
                    // Match
                    *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) * 2 + 1 + (sizeof(int64_t) * 2 * i)] = match;

                    // Offset
                    *(int64_t*)&bytecode_array->values[cur_switch_address + padding + sizeof(int64_t) * 3 + 1 + (sizeof(int64_t) * 2 * i)] = case_offset;
                  }
                }
                
                cur_op_switch_info = NULL;
                cur_default_address = -1;
                cur_case_addresses = NULL;
                
                break;
              }
              case SPerl_OP_C_CODE_CALL_FIELD: {
                
                if (!op_cur->lvalue) {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                  
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE)
                  {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BGETFIELD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SGETFIELD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IGETFIELD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LGETFIELD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FGETFIELD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DGETFIELD);
                  }
                  else {
                    assert(0);
                  }
                  
                  SPerl_NAME_INFO* name_info = op_cur->uv.name_info;
                  const char* field_name = name_info->resolved_name;
                  SPerl_OP* op_field = SPerl_HASH_search(sperl, parser->op_field_symtable, field_name, strlen(field_name));
                  SPerl_FIELD* field = op_field->uv.field;
                  
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (field->constant_pool_address >> 24) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (field->constant_pool_address >> 16) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (field->constant_pool_address >> 8) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, field->constant_pool_address & 0xFF);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_CALL_SUB: {
                
                // Call subroutine
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_INVOKESTATIC_WW);
                SPerl_NAME_INFO* name_info = op_cur->uv.name_info;
                const char* sub_name = name_info->resolved_name;
                
                SPerl_OP* op_sub = SPerl_HASH_search(sperl, parser->op_sub_symtable, sub_name, strlen(sub_name));
                SPerl_SUB* sub = op_sub->uv.sub;
                
                int32_t constant_pool_address = sub->constant_pool_address;
                
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant_pool_address >> 24) & 0xFF);
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant_pool_address >> 16) & 0xFF);
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant_pool_address >> 8) & 0xFF);
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant_pool_address & 0xFF);
                
                //  Goto exception handler
                if (try_stack->length > 0) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_GOTO);
                  
                  int32_t* address_ptr = SPerl_ALLOCATOR_PARSER_alloc_int(sperl, parser->allocator);
                  *address_ptr = bytecode_array->length - 1;
                  SPerl_ARRAY_push(sperl, goto_exception_handler_stack, address_ptr);
                  
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                }
                // Rethrow exception
                else {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ATHROW);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_DIE: {
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ATHROW);
                break;
              }
              case SPerl_OP_C_CODE_LAST: {
                // Add goto
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_GOTO);
                
                int32_t* address_ptr = SPerl_ALLOCATOR_PARSER_alloc_int(sperl, parser->allocator);
                *address_ptr = bytecode_array->length - 1;
                
                SPerl_ARRAY_push(sperl, goto_last_address_stack, address_ptr);
                
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                break;
              }
              case SPerl_OP_C_CODE_NEXT: {
                int32_t* address_ptr = SPerl_ARRAY_fetch(sperl, goto_loop_start_address_stack, goto_loop_start_address_stack->length - 1);
                int32_t address = *address_ptr;
                
                // Add "goto"
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_GOTO);
                
                // Jump offset
                int32_t jump_offset = address - (bytecode_array->length - 1) + 3;
                
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (jump_offset >> 8) & 0xFF);
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, jump_offset & 0xFF);
                
                break;
              }
              case SPerl_OP_C_CODE_BLOCK: {
                if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_IF) {
                  
                  // Has else block
                  if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_HAS_ELSE) {
                    // Prepare to jump to end of else block
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_GOTO);
                    
                    int32_t* address_ptr = SPerl_ALLOCATOR_PARSER_alloc_int(sperl, parser->allocator);
                    *address_ptr = bytecode_array->length - 1;
                    SPerl_ARRAY_push(sperl, goto_if_block_end_address_stack, address_ptr);
                    
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  }

                  assert(if_address_stack->length > 0);

                  // Set if jump address
                  int32_t* address_ptr = SPerl_ARRAY_pop(sperl, if_address_stack);
                  int32_t address = *address_ptr;
                  
                  // Jump offset
                  int32_t jump_offset = bytecode_array->length - address;
                  
                  // Set jump offset
                  bytecode_array->values[address + 1] = (jump_offset >> 8) & 0xFF;
                  bytecode_array->values[address + 2] = jump_offset & 0xFF;
                }
                else if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_ELSE) {
                  
                  assert(goto_if_block_end_address_stack->length > 0);
                  
                  int32_t* address_ptr = SPerl_ARRAY_pop(sperl, goto_if_block_end_address_stack);
                  int32_t address = *address_ptr;
                  
                  // Jump offset
                  int32_t jump_offset = bytecode_array->length - address;
                  
                  // Set jump offset
                  bytecode_array->values[address + 1] = (jump_offset >> 8) & 0xFF;
                  bytecode_array->values[address + 2] = jump_offset & 0xFF;
                }
                else if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_LOOP) {
                  
                  int32_t* goto_loop_start_address_ptr = SPerl_ARRAY_fetch(sperl, goto_loop_start_address_stack, goto_loop_start_address_stack->length - 1);
                  int32_t goto_loop_start_address = *goto_loop_start_address_ptr;
                  
                  // Jump offset
                  int32_t goto_loop_start_offset = bytecode_array->length - goto_loop_start_address;
                  
                  bytecode_array->values[goto_loop_start_address + 1] = (goto_loop_start_offset >> 8) & 0xFF;
                  bytecode_array->values[goto_loop_start_address + 2] = goto_loop_start_offset & 0xFF;
                }
                break;
              }
              case SPerl_OP_C_CODE_IF: {
                
                break;
              }
              case SPerl_OP_C_CODE_LOOP: {
                
                // Set last position
                while (goto_last_address_stack->length > 0) {
                  
                  int32_t* goto_last_address_ptr = SPerl_ARRAY_pop(sperl, goto_last_address_stack);
                  int32_t goto_last_address = *goto_last_address_ptr;
                  
                  // Last offset
                  int32_t goto_last_offset = bytecode_array->length - goto_last_address;
                  
                  bytecode_array->values[goto_last_address + 1] = (goto_last_offset >> 8) & 0xFF;
                  bytecode_array->values[goto_last_address + 2] = goto_last_offset & 0xFF;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_CONDITION: {
                SPerl_OP* op_condition_target = op_cur->first;

                if (op_condition_target->code == SPerl_OP_C_CODE_UNDEF) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNONNULL);
                }
                else if (op_condition_target->code == SPerl_OP_C_CODE_EQ) {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                  if (!resolved_type) {
                    resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->last);
                  }
                  
                  if (!resolved_type) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IF_ACMPNE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IF_LCMPNE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FCMPG);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DCMPG);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IF_ACMPNE);
                  }
                }
                else if (op_condition_target->code == SPerl_OP_C_CODE_NE) {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                  if (!resolved_type) {
                    resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->last);
                  }
                  
                  if (!resolved_type) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IF_ACMPEQ);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFEQ);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFEQ);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFEQ);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IF_LCMPEQ);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FCMPG);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFEQ);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DCMPG);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFEQ);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IF_ACMPEQ);
                  }
                }
                else if (op_condition_target->code == SPerl_OP_C_CODE_GT) {
                  
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFLE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFLE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFLE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IF_LCMPLE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FCMPL);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFLE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DCMPL);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFLE);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (op_condition_target->code == SPerl_OP_C_CODE_GE) {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFLT);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFLT);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFLT);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IF_LCMPLT);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FCMPL);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFLT);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DCMPL);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFLT);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (op_condition_target->code == SPerl_OP_C_CODE_LT) {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFGE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFGE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFGE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IF_LCMPGE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FCMPG);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFGE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DCMPG);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFGE);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (op_condition_target->code == SPerl_OP_C_CODE_LE) {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target->first);
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFGT);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SCMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFGT);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICMP);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFGT);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IF_LCMPGT);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FCMPG);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFGT);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DCMPG);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFGT);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (op_condition_target) {
                  
                  if (op_condition_target->code == SPerl_OP_C_CODE_IF) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                  }
                  else {
                    SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_condition_target);
                    if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_B2L);
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_S2L);
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_I2L);
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_F2L);
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                    }
                    else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_D2L);
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNE);
                    }
                    else {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IFNONNULL);
                    }
                  }
                }
                
                int32_t address = bytecode_array->length - 1;
                
                if (op_cur->flag & SPerl_OP_C_FLAG_CONDITION_IF) {
                  int32_t* address_ptr = SPerl_ALLOCATOR_PARSER_alloc_int(sperl, parser->allocator);
                  *address_ptr = address;
                  
                  SPerl_ARRAY_push(sperl, if_address_stack, address_ptr);
                  
                  // Prepare for bytecode position of branch
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, 0);
                }
                else if (op_cur->flag & SPerl_OP_C_FLAG_CONDITION_LOOP) {
                  assert(goto_loop_start_address_stack->length > 0);
                  
                  int32_t* goto_loop_start_address_ptr = SPerl_ARRAY_pop(sperl, goto_loop_start_address_stack);
                  int32_t goto_loop_start_address = *goto_loop_start_address_ptr;
                  
                  // Jump offset
                  int32_t goto_loop_start_offset = goto_loop_start_address - (bytecode_array->length - 1) + 3;
                  
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (goto_loop_start_offset >> 8) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, goto_loop_start_offset & 0xFF);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_ARRAY_LENGTH : {
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ARRAYLENGTH);
                
                break;
              }
              case SPerl_OP_C_CODE_LEFT_SHIFT: {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BSHL);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SSHL);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ISHL);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LSHL);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_RIGHT_SHIFT: {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BSHR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SSHR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ISHR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LSHR);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED: {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BUSHR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SUSHR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IUSHR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LUSHR);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_MALLOC: {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                
                if (SPerl_RESOLVED_TYPE_is_core_type_array(sperl, resolved_type)) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_MALLOCARRAY);
                  
                  if (strcmp(resolved_type->name, "byte[]") == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_RESOLVED_TYPE_C_ID_BYTE);
                  }
                  else if (strcmp(resolved_type->name, "short[]") == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_RESOLVED_TYPE_C_ID_SHORT);
                  }
                  else if (strcmp(resolved_type->name, "int[]") == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_RESOLVED_TYPE_C_ID_INT);
                  }
                  else if (strcmp(resolved_type->name, "long[]") == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_RESOLVED_TYPE_C_ID_LONG);
                  }
                  else if (strcmp(resolved_type->name, "float[]") == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_RESOLVED_TYPE_C_ID_FLOAT);
                  }
                  else if (strcmp(resolved_type->name, "double[]") == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_RESOLVED_TYPE_C_ID_DOUBLE);
                  }
                  else {
                    assert(0);
                  }
                }
                else {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_MALLOC);
                  
                  const char* package_name = op_cur->first->uv.type->resolved_type->name;
                  
                  SPerl_OP* op_package = SPerl_HASH_search(sperl, parser->op_package_symtable, package_name, strlen(package_name));
                  SPerl_PACKAGE* package = op_package->uv.package;
                  
                  int32_t constant_pool_address = package->constant_pool_address;
                  
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant_pool_address >> 24) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant_pool_address >> 16) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant_pool_address >> 8) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant_pool_address & 0xFF);
                }
                break;
              }
              
              case SPerl_OP_C_CODE_UNDEF: {
                
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ACONST_NULL);
                
                break;
              }
              case SPerl_OP_C_CODE_PRE_INC: {
                SPerl_BYTECODE_BUILDER_push_inc_bytecode(sperl, bytecode_array, op_cur, 1);
                SPerl_BYTECODE_BUILDER_push_load_bytecode(sperl, bytecode_array, op_cur->first);
                
                break;
              }
              case SPerl_OP_C_CODE_POST_INC: {
                SPerl_BYTECODE_BUILDER_push_load_bytecode(sperl, bytecode_array, op_cur->first);
                SPerl_BYTECODE_BUILDER_push_inc_bytecode(sperl, bytecode_array, op_cur, 1);
                
                break;
              }
              case SPerl_OP_C_CODE_PRE_DEC: {
                SPerl_BYTECODE_BUILDER_push_inc_bytecode(sperl, bytecode_array, op_cur, -1);
                SPerl_BYTECODE_BUILDER_push_load_bytecode(sperl, bytecode_array, op_cur->first);
                
                break;
              }
              case SPerl_OP_C_CODE_POST_DEC: {
                SPerl_BYTECODE_BUILDER_push_load_bytecode(sperl, bytecode_array, op_cur->first);
                SPerl_BYTECODE_BUILDER_push_inc_bytecode(sperl, bytecode_array, op_cur, -1);
                
                break;
              }
              case SPerl_OP_C_CODE_BIT_XOR: {
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BXOR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SXOR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IXOR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LXOR);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_BIT_OR: {
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BOR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SOR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IOR);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LOR);
                }
                              
                break;
              }
              case SPerl_OP_C_CODE_BIT_AND: {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BAND);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SAND);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IAND);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LAND);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_ARRAY_ELEM: {
                
                if (op_cur->lvalue) {
                  break;
                }
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                if (last_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE)
                  {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BALOAD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SALOAD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IALOAD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LALOAD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FALOAD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DALOAD);
                  }
                  else {
                    assert(0);
                  }
                }
                else {
                  assert(0);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_INCREFCOUNT: {
                
                SPerl_OP* op_var = op_cur->first;
                int32_t my_var_address = op_var->uv.var->op_my_var->uv.my_var->address;
                
                if (my_var_address > 0xFF) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_WIDE);
                }
                
                SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_INCREFCOUNT);
                
                if (my_var_address > 0xFF) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (my_var_address >> 8) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var_address);
                }
                else {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var_address);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_ASSIGN: {
                
                if (op_cur->first->code == SPerl_OP_C_CODE_VAR) {
                  SPerl_OP* op_var = op_cur->first;
                  int32_t my_var_address = op_var->uv.var->op_my_var->uv.my_var->address;
                  
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_var);
                  
                  if (my_var_address > 0xFF) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_WIDE);
                  }
                  
                  _Bool has_operand = 0;
                  
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, resolved_type)) {
                    if (my_var_address == 0) {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_STORE_0);
                    }
                    else if (my_var_address == 1) {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_STORE_1);
                    }
                    else if (my_var_address == 2) {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_STORE_2);
                    }
                    else if (my_var_address == 3) {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_STORE_3);
                    }
                    else {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_STORE);
                      has_operand = 1;
                    }
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ASTORE);
                    has_operand = 1;
                  }
                  
                  if (has_operand) {
                    if (my_var_address > 0xFF) {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (my_var_address >> 8) & 0xFF);
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var_address);
                    }
                    else {
                      SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, my_var_address);
                    }
                  }
                }
                else if (op_cur->first->code == SPerl_OP_C_CODE_ARRAY_ELEM) {
                  
                  SPerl_OP* op_array_elem = op_cur->first;
                  SPerl_OP* op_term_index = op_array_elem->last;
                  
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_array_elem);
                  SPerl_RESOLVED_TYPE* index_resolved_type = SPerl_OP_get_resolved_type(sperl, op_term_index);
                  
                  assert(index_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG);
                  
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BASTORE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SASTORE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IASTORE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LASTORE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FASTORE);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DASTORE);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_AASTORE);
                  }
                }
                else if (op_cur->first->code == SPerl_OP_C_CODE_CALL_FIELD) {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  
                  if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BPUTFIELD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SPUTFIELD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IPUTFIELD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LPUTFIELD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FPUTFIELD);
                  }
                  else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DPUTFIELD);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_APUTFIELD);
                  }
                  
                  // Call subroutine
                  SPerl_NAME_INFO* name_info = op_cur->first->uv.name_info;
                  const char* field_name = name_info->resolved_name;
                  SPerl_OP* op_field = SPerl_HASH_search(sperl, parser->op_field_symtable, field_name, strlen(field_name));
                  SPerl_FIELD* field = op_field->uv.field;
                  
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (field->constant_pool_address >> 24) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (field->constant_pool_address >> 16) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (field->constant_pool_address >> 8) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, field->constant_pool_address & 0xFF);
                }
                
                break;
              }
              
              case SPerl_OP_C_CODE_RETURN: {
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                if (!resolved_type) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_RETURN_VOID);
                }
                else {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_RETURN_VALUE);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_NEGATE: {
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BNEG);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SNEG);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_INEG);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LNEG);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FNEG);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DNEG);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_CONVERT: {
                
                SPerl_OP* op_src_term = op_cur->first;
                SPerl_OP* op_dist_type = op_cur->last;
                
                SPerl_RESOLVED_TYPE* src_resolved_type = SPerl_OP_get_resolved_type(sperl, op_src_term);
                SPerl_RESOLVED_TYPE* dist_resolved_type = SPerl_OP_get_resolved_type(sperl, op_dist_type);
                
                if (src_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    // None
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_B2S);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_B2I);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_B2L);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_B2F);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_B2D);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (src_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_S2B);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    // None
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_S2I);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_S2L);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_S2F);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_S2D);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (src_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_I2B);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_I2S);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    // None
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_I2L);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_I2F);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_I2D);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (src_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_L2B);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_L2S);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_L2I);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    // None
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_L2F);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_L2D);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (src_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_F2B);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_F2S);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_F2I);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_F2L);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    // None
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_F2D);
                  }
                  else {
                    assert(0);
                  }
                }
                else if (src_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_D2B);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_D2S);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_D2I);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_D2L);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_D2F);
                  }
                  else if (dist_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    // None
                  }
                  else {
                    assert(0);
                  }
                }
                else {
                  assert(0);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_POP: {
                
                SPerl_OP* op_first = op_cur->first;
                
                if (op_first->code != SPerl_OP_C_CODE_ASSIGN && op_first->code != SPerl_OP_C_CODE_RETURN && !op_first->lvalue) {
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_first);
                  
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, resolved_type)) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_POP);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_APOP);
                  }
                }
                
                break;
              }
              case SPerl_OP_C_CODE_ADD: {
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BADD);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SADD);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IADD);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LADD);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FADD);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DADD);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_SUBTRACT: {
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BSUB);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SSUB);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ISUB);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LSUB);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FSUB);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DSUB);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_MULTIPLY: {
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BMUL);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SMUL);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IMUL);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LMUL);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FMUL);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DMUL);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_DIVIDE: {
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BDIV);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SDIV);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IDIV);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LDIV);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FDIV);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DDIV);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_REMAINDER: {
                
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BREM);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SREM);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_INT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_IREM);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LREM);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FREM);
                }
                else if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DREM);
                }
                
                break;
              }
              case SPerl_OP_C_CODE_VAR: {
                if (op_cur->lvalue) {
                  break;
                }
                
                SPerl_BYTECODE_BUILDER_push_load_bytecode(sperl, bytecode_array, op_cur);
                
                break;
              }
              case SPerl_OP_C_CODE_CONSTANT: {
                SPerl_CONSTANT* constant = op_cur->uv.constant;
                
                if (op_cur->flag == SPerl_OP_C_FLAG_CONSTANT_CASE) {
                  break;
                }
                
                _Bool bytecode_set = 0;
                if (constant->code == SPerl_CONSTANT_C_CODE_BYTE) {
                  if (constant->uv.byte_value == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.byte_value == 1) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BCONST_1);
                    bytecode_set = 1;
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BBPUSH);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant->uv.byte_value & 0xFF);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPerl_CONSTANT_C_CODE_SHORT) {
                  if (constant->uv.short_value == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.short_value == 1) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SCONST_1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.short_value >= -128 && constant->uv.short_value <= 127) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BSPUSH);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant->uv.short_value & 0xFF);
                    bytecode_set = 1;
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SSPUSH);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant->uv.short_value >> 8) & 0xFF);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant->uv.short_value & 0xFF);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPerl_CONSTANT_C_CODE_INT) {
                  if (constant->uv.int_value == -1) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_M1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 1) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 2) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_2);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 3) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_3);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 4) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_4);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value == 5) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_ICONST_5);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value >= -128 && constant->uv.int_value <= 127) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BIPUSH);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant->uv.int_value & 0xFF);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.int_value >= -32768 && constant->uv.int_value <= 32767) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SIPUSH);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant->uv.int_value >> 8) & 0xFF);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant->uv.int_value & 0xFF);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPerl_CONSTANT_C_CODE_LONG) {
                  if (constant->uv.long_value == -1) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LCONST_M1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 1) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LCONST_1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 2) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LCONST_2);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 3) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LCONST_3);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 4) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LCONST_4);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value == 5) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LCONST_5);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value >= -128 && constant->uv.long_value <= 127) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_BLPUSH);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant->uv.long_value & 0xFF);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.long_value >= -32768 && constant->uv.long_value <= 32767) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_SLPUSH);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant->uv.long_value >> 8) & 0xFF);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant->uv.long_value & 0xFF);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPerl_CONSTANT_C_CODE_FLOAT) {
                  if (constant->uv.float_value == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.float_value == 1) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FCONST_1);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.float_value == 2) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_FCONST_2);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPerl_CONSTANT_C_CODE_DOUBLE) {
                  if (constant->uv.double_value == 0) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DCONST_0);
                    bytecode_set = 1;
                  }
                  else if (constant->uv.double_value == 1) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_DCONST_1);
                    bytecode_set = 1;
                  }
                }
                else if (constant->code == SPerl_CONSTANT_C_CODE_STRING) {
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_MALLOCSTRING);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant->constant_pool_address >> 24) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant->constant_pool_address >> 16) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant->constant_pool_address >> 8) & 0xFF);
                  SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant->constant_pool_address & 0xFF);
                  bytecode_set = 1;
                }
                
                if (!bytecode_set) {
                  if (constant->constant_pool_address <= 0xFF) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LDC);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant->constant_pool_address & 0xFF);
                  }
                  else if (constant->constant_pool_address <= 0xFFFF) {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LDC_W);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant->constant_pool_address >> 8) & 0xFF);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant->constant_pool_address & 0xFF);
                  }
                  else {
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, SPerl_BYTECODE_C_CODE_LDC_WW);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant->constant_pool_address >> 24) & 0xFF);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant->constant_pool_address >> 16) & 0xFF);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, (constant->constant_pool_address >> 8) & 0xFF);
                    SPerl_BYTECODE_ARRAY_push(sperl, bytecode_array, constant->constant_pool_address & 0xFF);
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
      
      // Set bytecode base to sub
      SPerl_CONSTANT_POOL_SUB* constant_pool_sub = (SPerl_CONSTANT_POOL_SUB*)&sperl->constant_pool->values[sub->constant_pool_address];
      constant_pool_sub->bytecode_base = sub->bytecode_base;
    }
  }
}
