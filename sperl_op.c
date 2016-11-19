#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"
#include "sperl_yacc.h"
#include "sperl_op.h"
#include "sperl_sub.h"
#include "sperl_parser.h"
#include "sperl_const_value.h"
#include "sperl_field.h"
#include "sperl_my_var.h"
#include "sperl_var.h"
#include "sperl_memory_pool.h"
#include "sperl_use.h"
#include "sperl_word.h"
#include "sperl_enum_value.h"
#include "sperl_descripter.h"
#include "sperl_type.h"
#include "sperl_type_word.h"
#include "sperl_type_array.h"
#include "sperl_type_sub.h"
#include "sperl_type_part.h"
#include "sperl_body.h"
#include "sperl_body_core.h"
#include "sperl_body_enum.h"
#include "sperl_body_class.h"
#include "sperl_package.h"
#include "sperl_name.h"
#include "sperl_opdef.h"
#include "sperl_name.h"
#include "sperl_opdef.h"
#include "sperl_resolved_type.h"



SPerl_char* const SPerl_OP_C_CODE_NAMES[] = {
  "constboolean",
  "constchar",
  "constint",
  "constlong",
  "constfloat",
  "constdouble",
  "conststring",
  "bitnot",
  "preinc",
  "postinc",
  "predec",
  "postdec",
  "complement",
  "negate",
  "plus",
  "d2f",
  "d2i",
  "d2l",
  "f2d",
  "f2i",
  "f2l",
  "i2b",
  "i2c",
  "i2d",
  "i2f",
  "i2l",
  "i2s",
  "l2d",
  "l2f",
  "l2i",
  "null",
  "lt",
  "le",
  "gt",
  "ge",
  "add",
  "subtract",
  "multiply",
  "divide",
  "bitand",
  "bitor",
  "modulo",
  "bitxor",
  "eq",
  "ne",
  "leftshift",
  "rightshift",
  "and",
  "or",
  "not",
  "list",
  "pushmark",
  "aelem",
  "field",
  "assign",
  "callsub",
  "subarg",
  "block",
  "my",
  "has",
  "cond",
  "use",
  "return",
  "last",
  "next",
  "package",
  "loop",
  "sub",
  "var",
  "statement",
  "grammer",
  "word",
  "if",
  "elsif",
  "else",
  "for",
  "while",
  "enum",
  "enumblock",
  "enumvalue",
  "classblock",
  "descripter",
  "anonsub",
  "type",
  "getenumvalue",
  "getfield",
  "converttype",
  "pop",
};

void SPerl_OP_check_ops(SPerl_PARSER* parser) {
  for (SPerl_int i = 0; i < parser->subs->length; i++) {
    SPerl_SUB* sub = SPerl_ARRAY_fetch(parser->subs, i);
    SPerl_OP* op_sub = sub->op;
    
    // my var informations
    SPerl_ARRAY* my_vars = SPerl_PARSER_new_array(parser, 0);
    SPerl_HASH* my_var_symtable = SPerl_PARSER_new_hash(parser, 0);
    
    // my variable stack
    SPerl_ARRAY* my_var_stack = SPerl_PARSER_new_array(parser, 0);
    
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
            *block_base_ptr = my_var_stack->length;
            SPerl_ARRAY_push(block_base_stack, block_base_ptr);
            block_base = *block_base_ptr;
          }
          else {
            block_start = 1;
          }
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

          switch (op_cur->group) {
            case SPerl_OP_C_GROUP_CONST: {
              SPerl_SUB* sub = op_sub->info;
              SPerl_CONST_VALUE* const_value = op_cur->info;
              SPerl_ARRAY_push(sub->const_values, const_value);
              break;
            }
            
            case SPerl_OP_C_GROUP_UNOP: {
              SPerl_OP* first = op_cur->first;
              SPerl_RESOLVED_TYPE* first_type = SPerl_OP_get_resolved_type(parser, first);
              SPerl_OPDEF* opdef = op_cur->info;
              
              break;
            }
            case SPerl_OP_C_GROUP_BINOP: {
              SPerl_OP* first = op_cur->first;
              SPerl_OP* last = op_cur->last;
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, last);
              SPerl_OPDEF* opdef = op_cur->info;
              
              // Can receive only core type
              if (!SPerl_TYPE_is_core_type(parser, first_resolved_type->id) || !SPerl_TYPE_is_core_type(parser, last_resolved_type->id)) {
                SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(op_cur->file));
                sprintf(message, "Error: %s operator can receive only core type at %s line %d\n",
                  opdef->symbol, op_cur->file, op_cur->line);
                SPerl_yyerror(parser, message);
              }
              // Insert type converting op
              SPerl_OP_insert_type_convert_op(parser, op_cur, first_resolved_type->id, last_resolved_type->id);
              break;
            }
            
            default:
            switch (op_cur->code) {
              // End of scope
              case SPerl_OP_C_CODE_BLOCK: {
                SPerl_int* block_base_ptr = SPerl_ARRAY_pop(block_base_stack);
                if (block_base_ptr) {
                  SPerl_int block_base = *block_base_ptr;
                  for (SPerl_int j = 0; j < my_var_stack->length - block_base; j++) {
                    SPerl_ARRAY_pop(my_var_stack);
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
                SPerl_VAR* var = op_cur->info;
                
                // Serach same name variable
                SPerl_MY_VAR* my_var = NULL;
                for (SPerl_int i = my_var_stack->length - 1 ; i >= 0; i--) {
                  SPerl_MY_VAR* my_var_tmp = SPerl_ARRAY_fetch(my_var_stack, i);
                  if (strcmp(var->name_word->value, my_var_tmp->name_word->value) == 0) {
                    my_var = my_var_tmp;
                    break;
                  }
                }
                
                if (my_var) {
                  // Add my var information to var
                  var->my_var = my_var;
                }
                else {
                  // Error
                  SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(var->name_word->value));
                  sprintf(message, "Error: \"my %s\" undeclared at %s line %d\n", var->name_word->value, op_cur->file, op_cur->line);
                  SPerl_yyerror(parser, message);
                }
                break;
              }
              case SPerl_OP_C_CODE_MY: {
                SPerl_MY_VAR* my_var = op_cur->info;
                
                // Serach same name variable
                SPerl_int found = 0;
                
                for (SPerl_int i = my_var_stack->length - 1 ; i >= block_base; i--) {
                  SPerl_MY_VAR* bef_my_var = SPerl_ARRAY_fetch(my_var_stack, i);
                  if (strcmp(my_var->name_word->value, bef_my_var->name_word->value) == 0) {
                    found = 1;
                    break;
                  }
                }
                
                if (found) {
                  SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(my_var->name_word->value));
                  sprintf(message, "Error: redeclaration of my \"%s\" at %s line %d\n", my_var->name_word->value, op_cur->file, op_cur->line);
                  SPerl_yyerror(parser, message);
                }
                else {
                  // Add my var information
                  my_var->id = parser->next_var_id++;
                  SPerl_ARRAY_push(my_vars, my_var);
                  my_var->sub = sub;
                  
                  SPerl_ARRAY_push(my_var_stack, my_var);
                }
                break;
              }

              case SPerl_OP_C_CODE_CALLSUB: {
                // Check sub name
                SPerl_NAME* name = op_cur->info;
                if (!name->anon) {
                  SPerl_OP_check_sub_name(parser, name);
                }
                break;
              }
              case SPerl_OP_C_CODE_GETFIELD: {
                // Check field name
                SPerl_NAME* name = op_cur->info;
                SPerl_OP_check_field_name(parser, name);
                break;
              }
              case SPerl_OP_C_CODE_GETENUMVALUE: {
                // Check enum name
                SPerl_NAME* name = op_cur->info;
                SPerl_OP_check_enum_name(parser, name);

                SPerl_char* enum_complete_name = name->complete_name;
                SPerl_ENUM_VALUE* enum_value = SPerl_HASH_search(parser->enum_complete_name_symtable, enum_complete_name, strlen(enum_complete_name));
                SPerl_CONST_VALUE* const_value = enum_value->const_value;
                
                // new const value
                SPerl_CONST_VALUE* new_const_value = SPerl_CONST_VALUE_new(parser);
                new_const_value->code = SPerl_CONST_VALUE_C_CODE_INT;
                new_const_value->uv.int_value = const_value->uv.int_value;
                new_const_value->resolved_type = const_value->resolved_type;
                SPerl_ARRAY_push(sub->const_values, new_const_value);
                
                // Replace getenumvalue to const
                op_cur->code = SPerl_OP_C_CODE_CONSTINT;
                op_cur->group = SPerl_OP_get_group(parser, op_cur->code);
                op_cur->info = new_const_value;
                op_cur->first = NULL;
                
                break;
              }
              case SPerl_OP_C_CODE_CONVERTTYPE: {
                SPerl_OP* op_type_dist = op_cur->first;
                SPerl_RESOLVED_TYPE* resolved_type_dist = op_type_dist->info;
                
                SPerl_OP* op_term = op_cur->last;
                SPerl_RESOLVED_TYPE* resolved_type_src = SPerl_OP_get_resolved_type(parser, op_term);
                
                // Can receive only core type
                if (!SPerl_TYPE_is_core_type(parser, resolved_type_src->id) || !SPerl_TYPE_is_core_type(parser, resolved_type_dist->id)) {
                  SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(op_cur->file));
                  sprintf(message, "Error: can't convert type %s to %s at %s line %d\n",
                    resolved_type_src->name, resolved_type_dist->name, op_cur->file, op_cur->line);
                  SPerl_yyerror(parser, message);
                }
                
                // Resolve converttype op
                SPerl_OP_resolve_op_converttype(parser, op_cur, resolved_type_src->id, resolved_type_dist->id);
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
    // Set my var information
    sub->my_vars = my_vars;
  }
}

