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
#include "sperl_const_value.h"
#include "sperl_field_info.h"
#include "sperl_my_var_info.h"
#include "sperl_var_info.h"
#include "sperl_memory_pool.h"
#include "sperl_use_info.h"
#include "sperl_word_info.h"
#include "sperl_enum_value_info.h"
#include "sperl_descripter_info.h"
#include "sperl_subtype_info.h"
#include "sperl_type_info.h"

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
  "while",
  "enum",
  "enumblock",
  "enumvalue",
  "classblock",
  "descripter",
  "anonsub",
  "type",
};

SPerl_OP* SPerl_OP_build_subtype(SPerl_PARSER* parser, SPerl_OP* op_wordtypes, SPerl_OP* op_wordtype) {
  SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_TYPE_INFO, op_wordtypes, op_wordtype);
  
  SPerl_TYPE_INFO* type_info = SPerl_TYPE_INFO_new(parser);
  type_info->type = SPerl_TYPE_INFO_TYPE_SUBTYPE;
  
  // sub type
  SPerl_SUBTYPE_INFO* subtype_info = SPerl_SUBTYPE_INFO_new(parser);
  subtype_info->return_type_info = op_wordtype->uv.pv;
  SPerl_ARRAY* argument_type_infos = SPerl_PARSER_new_array(parser, 0);
  {
    SPerl_OP* op_wordtype = op_wordtypes->first;
    while (op_wordtype = SPerl_OP_sibling(parser, op_wordtype)) {
      SPerl_ARRAY_push(argument_type_infos, op_wordtype->uv.pv);
    }
  }
  subtype_info->argument_type_infos = argument_type_infos;
  
  type_info->uv.subtype = subtype_info;
  
  op_type->uv.pv = type_info;
  
  return op_type;
}

SPerl_OP* SPerl_OP_build_wordtype(SPerl_PARSER* parser, SPerl_OP* op_word) {
  SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_TYPE_INFO, op_word, NULL);
  
  SPerl_TYPE_INFO* type_info = SPerl_TYPE_INFO_new(parser);
  type_info->type = SPerl_TYPE_INFO_TYPE_WORDTYPE;
  type_info->uv.name = op_word->uv.pv;
  
  op_type->uv.pv = type_info;
  
  return op_type;
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

