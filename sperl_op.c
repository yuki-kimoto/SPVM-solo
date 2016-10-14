#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"
#include "sperl_op.h"
#include "sperl_method_info.h"
#include "sperl_class_info.h"
#include "sperl_parser.h"
#include "sperl_const_info.h"
#include "sperl_field_info.h"
#include "sperl_my_var_info.h"
#include "sperl_var_info.h"
#include "sperl_memory_pool.h"
#include "sperl_use_info.h"

/* sperl_op.h */
SPerl_char* const SPerl_OP_names[] = {
  "null",
  "const",
  "lt",
  "le",
  "gt",
  "ge",
  "add",
  "subtract",
  "multiply",
  "divide",
  "and",
  "or",
  "bit_and",
  "bit_or",
  "modulo",
  "bit_xor",
  "not",
  "left_shift",
  "right_shift",
  "inc",
  "dec",
  "bit_not",
  "preinc",
  "postinc",
  "predec",
  "postdec",
  "complement",
  "negate",
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
  "eq",
  "ne",
  "statement",
  "grammer",
  "desctype",
  "word",
  "if",
  "elsif",
  "else",
  "for",
  "while"
};

static SPerl_boolean _is_core_type (SPerl_char* type_name) {
  if (strcmp(type_name, "boolean") == 0 || strcmp(type_name, "char") == 0 || strcmp(type_name, "byte") == 0
    || strcmp(type_name, "short") == 0 || strcmp(type_name, "int") == 0 || strcmp(type_name, "long") == 0
    || strcmp(type_name, "float") == 0 || strcmp(type_name, "double") == 0)
  {
    return 1;
  }
  else {
    return 0;
  }
}

void SPerl_OP_check(SPerl_PARSER* parser) {
  SPerl_ARRAY* class_infos = parser->class_infos;
  SPerl_HASH* class_info_symtable = parser->class_info_symtable;
  
  // Check field info type and descripter
  SPerl_int i;
  for (i = 0; i < class_infos->length; i++) {
    SPerl_CLASS_INFO* class_info = SPerl_ARRAY_fetch(class_infos, i);
    SPerl_ARRAY* field_infos = class_info->field_infos;
    SPerl_int j;
    for (j = 0; j < field_infos->length; j++) {
      SPerl_FIELD_INFO* field_info = SPerl_ARRAY_fetch(field_infos, j);
      if (
        !_is_core_type(field_info->type)
        && !SPerl_HASH_search(class_info_symtable, field_info->type, strlen(field_info->type))
        )
      {
        SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(field_info->type));
        sprintf(message, "Error: unknown type \"%s\" at %s line %d\n", field_info->type, field_info->op->file, field_info->op->line);
        SPerl_yyerror(parser, message);
      }
      
      // if (field_info->desc_flag && SPerl_DESCRIPTER_CONST || field_info->desc_flags && SPerl_DESCRIPTER_STATIC) {
      //   SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(field_info->type));
      //   sprintf(message, "Error: unknown descripter of has \"%s\" at %s line %d\n", field_info->type, field_info->op->file, field_info->op->line);
      //   SPerl_yyerror(parser, message);
      // }
    }
  }
}