SPerl_RESOLVED_TYPE* SPerl_OP_get_resolved_type(SPerl_PARSER* parser, SPerl_OP* op) {
  SPerl_RESOLVED_TYPE*  resolved_type;
  
  op->group;
  switch (op->group) {
    case SPerl_OP_C_GROUP_CONST: {
      SPerl_CONST_VALUE* const_value = op->info;
      resolved_type = const_value->resolved_type;
      break;
    }
    
    default:
    switch (op->code) {
      case SPerl_OP_C_CODE_VAR: {
        SPerl_VAR* var = op->info;
        resolved_type = var->my_var->type->resolved_type;
        break;
      }
      case SPerl_OP_C_CODE_CALLSUB: {
        SPerl_NAME* name = op->info;
        SPerl_char* complete_name = name->complete_name;
        SPerl_SUB* sub = SPerl_HASH_search(parser->sub_complete_name_symtable, complete_name, strlen(complete_name));
        resolved_type = sub->return_type->resolved_type;
        break;
      }
      case SPerl_OP_C_CODE_GETENUMVALUE: {
        SPerl_NAME* name = op->info;
        SPerl_char* complete_name = name->complete_name;
        SPerl_ENUM_VALUE* enum_value = SPerl_HASH_search(parser->enum_complete_name_symtable, complete_name, strlen(complete_name));
        resolved_type = enum_value->const_value->resolved_type;
        break;
      }
      case SPerl_OP_C_CODE_GETFIELD: {
        SPerl_NAME* name = op->info;
        SPerl_char* complete_name = name->complete_name;
        SPerl_FIELD* field = SPerl_HASH_search(parser->field_complete_name_symtable, complete_name, strlen(complete_name));
        resolved_type = field->type->resolved_type;
        break;
      }
      case SPerl_OP_C_CODE_ADD:
      case SPerl_OP_C_CODE_SUBTRACT:
      case SPerl_OP_C_CODE_MULTIPLY:
      case SPerl_OP_C_CODE_DIVIDE:
      {
        SPerl_OPDEF* opdef = op->info;
        resolved_type = opdef->return_type->resolved_type;
        break;
      }
      defaut:
        resolved_type = NULL;
    }
  }
  
  return resolved_type;
}

void SPerl_OP_insert_type_convert_op(SPerl_PARSER* parser, SPerl_OP* op, SPerl_int first_type_id, SPerl_int last_type_id) {
  // last int
  if (first_type_id < 4) {
    first_type_id = 4;
  }
  
  // last int
  if (last_type_id < 4) {
    first_type_id = 4;
  }
  
  if (first_type_id != last_type_id) {
    SPerl_boolean replace_first = 0;
    
    SPerl_OP* type_convert_op;
    if (first_type_id == SPerl_BODY_CORE_C_CODE_INT) {
      if (last_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
        type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_I2L, NULL, NULL);
        replace_first = 1;
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_I2F, NULL, NULL);
        replace_first = 1;
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
       type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_I2D, NULL, NULL);
       replace_first = 1;
      }
    }
    else if (first_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
      if (last_type_id == SPerl_BODY_CORE_C_CODE_INT) {
        type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_I2L, NULL, NULL);
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_L2F, NULL, NULL);
        replace_first = 1;
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
        type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_L2D, NULL, NULL);
        replace_first = 1;
      }
    }
    else if (first_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
      if (last_type_id == SPerl_BODY_CORE_C_CODE_INT) {
        type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_I2F, NULL, NULL);
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
        type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_L2F, NULL, NULL);
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
        type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_F2D, NULL, NULL);
        replace_first = 1;
      }
    }
    else if (first_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
      if (last_type_id == SPerl_BODY_CORE_C_CODE_INT) {
        type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_I2D, NULL, NULL);
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
        type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_L2D, NULL, NULL);
      }
      else if (last_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        type_convert_op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_F2D, NULL, NULL);
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