void SPerl_OP_check(SPerl_PARSER* parser) {
  SPerl_ARRAY* class_infos = parser->class_infos;
  SPerl_HASH* class_info_symtable = parser->class_info_symtable;
  
  // Check field info type and descripter
  for (SPerl_int i = 0; i < class_infos->length; i++) {
    
    SPerl_CLASS_INFO* class_info = SPerl_ARRAY_fetch(class_infos, i);
    
    // Check descripter
    if (class_info->type == SPerl_CLASS_INFO_TYPE_NORMAL) {
      SPerl_ARRAY* descripter_infos = class_info->descripter_infos;
      for (SPerl_int j = 0; j < descripter_infos->length; j++) {
        SPerl_DESCRIPTER_INFO* descripter_info = SPerl_ARRAY_fetch(descripter_infos, j);
        if (descripter_info->type != SPerl_DESCRIPTER_INFO_TYPE_VALUE && descripter_info->type != SPerl_DESCRIPTER_INFO_TYPE_ENUM)
        {
          SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_INFO_type_names[descripter_info->type]));
          sprintf(message, "Error: unknown descripter of package \"%s\" at %s line %d\n",
            SPerl_DESCRIPTER_INFO_type_names[descripter_info->type], descripter_info->op->file, descripter_info->op->line);
          SPerl_yyerror(parser, message);
        }
      }
    }
    
    if (class_info->type == SPerl_CLASS_INFO_TYPE_NORMAL) {
      // Check field
      SPerl_ARRAY* field_infos = class_info->field_infos;
      for (SPerl_int j = 0; j < field_infos->length; j++) {
        // Field type
        SPerl_FIELD_INFO* field_info = SPerl_ARRAY_fetch(field_infos, j);
        if (field_info->type_info->type == SPerl_TYPE_INFO_TYPE_WORDTYPE) {
          SPerl_WORD_INFO* type_name = field_info->type_info->uv.name;
          if (!SPerl_HASH_search(class_info_symtable, type_name->value, strlen(type_name->value))) {
            SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(type_name->value));
            sprintf(message, "Error: unknown type \"%s\" at %s line %d\n", type_name->value, type_name->op->file, type_name->op->line);
            SPerl_yyerror(parser, message);
          }
        }
        
        // Field descripter_infos
        SPerl_ARRAY* descripter_infos = field_info->descripter_infos;
        for (SPerl_int k = 0; k < descripter_infos->length; k++) {
          SPerl_DESCRIPTER_INFO* descripter_info = SPerl_ARRAY_fetch(descripter_infos, k);
          if (descripter_info->type != SPerl_DESCRIPTER_INFO_TYPE_CONST)
          {
            SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_INFO_type_names[descripter_info->type]));
            sprintf(message, "Error: unknown descripter of has \"%s\" at %s line %d\n",
              SPerl_DESCRIPTER_INFO_type_names[descripter_info->type], descripter_info->op->file, descripter_info->op->line);
            SPerl_yyerror(parser, message);
          }
        }
      }
      
      // Check method
      SPerl_ARRAY* method_infos = class_info->method_infos;
      for (SPerl_int j = 0; j < method_infos->length; j++) {
        // Check method type
        SPerl_METHOD_INFO* method_info = SPerl_ARRAY_fetch(method_infos, j);
        if (method_info->return_type_info->type == SPerl_TYPE_INFO_TYPE_WORDTYPE) {
          SPerl_WORD_INFO* return_type_name = method_info->return_type_info->uv.name;
          if (!SPerl_HASH_search(class_info_symtable, return_type_name->value, strlen(return_type_name->value))) {
            SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(return_type_name->value));
            sprintf(message, "Error: unknown type \"%s\" at %s line %d\n", return_type_name->value, return_type_name->op->file, return_type_name->op->line);
            SPerl_yyerror(parser, message);
          }
        }
        
        // Check method descripter_infos
        SPerl_ARRAY* descripter_infos = method_info->descripter_infos;
        SPerl_int k;
        for (SPerl_int k = 0; k < descripter_infos->length; k++) {
          SPerl_DESCRIPTER_INFO* descripter_info = SPerl_ARRAY_fetch(descripter_infos, k);
          if (descripter_info->type != SPerl_DESCRIPTER_INFO_TYPE_STATIC)
          {
            SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_INFO_type_names[descripter_info->type]));
            sprintf(message, "Error: unknown descripter of sub \"%s\" at %s line %d\n",
              SPerl_DESCRIPTER_INFO_type_names[descripter_info->type], descripter_info->op->file, descripter_info->op->line);
            SPerl_yyerror(parser, message);
          }
        }
        
        // Check my var information
        SPerl_ARRAY* my_var_infos = method_info->my_var_infos;
        for (SPerl_int k = 0; k < my_var_infos->length; k++) {
          SPerl_MY_VAR_INFO* my_var_info = SPerl_ARRAY_fetch(my_var_infos, k);

          if (my_var_info->type_info->type == SPerl_TYPE_INFO_TYPE_WORDTYPE) {
            SPerl_WORD_INFO* type_name = my_var_info->type_info->uv.name;
            if (!SPerl_HASH_search(class_info_symtable, type_name->value, strlen(type_name->value))) {
              SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(type_name->value));
              sprintf(message, "Error: unknown type \"%s\" at %s line %d\n", type_name->value, type_name->op->file, type_name->op->line);
              SPerl_yyerror(parser, message);
            }
          }
          
          // Check my_var descripter_infos
          SPerl_ARRAY* descripter_infos = my_var_info->descripter_infos;
          for (SPerl_int l = 0; l < descripter_infos->length; l++) {
            SPerl_DESCRIPTER_INFO* descripter_info = SPerl_ARRAY_fetch(descripter_infos, l);
            if (descripter_info->type != SPerl_DESCRIPTER_INFO_TYPE_CONST)
            {
              SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_INFO_type_names[descripter_info->type]));
              sprintf(message, "Error: unknown descripter of package \"%s\" at %s line %d\n",
                SPerl_DESCRIPTER_INFO_type_names[descripter_info->type], descripter_info->op->file, descripter_info->op->line);
              SPerl_yyerror(parser, message);
            }
          }
        }
      }
    }
  }
}