void SPerl_OP_build_const_pool(SPerl_PARSER* parser) {

  // Set constant informations
  SPerl_ARRAY* const_infos = parser->const_infos;
  
  // Create constant pool
  SPerl_int i;
  for (i = 0; i < const_infos->length; i++) {
    SPerl_CONST_INFO* const_info = SPerl_ARRAY_fetch(const_infos, i);
    
    const_info->pool_pos = parser->const_pool_pos;
    
    // Realloc
    if (parser->const_pool_pos - 1 >= parser->const_pool_capacity) {
      
      SPerl_int new_const_pool_capacity = parser->const_pool_capacity * 2;
      parser->const_pool = realloc(parser->const_pool, new_const_pool_capacity * sizeof(SPerl_int));
      memset(
        parser->const_pool + parser->const_pool_capacity,
        0,
        (new_const_pool_capacity - parser->const_pool_capacity) * sizeof(SPerl_int)
      );
      parser->const_pool_capacity = new_const_pool_capacity;
    }
    
    SPerl_int* const_pool = parser->const_pool;
    switch(const_info->type) {
      case SPerl_CONST_INFO_BOOLEAN:
      case SPerl_CONST_INFO_CHAR:
      case SPerl_CONST_INFO_BYTE:
      case SPerl_CONST_INFO_SHORT:
      case SPerl_CONST_INFO_INT:
        const_info->pool_pos = parser->const_pool_pos;
        *(const_pool + parser->const_pool_pos) = const_info->uv.int_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_INFO_LONG:
        const_info->pool_pos = parser->const_pool_pos;
        *(SPerl_long*)(const_pool + parser->const_pool_pos) = const_info->uv.long_value;
        parser->const_pool_pos += 2;
        break;
      case SPerl_CONST_INFO_FLOAT:
        const_info->pool_pos = parser->const_pool_pos;
        *(SPerl_float*)(const_pool + parser->const_pool_pos) = const_info->uv.float_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_INFO_DOUBLE:
        const_info->pool_pos = parser->const_pool_pos;
        *(SPerl_double*)(const_pool + parser->const_pool_pos) = const_info->uv.double_value;
        parser->const_pool_pos += 2;
        break;
    }
  }
}


SPerl_OP* SPerl_OP_build_GRAMMER(SPerl_PARSER* parser, SPerl_OP* op_packages) {
  SPerl_OP* op_grammer = SPerl_OP_newOP(parser, SPerl_OP_GRAMMER, op_packages, NULL);
  parser->op_grammer = op_grammer;
  
  // Name and type check
  SPerl_OP_check(parser);
  
  // Build constant pool
  SPerl_OP_build_const_pool(parser);

  return op_grammer;
}