void SPerl_OP_resolve_op_converttype(SPerl_PARSER* parser, SPerl_OP* op_converttype, SPerl_int src_type_id, SPerl_int dist_type_id) {
  
  if (src_type_id == dist_type_id) {
    op_converttype->code = SPerl_OP_C_CODE_NULL;
    op_converttype->group = SPerl_OP_get_group(parser, op_converttype->code);
  }
  else if (src_type_id != dist_type_id) {
    if (src_type_id == SPerl_BODY_CORE_C_CODE_INT) {
      if (dist_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
        op_converttype->code = SPerl_OP_C_CODE_I2L;
      }
      else if (dist_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        op_converttype->code = SPerl_OP_C_CODE_I2F;
      }
      else if (dist_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
       op_converttype->code = SPerl_OP_C_CODE_I2D;
      }
    }
    else if (src_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
      if (dist_type_id == SPerl_BODY_CORE_C_CODE_INT) {
        op_converttype->code = SPerl_OP_C_CODE_I2L;
      }
      else if (dist_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        op_converttype->code = SPerl_OP_C_CODE_L2F;
      }
      else if (dist_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
        op_converttype->code = SPerl_OP_C_CODE_L2D;
      }
    }
    else if (src_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
      if (dist_type_id == SPerl_BODY_CORE_C_CODE_INT) {
        op_converttype->code = SPerl_OP_C_CODE_I2F;
      }
      else if (dist_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
        op_converttype->code = SPerl_OP_C_CODE_L2F;
      }
      else if (dist_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
        op_converttype->code = SPerl_OP_C_CODE_F2D;
      }
    }
    else if (src_type_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
      if (dist_type_id == SPerl_BODY_CORE_C_CODE_INT) {
        op_converttype->code = SPerl_OP_C_CODE_I2D;
      }
      else if (dist_type_id == SPerl_BODY_CORE_C_CODE_LONG) {
        op_converttype->code = SPerl_OP_C_CODE_L2D;
      }
      else if (dist_type_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        op_converttype->code = SPerl_OP_C_CODE_F2D;
      }
    }
    op_converttype->group = SPerl_OP_get_group(parser, op_converttype->code);
  }
}

void SPerl_OP_resolve_types(SPerl_PARSER* parser) {
  SPerl_ARRAY* types = parser->types;

  for (SPerl_int i = 0; i < types->length; i++) {
    SPerl_TYPE* type = SPerl_ARRAY_fetch(types, i);
    SPerl_TYPE_build_parts(parser, type);
    SPerl_TYPE_build_name(parser, type);
  }
  
  for (SPerl_int i = 0; i < types->length; i++) {
    SPerl_TYPE* type = SPerl_ARRAY_fetch(types, i);
    SPerl_OP_resolve_type(parser, type);
  }
}

void SPerl_OP_check(SPerl_PARSER* parser) {
  
  // Check descripters(Not used)
  SPerl_OP_check_descripters(parser);
  
  // Resolve types
  SPerl_OP_resolve_types(parser);
  
  // Check types
  SPerl_OP_check_ops(parser);
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
      SPerl_ARRAY* descripters = body_class->descripters;
      for (SPerl_int j = 0; j < descripters->length; j++) {
        SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, j);
        if (descripter->code != SPerl_DESCRIPTER_C_CODE_VALUE && descripter->code != SPerl_DESCRIPTER_C_CODE_ENUM)
        {
          SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_CODE_NAMES[descripter->code]));
          sprintf(message, "Error: unknown descripter of package \"%s\" at %s line %d\n",
            SPerl_DESCRIPTER_CODE_NAMES[descripter->code], descripter->op->file, descripter->op->line);
          SPerl_yyerror(parser, message);
        }
      }
      
      // Check field
      SPerl_ARRAY* fields = body_class->fields;
      for (SPerl_int j = 0; j < fields->length; j++) {
        SPerl_FIELD* field = SPerl_ARRAY_fetch(fields, j);

        // Check field descripters(Not used)
        SPerl_ARRAY* descripters = field->descripters;
        for (SPerl_int k = 0; k < descripters->length; k++) {
          SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, k);
          if (descripter->code != SPerl_DESCRIPTER_C_CODE_CONST)
          {
            SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_CODE_NAMES[descripter->code]));
            sprintf(message, "Error: unknown descripter of has \"%s\" at %s line %d\n",
              SPerl_DESCRIPTER_CODE_NAMES[descripter->code], descripter->op->file, descripter->op->line);
            SPerl_yyerror(parser, message);
          }
        }
      }
    }
  }

  // Check subs
  SPerl_ARRAY* subs = parser->subs;
  for (SPerl_int i = 0; i < subs->length; i++) {
    SPerl_SUB* sub = SPerl_ARRAY_fetch(subs, i);
    
    // Check sub descripters(Not used)
    SPerl_ARRAY* descripters = sub->descripters;
    SPerl_int k;
    for (SPerl_int j = 0; j < descripters->length; j++) {
      SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, j);
      if (descripter->code != SPerl_DESCRIPTER_C_CODE_STATIC)
      {
        SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_CODE_NAMES[descripter->code]));
        sprintf(message, "Error: unknown descripter of sub \"%s\" at %s line %d\n",
          SPerl_DESCRIPTER_CODE_NAMES[descripter->code], descripter->op->file, descripter->op->line);
        SPerl_yyerror(parser, message);
      }
    }
    
    // Check my var information
    SPerl_ARRAY* my_vars = sub->my_vars;
    for (SPerl_int j = 0; j < my_vars->length; j++) {
      SPerl_MY_VAR* my_var = SPerl_ARRAY_fetch(my_vars, k);
      
      // Check my_var descripters(Not used)
      SPerl_ARRAY* descripters = my_var->descripters;
      for (SPerl_int l = 0; l < descripters->length; l++) {
        SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, l);
        if (descripter->code != SPerl_DESCRIPTER_C_CODE_CONST)
        {
          SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_CODE_NAMES[descripter->code]));
          sprintf(message, "Error: unknown descripter of my \"%s\" at %s line %d\n",
            SPerl_DESCRIPTER_CODE_NAMES[descripter->code], descripter->op->file, descripter->op->line);
          SPerl_yyerror(parser, message);
        }
      }
    }
  }
}