void SPerl_OP_build_const_pool(SPerl_PARSER* parser) {

  // Set constant informations
  SPerl_ARRAY* const_values = parser->const_values;
  
  // Create constant pool
  for (SPerl_int i = 0; i < const_values->length; i++) {
    
    SPerl_CONST_VALUE* const_value = SPerl_ARRAY_fetch(const_values, i);
    
    const_value->pool_pos = parser->const_pool_pos;
    
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
    switch(const_value->type) {
      case SPerl_CONST_VALUE_TYPE_BOOLEAN:
      case SPerl_CONST_VALUE_TYPE_CHAR:
      case SPerl_CONST_VALUE_TYPE_BYTE:
      case SPerl_CONST_VALUE_TYPE_SHORT:
      case SPerl_CONST_VALUE_TYPE_INT:
        const_value->pool_pos = parser->const_pool_pos;
        *(const_pool + parser->const_pool_pos) = const_value->uv.int_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_VALUE_TYPE_LONG:
        const_value->pool_pos = parser->const_pool_pos;
        *(SPerl_long*)(const_pool + parser->const_pool_pos) = const_value->uv.long_value;
        parser->const_pool_pos += 2;
        break;
      case SPerl_CONST_VALUE_TYPE_FLOAT:
        const_value->pool_pos = parser->const_pool_pos;
        *(SPerl_float*)(const_pool + parser->const_pool_pos) = const_value->uv.float_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_VALUE_TYPE_DOUBLE:
        const_value->pool_pos = parser->const_pool_pos;
        *(SPerl_double*)(const_pool + parser->const_pool_pos) = const_value->uv.double_value;
        parser->const_pool_pos += 2;
        break;
    }
  }
}