SPerl_OP* SPerl_OP_build_PACKAGE(SPerl_PARSER* parser, SPerl_OP* op_package, SPerl_OP* op_pkgname, SPerl_OP* op_block, SPerl_OP* op_descripters) {
  SPerl_int i;
  
  SPerl_OP_sibling_splice(parser, op_package, NULL, 0, op_pkgname);
  SPerl_OP_sibling_splice(parser, op_package, op_pkgname, 0, op_block);
  if (op_descripters) {
    SPerl_OP_sibling_splice(parser, op_package, op_block, 0, op_descripters);
  }
  
  SPerl_char* class_name = op_pkgname->uv.pv;
  SPerl_CLASS_INFO* found_class_info = SPerl_HASH_search(
    parser->class_info_symtable,
    class_name,
    strlen(class_name)
  );
  
          
  if (found_class_info) {
    SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(class_name));
    sprintf(message, "Error: redeclaration of package \"%s\" at %s line %d\n", class_name, op_package->file, op_package->line);
    SPerl_yyerror(parser, message);
  }
  else {

    SPerl_CLASS_INFO* class_info = SPerl_CLASS_INFO_new(parser);
    class_info->name = class_name;
    class_info->op_block = op_block;
    class_info->alias = SPerl_PARSER_new_hash(parser, 0);
    
    // Search field and methods
    SPerl_ARRAY* field_infos = SPerl_PARSER_new_array(parser, 0);;
    SPerl_HASH* field_info_symtable = SPerl_PARSER_new_hash(parser, 0);
    SPerl_ARRAY* method_infos = SPerl_PARSER_new_array(parser, 0);;
    SPerl_HASH* method_info_symtable = SPerl_PARSER_new_hash(parser, 0);
    
    // Run ast
    SPerl_ARRAY* op_stack = SPerl_PARSER_new_array(parser, 0);
    SPerl_OP* op_cur = op_package;
    while (op_cur) {
      SPerl_OP* op_first = NULL;
      // Field
      if (op_cur->type == SPerl_OP_HAS) {
        SPerl_FIELD_INFO* field_info = (SPerl_FIELD_INFO*)op_cur->uv.pv;
        SPerl_char* field_name = field_info->name;
        SPerl_CLASS_INFO* found_field_info
          = SPerl_HASH_search(field_info_symtable, field_name, strlen(field_name));
        if (found_field_info) {
          SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(field_name));
          sprintf(message, "Error: redeclaration of has \"%s\" at %s line %d\n", field_name, op_cur->file, op_cur->line);
          SPerl_yyerror(parser, message);
        }
        else {
          field_info->class_info = class_info;
          field_info->op = op_cur;
          SPerl_ARRAY_push(field_infos, field_info);
          SPerl_HASH_insert(field_info_symtable, field_name, strlen(field_name), field_info);
        }
      }
      // Method
      else if (op_cur->type == SPerl_OP_SUB) {
        SPerl_METHOD_INFO* method_info = op_cur->uv.pv;
        SPerl_char* method_name = method_info->name;
        SPerl_METHOD_INFO* found_method_info
          = SPerl_HASH_search(method_info_symtable, method_name, strlen(method_name));
        
        if (found_method_info) {
          SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(method_name));
          sprintf(message, "Error: redeclaration of sub \"%s\" at %s line %d\n", method_name, op_cur->file, op_cur->line);
          SPerl_yyerror(parser, message);
        }
        else {
          method_info->class_info = class_info;
          SPerl_ARRAY_push(method_infos, method_info);
          SPerl_HASH_insert(method_info_symtable, method_name, strlen(method_name), method_info);
        }
      }
      else if (op_cur->type == SPerl_OP_USE) {
        SPerl_USE_INFO* use_info = (SPerl_USE_INFO*)op_cur->uv.pv;
        
        SPerl_char* class_name = use_info->class_name;
        SPerl_ARRAY_push(parser->class_stack, class_name);
        
        SPerl_char* alias_name = use_info->alias_name;
        if (alias_name) {
          SPerl_HASH_insert(class_info->alias, alias_name, strlen(alias_name), class_info);
        }
      }
      else {
        op_first = op_cur->first;
      }
      
      if (op_first) {
        SPerl_ARRAY_push(op_stack, op_cur);
        op_cur = op_first;
      }
      else {
        SPerl_OP* op_sib = SPerl_OP_sibling(parser, op_cur);
        
        // Next sibling
        if (op_sib) {
          op_cur = op_sib;
        }
        // Next is parent
        else {
          SPerl_OP* op_parent;
          while (1) {
            op_parent = (SPerl_OP*)SPerl_ARRAY_pop(op_stack);
            if (op_parent) {
              SPerl_OP* op_parent_sib = SPerl_OP_sibling(parser, op_parent);
              if (op_parent_sib) {
                // Next is parent's sibling
                op_cur = op_parent_sib;
                break;
              }
            }
            else {
              op_cur = NULL;
              break;
            }
          }
        }
      }
    }
    
    // Set filed and method information
    class_info->field_infos = field_infos;
    class_info->field_info_symtable = field_info_symtable;
    class_info->method_infos = method_infos;
    class_info->method_info_symtable = method_info_symtable;
   
    // Add class information
    SPerl_ARRAY_push(parser->class_infos, class_info);
    SPerl_HASH_insert(parser->class_info_symtable, class_name, strlen(class_name), class_info);
  }
  
  return op_package;
}

SPerl_OP* SPerl_OP_build_USE(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_pkgname, SPerl_OP* op_pkgalias) {
  SPerl_OP_sibling_splice(parser, op_use, NULL, 0, op_pkgname);
  if (op_pkgalias) {
    SPerl_OP_sibling_splice(parser, op_use, op_pkgname, 0, op_pkgalias);
  }
  
  SPerl_USE_INFO* use_info = SPerl_USE_INFO_new(parser);
  use_info->class_name = op_pkgname->uv.pv;
  if (op_pkgalias) {
    use_info->alias_name = op_pkgalias->uv.pv;
  }
  op_use->uv.pv = use_info;
  
  return op_use;
}