void SPerl_OP_check_sub_name(SPerl_PARSER* parser, SPerl_NAME* name) {
  SPerl_char* sub_abs_name;
  SPerl_OP* op;
  if (name->abs_name_word) {
    sub_abs_name = name->abs_name_word->value;
    op = name->abs_name_word->op;
  }
  else if (name->var) {
    SPerl_char* package_name = name->var->my_var->sub->package_name;
    SPerl_char* base_name = name->base_name_word->value;
    sub_abs_name = SPerl_OP_create_abs_name(parser, package_name, base_name);
    op = name->var->op;
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
    SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(sub_complete_name));
    sprintf(message, "Error: unknown sub \"%s\" at %s line %d\n",
      sub_complete_name, op->file, op->line);
    SPerl_yyerror(parser, message);
  }
}

void SPerl_OP_check_field_name(SPerl_PARSER* parser, SPerl_NAME* name) {
  SPerl_char* package_name = name->var->my_var->sub->package_name;
  SPerl_WORD* base_name_word = name->base_name_word;
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
    SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(complete_name));
    sprintf(message, "Error: unknown field \"%s\" at %s line %d\n",
      complete_name, base_name_word->op->file, base_name_word->op->line);
    SPerl_yyerror(parser, message);
  }
}

void SPerl_OP_check_enum_name(SPerl_PARSER* parser, SPerl_NAME* name) {
  SPerl_WORD* abs_name_word = name->abs_name_word;
  SPerl_char* abs_name = abs_name_word->value;
  SPerl_char* complete_name = abs_name;
  name->complete_name = complete_name;
  
  SPerl_ENUM_VALUE* found_enum= SPerl_HASH_search(
    parser->enum_complete_name_symtable,
    complete_name,
    strlen(complete_name)
  );
  
  if (!found_enum) {
    SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(complete_name));
    sprintf(message, "Error: unknown enum \"%s\" at %s line %d\n",
      complete_name, abs_name_word->op->file, abs_name_word->op->line);
    SPerl_yyerror(parser, message);
  }
}

SPerl_OP* SPerl_OP_build_getenumvalue(SPerl_PARSER* parser, SPerl_OP* op_enumname) {
  SPerl_OP* op_getenumvalue = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_GETENUMVALUE, op_enumname, NULL);
  
  SPerl_NAME* name = SPerl_NAME_new(parser);
  name->code = SPerl_NAME_C_CODE_ENUM;
  name->abs_name_word = op_enumname->info;
  
  op_getenumvalue->info = name;
  
  return op_getenumvalue;
}

SPerl_OP* SPerl_OP_build_getfield(SPerl_PARSER* parser, SPerl_OP* op_var, SPerl_OP* op_fieldname) {
  SPerl_OP* op_getfield = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_GETFIELD, NULL, NULL);
  SPerl_OP_sibling_splice(parser, op_getfield, NULL, 0, op_var);
  SPerl_OP_sibling_splice(parser, op_getfield, op_var, 0, op_fieldname);
  
  SPerl_NAME* name = SPerl_NAME_new(parser);
  name->code = SPerl_NAME_C_CODE_FIELD;
  name->var = op_var->info;
  name->base_name_word = op_fieldname->info;
  
  op_getfield->info = name;
  
  return op_getfield;
}

SPerl_OP* SPerl_OP_build_converttype(SPerl_PARSER* parser, SPerl_OP* op_type, SPerl_OP* op_term) {
  
  SPerl_OP* op_converttype = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_CONVERTTYPE, op_type, op_term);
  op_converttype->file = op_type->file;
  op_converttype->line = op_type->line;
  
  return op_converttype;
}

