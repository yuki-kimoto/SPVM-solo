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
#include "sperl_constant.h"
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
#include "sperl_op_info.h"
#include "sperl_name.h"
#include "sperl_op_info.h"
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
  "iinc",
};

void SPerl_OP_check_ops(SPerl_PARSER* parser) {
  for (SPerl_int i = 0; i < parser->op_subs->length; i++) {
    SPerl_OP* op_sub = SPerl_ARRAY_fetch(parser->op_subs, i);
    SPerl_SUB* sub = op_sub->uv.sub;
    
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
            case SPerl_OP_C_GROUP_INCDEC: {
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
            }
            case SPerl_OP_C_GROUP_CONST: {
              SPerl_SUB* sub = op_sub->uv.sub;
              SPerl_CONSTANT* constant = op_cur->uv.constant;
              SPerl_ARRAY_push(sub->constants, constant);
              break;
            }
            
            case SPerl_OP_C_GROUP_UNOP: {
              SPerl_OP* first = op_cur->first;
              SPerl_RESOLVED_TYPE* first_type = SPerl_OP_get_resolved_type(parser, first);
              SPerl_OP_INFO* op_info = op_cur->uv.op_info;
              
              break;
            }
            case SPerl_OP_C_GROUP_BINOP: {
              SPerl_OP* first = op_cur->first;
              SPerl_OP* last = op_cur->last;
              SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(parser, first);
              SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(parser, last);
              SPerl_OP_INFO* op_info = op_cur->uv.op_info;
              
              // Can receive only core type
              if (!SPerl_TYPE_is_core_type(parser, first_resolved_type->id) || !SPerl_TYPE_is_core_type(parser, last_resolved_type->id)) {
                SPerl_yyerror_format(parser, "%s operator can receive only core type at %s line %d\n", op_info->symbol, op_cur->file, op_cur->line);
                break;
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
                SPerl_VAR* var = op_cur->uv.var;
                
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
                  SPerl_yyerror_format(parser, "\"my %s\" undeclared at %s line %d\n", var->name_word->value, op_cur->file, op_cur->line);
                  parser->fatal_error = 1;
                  return;
                }
                break;
              }
              case SPerl_OP_C_CODE_MY: {
                SPerl_MY_VAR* my_var = op_cur->uv.my_var;
                
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
                  SPerl_yyerror_format(parser, "redeclaration of my \"%s\" at %s line %d\n", my_var->name_word->value, op_cur->file, op_cur->line);
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
                SPerl_NAME* name = op_cur->uv.name;
                if (!name->anon) {
                  SPerl_OP_check_sub_name(parser, name);
                  if (parser->fatal_error) {
                    return;
                  }
                }
                break;
              }
              case SPerl_OP_C_CODE_GETFIELD: {
                // Check field name
                SPerl_NAME* name = op_cur->uv.name;
                SPerl_OP_check_field_name(parser, name);
                if (parser->fatal_error) {
                  return;
                }
                break;
              }
              case SPerl_OP_C_CODE_GETENUMVALUE: {
                // Check enum name
                SPerl_NAME* name = op_cur->uv.name;
                SPerl_OP_check_enum_name(parser, name);
                if (parser->fatal_error) {
                  return;
                }

                SPerl_char* enum_complete_name = name->complete_name;
                SPerl_ENUM_VALUE* enum_value = SPerl_HASH_search(parser->enum_complete_name_symtable, enum_complete_name, strlen(enum_complete_name));
                SPerl_CONSTANT* constant = enum_value->constant;
                
                // new const value
                SPerl_CONSTANT* new_constant = SPerl_CONSTANT_new(parser);
                new_constant->code = SPerl_CONSTANT_C_CODE_INT;
                new_constant->uv.int_value = constant->uv.int_value;
                new_constant->resolved_type = constant->resolved_type;
                SPerl_ARRAY_push(sub->constants, new_constant);
                
                // Replace getenumvalue to const
                SPerl_OP_replace_code(parser, op_cur, SPerl_OP_C_CODE_CONSTINT);
                op_cur->uv.constant = new_constant;
                op_cur->first = NULL;
                
                break;
              }
              case SPerl_OP_C_CODE_CONVERTTYPE: {
                SPerl_OP* op_type_dist = op_cur->first;
                SPerl_TYPE* type_dist = op_type_dist->uv.type;
                SPerl_RESOLVED_TYPE* resolved_type_dist = type_dist->resolved_type;
                
                SPerl_OP* op_term = op_cur->last;
                SPerl_RESOLVED_TYPE* resolved_type_src = SPerl_OP_get_resolved_type(parser, op_term);
                
                // Can receive only core type
                if (!SPerl_TYPE_is_core_type(parser, resolved_type_src->id) || !SPerl_TYPE_is_core_type(parser, resolved_type_dist->id)) {
                  SPerl_yyerror_format(parser, "can't convert type %s to %s at %s line %d\n",
                    resolved_type_src->name, resolved_type_dist->name, op_cur->file, op_cur->line);
                }
                
                // Resolve converttype op
                SPerl_OP_resolve_converttype(parser, op_cur, resolved_type_src, resolved_type_dist);
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
      SPerl_CONSTANT* constant = op->uv.constant;
      resolved_type = constant->resolved_type;
      break;
    }
    
    default:
    switch (op->code) {
      case SPerl_OP_C_CODE_VAR: {
        SPerl_VAR* var = op->uv.var;
        resolved_type = var->my_var->type->resolved_type;
        break;
      }
      case SPerl_OP_C_CODE_CALLSUB: {
        SPerl_NAME* name = op->uv.name;
        SPerl_char* complete_name = name->complete_name;
        SPerl_SUB* sub = SPerl_HASH_search(parser->sub_complete_name_symtable, complete_name, strlen(complete_name));
        resolved_type = sub->return_type->resolved_type;
        break;
      }
      case SPerl_OP_C_CODE_GETENUMVALUE: {
        SPerl_NAME* name = op->uv.name;
        SPerl_char* complete_name = name->complete_name;
        SPerl_ENUM_VALUE* enum_value = SPerl_HASH_search(parser->enum_complete_name_symtable, complete_name, strlen(complete_name));
        resolved_type = enum_value->constant->resolved_type;
        break;
      }
      case SPerl_OP_C_CODE_GETFIELD: {
        SPerl_NAME* name = op->uv.name;
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
        SPerl_OP_INFO* op_info = op->uv.op_info;
        resolved_type = op_info->return_resolved_type;
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

void SPerl_OP_resolve_converttype(SPerl_PARSER* parser, SPerl_OP* op_converttype, SPerl_RESOLVED_TYPE* resolved_type_src, SPerl_RESOLVED_TYPE* resolved_type_dist) {
  
  SPerl_int src_id = resolved_type_src->id;
  SPerl_int dist_id = resolved_type_dist->id;
  
  SPerl_int new_code;
  if (src_id == dist_id) {
    new_code = SPerl_OP_C_CODE_NULL;
  }
  else if (src_id != dist_id) {
    if (src_id == SPerl_BODY_CORE_C_CODE_INT) {
      if (dist_id == SPerl_BODY_CORE_C_CODE_LONG) {
        new_code = SPerl_OP_C_CODE_I2L;
      }
      else if (dist_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        new_code = SPerl_OP_C_CODE_I2F;
      }
      else if (dist_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
       new_code = SPerl_OP_C_CODE_I2D;
      }
    }
    else if (src_id == SPerl_BODY_CORE_C_CODE_LONG) {
      if (dist_id == SPerl_BODY_CORE_C_CODE_INT) {
        new_code = SPerl_OP_C_CODE_I2L;
      }
      else if (dist_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        new_code = SPerl_OP_C_CODE_L2F;
      }
      else if (dist_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
        new_code = SPerl_OP_C_CODE_L2D;
      }
    }
    else if (src_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
      if (dist_id == SPerl_BODY_CORE_C_CODE_INT) {
        new_code = SPerl_OP_C_CODE_I2F;
      }
      else if (dist_id == SPerl_BODY_CORE_C_CODE_LONG) {
        new_code = SPerl_OP_C_CODE_L2F;
      }
      else if (dist_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
        new_code = SPerl_OP_C_CODE_F2D;
      }
    }
    else if (src_id == SPerl_BODY_CORE_C_CODE_DOUBLE) {
      if (dist_id == SPerl_BODY_CORE_C_CODE_INT) {
        new_code = SPerl_OP_C_CODE_I2D;
      }
      else if (dist_id == SPerl_BODY_CORE_C_CODE_LONG) {
        new_code = SPerl_OP_C_CODE_L2D;
      }
      else if (dist_id == SPerl_BODY_CORE_C_CODE_FLOAT) {
        new_code = SPerl_OP_C_CODE_F2D;
      }
    }
  }
  SPerl_OP_replace_code(parser, op_converttype, new_code);
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
      SPerl_ARRAY* fields = body_class->fields;
      for (SPerl_int j = 0; j < fields->length; j++) {
        SPerl_FIELD* field = SPerl_ARRAY_fetch(fields, j);

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
    SPerl_ARRAY* my_vars = sub->my_vars;
    for (SPerl_int j = 0; j < my_vars->length; j++) {
      SPerl_MY_VAR* my_var = SPerl_ARRAY_fetch(my_vars, k);
      
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
    SPerl_yyerror_format(parser, "unknown sub \"%s\" at %s line %d\n",
      sub_complete_name, op->file, op->line);
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
    SPerl_yyerror_format(parser, "unknown field \"%s\" at %s line %d\n",
      complete_name, base_name_word->op->file, base_name_word->op->line);
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
    SPerl_yyerror_format(parser, "unknown enum \"%s\" at %s line %d\n",
      complete_name, abs_name_word->op->file, abs_name_word->op->line);
  }
}

SPerl_OP* SPerl_OP_build_getenumvalue(SPerl_PARSER* parser, SPerl_OP* op_enumname) {
  SPerl_OP* op_getenumvalue = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_GETENUMVALUE, op_enumname, NULL);
  
  SPerl_NAME* name = SPerl_NAME_new(parser);
  name->code = SPerl_NAME_C_CODE_ENUM;
  name->abs_name_word = op_enumname->uv.word;
  
  op_getenumvalue->uv.name = name;
  
  return op_getenumvalue;
}

SPerl_OP* SPerl_OP_build_getfield(SPerl_PARSER* parser, SPerl_OP* op_var, SPerl_OP* op_fieldname) {
  SPerl_OP* op_getfield = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_GETFIELD, NULL, NULL);
  SPerl_OP_sibling_splice(parser, op_getfield, NULL, 0, op_var);
  SPerl_OP_sibling_splice(parser, op_getfield, op_var, 0, op_fieldname);
  
  SPerl_NAME* name = SPerl_NAME_new(parser);
  name->code = SPerl_NAME_C_CODE_FIELD;
  name->var = op_var->uv.var;
  name->base_name_word = op_fieldname->uv.word;
  
  op_getfield->uv.name = name;
  
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
  if (parser->fatal_error) {
    return NULL;
  }

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
    SPerl_ARRAY* constants = sub->constants;
    
    // Constant pool length
    SPerl_int const_pool_length = 0;
    for (SPerl_int j = 0; j < constants->length; j++) {
      SPerl_CONSTANT* constant = SPerl_ARRAY_fetch(constants, j);
      switch(constant->code) {
        case SPerl_CONSTANT_C_CODE_BOOLEAN:
        case SPerl_CONSTANT_C_CODE_CHAR:
        case SPerl_CONSTANT_C_CODE_INT:
          const_pool_length += 1;
          break;
        case SPerl_CONSTANT_C_CODE_LONG:
          const_pool_length += 2;
          break;
        case SPerl_CONSTANT_C_CODE_FLOAT:
          const_pool_length += 1;
          break;
        case SPerl_CONSTANT_C_CODE_DOUBLE:
          const_pool_length += 2;
          break;
      }
    }
    sub->const_pool_length = const_pool_length;
    
    // Create constant pool
    SPerl_int* const_pool = calloc(const_pool_length, sizeof(SPerl_int));
    SPerl_int const_pool_pos = 0;
    for (SPerl_int j = 0; j < constants->length; j++) {
      SPerl_CONSTANT* constant = SPerl_ARRAY_fetch(constants, j);
      
      constant->pool_pos = const_pool_pos;
      
      switch(constant->code) {
        case SPerl_CONSTANT_C_CODE_BOOLEAN:
        case SPerl_CONSTANT_C_CODE_CHAR:
        case SPerl_CONSTANT_C_CODE_INT:
          constant->pool_pos = const_pool_pos;
          *(const_pool + const_pool_pos) = constant->uv.int_value;
          const_pool_pos += 1;
          break;
        case SPerl_CONSTANT_C_CODE_LONG:
          constant->pool_pos = const_pool_pos;
          *(SPerl_long*)(const_pool + const_pool_pos) = constant->uv.long_value;
          const_pool_pos += 2;
          break;
        case SPerl_CONSTANT_C_CODE_FLOAT:
          constant->pool_pos = const_pool_pos;
          *(SPerl_float*)(const_pool + const_pool_pos) = constant->uv.float_value;
          const_pool_pos += 1;
          break;
        case SPerl_CONSTANT_C_CODE_DOUBLE:
          constant->pool_pos = const_pool_pos;
          *(SPerl_double*)(const_pool + const_pool_pos) = constant->uv.double_value;
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
  
  SPerl_WORD* package_name_word = op_packagename->uv.word;
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
          enum_value->name_word = op_enumvalue->first->uv.word;
          if (op_enumvalue->last) {
            enum_value->constant = op_enumvalue->last->uv.constant;
          }
          
          SPerl_CONSTANT* constant;
          if (enum_value->constant) {
            constant = enum_value->constant;
            start_value = constant->uv.int_value + 1;
          }
          else {
            constant = SPerl_CONSTANT_new(parser);
            constant->code = SPerl_CONSTANT_C_CODE_INT;
            constant->uv.int_value = start_value;
            constant->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "int", strlen("int"));
            
            enum_value->constant = constant;
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
        SPerl_ARRAY* fields = SPerl_PARSER_new_array(parser, 0);
        SPerl_HASH* field_symtable = SPerl_PARSER_new_hash(parser, 0);
        SPerl_ARRAY* uses = SPerl_PARSER_new_array(parser, 0);
        SPerl_HASH* use_symtable = SPerl_PARSER_new_hash(parser, 0);
        
        // Collect field and use information
        SPerl_OP* op_decls = op_block->first;
        SPerl_OP* op_decl = op_decls->first;
        while (op_decl = SPerl_OP_sibling(parser, op_decl)) {
          // Use
          if (op_decl->code == SPerl_OP_C_CODE_USE) {
            SPerl_OP* op_use = op_decl;
            SPerl_USE* use = op_use->uv.use;
            SPerl_char* use_type_name = use->package_name_word->value;
            SPerl_USE* found_use
              = SPerl_HASH_search(use_symtable, use_type_name, strlen(use_type_name));
            
            if (found_use) {
              SPerl_yyerror_format(parser, "redeclaration of use \"%s\" at %s line %d\n", use_type_name, use->op->file, use->op->line);
            }
            else {
              SPerl_ARRAY_push(parser->use_stack, use);
              SPerl_ARRAY_push(uses, use);
              
              SPerl_HASH_insert(use_symtable, use_type_name, strlen(use_type_name), use);
            }
          }
          // Field
          else if (op_decl->code == SPerl_OP_C_CODE_HAS) {
            SPerl_OP* op_has = op_decl;
            SPerl_FIELD* field = op_has->uv.field;
            SPerl_char* field_name = field->name_word->value;
            SPerl_FIELD* found_field
              = SPerl_HASH_search(field_symtable, field_name, strlen(field_name));
            if (found_field) {
              SPerl_yyerror_format(parser, "redeclaration of has \"%s\" at %s line %d\n", field_name, op_has->file, op_has->line);
            }
            else {
              // Value class only have core type field
              if (body_class->is_value_class) {
                SPerl_boolean is_core_type = SPerl_TYPE_is_core_type_name(parser, field->type);
                if (!is_core_type) {
                  SPerl_yyerror_format(parser, "value class has only core type field at %s line %d\n", op_has->file, op_has->line);
                }
              }
              
              field->body_class = body_class;
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
        SPerl_int i = parser->op_subs->length - 1;
        while (1) {
          if (i < 0) {
            break;
          }
          SPerl_OP* op_sub = SPerl_ARRAY_fetch(parser->op_subs, i);
          SPerl_SUB* sub = op_sub->uv.sub;
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
              SPerl_yyerror_format(parser, "redeclaration of sub \"%s\" at %s line %d\n", sub_complete_name, op_sub->file, op_sub->line);
            }
            // Unknown sub
            else {
              SPerl_HASH_insert(sub_complete_name_symtable, sub_complete_name, strlen(sub_complete_name), sub);
            }
            i--;
          }
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
      type = op_type->uv.type;
      package->type = type;
    }
    
    
    // Add package
    op_package->uv.package = package;
    SPerl_ARRAY_push(parser->op_packages, op_package);
    SPerl_HASH_insert(parser->package_symtable, package_name, strlen(package_name), type);
  }
  
  return op_package;
}

SPerl_OP* SPerl_OP_build_decluse(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_packagename, SPerl_OP* op_packagealias) {
  SPerl_OP_sibling_splice(parser, op_use, NULL, 0, op_packagename);
  SPerl_OP_sibling_splice(parser, op_use, op_packagename, 0, op_packagealias);
  
  SPerl_USE* use = SPerl_USE_new(parser);
  use->package_name_word = op_packagename->uv.word;
  use->op = op_use;
  op_use->uv.use = use;
  
  return op_use;
}

SPerl_OP* SPerl_OP_build_declmy(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_descripters, SPerl_OP* op_type) {
  
  SPerl_OP_sibling_splice(parser, op_my, NULL, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_my, op_descripters, 0, op_type);
  
  SPerl_OP_sibling_splice(parser, op_var, NULL, 0, op_my);
  
  // Create my var information
  SPerl_MY_VAR* my_var = SPerl_MY_VAR_new(parser);
  SPerl_VAR* var = op_var->uv.var;
  my_var->name_word = var->name_word;
  
  // descripters
  my_var->op_descripters = SPerl_OP_create_op_descripters_array(parser, op_descripters);
  
  // type
  my_var->type = op_type->uv.type;
  
  // Add my_var information to op
  op_my->uv.my_var = my_var;
  
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
  field->name_word = op_field_name->uv.word;

  // Descripters
  field->op_descripters = SPerl_OP_create_op_descripters_array(parser, op_descripters);
  
  // Type
  field->type = op_type->uv.type;
  
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
    sub->name_word = op_subname->uv.word;
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
  sub->return_type = op_type->uv.type;
  
  // Save block
  sub->op_block = op_block;
  
  // ID
  sub->id = parser->op_subs->length;
  
  // Add sub information
  SPerl_ARRAY_push(parser->op_subs, op_sub);
  
  op_sub->uv.sub = sub;
  
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
    SPerl_WORD* sub_name_word = op_subname->uv.word;
    SPerl_char* sub_name = sub_name_word->value;
    
    if (strstr(sub_name, ":")) {
      name->abs_name_word = sub_name_word;
    }
    else {
      if (op_invocant->code == SPerl_OP_C_CODE_VAR) {
        name->var = op_invocant->uv.var;
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
  
  op_callsub->uv.name = name;
  
  return op_callsub;
}

SPerl_OP* SPerl_OP_build_callop(SPerl_PARSER* parser, SPerl_OP* op_callop, SPerl_OP* op_first, SPerl_OP* op_last) {
  
  // Build OP_SUB
  SPerl_OP_sibling_splice(parser, op_callop, NULL, 0, op_first);
  if (op_last) {
    SPerl_OP_sibling_splice(parser, op_callop, op_first, 0, op_last);
  }
  
  SPerl_OP_INFO* op_info = SPerl_OP_INFO_new(parser);
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
  op_info->symbol = symbol;
  
  op_callop->uv.op_info = op_info;
  
  return op_callop;
}

SPerl_OP* SPerl_OP_build_wordtype(SPerl_PARSER* parser, SPerl_OP* op_wordtype) {
  SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, op_wordtype, NULL);
  
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_WORD;
  
  SPerl_TYPE_WORD* type_word = SPerl_TYPE_WORD_new(parser);
  type_word->name_word = op_wordtype->uv.word;
  type->uv.type_word = type_word;

  SPerl_ARRAY_push(parser->types, type);
  
  op_type->uv.type = type;
  op_type->file = op_wordtype->file;
  op_type->line = op_wordtype->line;
  
  return op_type;
}

SPerl_OP* SPerl_OP_build_arraytype(SPerl_PARSER* parser, SPerl_OP* op_type) {
  SPerl_OP* op_type_array = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, op_type, NULL);
  
  // Type array
  SPerl_TYPE_ARRAY* type_array = SPerl_TYPE_ARRAY_new(parser);
  type_array->type = op_type->uv.type;
  
  // Type
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_ARRAY;
  type->uv.type_array = type_array;

  SPerl_ARRAY_push(parser->types, type);
  
  op_type_array->uv.type = type;
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
      SPerl_ARRAY_push(argument_types, op_argument_type->uv.type);
    }
  }
  type_sub->return_type = op_return_type->uv.type;
  if (file == NULL) {
    file = op_return_type->file;
    line = op_return_type->line;
  }
  type_sub->argument_types = argument_types;
  
  type->uv.type_sub = type_sub;

  SPerl_ARRAY_push(parser->types, type);
  
  op_type->uv.type = type;
  op_type->file = file;
  op_type->line = line;
  
  return op_type;
}

SPerl_OP* SPerl_OP_newOP(SPerl_PARSER* parser, SPerl_char type, SPerl_OP* first, SPerl_OP* last) {
  return SPerl_OP_newOP_flag(parser, type, first, last, 0, 0);
}

void SPerl_OP_replace_code(SPerl_PARSER* parser, SPerl_OP* op, SPerl_int code) {
  op->code = code;
  op->group = SPerl_OP_get_group(parser, code);
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