SPerl_OP* SPerl_OP_build_PACKAGE(SPerl_PARSER* parser, SPerl_OP* op_package, SPerl_OP* op_pkgname, SPerl_OP* op_type, SPerl_OP* op_descripters, SPerl_OP* op_block) {
  SPerl_int i;
  
  SPerl_OP_sibling_splice(parser, op_package, NULL, 0, op_pkgname);
  SPerl_OP_sibling_splice(parser, op_package, op_pkgname, 0, op_type);
  SPerl_OP_sibling_splice(parser, op_package, op_type, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_package, op_descripters, 0, op_block);
  
  SPerl_char* class_name = ((SPerl_WORD_INFO*)op_pkgname->uv.pv)->value;

  SPerl_HASH* typemap = parser->typemap;

  SPerl_TYPE_INFO* found_typemap = SPerl_HASH_search(parser->typemap, class_name, strlen(class_name));
  
  if (found_typemap) {
    SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(class_name));
    sprintf(message, "Error: redeclaration of package \"%s\" at %s line %d\n", class_name, op_package->file, op_package->line);
    SPerl_yyerror(parser, message);
  }
  else {
    // Class or enum
    if (op_type->type == SPerl_OP_NULL) {
      SPerl_CLASS_INFO* class_info = SPerl_CLASS_INFO_new(parser);
      class_info->name = op_pkgname->uv.pv;
      class_info->op_block = op_block;
      class_info->alias = SPerl_PARSER_new_hash(parser, 0);
      
      class_info->descripter_infos = SPerl_OP_create_descripter_infos(parser, op_descripters);

      // Enum
      if (op_descripters->type == SPerl_OP_ENUM) {
        class_info->type = SPerl_CLASS_INFO_TYPE_ENUM;
      }
      // Normal class
      else {
        class_info->type = SPerl_CLASS_INFO_TYPE_NORMAL;
      }
      
      // Class
      if (class_info->type == SPerl_CLASS_INFO_TYPE_NORMAL) {
        // Search use and field
        SPerl_ARRAY* field_infos = SPerl_PARSER_new_array(parser, 0);
        SPerl_HASH* field_info_symtable = SPerl_PARSER_new_hash(parser, 0);
        SPerl_ARRAY* use_infos = SPerl_PARSER_new_array(parser, 0);
        SPerl_HASH* use_info_symtable = SPerl_PARSER_new_hash(parser, 0);
        
        // Collect field and use information
        SPerl_OP* op_usehassubs = op_block->first;
        SPerl_OP* op_usehassub = op_usehassubs->first;
        while (op_usehassub = SPerl_OP_sibling(parser, op_usehassub)) {
          // Use
          if (op_usehassub->type == SPerl_OP_USE) {
            SPerl_USE_INFO* use_info = op_usehassub->uv.pv;
            SPerl_char* use_class_name = use_info->class_name_word_info->value;
            SPerl_USE_INFO* found_use_info
              = SPerl_HASH_search(use_info_symtable, use_class_name, strlen(use_class_name));
            
            if (found_use_info) {
              SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(use_class_name));
              sprintf(message, "Error: redeclaration of use \"%s\" at %s line %d\n", use_class_name, use_info->op->file, use_info->op->line);
              SPerl_yyerror(parser, message);
            }
            else {
              SPerl_ARRAY_push(parser->use_info_stack, use_info);
              SPerl_ARRAY_push(use_infos, use_info);
              
              if (use_info->alias_name_word_info) {
                SPerl_char* alias_name = use_info->alias_name_word_info->value;
                SPerl_HASH_insert(class_info->alias, alias_name, strlen(alias_name), class_info);
              }
              SPerl_HASH_insert(use_info_symtable, use_class_name, strlen(use_class_name), use_info);
            }
          }
          // Field
          else if (op_usehassub->type == SPerl_OP_HAS) {
            SPerl_FIELD_INFO* field_info = (SPerl_FIELD_INFO*)op_usehassub->uv.pv;
            SPerl_char* field_name = field_info->name_word_info->value;
            SPerl_FIELD_INFO* found_field_info
              = SPerl_HASH_search(field_info_symtable, field_name, strlen(field_name));
            if (found_field_info) {
              SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(field_name));
              sprintf(message, "Error: redeclaration of has \"%s\" at %s line %d\n", field_name, field_info->op->file, field_info->op->line);
              SPerl_yyerror(parser, message);
            }
            else {
              field_info->class_info = class_info;
              field_info->op = op_usehassub;
              SPerl_ARRAY_push(field_infos, field_info);
              SPerl_HASH_insert(field_info_symtable, field_name, strlen(field_name), field_info);
            }
          }
        }
        // Set filed and method information
        class_info->field_infos = field_infos;
        class_info->field_info_symtable = field_info_symtable;
        class_info->use_infos = use_infos;
        class_info->use_info_symtable = use_info_symtable;

        // Method information
        SPerl_HASH* method_info_symtable = SPerl_PARSER_new_hash(parser, 0);
        SPerl_int i;
        for (i = 0; i < parser->current_method_infos->length; i++) {
          SPerl_METHOD_INFO* method_info = SPerl_ARRAY_fetch(parser->current_method_infos, i);
          
          SPerl_METHOD_INFO* found_method_info = NULL;
          SPerl_char* method_name;
          if (!method_info->anon) {
            method_name = method_info->name->value;
            SPerl_METHOD_INFO* found_method_info
              = SPerl_HASH_search(method_info_symtable, method_name, strlen(method_name));
          }
          
          if (found_method_info) {
            SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(method_name));
            sprintf(message, "Error: redeclaration of sub \"%s\" at %s line %d\n", method_name, method_info->op->file, method_info->op->line);
            SPerl_yyerror(parser, message);
          }
          else {
            method_info->class_info = class_info;
            if (!method_info->anon) {
              SPerl_HASH_insert(method_info_symtable, method_name, strlen(method_name), method_info);
            }
          }
        }
        class_info->method_infos = parser->current_method_infos;
        parser->current_method_infos = SPerl_PARSER_new_array(parser, 0);
        class_info->method_info_symtable = method_info_symtable;
      }
      
      // Enum
      else if (class_info->type == SPerl_CLASS_INFO_TYPE_ENUM) {
        // Values
        SPerl_ARRAY* enum_value_infos = SPerl_PARSER_new_array(parser, 0);
        
        // Starting value
        SPerl_long start_value = 0;
        
        SPerl_OP* op_enumvalues = op_block->first;
        SPerl_OP* op_enumvalue = op_enumvalues->first;
        while (op_enumvalue = SPerl_OP_sibling(parser, op_enumvalue)) {
          SPerl_ENUM_VALUE_INFO* enum_value_info = SPerl_ENUM_VALUE_INFO_new(parser);
          enum_value_info->name = op_enumvalue->first->uv.pv;
          if (op_enumvalue->last) {
            enum_value_info->value = op_enumvalue->last->uv.pv;
          }
          
          SPerl_CONST_VALUE* const_value;
          if (enum_value_info->value) {
            const_value = enum_value_info->value;
            start_value = const_value->uv.int_value + 1;
          }
          else {
            const_value = SPerl_CONST_VALUE_new(parser);
            const_value->type = SPerl_CONST_VALUE_TYPE_INT;
            const_value->uv.int_value = start_value;
            enum_value_info->value = const_value;
            start_value++;
          }
          SPerl_ARRAY_push(parser->const_values, const_value);
          SPerl_ARRAY_push(enum_value_infos, enum_value_info);
        }
        
        class_info->enum_value_infos = enum_value_infos;
      }
      
      // Add class information
      SPerl_ARRAY_push(parser->class_infos, class_info);
      SPerl_HASH_insert(parser->class_info_symtable, class_name, strlen(class_name), class_info);

      SPerl_TYPE_INFO* type_info = SPerl_TYPE_INFO_new(parser);
      type_info->type = SPerl_TYPE_INFO_TYPE_WORDTYPE;
      type_info->uv.name = op_pkgname->uv.pv;
      
      SPerl_HASH_insert(parser->typemap, class_name, strlen(class_name), type_info);
    }
    // Typedef
    else {
      // Class name
      SPerl_WORD_INFO* class_name_word_info = op_pkgname->uv.pv;
      SPerl_TYPE_INFO* type_info = op_type->uv.pv;
      SPerl_HASH_insert(parser->typemap, class_name_word_info->value, strlen(class_name_word_info->value), type_info);
      
      // Add use information
      SPerl_OP* op_use = SPerl_OP_newOP(parser, SPerl_OP_USE, NULL, NULL);
      op_use->file = op_type->file;
      op_use->line = op_type->line;
      SPerl_USE_INFO* use_info = SPerl_USE_INFO_new(parser);
      use_info->class_name_word_info = class_name_word_info;
      op_use->uv.pv = use_info;
      SPerl_ARRAY_push(parser->use_info_stack, use_info);
    }
  }
  
  return op_package;
}