SPerl_OP* SPerl_OP_build_grammer(SPerl_PARSER* parser, SPerl_OP* op_packages) {
  SPerl_OP* op_grammer = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_GRAMMER, op_packages, NULL);
  parser->op_grammer = op_grammer;

  // Resovle types, check types, descripters, and names.
  SPerl_OP_check(parser);

  // Build constant pool
  SPerl_OP_build_const_pool(parser);
  
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
      else if (part->code == SPerl_TYPE_PART_C_CODE_CHAR) {
        resolved_type_name_length++;
        SPerl_ARRAY_push(resolved_type_part_names, part->uv.char_name);
      }
      else {
        SPerl_WORD* part_name_word = part->uv.name_word;
        SPerl_char* part_name = part_name_word->value;
        
        SPerl_TYPE* found_type = SPerl_HASH_search(package_symtable, part_name, strlen(part_name));
        if (found_type) {
          SPerl_boolean is_self
            = type->code == SPerl_TYPE_C_CODE_WORD && found_type->code == SPerl_TYPE_C_CODE_WORD
            && strcmp(type->uv.type_word->name_word->value, found_type->uv.type_word->name_word->value) == 0;
          
          if (is_self) {
            resolved_type_name_length += strlen(found_type->uv.type_word->name_word->value);
            SPerl_char* found_part_name = found_type->uv.type_word->name_word->value;
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
          SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(part_name));
          sprintf(message, "Error: unknown package \"%s\" at %s line %d\n", part_name, part_name_word->op->file, part_name_word->op->line);
          SPerl_yyerror(parser, message);
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
  SPerl_ARRAY* subs = parser->subs;
  
  for (SPerl_int i = 0; i < subs->length; i++) {
    // Subroutine
    SPerl_SUB* sub = SPerl_ARRAY_fetch(subs, i);
    
    // Set constant informations
    SPerl_ARRAY* const_values = sub->const_values;
    
    // Constant pool length
    SPerl_int const_pool_length = 0;
    for (SPerl_int j = 0; j < const_values->length; j++) {
      SPerl_CONST_VALUE* const_value = SPerl_ARRAY_fetch(const_values, j);
      switch(const_value->code) {
        case SPerl_CONST_VALUE_C_CODE_BOOLEAN:
        case SPerl_CONST_VALUE_C_CODE_CHAR:
        case SPerl_CONST_VALUE_C_CODE_INT:
          const_pool_length += 1;
          break;
        case SPerl_CONST_VALUE_C_CODE_LONG:
          const_pool_length += 2;
          break;
        case SPerl_CONST_VALUE_C_CODE_FLOAT:
          const_pool_length += 1;
          break;
        case SPerl_CONST_VALUE_C_CODE_DOUBLE:
          const_pool_length += 2;
          break;
      }
    }
    sub->const_pool_length = const_pool_length;
    
    // Create constant pool
    SPerl_int* const_pool = calloc(const_pool_length, sizeof(SPerl_int));
    SPerl_int const_pool_pos = 0;
    for (SPerl_int j = 0; j < const_values->length; j++) {
      SPerl_CONST_VALUE* const_value = SPerl_ARRAY_fetch(const_values, j);
      
      const_value->pool_pos = const_pool_pos;
      
      switch(const_value->code) {
        case SPerl_CONST_VALUE_C_CODE_BOOLEAN:
        case SPerl_CONST_VALUE_C_CODE_CHAR:
        case SPerl_CONST_VALUE_C_CODE_INT:
          const_value->pool_pos = const_pool_pos;
          *(const_pool + const_pool_pos) = const_value->uv.int_value;
          const_pool_pos += 1;
          break;
        case SPerl_CONST_VALUE_C_CODE_LONG:
          const_value->pool_pos = const_pool_pos;
          *(SPerl_long*)(const_pool + const_pool_pos) = const_value->uv.long_value;
          const_pool_pos += 2;
          break;
        case SPerl_CONST_VALUE_C_CODE_FLOAT:
          const_value->pool_pos = const_pool_pos;
          *(SPerl_float*)(const_pool + const_pool_pos) = const_value->uv.float_value;
          const_pool_pos += 1;
          break;
        case SPerl_CONST_VALUE_C_CODE_DOUBLE:
          const_value->pool_pos = const_pool_pos;
          *(SPerl_double*)(const_pool + const_pool_pos) = const_value->uv.double_value;
          const_pool_pos += 2;
          break;
      }
    }
    sub->const_pool = const_pool;
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

SPerl_OP* SPerl_OP_build_package(SPerl_PARSER* parser, SPerl_OP* op_package, SPerl_OP* op_packagename, SPerl_OP* op_type, SPerl_OP* op_descripters, SPerl_OP* op_block) {
  SPerl_int i;
  
  SPerl_OP_sibling_splice(parser, op_package, NULL, 0, op_packagename);
  SPerl_OP_sibling_splice(parser, op_package, op_packagename, 0, op_type);
  SPerl_OP_sibling_splice(parser, op_package, op_type, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_package, op_descripters, 0, op_block);
  
  SPerl_WORD* package_name_word = op_packagename->info;
  SPerl_char* package_name = package_name_word->value;
  SPerl_HASH* package_symtable = parser->package_symtable;
  
  // Redeclaration package error
  SPerl_TYPE* found_package = SPerl_HASH_search(package_symtable, package_name, strlen(package_name));
  if (found_package) {
    SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(package_name));
    sprintf(message, "Error: redeclaration of package \"%s\" at %s line %d\n", package_name, op_package->file, op_package->line);
    SPerl_yyerror(parser, message);
  }
  else {
    // Package
    SPerl_PACKAGE* package = SPerl_PACKAGE_new(parser);
    package->name_word = package_name_word;
    
    // Type
    SPerl_TYPE* type;
    
    // Class type or enum type
    if (op_type->code == SPerl_OP_C_CODE_NULL) {
      
      // Type(type is same as package name)
      type = SPerl_TYPE_new(parser);
      type->code = SPerl_TYPE_C_CODE_WORD;
      SPerl_TYPE_WORD* type_word = SPerl_TYPE_WORD_new(parser);
      type_word->name_word = package_name_word;
      type->uv.type_word = type_word;
      
      // Body
      SPerl_BODY* body = SPerl_BODY_new(parser);
      body->name = package_name;
      
      // Enum type
      if (op_descripters->code == SPerl_OP_C_CODE_ENUM) {
        body->code = SPerl_BODY_C_CODE_ENUM;
        
        // Enum values
        SPerl_ARRAY* enum_values = SPerl_PARSER_new_array(parser, 0);
        
        // Starting value
        SPerl_long start_value = 0;
        
        SPerl_OP* op_enumvalues = op_block->first;
        SPerl_OP* op_enumvalue = op_enumvalues->first;
        while (op_enumvalue = SPerl_OP_sibling(parser, op_enumvalue)) {
          SPerl_ENUM_VALUE* enum_value = SPerl_ENUM_VALUE_new(parser);
          enum_value->name_word = op_enumvalue->first->info;
          if (op_enumvalue->last) {
            enum_value->const_value = op_enumvalue->last->info;
          }
          
          SPerl_CONST_VALUE* const_value;
          if (enum_value->const_value) {
            const_value = enum_value->const_value;
            start_value = const_value->uv.int_value + 1;
          }
          else {
            const_value = SPerl_CONST_VALUE_new(parser);
            const_value->code = SPerl_CONST_VALUE_C_CODE_INT;
            const_value->uv.int_value = start_value;
            const_value->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "int", strlen("int"));
            
            enum_value->const_value = const_value;
            start_value++;
          }
          
          SPerl_ARRAY_push(enum_values, enum_value);
          SPerl_char* enum_complete_name = SPerl_OP_create_complete_name(parser, package_name, enum_value->name_word->value);
          SPerl_HASH_insert(parser->enum_complete_name_symtable, enum_complete_name, strlen(enum_complete_name), enum_value);
        }
        
        // Set enum body
        SPerl_BODY_ENUM* body_enum = SPerl_BODY_ENUM_new(parser);
        body_enum->enum_values = enum_values;
        body->uv.body_enum = body_enum;
      }
      // Class type
      else {
        // Add type
        package->type = type;
        SPerl_ARRAY_push(parser->types, type);
      
        body->code = SPerl_BODY_C_CODE_CLASS;

        SPerl_BODY_CLASS* body_class = SPerl_BODY_CLASS_new(parser);
        body_class->op_block = op_block;
        body_class->alias = SPerl_PARSER_new_hash(parser, 0);
        body_class->descripters = SPerl_OP_create_descripters(parser, op_descripters);
        body_class->code = SPerl_BODY_CLASS_C_CODE_NORMAL;
        
        // Class is value class
        for (SPerl_int i = 0; i < body_class->descripters->length; i++) {
          SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(body_class->descripters, i);
          if (descripter->code == SPerl_DESCRIPTER_C_CODE_VALUE) {
            body_class->is_value_class = 1;
            break;
          }
        }
        
        // Search use and field
        SPerl_ARRAY* fields = SPerl_PARSER_new_array(parser, 0);
        SPerl_HASH* field_symtable = SPerl_PARSER_new_hash(parser, 0);
        SPerl_ARRAY* uses = SPerl_PARSER_new_array(parser, 0);
        SPerl_HASH* use_symtable = SPerl_PARSER_new_hash(parser, 0);
        
        // Collect field and use information
        SPerl_OP* op_usehassubs = op_block->first;
        SPerl_OP* op_usehassub = op_usehassubs->first;
        while (op_usehassub = SPerl_OP_sibling(parser, op_usehassub)) {
          // Use
          if (op_usehassub->code == SPerl_OP_C_CODE_USE) {
            SPerl_USE* use = op_usehassub->info;
            SPerl_char* use_type_name = use->package_name_word->value;
            SPerl_USE* found_use
              = SPerl_HASH_search(use_symtable, use_type_name, strlen(use_type_name));
            
            if (found_use) {
              SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(use_type_name));
              sprintf(message, "Error: redeclaration of use \"%s\" at %s line %d\n", use_type_name, use->op->file, use->op->line);
              SPerl_yyerror(parser, message);
            }
            else {
              SPerl_ARRAY_push(parser->use_stack, use);
              SPerl_ARRAY_push(uses, use);
              
              SPerl_HASH_insert(use_symtable, use_type_name, strlen(use_type_name), use);
            }
          }
          // Field
          else if (op_usehassub->code == SPerl_OP_C_CODE_HAS) {
            SPerl_FIELD* field = op_usehassub->info;
            SPerl_char* field_name = field->name_word->value;
            SPerl_FIELD* found_field
              = SPerl_HASH_search(field_symtable, field_name, strlen(field_name));
            if (found_field) {
              SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(field_name));
              sprintf(message, "Error: redeclaration of has \"%s\" at %s line %d\n", field_name, field->op->file, field->op->line);
              SPerl_yyerror(parser, message);
            }
            else {
              // Value class only have core type field
              if (body_class->is_value_class) {
                SPerl_boolean is_core_type = SPerl_TYPE_is_core_type_name(parser, field->type);
                if (!is_core_type) {
                  SPerl_char* message = SPerl_PARSER_new_string(parser, 200);
                  sprintf(message, "Error: value class has only core type field at %s line %d\n", field->op->file, field->op->line);
                  SPerl_yyerror(parser, message);
                }
              }
              
              field->body_class = body_class;
              field->op = op_usehassub;
              SPerl_ARRAY_push(fields, field);
              SPerl_HASH_insert(field_symtable, field_name, strlen(field_name), field);
              
              // Field complete name
              SPerl_char* field_complete_name = SPerl_OP_create_complete_name(parser, package_name, field_name);
              SPerl_HASH_insert(parser->field_complete_name_symtable, field_complete_name, strlen(field_complete_name), field);
            }
          }
        }
        // Set filed and sub information
        body_class->fields = fields;
        body_class->field_symtable = field_symtable;
        body_class->uses = uses;
        body_class->use_symtable = use_symtable;

        // Method information
        SPerl_HASH* sub_complete_name_symtable = parser->sub_complete_name_symtable;
        SPerl_int i = parser->subs->length - 1;
        while (1) {
          if (i < 0) {
            break;
          }
          SPerl_SUB* sub = SPerl_ARRAY_fetch(parser->subs, i);
          if (sub->package_name) {
            break;
          }
          
          if (!sub->anon) {
            SPerl_char* sub_name = sub->name_word->value;
            SPerl_char* sub_abs_name = SPerl_OP_create_abs_name(parser, package_name, sub_name);
            SPerl_char* sub_complete_name = SPerl_OP_create_sub_complete_name(parser, sub_abs_name, sub->argument_count);
            
            SPerl_SUB* found_sub = NULL;
            found_sub = SPerl_HASH_search(sub_complete_name_symtable, sub_complete_name, strlen(sub_complete_name));
            
            if (found_sub) {
              SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(sub_complete_name));
              sprintf(message, "Error: redeclaration of sub \"%s\" at %s line %d\n", sub_complete_name, sub->op->file, sub->op->line);
              SPerl_yyerror(parser, message);
            }
            // Unknown sub
            else {
              sub->body_class = body_class;
              SPerl_HASH_insert(sub_complete_name_symtable, sub_complete_name, strlen(sub_complete_name), sub);
            }
            i--;
          }
          sub->body_class = body_class;
          sub->package_name = package_name;
        }
        
        // Set body
        body->uv.body_class = body_class;
      }
      SPerl_ARRAY_push(parser->bodys, body);
      SPerl_HASH_insert(parser->body_symtable, body->name, strlen(body->name), body);
    }
    // Typedef
    else {
      type = op_type->info;
      package->type = type;
    }
    
    
    // Add package
    SPerl_ARRAY_push(parser->packages, package);
    SPerl_HASH_insert(parser->package_symtable, package_name, strlen(package_name), type);
  }
  
  return op_package;
}