SPerl_OP* SPerl_OP_build_MY(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_desctype) {
  if (!op_my) {
    op_my = SPerl_OP_newOP(parser, SPerl_OP_MY, NULL, NULL);
  }
  SPerl_OP_sibling_splice(parser, op_my, NULL, 0, op_var);
  SPerl_OP_sibling_splice(parser, op_my, op_var, 0, op_desctype);
  
  // Create my var information
  SPerl_MY_VAR_INFO* my_var_info = SPerl_MY_VAR_INFO_new(parser);
  SPerl_VAR_INFO* var_info = (SPerl_VAR_INFO*)op_var->uv.pv;
  my_var_info->name = var_info->name;
  
  // type
  my_var_info->type = op_desctype->first->uv.pv;
  
  // descripters
  SPerl_OP* op_descripters = op_desctype->last;
  my_var_info->descripters = SPerl_OP_create_descripters(parser, op_descripters);
  
  // Add my_var information to op
  op_my->uv.pv = my_var_info;
  
  return op_my;
}

SPerl_OP* SPerl_OP_build_HAS(SPerl_PARSER* parser, SPerl_OP* op_has, SPerl_OP* op_field_name, SPerl_OP* op_desctype) {
  SPerl_OP_sibling_splice(parser, op_has, NULL, 0, op_field_name);
  SPerl_OP_sibling_splice(parser, op_has, op_field_name, 0, op_desctype);
  
  // Create field information
  SPerl_FIELD_INFO* field_info = SPerl_FIELD_INFO_new(parser);
  field_info->name = op_field_name->uv.pv;
  
  // type
  field_info->type = op_desctype->first->uv.pv;
  
  // descripters
  SPerl_OP* op_descripters = op_desctype->last;
  field_info->descripters = SPerl_OP_create_descripters(parser, op_descripters);
  
  op_has->uv.pv = field_info;
  
  return op_has;
}

SPerl_OP* SPerl_OP_build_CONST(SPerl_PARSER* parser, SPerl_OP* op_const) {
  
  SPerl_CONST_INFO* const_info = (SPerl_CONST_INFO*)op_const->uv.pv;
  SPerl_ARRAY_push(parser->const_infos, const_info);
  
  return op_const;
}

SPerl_ARRAY* SPerl_OP_create_descripters(SPerl_PARSER* parser, SPerl_OP* op_descripters) {
  
  SPerl_ARRAY* descripters = SPerl_PARSER_new_array(parser, 0);
  
  if (!op_descripters) {
    return descripters;
  }
  
  // descripters is list of descripter or descripter
  if (op_descripters->type == SPerl_OP_LIST || op_descripters->type == SPerl_OP_WORD) {
    SPerl_OP* op_next;
    if (op_descripters->type == SPerl_OP_LIST) {
      op_next = SPerl_OP_sibling(parser, op_descripters->first);
    }
    else {
      op_next = op_descripters;
    }
    
    while (op_next) {
      SPerl_char* descripter = op_next->uv.pv;
      SPerl_ARRAY_push(descripters, descripter);
      op_next = SPerl_OP_sibling(parser, op_next);
    }
  }
  
  return descripters;
}