SPerl_OP* SPerl_OP_build_USE(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_pkgname, SPerl_OP* op_pkgalias) {
  SPerl_OP_sibling_splice(parser, op_use, NULL, 0, op_pkgname);
  SPerl_OP_sibling_splice(parser, op_use, op_pkgname, 0, op_pkgalias);
  
  SPerl_USE_INFO* use_info = SPerl_USE_INFO_new(parser);
  use_info->class_name_word_info = op_pkgname->uv.pv;
  if (op_pkgalias->type != SPerl_OP_NULL) {
    use_info->alias_name_word_info = op_pkgalias->uv.pv;
  }
  use_info->op = op_use;
  op_use->uv.pv = use_info;
  
  return op_use;
}

SPerl_OP* SPerl_OP_build_MY(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_descripters, SPerl_OP* op_type) {

  SPerl_OP_sibling_splice(parser, op_my, NULL, 0, op_var);
  SPerl_OP_sibling_splice(parser, op_my, op_var, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_my, op_descripters, 0, op_type);
  
  // Create my var information
  SPerl_MY_VAR_INFO* my_var_info = SPerl_MY_VAR_INFO_new(parser);
  SPerl_VAR_INFO* var_info = (SPerl_VAR_INFO*)op_var->uv.pv;
  my_var_info->name_word_info = var_info->name_word_info;
  
  // descripter_infos
  my_var_info->descripter_infos = SPerl_OP_create_descripter_infos(parser, op_descripters);
  
  // type
  my_var_info->type_info = op_type->uv.pv;
  
  // Add my_var information to op
  op_my->uv.pv = my_var_info;
  
  return op_my;
}