SPerl_OP* SPerl_OP_build_decluse(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_packagename, SPerl_OP* op_packagealias) {
  SPerl_OP_sibling_splice(parser, op_use, NULL, 0, op_packagename);
  SPerl_OP_sibling_splice(parser, op_use, op_packagename, 0, op_packagealias);
  
  SPerl_USE* use = SPerl_USE_new(parser);
  use->package_name_word = op_packagename->info;
  use->op = op_use;
  op_use->info = use;
  
  return op_use;
}

SPerl_OP* SPerl_OP_build_declmy(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_descripters, SPerl_OP* op_type) {
  
  SPerl_OP_sibling_splice(parser, op_my, NULL, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_my, op_descripters, 0, op_type);
  
  SPerl_OP_sibling_splice(parser, op_var, NULL, 0, op_my);
  
  // Create my var information
  SPerl_MY_VAR* my_var = SPerl_MY_VAR_new(parser);
  SPerl_VAR* var = op_var->info;
  my_var->name_word = var->name_word;
  
  // descripters
  my_var->descripters = SPerl_OP_create_descripters(parser, op_descripters);
  
  // type
  my_var->type = op_type->info;
  
  // Add my_var information to op
  op_my->info = my_var;
  
  return op_var;
}

SPerl_OP* SPerl_OP_build_declhas(SPerl_PARSER* parser, SPerl_OP* op_has, SPerl_OP* op_field_name, SPerl_OP* op_descripters, SPerl_OP* op_type) {
  
  // Build OP
  SPerl_OP_sibling_splice(parser, op_has, NULL, 0, op_field_name);
  SPerl_OP_sibling_splice(parser, op_has, op_field_name, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_has, op_descripters, 0, op_type);
  
  // Create field information
  SPerl_FIELD* field = SPerl_FIELD_new(parser);
  
  // Name
  field->name_word = op_field_name->info;

  // Descripters
  field->descripters = SPerl_OP_create_descripters(parser, op_descripters);
  
  // Type
  field->type = op_type->info;
  
  // OP
  field->op = op_has;
  
  // Set field informaiton
  op_has->info = field;
  
  return op_has;
}