SPerl_OP* SPerl_OP_build_SUB(SPerl_PARSER* parser, SPerl_OP* op_sub, SPerl_OP* op_subname, SPerl_OP* op_optsubargs, SPerl_OP* op_desctype, SPerl_OP* op_block) {
  
  // Build OP_SUB
  if (!op_optsubargs) {
    op_optsubargs = SPerl_OP_newOP(parser, SPerl_OP_NULL, NULL, NULL);
  }
  SPerl_OP_sibling_splice(parser, op_sub, NULL, 0, op_subname);
  SPerl_OP_sibling_splice(parser, op_sub, op_subname, 0, op_optsubargs);
  SPerl_OP_sibling_splice(parser, op_sub, op_optsubargs, 0, op_desctype);
  SPerl_OP_sibling_splice(parser, op_sub, op_desctype, 0, op_block);
  
  // Create method infomation
  SPerl_METHOD_INFO* method_info = SPerl_METHOD_INFO_new(parser);
  method_info->name = op_subname->uv.pv;
  
  // subargs
  // subargs is NULL
  if (op_optsubargs->type == SPerl_OP_NULL) {
    method_info->argument_count = 0;
  }
  // subargs is subarg
  else if (op_optsubargs->type == SPerl_OP_MY) {
    
    // Argument count
    method_info->argument_count = 1;
  }
  // subargs is list of subarg
  else if (op_optsubargs->type == SPerl_OP_LIST) {
    SPerl_int argument_count = 0;
    SPerl_OP* op_subarg = op_optsubargs->first;
    while (op_subarg = SPerl_OP_sibling(parser, op_subarg)) {
      // Increment argument count
      argument_count++;
    }
    method_info->argument_count = argument_count;
  }
  
  // return type
  method_info->return_type = op_desctype->first->uv.pv;
  
  // descripters
  SPerl_OP* op_descripters = op_desctype->last;
  method_info->descripters = SPerl_OP_create_descripters(parser, op_descripters);
  
  // Save block
  method_info->op_block = op_block;
  
  // my var informations
  SPerl_ARRAY* my_var_infos = SPerl_PARSER_new_array(parser, 0);
  SPerl_HASH* my_var_info_symtable = SPerl_PARSER_new_hash(parser, 0);
  
  // my variable stack
  SPerl_ARRAY* my_var_stack = SPerl_PARSER_new_array(parser, 0);
  
  // block base position stack
  SPerl_ARRAY* block_base_stack = SPerl_PARSER_new_array(parser, 0);
  
  // Run in AST
  SPerl_ARRAY* op_stack = SPerl_PARSER_new_array(parser, 0);
  SPerl_OP* op_cur = op_sub;
  SPerl_boolean block_start;
  while (op_cur) {
    // Current block base
    SPerl_int block_base;
    if (block_base_stack->length == 0) {
      block_base = 0;
    }
    else {
      SPerl_int* block_base_ptr = SPerl_ARRAY_fetch(block_base_stack, block_base_stack->length - 1);
      block_base = *block_base_ptr;
    }
    
    // Add my var
    if (op_cur->type == SPerl_OP_BLOCK) {
      if (block_start) {
        SPerl_int* block_base_ptr = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_int));
        *block_base_ptr = my_var_stack->length;
        SPerl_ARRAY_push(block_base_stack, block_base_ptr);
      }
      else {
        block_start = 1;
      }
    }
    else if (op_cur->type == SPerl_OP_MY) {
      SPerl_MY_VAR_INFO* my_var_info = (SPerl_MY_VAR_INFO*)op_cur->uv.pv;
      
      // Serach same name variable
      SPerl_int found = 0;
      SPerl_int i;
      
      for (i = my_var_stack->length - 1 ; i >= block_base; i--) {
        SPerl_MY_VAR_INFO* bef_my_var_info = SPerl_ARRAY_fetch(my_var_stack, i);
        if (strcmp(my_var_info->name, bef_my_var_info->name) == 0) {
          found = 1;
          break;
        }
      }
      
      if (found) {
        SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(my_var_info->name));
        sprintf(message, "Error: redeclaration of my \"%s\" at %s line %d\n", my_var_info->name, op_cur->file, op_cur->line);
        SPerl_yyerror(parser, message);
      }
      else {
        // Add my var information
        my_var_info->id = parser->next_var_id++;
        SPerl_ARRAY_push(my_var_infos, my_var_info);
        my_var_info->method_info = method_info;
        
        SPerl_ARRAY_push(my_var_stack, my_var_info);
      }
    }
    else if (op_cur->type == SPerl_OP_VAR) {
      SPerl_VAR_INFO* var_info = (SPerl_VAR_INFO*)op_cur->uv.pv;
      
      // Serach same name variable
      SPerl_int i;
      SPerl_MY_VAR_INFO* my_var_info = NULL;
      for (i = my_var_stack->length - 1 ; i >= 0; i--) {
        SPerl_MY_VAR_INFO* my_var_info_tmp = SPerl_ARRAY_fetch(my_var_stack, i);
        if (strcmp(var_info->name, my_var_info_tmp->name) == 0) {
          my_var_info = my_var_info_tmp;
          break;
        }
      }
      
      if (my_var_info) {
        // Add my var information to var
        var_info->my_var_info = my_var_info;
      }
      else {
        // Error
        SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(var_info->name));
        sprintf(message, "Error: \"%s\" undeclared at %s line %d\n", var_info->name, op_cur->file, op_cur->line);
        SPerl_yyerror(parser, message);
      }
    }
    
    if (op_cur->first) {
      SPerl_ARRAY_push(op_stack, op_cur);
      op_cur = op_cur->first;
    }
    else {
      SPerl_OP* op_sib = SPerl_OP_sibling(parser, op_cur);
      
      // Next sibling
      if (op_sib) {
        op_cur = op_sib;
      }
      // Next is parent
      else {
        SPerl_OP* op_parent;
        while (1) {
          op_parent = (SPerl_OP*)SPerl_ARRAY_pop(op_stack);
          if (op_parent) {
            
            // End of scope
            if (op_parent->type == SPerl_OP_BLOCK) {
              SPerl_int* block_base_ptr = SPerl_ARRAY_pop(block_base_stack);
              if (block_base_ptr) {
                SPerl_int block_base = *block_base_ptr;
                SPerl_int j;
                for (j = 0; j < my_var_stack->length - block_base; j++) {
                  SPerl_ARRAY_pop(my_var_stack);
                }
              }
            }
            
            SPerl_OP* op_parent_sib = SPerl_OP_sibling(parser, op_parent);
            if (op_parent_sib) {
              // Next is parent's sibling
              op_cur = op_parent_sib;
              break;
            }
          }
          else {
            op_cur = NULL;
            break;
          }
        }
      }
    }
  }
  
  // Set my var information
  method_info->my_var_infos = my_var_infos;
  
  op_sub->uv.pv = method_info;
  
  return op_sub;
}

