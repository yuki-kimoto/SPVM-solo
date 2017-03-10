#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_allocator_parser.h"
#include "sperl_yacc_util.h"
#include "sperl_op.h"
#include "sperl_sub.h"
#include "sperl_constant.h"
#include "sperl_field.h"
#include "sperl_my_var.h"
#include "sperl_var.h"
#include "sperl_memory_pool.h"
#include "sperl_enumeration_value.h"
#include "sperl_type.h"
#include "sperl_type_component_name.h"
#include "sperl_type_component_array.h"
#include "sperl_type_part.h"
#include "sperl_enumeration.h"
#include "sperl_package.h"
#include "sperl_name_info.h"
#include "sperl_resolved_type.h"
#include "sperl_switch_info.h"
#include "sperl_constant_pool.h"

void SPerl_OP_CHECKER_check(SPerl* sperl) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  for (size_t package_pos = 0; package_pos < parser->op_packages->length; package_pos++) {
    SPerl_OP* op_package = SPerl_ARRAY_fetch(sperl, parser->op_packages, package_pos);
    SPerl_PACKAGE* package = op_package->uv.package;
    
    if (strchr(package->op_name->uv.name, '_') != NULL) {
      SPerl_yyerror_format(sperl, "Package name can't contain _ at %s line %d\n", op_package->file, op_package->line);
      parser->fatal_error = 1;
      return;
    }
    
    // Push package information to constant pool
    package->constant_pool_address = sperl->constant_pool->length;
    SPerl_CONSTANT_POOL_push_package(sperl, sperl->constant_pool, package);
    
    // Push field information to constant pool
    for (size_t field_pos = 0; field_pos < package->op_fields->length; field_pos++) {
      SPerl_OP* op_field = SPerl_ARRAY_fetch(sperl, package->op_fields, field_pos);
      SPerl_FIELD* field = op_field->uv.field;
      field->constant_pool_address = sperl->constant_pool->length;
      SPerl_CONSTANT_POOL_push_field(sperl, sperl->constant_pool, field);
    }
    
    for (size_t sub_pos = 0; sub_pos < package->op_subs->length; sub_pos++) {
      
      SPerl_OP* op_sub = SPerl_ARRAY_fetch(sperl, package->op_subs, sub_pos);
      SPerl_SUB* sub = op_sub->uv.sub;
      
      if (sub->is_constant) {
        continue;
      }
      
      // my var informations
      SPerl_ARRAY* op_my_vars = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser, 0);
      
      // my variable stack
      SPerl_ARRAY* op_my_var_stack = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser, 0);
      
      // block base position stack
      SPerl_ARRAY* block_base_stack = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser, 0);
      int32_t block_base = 0;
      _Bool block_start = 0;
      
      // In switch statement
      _Bool in_switch = 0;
      
      // Current case statements
      SPerl_ARRAY* cur_case_ops = NULL;
      
      // Current default statement
      SPerl_OP* cur_default_op = NULL;
      
      // op count
      int32_t op_count = 0;
      
      int32_t next_my_var_address = 0;
      
      // Run OPs
      SPerl_OP* op_base = op_sub;
      SPerl_OP* op_cur = op_base;
      _Bool finish = 0;
      while (op_cur) {
        
        op_count++;
        
        // [START]Preorder traversal position
        
        switch (op_cur->code) {

          case SPerl_OP_C_CODE_DECL_FIELD: {
            SPerl_FIELD* field = op_cur->uv.field;
            SPerl_RESOLVED_TYPE* resolved_type = field->op_type->uv.type->resolved_type;
            if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, resolved_type) && !SPerl_RESOLVED_TYPE_is_core_type_array(sperl, resolved_type)) {
              SPerl_yyerror_format(sperl, "filed type must be core type or core type array at %s line %d\n", op_cur->file, op_cur->line);
              parser->fatal_error = 1;
              return;
            }
            
            break;
          }
          case SPerl_OP_C_CODE_AND: {
            
            // Convert && to if statement
            SPerl_OP_convert_and_to_if(sperl, op_cur);
            
            break;
          }
          case SPerl_OP_C_CODE_OR: {
            
            // Convert || to if statement
            SPerl_OP_convert_or_to_if(sperl, op_cur);
            
            break;
          }
          case SPerl_OP_C_CODE_NOT: {
            // Convert ! to if statement
            SPerl_OP_convert_not_to_if(sperl, op_cur);
            
            break;
          }
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
              assert(op_my_var_stack->length <= SPerl_OP_LIMIT_LEXICAL_VARIABLES);
              int32_t* block_base_ptr = SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, parser, sizeof(int32_t));
              *block_base_ptr = (int32_t) op_my_var_stack->length;
              SPerl_ARRAY_push(sperl, block_base_stack, block_base_ptr);
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
              case SPerl_OP_C_CODE_CONSTANT: {
                SPerl_CONSTANT* constant = op_cur->uv.constant;
                
                SPerl_CONSTANT_POOL* constant_pool = sperl->constant_pool;
                
                constant->constant_pool_address = constant_pool->length;
                
                switch (constant->code) {
                  case SPerl_CONSTANT_C_CODE_INT:
                    SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, constant);
                    break;
                  case SPerl_CONSTANT_C_CODE_LONG:
                    SPerl_CONSTANT_POOL_push_long(sperl, constant_pool, constant);
                    break;
                  case SPerl_CONSTANT_C_CODE_FLOAT:
                    SPerl_CONSTANT_POOL_push_float(sperl, constant_pool, constant);
                    break;
                  case SPerl_CONSTANT_C_CODE_DOUBLE:
                    SPerl_CONSTANT_POOL_push_double(sperl, constant_pool, constant);
                    break;
                  case SPerl_CONSTANT_C_CODE_STRING:
                    SPerl_CONSTANT_POOL_push_string(sperl, constant_pool, constant);
                    break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_POP: {
                if (op_cur->first->code == SPerl_OP_C_CODE_CALL_SUB) {
                  SPerl_OP* op_call_sub = op_cur->first;
                  
                  const char* sub_name = op_call_sub->uv.name_info->resolved_name;
                  
                  SPerl_OP* op_sub= SPerl_HASH_search(sperl, 
                    parser->op_sub_symtable,
                    sub_name,
                    strlen(sub_name)
                  );
                  SPerl_SUB* sub = op_sub->uv.sub;
                  
                  if (sub->op_return_type->code == SPerl_OP_C_CODE_VOID) {
                    op_cur->code = SPerl_OP_C_CODE_NULL;
                  }
                }
                break;
              }
              case SPerl_OP_C_CODE_DEFAULT: {
                if (cur_default_op) {
                  SPerl_yyerror_format(sperl, "multiple default is forbidden at %s line %d\n", op_cur->file, op_cur->line);
                  parser->fatal_error = 1;
                  break;
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
                if (op_term_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_yyerror_format(sperl, "case need long at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                if (!cur_case_ops) {
                  cur_case_ops = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser, 0);
                }
                SPerl_ARRAY_push(sperl, cur_case_ops, op_cur);
                
                break;
              }
              case SPerl_OP_C_CODE_SWITCH: {
                
                SPerl_OP* op_switch_condition = op_cur->first;
                
                SPerl_RESOLVED_TYPE* term_resolved_type = SPerl_OP_get_resolved_type(sperl, op_switch_condition->first);
                
                if (term_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_yyerror_format(sperl, "switch need long at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                in_switch = 0;
                cur_default_op = NULL;
                cur_case_ops = NULL;
                
                // tableswitch if the following. SWITCHRTIO is 1.5 by default
                // 4 + range <= (3 + 2 * length) * SWITCHRTIO
                
                SPerl_SWITCH_INFO* switch_info = op_cur->uv.switch_info;
                SPerl_ARRAY* op_cases = switch_info->op_cases;
                size_t const length = op_cases->length;
                if (length > SPerl_OP_LIMIT_CASES) {
                  SPerl_yyerror_format(sperl, "too many case statements in switch at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }

                int32_t min = SPerl_BASE_C_INT_MAX;
                int32_t max = SPerl_BASE_C_INT_MIN;
                
                for (size_t i = 0; i < length; i++) {
                  SPerl_OP* op_case = SPerl_ARRAY_fetch(sperl, op_cases, i);
                  SPerl_OP* op_constant = op_case->first;
                  int32_t value = op_constant->uv.constant->uv.int_value;
                  if (value < min) {
                    min = value;
                  }
                  if (value > max) {
                    max = value;
                  }
                }
                
                double range = (double) max - (double) min;
                
                int32_t code;
                if (4.0 + range <= (3.0 + 2.0 * (double) length) * 1.5) {
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
              
              case SPerl_OP_C_CODE_EQ: {
                
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                // TERM == TERM
                if (first_resolved_type && last_resolved_type) {
                  // core == core
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) && SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    if (first_resolved_type->id != last_resolved_type->id) {
                      SPerl_yyerror_format(sperl, "== operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                  }
                  // core == OBJ
                  else if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "== left value must be object at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  // OBJ == core
                  else if (SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, "== right value must be object at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
                // undef == TERM
                else if (!first_resolved_type) {
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, "== right value must be object at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
                // TERM == undef
                else if (!last_resolved_type) {
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "== left value must be object at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
                
                break;
              }
              case SPerl_OP_C_CODE_NE: {

                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);

                // TERM == TERM
                if (first_resolved_type && last_resolved_type) {
                  // core == core
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) && SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    if (first_resolved_type->id != last_resolved_type->id) {
                      SPerl_yyerror_format(sperl, "!= operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                  }
                  // core == OBJ
                  else if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "!= left value must be object at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  // OBJ == core
                  else if (SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, "!= right value must be object at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
                // undef == TERM
                else if (!first_resolved_type) {
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, "!= right value must be object at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
                // TERM == undef
                else if (!last_resolved_type) {
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "!= left value must be object at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
                
                break;
              }
              case SPerl_OP_C_CODE_LT: {

                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                // undef check
                if (!first_resolved_type) {
                  SPerl_yyerror_format(sperl, "< left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                if (!last_resolved_type) {
                  SPerl_yyerror_format(sperl, "< right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Can receive only core type
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                  SPerl_yyerror_format(sperl, "< left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                  SPerl_yyerror_format(sperl, "< right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }

                if (first_resolved_type->id != last_resolved_type->id) {
                  SPerl_yyerror_format(sperl, "< operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }

                break;
              }
              case SPerl_OP_C_CODE_LE: {

                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);

                // undef check
                if (!first_resolved_type) {
                  SPerl_yyerror_format(sperl, "<= left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                if (!last_resolved_type) {
                  SPerl_yyerror_format(sperl, "<= right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                                
                // Can receive only core type
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                  SPerl_yyerror_format(sperl, "<= left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                  SPerl_yyerror_format(sperl, "<= right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }

                if (first_resolved_type->id != last_resolved_type->id) {
                  SPerl_yyerror_format(sperl, "<= operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_GT: {

                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);

                // undef check
                if (!first_resolved_type) {
                  SPerl_yyerror_format(sperl, "> left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                if (!last_resolved_type) {
                  SPerl_yyerror_format(sperl, "> right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Can receive only core type
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                  SPerl_yyerror_format(sperl, "> left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                  SPerl_yyerror_format(sperl, "> right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }

                if (first_resolved_type->id != last_resolved_type->id) {
                  SPerl_yyerror_format(sperl, "> operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_GE: {

                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);

                // undef check
                if (!first_resolved_type) {
                  SPerl_yyerror_format(sperl, "<= left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                if (!last_resolved_type) {
                  SPerl_yyerror_format(sperl, "<= right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Can receive only core type
                if (SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) && !SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                  SPerl_yyerror_format(sperl, ">= left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type) && SPerl_RESOLVED_TYPE_is_core_type(sperl, last_resolved_type)) {
                  SPerl_yyerror_format(sperl, ">= right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }

                if (first_resolved_type->id != last_resolved_type->id) {
                  SPerl_yyerror_format(sperl, ">= operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
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
                if (last_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_yyerror_format(sperl, "<< operator right value must be long at %s line %d\n", op_cur->file, op_cur->line);
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
                if (last_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_yyerror_format(sperl, ">> operator right value must be long at %s line %d\n", op_cur->file, op_cur->line);
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
                if (last_resolved_type->id > SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_yyerror_format(sperl, ">>> operator right value must be long at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_MALLOC: {
                SPerl_OP* op_type = op_cur->first;
                SPerl_RESOLVED_TYPE* resolved_type = op_type->uv.type->resolved_type;
                
                if (SPerl_RESOLVED_TYPE_is_core_type_array(sperl, resolved_type)) {
                  SPerl_OP* op_index_term = op_type->last;
                  SPerl_RESOLVED_TYPE* index_resolved_type = SPerl_OP_get_resolved_type(sperl, op_index_term);
                  
                  if (!index_resolved_type) {
                    SPerl_yyerror_format(sperl, "new operator can't create array which don't have length \"%s\" at %s line %d\n", resolved_type->name, op_cur->file, op_cur->line);
                    break;
                  }
                  else if (index_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    // OK
                  }
                  else {
                    SPerl_yyerror_format(sperl, "new operator can't create array which don't have long length \"%s\" at %s line %d\n", resolved_type->name, op_cur->file, op_cur->line);
                    break;
                  }
                }
                else if (SPerl_RESOLVED_TYPE_is_array(sperl, resolved_type)) {
                  SPerl_yyerror_format(sperl,
                    "new operator can't receive object array at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                else {
                  if (SPerl_RESOLVED_TYPE_contain_sub(sperl, resolved_type)) {
                    SPerl_yyerror_format(sperl,
                      "new operator can't receive sub type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  if (SPerl_RESOLVED_TYPE_is_core_type(sperl, resolved_type)) {
                    SPerl_yyerror_format(sperl,
                      "new operator can't receive core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  SPerl_OP* op_package = SPerl_HASH_search(sperl, parser->op_package_symtable, resolved_type->name, strlen(resolved_type->name));
                  
                  if (!op_package) {
                    SPerl_yyerror_format(sperl, "new operator can't receive non package name \"%s\" at %s line %d\n", resolved_type->name, op_cur->file, op_cur->line);
                    break;
                  }
                  else {
                    SPerl_PACKAGE* package = op_package->uv.package;
                    if (!package->op_fields->length) {
                      SPerl_yyerror_format(sperl, "new operator can't receive package which don't have fields \"%s\" at %s line %d\n", resolved_type->name, op_cur->file, op_cur->line);
                      break;
                    }
                  }
                }
                
                break;
              }
              case SPerl_OP_C_CODE_BIT_XOR: {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                // Can receive only core type
                if (first_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT || last_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_yyerror_format(sperl,
                    "& operator can receive only integral type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_BIT_OR: {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                // Can receive only core type
                if (first_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT || last_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_yyerror_format(sperl,
                    "& operator can receive only integral type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_BIT_AND: {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                // Can receive only core type
                if (first_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT || last_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                  SPerl_yyerror_format(sperl,
                    "& operator can receive only integral type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
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
                if (last_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_yyerror_format(sperl, "array index must be long at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_ASSIGN: {
                
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                // Type assumption
                if (!first_resolved_type) {
                  SPerl_OP* op_var = op_cur->first;
                  SPerl_MY_VAR* my_var = op_var->uv.var->op_my_var->uv.my_var;
                  SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, my_var->op_term_assumption);
                  
                  if (resolved_type) {
                    SPerl_OP* op_type = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_TYPE, op_cur->file, op_cur->line);
                    SPerl_TYPE* type = SPerl_TYPE_new(sperl);
                    type->resolved_type = resolved_type;
                    op_type->uv.type = type;
                    my_var->op_type = op_type;
                  }
                  else {
                    SPerl_yyerror_format(sperl, "Type can't be detected at %s line %d\n", op_cur->first->file, op_cur->first->line);
                    parser->fatal_error = 1;
                    return;
                  }
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
                  SPerl_RESOLVED_TYPE* sub_return_resolved_type = SPerl_OP_get_resolved_type(sperl, sub->op_return_type);
                  
                  _Bool is_invalid = 0;
                  
                  // Undef
                  if (op_cur->first->code == SPerl_OP_C_CODE_UNDEF) {
                    if (sub->op_return_type->code == SPerl_OP_C_CODE_VOID) {
                      is_invalid = 1;
                    }
                    else {
                      if (SPerl_RESOLVED_TYPE_is_core_type(sperl, sub_return_resolved_type)) {
                        is_invalid = 1;
                      }
                    }
                  }
                  // Normal
                  else if (op_cur->first) {
                    if (first_resolved_type->id != sub_return_resolved_type->id) {
                      is_invalid = 1;
                    }
                  }
                  // Empty
                  else {
                    if (sub->op_return_type->code != SPerl_OP_C_CODE_VOID) {
                      is_invalid = 1;
                    }
                  }
                  
                  if (is_invalid) {
                    SPerl_yyerror_format(sperl, "Invalid return type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                }
                
                break;
              }
              case SPerl_OP_C_CODE_NEGATE: {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                
                // Must be int, long, float, double
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                  SPerl_yyerror_format(sperl, "Type of - operator right value must be int, long, float, double at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_PLUS: {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                
                // Must be int, long, float, double
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                  SPerl_yyerror_format(sperl, "Type of + operator right value must be int, long, float, double at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_ADD: {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                // Left value must not be undef
                if (!first_resolved_type) {
                  SPerl_yyerror_format(sperl, "+ operator left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Right value Must not be undef
                if (!last_resolved_type) {
                  SPerl_yyerror_format(sperl, "+ operator right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Must be same type
                if (first_resolved_type->id != last_resolved_type->id) {
                  SPerl_yyerror_format(sperl, "Type of + operator left and right value must be same at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                                                
                // Value must be int, long, float, double
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                  SPerl_yyerror_format(sperl, "Type of + operator left and right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_SUBTRACT: {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                // Left value must not be undef
                if (!first_resolved_type) {
                  SPerl_yyerror_format(sperl, "- operator left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Right value Must not be undef
                if (!last_resolved_type) {
                  SPerl_yyerror_format(sperl, "- operator right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Must be same type
                if (first_resolved_type->id != last_resolved_type->id) {
                  SPerl_yyerror_format(sperl, "Type of - operator left and right value must be same at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                                                
                // Value must be int, long, float, double
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                  SPerl_yyerror_format(sperl, "Type of - operator left and right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_MULTIPLY: {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                // Left value must not be undef
                if (!first_resolved_type) {
                  SPerl_yyerror_format(sperl, "* operator left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Right value Must not be undef
                if (!last_resolved_type) {
                  SPerl_yyerror_format(sperl, "* operator right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Must be same type
                if (first_resolved_type->id != last_resolved_type->id) {
                  SPerl_yyerror_format(sperl, "Type of * operator left and right value must be same at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                                                
                // Value must be int, long, float, double
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                  SPerl_yyerror_format(sperl, "Type of * operator left and right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_DIVIDE: {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                // Left value must not be undef
                if (!first_resolved_type) {
                  SPerl_yyerror_format(sperl, "/ operator left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Right value Must not be undef
                if (!last_resolved_type) {
                  SPerl_yyerror_format(sperl, "/ operator right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Must be same type
                if (first_resolved_type->id != last_resolved_type->id) {
                  SPerl_yyerror_format(sperl, "Type of / operator left and right value must be same at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                                                
                // Value must be int, long, float, double
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                  SPerl_yyerror_format(sperl, "Type of / operator left and right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_REMAINDER: {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                
                // Left value must not be undef
                if (!first_resolved_type) {
                  SPerl_yyerror_format(sperl, "% operator left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Right value Must not be undef
                if (!last_resolved_type) {
                  SPerl_yyerror_format(sperl, "% operator right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                // Must be same type
                if (first_resolved_type->id != last_resolved_type->id) {
                  SPerl_yyerror_format(sperl, "Type of % operator left and right value must be same at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                                                
                // Value must be int, long, float, double
                if (!SPerl_RESOLVED_TYPE_is_core_type(sperl, first_resolved_type)) {
                  SPerl_yyerror_format(sperl, "Type of % operator left and right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_DIE: {
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                
                if (!first_resolved_type || strcmp(first_resolved_type->name, "byte[]") != 0) {
                  SPerl_yyerror_format(sperl, "die can only receive byte array at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                break;
              }
              case SPerl_OP_C_CODE_PRE_INC:
              case SPerl_OP_C_CODE_POST_INC:
              case SPerl_OP_C_CODE_PRE_DEC:
              case SPerl_OP_C_CODE_POST_DEC: {
                SPerl_OP* op_first = op_cur->first;
                if (op_first->code != SPerl_OP_C_CODE_VAR) {
                  SPerl_yyerror_format(sperl, "invalid lvalue in increment at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_first);
                
                // Only int or long
                if (first_resolved_type->id > SPerl_RESOLVED_TYPE_C_ID_LONG) {
                  SPerl_yyerror_format(sperl, "Type of increment or decrement target must be integral at %s line %d\n", op_cur->file, op_cur->line);
                  break;
                }
                
                op_cur->first->lvalue = 1;
                
                break;
              }
              // End of scope
              case SPerl_OP_C_CODE_BLOCK: {
                assert(op_my_var_stack->length <= SPerl_OP_LIMIT_LEXICAL_VARIABLES);

                int32_t* block_base_ptr = SPerl_ARRAY_pop(sperl, block_base_stack);
                if (block_base_ptr) {
                  int32_t const pop_count = (int32_t) op_my_var_stack->length - *block_base_ptr;
                  for (int32_t j = 0; j < pop_count; j++) {
                    SPerl_ARRAY_pop(sperl, op_my_var_stack);
                  }
                }
                int32_t* before_block_base_ptr = SPerl_ARRAY_fetch(sperl, block_base_stack, block_base_stack->length - 1);
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
                
                // Search same name variable
                SPerl_OP* op_my_var = NULL;
                for (size_t i = op_my_var_stack->length; i-- > 0; ) {
                  SPerl_OP* op_my_var_tmp = SPerl_ARRAY_fetch(sperl, op_my_var_stack, i);
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
                
                // Search same name variable
                int32_t found = 0;

                assert(op_my_var_stack->length <= SPerl_OP_LIMIT_LEXICAL_VARIABLES);
                for (int32_t i = (int32_t) op_my_var_stack->length; i-- > block_base; ) {
                  SPerl_OP* op_bef_my_var = SPerl_ARRAY_fetch(sperl, op_my_var_stack, i);
                  SPerl_MY_VAR* bef_my_var = op_bef_my_var->uv.my_var;
                  if (strcmp(my_var->op_name->uv.name, bef_my_var->op_name->uv.name) == 0) {
                    found = 1;
                    break;
                  }
                }
                
                if (found) {
                  SPerl_yyerror_format(sperl, "redeclaration of my \"%s\" at %s line %d\n", my_var->op_name->uv.name, op_cur->file, op_cur->line);
                  break;
                }
                else if (op_my_var_stack->length > SPerl_OP_LIMIT_LEXICAL_VARIABLES) {
                  SPerl_yyerror_format(sperl, "too many lexical variables, my \"%s\" ignored at %s line %d\n", my_var->op_name->uv.name, op_cur->file, op_cur->line);
                  parser->fatal_error = 1;
                  break;
                }
                else {
                  my_var->address = next_my_var_address++;
                  SPerl_ARRAY_push(sperl, op_my_vars, op_cur);
                  SPerl_ARRAY_push(sperl, op_my_var_stack, op_cur);
                }
                break;
              }
              case SPerl_OP_C_CODE_CALL_SUB: {
                
                
                // Check sub name
                SPerl_OP_resolve_sub_name(sperl, op_package, op_cur);
                
                const char* sub_name = op_cur->uv.name_info->resolved_name;
                
                SPerl_OP* found_op_sub= SPerl_HASH_search(sperl, 
                  parser->op_sub_symtable,
                  sub_name,
                  strlen(sub_name)
                );
                if (!found_op_sub) {
                  SPerl_yyerror_format(sperl, "unknown sub \"%s\" at %s line %d\n",
                    sub_name, op_cur->file, op_cur->line);
                  break;
                }
                
                // Constant
                SPerl_SUB* found_sub = found_op_sub->uv.sub;

                size_t sub_args_count = found_sub->op_args->length;
                SPerl_OP* op_list_args = op_cur->last;
                SPerl_OP* op_term = op_list_args->first;
                size_t call_sub_args_count = 0;
                while ((op_term = SPerl_OP_sibling(sperl, op_term))) {
                  call_sub_args_count++;
                  if (call_sub_args_count > sub_args_count) {
                    SPerl_yyerror_format(sperl, "Too may arguments. sub \"%s\" at %s line %d\n", sub_name, op_cur->file, op_cur->line);
                    return;
                  }
                  
                  _Bool is_invalid = 0;
                  
                  SPerl_OP* op_sub_arg_my_var = SPerl_ARRAY_fetch(sperl, found_sub->op_args, call_sub_args_count - 1);
                  
                  SPerl_RESOLVED_TYPE* sub_arg_resolved_type = SPerl_OP_get_resolved_type(sperl, op_sub_arg_my_var);
                  
                  // Undef
                  if (op_term->code == SPerl_OP_C_CODE_UNDEF) {
                    if (SPerl_RESOLVED_TYPE_is_core_type(sperl, sub_arg_resolved_type)) {
                      is_invalid = 1;
                    }
                  }
                  // Normal
                  else if (op_term) {
                    SPerl_RESOLVED_TYPE* op_term_resolved_type = SPerl_OP_get_resolved_type(sperl, op_term);
                    
                    if (op_term_resolved_type->id !=  sub_arg_resolved_type->id) {
                      is_invalid = 1;
                    }
                  }
                  if (is_invalid) {
                    SPerl_yyerror_format(sperl, "Argument %d type is invalid. sub \"%s\" at %s line %d\n", (int) call_sub_args_count, sub_name, op_cur->file, op_cur->line);
                    return;
                  }
                }
                
                if (call_sub_args_count < sub_args_count) {
                  SPerl_yyerror_format(sperl, "Too few argument. sub \"%s\" at %s line %d\n", sub_name, op_cur->file, op_cur->line);
                  return;
                }
                
                // Constant subroutine
                if (found_sub->is_constant) {
                  // Replace sub to constant
                  op_cur->code = SPerl_OP_C_CODE_CONSTANT;
                  op_cur->uv.constant = found_sub->op_block->uv.constant;
                  
                  op_cur->first = NULL;
                  op_cur->last = NULL;
                }
                
                break;
              }
              case SPerl_OP_C_CODE_FIELD: {
                SPerl_OP* op_term = op_cur->first;
                SPerl_OP* op_name = op_cur->last;
                
                if (op_term->code != SPerl_OP_C_CODE_VAR
                  && op_term->code != SPerl_OP_C_CODE_ARRAY_ELEM
                  && op_term->code != SPerl_OP_C_CODE_FIELD
                  && op_term->code != SPerl_OP_C_CODE_CALL_SUB)
                {
                  SPerl_yyerror_format(sperl, "field invoker is invalid \"%s\" at %s line %d\n",
                    op_name->uv.name, op_cur->file, op_cur->line);
                  parser->fatal_error = 1;
                  break;
                }
                
                // Check field name
                SPerl_OP_resolve_field_name(sperl, op_cur);
                
                const char* field_abs_name = op_cur->uv.name_info->resolved_name;
                
                SPerl_OP* found_op_field= SPerl_HASH_search(sperl, 
                  parser->op_field_symtable,
                  field_abs_name,
                  strlen(field_abs_name)
                );
                if (!found_op_field) {
                  SPerl_yyerror_format(sperl, "unknown field \"%s\" at %s line %d\n",
                    field_abs_name, op_cur->file, op_cur->line);
                  parser->fatal_error = 1;
                  break;
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
                  break;
                }
              }
              break;
            }
            
            // [END]Postorder traversal position
            
            if (op_cur == op_base) {
              
              SPerl_OP* op_block = op_cur->last;
              SPerl_OP* op_statements = op_block->first;
              
              if (!sub->is_native && op_statements->last->code != SPerl_OP_C_CODE_RETURN) {
                // Add return to the end of subroutine
                SPerl_OP* op_return = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_RETURN, op_cur->file, op_cur->line);
                if (sub->op_return_type->code != SPerl_OP_C_CODE_VOID) {
                  SPerl_RESOLVED_TYPE* op_return_resolved_type = SPerl_OP_get_resolved_type(sperl, sub->op_return_type);
                  if (op_return_resolved_type) {
                    if (SPerl_RESOLVED_TYPE_is_core_type(sperl, op_return_resolved_type)) {
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
      sub->operand_stack_max = op_count * 2;
      
      // Push sub information to constant pool
      sub->constant_pool_address = sperl->constant_pool->length;
      SPerl_CONSTANT_POOL_push_sub(sperl, sperl->constant_pool, sub);
    }
  }
}