SPerl_ARRAY* SPerl_OP_create_descripters(SPerl_PARSER* parser, SPerl_OP* op_descripters) {
  
  SPerl_ARRAY* descripters = SPerl_PARSER_new_array(parser, 0);

  // descripters is enum or list of descripter
  if (op_descripters->code == SPerl_OP_C_CODE_ENUM) {
    SPerl_DESCRIPTER* descripter = SPerl_DESCRIPTER_new(parser);
    descripter->code = SPerl_DESCRIPTER_C_CODE_ENUM;
    SPerl_ARRAY_push(descripters, descripter);
  }
  else {
    SPerl_OP* op_descripter = op_descripters->first;
    while (op_descripter = SPerl_OP_sibling(parser, op_descripter)) {
      SPerl_ARRAY_push(descripters, op_descripter->info);
    }
  }
  
  return descripters;
}

SPerl_OP* SPerl_OP_build_declsub(SPerl_PARSER* parser, SPerl_OP* op_sub, SPerl_OP* op_subname, SPerl_OP* op_subargs, SPerl_OP* op_descripters, SPerl_OP* op_type, SPerl_OP* op_block) {
  
  // Build OP_SUB
  SPerl_OP_sibling_splice(parser, op_sub, NULL, 0, op_subname);
  SPerl_OP_sibling_splice(parser, op_sub, op_subname, 0, op_subargs);
  SPerl_OP_sibling_splice(parser, op_sub, op_subargs, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_sub, op_descripters, 0, op_type);
  SPerl_OP_sibling_splice(parser, op_sub, op_type, 0, op_block);
  
  // Create sub infomation
  SPerl_SUB* sub = SPerl_SUB_new(parser);
  if (op_subname->code == SPerl_OP_C_CODE_NULL) {
    sub->anon = 1;
  }
  else {
    sub->name_word = op_subname->info;
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
  sub->descripters = SPerl_OP_create_descripters(parser, op_descripters);
  
  // return type
  sub->return_type = op_type->info;
  
  // Save block
  sub->op_block = op_block;
  
  // Save op
  sub->op = op_sub;

  // ID
  sub->id = parser->subs->length;
  
  // Add sub information
  SPerl_ARRAY_push(parser->subs, sub);
  
  op_sub->info = sub;
  
  return op_sub;
}

SPerl_OP* SPerl_OP_build_callsub(SPerl_PARSER* parser, SPerl_OP* op_invocant, SPerl_OP* op_subname, SPerl_OP* op_terms, SPerl_boolean anon) {
  
  // Build OP_SUB
  SPerl_OP* op_callsub = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_CALLSUB, NULL, NULL);
  SPerl_OP_sibling_splice(parser, op_callsub, NULL, 0, op_subname);
  SPerl_OP_sibling_splice(parser, op_callsub, op_subname, 0, op_terms);
  
  SPerl_NAME* name = SPerl_NAME_new(parser);
  name->code = SPerl_NAME_C_CODE_SUB;
  
  if (!anon) {
    SPerl_WORD* sub_name_word = op_subname->info;
    SPerl_char* sub_name = sub_name_word->value;
    
    if (strstr(sub_name, ":")) {
      name->abs_name_word = sub_name_word;
    }
    else {
      if (op_invocant->code == SPerl_OP_C_CODE_VAR) {
        name->var = op_invocant->info;
        name->base_name_word = sub_name_word;
      }
      else {
        SPerl_WORD* sub_abs_name_word = SPerl_WORD_new(parser);
        SPerl_char* sub_abs_name = SPerl_OP_create_abs_name(parser, "CORE", sub_name);
        sub_abs_name_word->value = sub_abs_name;
        sub_abs_name_word->op = op_subname;
        name->abs_name_word = sub_abs_name_word;
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
  
  op_callsub->info = name;
  
  return op_callsub;
}

SPerl_OP* SPerl_OP_build_callop(SPerl_PARSER* parser, SPerl_OP* op_callop, SPerl_OP* op_first, SPerl_OP* op_last) {
  
  // Build OP_SUB
  SPerl_OP_sibling_splice(parser, op_callop, NULL, 0, op_first);
  if (op_last) {
    SPerl_OP_sibling_splice(parser, op_callop, op_first, 0, op_last);
  }
  
  SPerl_OPDEF* opdef = SPerl_OPDEF_new(parser);
  SPerl_char* symbol;
  switch (op_callop->code) {
    case SPerl_OP_C_CODE_PLUS:
    case SPerl_OP_C_CODE_ADD:
      symbol = "+";
      break;
    case SPerl_OP_C_CODE_PREINC:
    case SPerl_OP_C_CODE_POSTINC:
      symbol = "++";
      break;
    case SPerl_OP_C_CODE_PREDEC:
    case SPerl_OP_C_CODE_POSTDEC:
      symbol = "--";
      break;
    case SPerl_OP_C_CODE_BITNOT:
      symbol = "~";
      break;
    case SPerl_OP_C_CODE_NOT:
      symbol = "!";
      break;
    case SPerl_OP_C_CODE_SUBTRACT:
    case SPerl_OP_C_CODE_NEGATE:
      symbol = "-";
      break;
    case SPerl_OP_C_CODE_MULTIPLY:
      symbol = "*";
      break;
    case SPerl_OP_C_CODE_BITAND:
      symbol = "&";
      break;
    case SPerl_OP_C_CODE_BITOR:
      symbol = "|";
      break;
    case SPerl_OP_C_CODE_LEFTSHIFT:
      symbol = "<<";
      break;
    case SPerl_OP_C_CODE_RIGHTSHIFT:
      symbol = ">>";
      break;
    case SPerl_OP_C_CODE_AND:
      symbol = "&&";
      break;
    case SPerl_OP_C_CODE_OR:
      symbol = "||";
      break;
    case SPerl_OP_C_CODE_LT:
      symbol = "<";
      break;
    case SPerl_OP_C_CODE_LE:
      symbol = "=<";
      break;
    case SPerl_OP_C_CODE_GT:
      symbol = ">";
      break;
    case SPerl_OP_C_CODE_GE:
      symbol = "=>";
      break;
    case SPerl_OP_C_CODE_EQ:
      symbol = "==";
      break;
    case SPerl_OP_C_CODE_NE:
      symbol = "!=";
      break;
  }
  opdef->symbol = symbol;
  
  op_callop->info = opdef;
  
  return op_callop;
}

SPerl_OP* SPerl_OP_build_wordtype(SPerl_PARSER* parser, SPerl_OP* op_wordtype) {
  SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, op_wordtype, NULL);
  
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_WORD;
  
  SPerl_TYPE_WORD* type_word = SPerl_TYPE_WORD_new(parser);
  type_word->name_word = op_wordtype->info;
  type->uv.type_word = type_word;

  SPerl_ARRAY_push(parser->types, type);
  
  op_type->info = type;
  op_type->file = op_wordtype->file;
  op_type->line = op_wordtype->line;
  
  return op_type;
}

SPerl_OP* SPerl_OP_build_arraytype(SPerl_PARSER* parser, SPerl_OP* op_type) {
  SPerl_OP* op_type_array = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, op_type, NULL);
  
  // Type array
  SPerl_TYPE_ARRAY* type_array = SPerl_TYPE_ARRAY_new(parser);
  type_array->type = op_type->info;
  
  // Type
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_ARRAY;
  type->uv.type_array = type_array;

  SPerl_ARRAY_push(parser->types, type);
  
  op_type_array->info = type;
  op_type_array->file = op_type->file;
  op_type_array->line = op_type->line;
  
  return op_type_array;
}

SPerl_OP* SPerl_OP_build_subtype(SPerl_PARSER* parser, SPerl_OP* op_argument_types, SPerl_OP* op_return_type) {
  SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, op_argument_types, op_return_type);
  
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_SUB;
  
  SPerl_char* file = NULL;
  SPerl_int line = -1;
  
  // sub type
  SPerl_TYPE_SUB* type_sub = SPerl_TYPE_SUB_new(parser);
  SPerl_ARRAY* argument_types = SPerl_PARSER_new_array(parser, 0);
  {
    SPerl_OP* op_argument_type = op_argument_types->first;
    while (op_argument_type = SPerl_OP_sibling(parser, op_argument_type)) {
      if (file == NULL) {
        file = op_argument_type->file;
        line = op_argument_type->line;
      }
      SPerl_ARRAY_push(argument_types, op_argument_type->info);
    }
  }
  type_sub->return_type = op_return_type->info;
  if (file == NULL) {
    file = op_return_type->file;
    line = op_return_type->line;
  }
  type_sub->argument_types = argument_types;
  
  type->uv.type_sub = type_sub;

  SPerl_ARRAY_push(parser->types, type);
  
  op_type->info = type;
  op_type->file = file;
  op_type->line = line;
  
  return op_type;
}