SPerl_OP* SPerl_OP_newOP_LIST(SPerl_PARSER* parser) {
  
  SPerl_OP* op_pushmark = SPerl_OP_newOP(parser, SPerl_OP_PUSHMARK, NULL, NULL);
  
  return SPerl_OP_newOP(parser, SPerl_OP_LIST, op_pushmark, NULL);
}

SPerl_OP* SPerl_OP_newOP(SPerl_PARSER* parser, SPerl_char type, SPerl_OP* first, SPerl_OP* last) {
  return SPerl_OP_newOP_flag(parser, type, first, last, 0, 0);
}

SPerl_OP* SPerl_OP_newOP_flag(SPerl_PARSER* parser, SPerl_char type, SPerl_OP* first, SPerl_OP* last, SPerl_char flags, SPerl_char private) {
        
  SPerl_OP *op = (SPerl_OP*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_OP));
  
  memset(op, 0, sizeof(SPerl_OP));
  
  op->type = type;
  op->first = first;
  op->flags = flags;
  op->private = private;
  
  if (last) {
    if (!first) {
      first = (SPerl_OP*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_OP));
      first->type = SPerl_OP_NULL;
    }
    
    op->last = last;
    SPerl_OP_moresib_set(parser, first, last);
    if (op->last)
      SPerl_OP_lastsib_set(parser, op->last, (SPerl_OP*)op);
  }
  else if (first) {
    SPerl_OP_lastsib_set(parser, op->first, (SPerl_OP*)op);
  }

  return (SPerl_OP *)op;
}

SPerl_OP* SPerl_OP_append_elem(SPerl_PARSER* parser, SPerl_OP *first, SPerl_OP *last) {
  if (!first)
    return last;

  if (!last)
    return first;
  
  if (first->type == SPerl_OP_LIST) {
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