SPerl_OP* SPerl_OP_build_HAS(SPerl_PARSER* parser, SPerl_OP* op_has, SPerl_OP* op_field_name, SPerl_OP* op_descripters, SPerl_OP* op_type) {
  
  // Build OP
  SPerl_OP_sibling_splice(parser, op_has, NULL, 0, op_field_name);
  SPerl_OP_sibling_splice(parser, op_has, op_field_name, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_has, op_descripters, 0, op_type);
  
  // Create field information
  SPerl_FIELD_INFO* field_info = SPerl_FIELD_INFO_new(parser);
  
  // Name
  field_info->name_word_info = op_field_name->uv.pv;

  // Descripters
  field_info->descripter_infos = SPerl_OP_create_descripter_infos(parser, op_descripters);
  
  // Type
  field_info->type_info = op_type->uv.pv;
  
  // Set field informaiton
  op_has->uv.pv = field_info;
  
  return op_has;
}

SPerl_OP* SPerl_OP_build_CONST_VALUE(SPerl_PARSER* parser, SPerl_OP* op_const) {

  SPerl_CONST_VALUE* const_value = (SPerl_CONST_VALUE*)op_const->uv.pv;
  SPerl_ARRAY_push(parser->const_values, const_value);
  
  return op_const;
}

SPerl_ARRAY* SPerl_OP_create_descripter_infos(SPerl_PARSER* parser, SPerl_OP* op_descripters) {
  
  SPerl_ARRAY* descripter_infos = SPerl_PARSER_new_array(parser, 0);

  // descripter_infos is enum or list of descripter
  if (op_descripters->type == SPerl_OP_ENUM) {
    SPerl_DESCRIPTER_INFO* descripter_info = SPerl_DESCRIPTER_INFO_new(parser);
    descripter_info->type = SPerl_DESCRIPTER_INFO_TYPE_ENUM;
    SPerl_ARRAY_push(descripter_infos, descripter_info);
  }
  else {
    SPerl_OP* op_descripter = op_descripters->first;
    while (op_descripter = op_descripter->moresib ? op_descripter->sibparent : NULL) {
      SPerl_ARRAY_push(descripter_infos, op_descripter->uv.pv);
    }
  }
  
  return descripter_infos;
}

