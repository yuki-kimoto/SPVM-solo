#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_allocator.h"
#include "sperl_yacc_util.h"
#include "sperl_op.h"
#include "sperl_sub.h"
#include "sperl_constant.h"
#include "sperl_field.h"
#include "sperl_my_var.h"
#include "sperl_var.h"
#include "sperl_memory_pool.h"
#include "sperl_use.h"
#include "sperl_enumeration_value.h"
#include "sperl_type.h"
#include "sperl_type_component_name.h"
#include "sperl_type_component_array.h"
#include "sperl_type_component_sub.h"
#include "sperl_type_part.h"
#include "sperl_enumeration.h"
#include "sperl_package.h"
#include "sperl_name_info.h"
#include "sperl_resolved_type.h"
#include "sperl_switch_info.h"

void SPerl_OP_CHECKER_check(SPerl* sperl) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  for (int32_t sub_pos = 0; sub_pos < parser->op_subs->length; sub_pos++) {
    SPerl_OP* op_sub = SPerl_ARRAY_fetch(parser->op_subs, sub_pos);
    SPerl_SUB* sub = op_sub->uv.sub;
      
    // my var informations
    int32_t next_my_var_pos = 0;
    SPerl_ARRAY* op_my_vars = SPerl_ALLOCATOR_new_array(sperl, 0);
    
    // my variable stack
    SPerl_ARRAY* op_my_var_stack = SPerl_ALLOCATOR_new_array(sperl, 0);
    
    // block base position stack
    SPerl_ARRAY* block_base_stack = SPerl_ALLOCATOR_new_array(sperl, 0);
    int32_t block_base = 0;
    _Bool block_start = 0;
    
    // In switch statement
    _Bool in_switch = 0;
    
    // Current case statements
    SPerl_ARRAY* cur_case_ops = NULL;
    
    // Current default statement
    SPerl_OP* cur_default_op = NULL;
    
    // Operand stack max;
    int32_t operand_stack_max = 0;
    
    // Operand stack count
    int32_t operand_stack_count = 0;
    
    // Call stack max
    int32_t call_stack_max = 0;
    
    // Argument total max size of subroutine call
    int32_t argument_total_size_max = 0;
    
    // Run OPs
    SPerl_OP* op_base = op_sub;
    SPerl_OP* op_cur = op_base;
    _Bool finish = 0;
    while (op_cur) {
      
      // [START]Preorder traversal position
      
      switch (op_cur->code) {
        case SPerl_OP_C_CODE_SWITCH: {
          if (in_switch) {
            SPerl_yyerror_format(sperl, "duplicate switch is forbidden at %s line %d\n", op_cur->file, op_cur->line);
            parser->fatal_error = 1;
            return;
          }
          else {
            in_switch = 1;
          }
          
          break;
        }
        // Start scope
        case SPerl_OP_C_CODE_BLOCK: {
          if (block_start) {
            int32_t* block_base_ptr = SPerl_MEMORY_POOL_alloc(sperl->memory_pool, sizeof(int32_t));
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
        case SPerl_OP_C_CODE_FIELD: {
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
            case SPerl_OP_C_CODE_DEFAULT: {
              if (cur_default_op) {
                SPerl_yyerror_format(sperl, "multiple default is forbidden at %s line %d\n", op_cur->file, op_cur->line);
                parser->fatal_error = 1;
                return;
              }
              else {
                cur_default_op = op_cur;
              }
              break;
            }
            case SPerl_OP_C_CODE_CASE: {
              
              SPerl_OP* op_term = op_cur->first;
              
              if (op_term->code != SPerl_OP_C_CODE_CONSTANT) {
                SPerl_yyerror_format(sperl, "case need constant at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              SPerl_RESOLVED_TYPE* op_term_resolved_type = SPerl_OP_get_resolved_type(sperl, op_term);
              if (op_term_resolved_type->id > SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_yyerror_format(sperl, "case need int at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              if (!cur_case_ops) {
                cur_case_ops = SPerl_ALLOCATOR_new_array(sperl, 0);
              }
              SPerl_ARRAY_push(cur_case_ops, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_SWITCH: {
              
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              
              if (first_resolved_type->id > SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_yyerror_format(sperl, "switch need int at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              in_switch = 0;
              cur_default_op = NULL;
              cur_case_ops = NULL;
              
              // tableswitch if the following. SWITCHRTIO is 1.5 by default
              // 4 + range <= (3 + 2 * length) * SWITCHRTIO
              
              SPerl_SWITCH_INFO* switch_info = op_cur->uv.switch_info;
              SPerl_ARRAY* op_cases = switch_info->op_cases;
              int32_t length = op_cases->length;
              int32_t min = SPerl_BASE_C_INT_MAX;
              int32_t max = SPerl_BASE_C_INT_MIN;
              
              for (int32_t i = 0; i < op_cases->length; i++) {
                SPerl_OP* op_case = SPerl_ARRAY_fetch(op_cases, i);
                SPerl_OP* op_constant = op_case->first;
                int32_t value = op_constant->uv.constant->uv.int_value;
                if (value < min) {
                  min = value;
                }
                if (value > max) {
                  max = value;
                }
              }
              
              int32_t range = max - min;
              
              int32_t code;
              if (4 + range <= (3 + 2 * length) * 1.5) {
                code = SPerl_SWITCH_INFO_C_CODE_TABLESWITCH;
              }
              else {
                code = SPerl_SWITCH_INFO_C_CODE_LOOKUPSWITCH;
              }
              
              switch_info->code = code;
              switch_info->min = min;
              switch_info->max = max;
              
              break;
            }
            
            case SPerl_OP_C_CODE_CONDITION: {
              if ((op_cur->first && !op_cur->last) || op_cur->first == op_cur->last) {
                SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                if (!resolved_type) {
                  SPerl_OP_convert_to_op_constant_false(sperl, op_cur->first);
                }
              }
              
              break;
            }
            case SPerl_OP_C_CODE_AND: {
              if (!op_cur->condition) {
                SPerl_yyerror_format(sperl, "&& operator can use only condition context at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Convert && to if statement
              SPerl_OP_convert_and_to_if(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_OR: {
              if (!op_cur->condition) {
                SPerl_yyerror_format(sperl, "|| operator can use only condition context at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }

              // Convert || to if statement
              SPerl_OP_convert_or_to_if(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_NOT: {
              if (!op_cur->condition) {
                SPerl_yyerror_format(sperl, "! operator can use only condition context at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Convert ! to if statement
              SPerl_OP_convert_not_to_if(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_EQ: {
              if (!op_cur->condition) {
                SPerl_yyerror_format(sperl, "== operator can use only condition context at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              if (!first_resolved_type && !last_resolved_type) {
                // Convert to constant true
                SPerl_OP_convert_to_op_constant_true(sperl, op_cur);
                op_cur->first = NULL;
                op_cur->last = NULL;
              }
              else {
                // undef
                if (!first_resolved_type) {
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, "== right value must be reference at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
                else if (!last_resolved_type) {
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "== left value must be reference at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
                else {
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) && SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    // Insert type converting op
                    SPerl_OP_insert_op_convert(sperl, op_cur);
                  }
                  else if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "== right value must be reference at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  else if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, "== left value must be reference at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
              }
              
              break;
            }
            case SPerl_OP_C_CODE_NE: {
              if (!op_cur->condition) {
                SPerl_yyerror_format(sperl, "== operator can use only condition context at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }

              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);

              if (!first_resolved_type && !last_resolved_type) {
                // Convert to constant true
                SPerl_OP_convert_to_op_constant_false(sperl, op_cur);
                op_cur->first = NULL;
                op_cur->last = NULL;
              }
              else {
                // undef
                if (!first_resolved_type && last_resolved_type) {
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, "!= right value must be reference at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }                
                }
                else if (first_resolved_type && !last_resolved_type) {
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "!= left value must be reference at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
                else {
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) && SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    // Insert type converting op
                    SPerl_OP_insert_op_convert(sperl, op_cur);
                  }
                  else if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "!= right value must be reference at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  else if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, "!= left value must be reference at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
              }
              
              break;
            }
            case SPerl_OP_C_CODE_LT: {
              if (!op_cur->condition) {
                SPerl_yyerror_format(sperl, "< operator can use only condition context at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }

              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                SPerl_yyerror_format(sperl, "< left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                SPerl_yyerror_format(sperl, "< right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_LE: {
              if (!op_cur->condition) {
                SPerl_yyerror_format(sperl, "<= operator can use only condition context at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }

              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                SPerl_yyerror_format(sperl, "<= left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                SPerl_yyerror_format(sperl, "<= right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_GT: {
              if (!op_cur->condition) {
                SPerl_yyerror_format(sperl, "> operator can use only condition context at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }

              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                SPerl_yyerror_format(sperl, "> left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                SPerl_yyerror_format(sperl, "> right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_GE: {
              if (!op_cur->condition) {
                SPerl_yyerror_format(sperl, ">= operator can use only condition context at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }

              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) && !SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                SPerl_yyerror_format(sperl, ">= left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) && SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                SPerl_yyerror_format(sperl, ">= right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_LEFT_SHIFT: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_integral(sperl, first_resolved_type)) {
                SPerl_yyerror_format(sperl, "<< operator left value must be integral at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              if (last_resolved_type->id > SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_yyerror_format(sperl, "<< operator right value must be int at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_RIGHT_SHIFT: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_integral(sperl, first_resolved_type)) {
                SPerl_yyerror_format(sperl, ">> operator left value must be integral at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              if (last_resolved_type->id > SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_yyerror_format(sperl, ">> operator right value must be int at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_integral(sperl, first_resolved_type)) {
                SPerl_yyerror_format(sperl, ">>> operator left value must be integral at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              if (last_resolved_type->id > SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_yyerror_format(sperl, ">>> operator right value must be int at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_NEW_TYPE: {
              SPerl_OP* op_type = op_cur->first;
              SPerl_RESOLVED_TYPE* resolved_type = op_type->uv.type->resolved_type;
              
              if (SPerl_RESOLVED_TYPE_contain_sub(sperl, resolved_type) && !SPerl_RESOLVED_TYPE_is_array(sperl, resolved_type)) {
                SPerl_yyerror_format(sperl,
                  "new operator can't receive sub type %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                SPerl_yyerror_format(sperl,
                  "new operator can't receive core type %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_BIT_XOR: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (first_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT || last_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                SPerl_yyerror_format(sperl,
                  "& operator can receive only boolean, char, char, short, int, long type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_BIT_OR: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (first_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT || last_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                SPerl_yyerror_format(sperl,
                  "& operator can receive only boolean, char, char, short, int, long type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_BIT_AND: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (first_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT || last_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                SPerl_yyerror_format(sperl,
                  "& operator can receive only boolean, char, char, short, int, long type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_ARRAY_LENGTH: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              
              // First value must be array
              _Bool first_resolved_type_is_array = SPerl_RESOLVED_TYPE_is_array(sperl, first_resolved_type);
              if (!first_resolved_type_is_array) {
                SPerl_yyerror_format(sperl, "right of @ must be array at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_ARRAY_ELEM: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // First value must be array
              _Bool first_resolved_type_is_array = SPerl_RESOLVED_TYPE_is_array(sperl, first_resolved_type);
              if (!first_resolved_type_is_array) {
                SPerl_yyerror_format(sperl, "left value must be array at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              // Last value must be integer
              if (last_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_yyerror_format(sperl, "array index must be integer at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_ASSIGN: {
              // Type assumption
              if (op_cur->first->first && op_cur->first->first->code == SPerl_OP_C_CODE_DECL_MY_VAR) {
                SPerl_OP* op_my_var = op_cur->first->first;
                if (op_my_var->uv.my_var->op_type->code == SPerl_OP_C_CODE_NULL) {
                  SPerl_OP* op_type = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_TYPE, op_my_var->file, op_my_var->line);
                  SPerl_TYPE* type = SPerl_TYPE_new(sperl);
                  type->resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  op_type->uv.type = type;
                  
                  op_my_var->uv.my_var->op_type = op_type;
                }
              }
              
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              if (!first_resolved_type) {
                SPerl_yyerror_format(sperl, "Type can't be detected at %s line %d\n", op_cur->first->file, op_cur->first->line);
                parser->fatal_error = 1;
                return;
              }
              
              // Convert type
              if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) && SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                SPerl_OP_insert_op_convert(sperl, op_cur);
              }
              
              first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              if (first_resolved_type->id != last_resolved_type->id) {
                SPerl_yyerror_format(sperl, "Invalid type at %s line %d\n", op_cur->file, op_cur->line);
                parser->fatal_error = 1;
                return;
              }
              
              // Insert var op
              if (op_cur->last->code == SPerl_OP_C_CODE_ASSIGN) {
                SPerl_OP* op_var = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_VAR, op_cur->file, op_cur->line);
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
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* op_return_resolved_type = sub->op_return_type->uv.type->resolved_type;
                
                if (op_return_resolved_type) {
                  // Can receive only core type
                  if (first_resolved_type->id != op_return_resolved_type->id) {
                    SPerl_yyerror_format(sperl, "Invalid return type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
                // void
                else {
                  if (first_resolved_type) {
                    SPerl_yyerror_format(sperl, "Invalid return type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
              }
              
              break;
            }
            case SPerl_OP_C_CODE_NEGATE: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                SPerl_yyerror_format(sperl, "- operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              break;              
            }
            case SPerl_OP_C_CODE_PLUS: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                SPerl_yyerror_format(sperl, "+ operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_ADD: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) || !SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                SPerl_yyerror_format(sperl, "+ operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_SUBTRACT: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) || !SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                SPerl_yyerror_format(sperl, "- operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_MULTIPLY: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) || !SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                SPerl_yyerror_format(sperl, "* operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_DIVIDE: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) || !SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                SPerl_yyerror_format(sperl, "/ operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_REMAINDER: {
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) || !SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                SPerl_yyerror_format(sperl, "% operator can receive only core type at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              // Insert type converting op
              SPerl_OP_insert_op_convert(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_PRE_INC:
            case SPerl_OP_C_CODE_POST_INC:
            case SPerl_OP_C_CODE_PRE_DEC:
            case SPerl_OP_C_CODE_POST_DEC: {
              SPerl_OP* first = op_cur->first;
              if (first->code != SPerl_OP_C_CODE_VAR) {
                SPerl_yyerror_format(sperl, "invalid lvalue in increment at %s line %d\n", op_cur->file, op_cur->line);
                break;
              }
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, first);
              
              // Only int or long
              if (first_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_INT) {
                SPerl_yyerror_format(sperl, "must be int in increment at %s line %d\n", op_cur->file, op_cur->line);
              }

              break;
            }
            case SPerl_OP_C_CODE_CONSTANT: {

              SPerl_ARRAY_push(parser->op_constants, op_cur);
              break;
            }
            // End of scope
            case SPerl_OP_C_CODE_BLOCK: {
              int32_t* block_base_ptr = SPerl_ARRAY_pop(block_base_stack);
              if (block_base_ptr) {
                int32_t block_base = *block_base_ptr;
                for (int32_t j = 0; j < op_my_var_stack->length - block_base; j++) {
                  SPerl_ARRAY_pop(op_my_var_stack);
                }
              }
              int32_t* before_block_base_ptr = SPerl_ARRAY_fetch(block_base_stack, block_base_stack->length - 1);
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
              
              // First child is my_var, but my_var don't have type and don't sibling to detect type
              if (op_cur->first && op_cur->first->code == SPerl_OP_C_CODE_DECL_MY_VAR) {
                SPerl_OP* op_my_var = op_cur->first;
                if (op_my_var->uv.my_var->op_type->code == SPerl_OP_C_CODE_NULL && !op_cur->moresib) {
                  // Error
                  SPerl_yyerror_format(sperl, "\"my %s\" can't detect type at %s line %d\n", var->op_name->uv.name, op_cur->file, op_cur->line);
                  parser->fatal_error = 1;
                  return;
                }
              }
              
              // Serach same name variable
              SPerl_OP* op_my_var = NULL;
              for (int32_t i = op_my_var_stack->length - 1 ; i >= 0; i--) {
                SPerl_OP* op_my_var_tmp = SPerl_ARRAY_fetch(op_my_var_stack, i);
                SPerl_MY_VAR* my_var_tmp = op_my_var_tmp->uv.my_var;
                if (strcmp(var->op_name->uv.name, my_var_tmp->op_name->uv.name) == 0) {
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
                SPerl_yyerror_format(sperl, "\"my %s\" undeclared at %s line %d\n", var->op_name->uv.name, op_cur->file, op_cur->line);
                parser->fatal_error = 1;
                return;
              }
              break;
            }
            case SPerl_OP_C_CODE_DECL_MY_VAR: {
              SPerl_MY_VAR* my_var = op_cur->uv.my_var;
              
              // Serach same name variable
              int32_t found = 0;
              
              for (int32_t i = op_my_var_stack->length - 1 ; i >= block_base; i--) {
                SPerl_OP* op_bef_my_var = SPerl_ARRAY_fetch(op_my_var_stack, i);
                SPerl_MY_VAR* bef_my_var = op_bef_my_var->uv.my_var;
                if (strcmp(my_var->op_name->uv.name, bef_my_var->op_name->uv.name) == 0) {
                  found = 1;
                  break;
                }
              }
              
              if (found) {
                SPerl_yyerror_format(sperl, "redeclaration of my \"%s\" at %s line %d\n", my_var->op_name->uv.name, op_cur->file, op_cur->line);
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
              SPerl_OP_resolve_sub_name(sperl, sub->op_package, op_cur);
              
              const char* sub_abs_name = op_cur->uv.name_info->resolved_name;
              
              SPerl_SUB* found_sub= SPerl_HASH_search(
                parser->sub_abs_name_symtable,
                sub_abs_name,
                strlen(sub_abs_name)
              );
              if (!found_sub) {
                SPerl_yyerror_format(sperl, "unknown sub \"%s\" at %s line %d\n",
                  sub_abs_name, op_cur->file, op_cur->line);
              }

              // Add call stack max size
              int32_t argument_total_size = 0;
              for (int32_t i = 0; i < found_sub->op_sub_args->length; i++) {
                SPerl_ARRAY* op_sub_args = found_sub->op_sub_args;
                SPerl_OP* op_sub_arg = SPerl_ARRAY_fetch(op_sub_args, i);
                SPerl_MY_VAR* my_var = op_sub_arg->uv.my_var;
                SPerl_RESOLVED_TYPE* resolved_type = my_var->op_type->uv.type->resolved_type;
                if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  argument_total_size += 2;
                }
                else {
                  argument_total_size++;
                }
              }
              if (argument_total_size_max > argument_total_size) {
                argument_total_size_max = argument_total_size;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_FIELD: {
              // Check field name
              SPerl_OP_resolve_field_name(sperl, op_cur);
              
              const char* field_abs_name = op_cur->uv.name_info->resolved_name;
              
              SPerl_FIELD* found_field= SPerl_HASH_search(
                parser->field_abs_name_symtable,
                field_abs_name,
                strlen(field_abs_name)
              );
              if (!found_field) {
                SPerl_yyerror_format(sperl, "unknown field \"%s\" at %s line %d\n",
                  field_abs_name, op_cur->file, op_cur->line);
                parser->fatal_error = 1;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_CONVERT: {
              
              SPerl_OP* op_term = op_cur->first;
              SPerl_OP* op_type = op_cur->last;
              
              SPerl_RESOLVED_TYPE* op_term_resolved_type = SPerl_OP_get_resolved_type(sperl, op_term);
              SPerl_RESOLVED_TYPE* op_type_resolved_type = SPerl_OP_get_resolved_type(sperl, op_type);;
              
              // Can receive only core type
              if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, op_term_resolved_type) || !SPerl_RESOLVED_TYPE_is_core_type(sperl, op_type_resolved_type)) {
                SPerl_yyerror_format(sperl, "can't convert type %s to %s at %s line %d\n",
                  op_term_resolved_type->name, op_type_resolved_type->name, op_cur->file, op_cur->line);
              }
            }
            break;
          }
          
          // [END]Postorder traversal position
          
          if (op_cur == op_base) {
            
            SPerl_OP* op_block = op_cur->last;
            SPerl_OP* op_statements = op_block->first;
            
            if (op_statements->last->code != SPerl_OP_C_CODE_RETURN) {
              // Add return to the end of subroutine
              SPerl_RESOLVED_TYPE* op_return_resolved_type = sub->op_return_type->uv.type->resolved_type;
              SPerl_OP* op_return = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_RETURN, op_cur->file, op_cur->line);
              if (op_return_resolved_type) {
                if (op_return_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                  SPerl_CONSTANT* constant = SPerl_CONSTANT_new(sperl);
                  if (op_return_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                    constant->code = SPerl_CONSTANT_C_CODE_INT;
                    constant->uv.int_value = 0;
                  }
                  else if (op_return_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    constant->code = SPerl_CONSTANT_C_CODE_LONG;
                    constant->uv.long_value = 0;
                  }
                  else if (op_return_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    constant->code = SPerl_CONSTANT_C_CODE_FLOAT;
                    constant->uv.float_value = 0;
                  }
                  else if (op_return_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                    constant->code = SPerl_CONSTANT_C_CODE_DOUBLE;
                    constant->uv.double_value = 0;
                  }
                  SPerl_OP* op_constant = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_CONSTANT, op_cur->file, op_cur->line);
                  op_constant->uv.constant = constant;
                  
                  SPerl_OP_sibling_splice(sperl, op_return, NULL, 0, op_constant);
                }
                // Reference
                else {
                  // Undef
                  SPerl_OP* op_undef = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_UNDEF, op_cur->file, op_cur->line);
                  SPerl_OP_sibling_splice(sperl, op_return, NULL, 0, op_undef);
                }
              }
              
              SPerl_OP_sibling_splice(sperl, op_statements, op_statements->last, 0, op_return);
            }
            
            // Finish
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
    // Set my var information
    sub->op_my_vars = op_my_vars;
    
    // Operand stack max
    sub->operand_stack_max = operand_stack_max;
    
    // Calculate call_stack_max
    int32_t argument_count = sub->op_sub_args->length;
    for (int32_t i = argument_count; i < op_my_vars->length; i++) {
      SPerl_OP* op_my_var = SPerl_ARRAY_fetch(op_my_vars, i);
      SPerl_MY_VAR* my_var = op_my_var->uv.my_var;
      SPerl_RESOLVED_TYPE* resolved_type = my_var->op_type->uv.type->resolved_type;
      if (resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG || resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
        call_stack_max += 2;
      }
      else {
        call_stack_max++;
      }
    }
    call_stack_max += argument_total_size_max;
    sub->call_stack_max = call_stack_max;
  }
}