SPerl_OP* SPerl_OP_newOP(SPerl_PARSER* parser, SPerl_char type, SPerl_OP* first, SPerl_OP* last) {
  return SPerl_OP_newOP_flag(parser, type, first, last, 0, 0);
}

SPerl_int SPerl_OP_get_group(SPerl_PARSER* parser, SPerl_int op_code) {
  
  // Group
  SPerl_int group = 0;
  switch (op_code) {
    // Constant value
    case SPerl_OP_C_CODE_CONSTBOOLEAN:
    case SPerl_OP_C_CODE_CONSTCHAR:
    case SPerl_OP_C_CODE_CONSTINT:
    case SPerl_OP_C_CODE_CONSTLONG:
    case SPerl_OP_C_CODE_CONSTFLOAT:
    case SPerl_OP_C_CODE_CONSTDOUBLE:
    case SPerl_OP_C_CODE_CONSTSTRING:
      group = SPerl_OP_C_GROUP_CONST;
      break;
    
    // Logical OP
    case SPerl_OP_C_CODE_AND:
    case SPerl_OP_C_CODE_OR:
    case SPerl_OP_C_CODE_NOT:
      group = SPerl_OP_C_GROUP_LOGICALOP;
      break;
    
    // Assign OP
    case SPerl_OP_C_CODE_ASSIGN:
      group = SPerl_OP_C_GROUP_ASSIGNOP;
      break;
    
    // Binary OP
    case SPerl_OP_C_CODE_LT:
    case SPerl_OP_C_CODE_LE:
    case SPerl_OP_C_CODE_GT:
    case SPerl_OP_C_CODE_GE:
    case SPerl_OP_C_CODE_ADD:
    case SPerl_OP_C_CODE_SUBTRACT:
    case SPerl_OP_C_CODE_MULTIPLY:
    case SPerl_OP_C_CODE_DIVIDE:
    case SPerl_OP_C_CODE_BITAND:
    case SPerl_OP_C_CODE_BITOR:
    case SPerl_OP_C_CODE_MODULO:
    case SPerl_OP_C_CODE_BITXOR:
    case SPerl_OP_C_CODE_EQ:
    case SPerl_OP_C_CODE_NE:
    case SPerl_OP_C_CODE_LEFTSHIFT:
    case SPerl_OP_C_CODE_RIGHTSHIFT:
      group = SPerl_OP_C_GROUP_BINOP;
      break;
    
    // Unary op
    case SPerl_OP_C_CODE_BITNOT:
    case SPerl_OP_C_CODE_COMPLEMENT:
    case SPerl_OP_C_CODE_NEGATE:
    case SPerl_OP_C_CODE_PLUS:
    case SPerl_OP_C_CODE_D2F:
    case SPerl_OP_C_CODE_D2I:
    case SPerl_OP_C_CODE_D2L:
    case SPerl_OP_C_CODE_F2D:
    case SPerl_OP_C_CODE_F2I:
    case SPerl_OP_C_CODE_F2L:
    case SPerl_OP_C_CODE_I2B:
    case SPerl_OP_C_CODE_I2C:
    case SPerl_OP_C_CODE_I2D:
    case SPerl_OP_C_CODE_I2F:
    case SPerl_OP_C_CODE_I2L:
    case SPerl_OP_C_CODE_I2S:
    case SPerl_OP_C_CODE_L2D:
    case SPerl_OP_C_CODE_L2F:
    case SPerl_OP_C_CODE_L2I:
      group = SPerl_OP_C_GROUP_UNOP;
      break;
    case SPerl_OP_C_CODE_PREINC:
    case SPerl_OP_C_CODE_POSTINC:
    case SPerl_OP_C_CODE_PREDEC:
    case SPerl_OP_C_CODE_POSTDEC:
      group = SPerl_OP_C_GROUP_INCDEC;
      break;
  }
  
  return group;
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
  
  // Set group
  op->group = SPerl_OP_get_group(parser, code);
  
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