SPerl_OP* SPerl_OP_build_SUB(SPerl_PARSER* parser, SPerl_OP* op_sub, SPerl_OP* op_subname, SPerl_OP* op_subargs, SPerl_OP* op_descripters, SPerl_OP* op_type, SPerl_OP* op_block) {
  
  // Build OP_SUB
  SPerl_OP_sibling_splice(parser, op_sub, NULL, 0, op_subname);
  SPerl_OP_sibling_splice(parser, op_sub, op_subname, 0, op_subargs);
  SPerl_OP_sibling_splice(parser, op_sub, op_subargs, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_sub, op_descripters, 0, op_type);
  SPerl_OP_sibling_splice(parser, op_sub, op_type, 0, op_block);
  
  // Create method infomation
  SPerl_METHOD_INFO* method_info = SPerl_METHOD_INFO_new(parser);
  if (op_subname->type == SPerl_OP_NULL) {
    method_info->anon = 1;
  }
  else {
    method_info->name = op_subname->uv.pv;
  }
  
  // subargs
  SPerl_int argument_count = 0;
  SPerl_OP* op_subarg = op_subargs->first;
  while (op_subarg = SPerl_OP_sibling(parser, op_subarg)) {
    // Increment argument count
    argument_count++;
  }
  method_info->argument_count = argument_count;

  // descripter_infos
  method_info->descripter_infos = SPerl_OP_create_descripter_infos(parser, op_descripters);
  
  // return type
  method_info->return_type_info = op_type->uv.pv;
  
  // Save block
  method_info->op_block = op_block;
  
  // Add method information
  SPerl_ARRAY_push(parser->current_method_infos, method_info);
  
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
    if (op_cur->type == SPerl_OP_VAR) {
      SPerl_VAR_INFO* var_info = (SPerl_VAR_INFO*)op_cur->uv.pv;
      
      // Serach same name variable
      SPerl_MY_VAR_INFO* my_var_info = NULL;
      for (SPerl_int i = my_var_stack->length - 1 ; i >= 0; i--) {
        SPerl_MY_VAR_INFO* my_var_info_tmp = SPerl_ARRAY_fetch(my_var_stack, i);
        if (strcmp(var_info->name_word_info->value, my_var_info_tmp->name_word_info->value) == 0) {
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
        SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(var_info->name_word_info->value));
        sprintf(message, "Error: \"%s\" undeclared at %s line %d\n", var_info->name_word_info->value, op_cur->file, op_cur->line);
        SPerl_yyerror(parser, message);
      }
    }
    else if (op_cur->type == SPerl_OP_MY) {
      SPerl_MY_VAR_INFO* my_var_info = (SPerl_MY_VAR_INFO*)op_cur->uv.pv;
      
      // Serach same name variable
      SPerl_int found = 0;
      
      for (SPerl_int i = my_var_stack->length - 1 ; i >= block_base; i--) {
        SPerl_MY_VAR_INFO* bef_my_var_info = SPerl_ARRAY_fetch(my_var_stack, i);
        if (strcmp(my_var_info->name_word_info->value, bef_my_var_info->name_word_info->value) == 0) {
          found = 1;
          break;
        }
      }
      
      if (found) {
        SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(my_var_info->name_word_info->value));
        sprintf(message, "Error: redeclaration of my \"%s\" at %s line %d\n", my_var_info->name_word_info->value, op_cur->file, op_cur->line);
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
    else if (op_cur->type == SPerl_OP_BLOCK) {
      if (block_start) {
        SPerl_int* block_base_ptr = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_int));
        *block_base_ptr = my_var_stack->length;
        SPerl_ARRAY_push(block_base_stack, block_base_ptr);
      }
      else {
        block_start = 1;
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
                for (SPerl_int j = 0; j < my_var_stack->length - block_base; j++) {
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

SPerl_OP* SPerl_OP_newOP_LIST(SPerl_PARSER* parser) {
  
  SPerl_OP* op_pushmark = SPerl_OP_newOP(parser, SPerl_OP_PUSHMARK, NULL, NULL);
  
  return SPerl_OP_newOP(parser, SPerl_OP_LIST, op_pushmark, NULL);
}

SPerl_OP* SPerl_OP_newOP_NULL(SPerl_PARSER* parser) {
  
  return SPerl_OP_newOP(parser, SPerl_OP_NULL, NULL, NULL);
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
