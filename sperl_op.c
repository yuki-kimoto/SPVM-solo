#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"
#include "sperl_yacc.h"
#include "sperl_op.h"
#include "sperl_method.h"
#include "sperl_class.h"
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
#include "sperl_subtype.h"
#include "sperl_type.h"

/* sperl_op.h */
SPerl_char* const SPerl_OP_C_NAMES[] = {
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
  "typedef"
};

SPerl_OP* SPerl_OP_build_subtype(SPerl_PARSER* parser, SPerl_OP* op_wordtypes, SPerl_OP* op_wordtype) {
  SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_C_TYPE_TYPEOP, op_wordtypes, op_wordtype);
  
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->type = SPerl_TYPE_C_SUB;
  
  // sub type
  SPerl_SUBTYPE* subtype = SPerl_SUBTYPE_new(parser);
  subtype->return_type = op_wordtype->uv.pv;
  SPerl_ARRAY* argument_types = SPerl_PARSER_new_array(parser, 0);
  {
    SPerl_OP* op_wordtype = op_wordtypes->first;
    while (op_wordtype = SPerl_OP_sibling(parser, op_wordtype)) {
      SPerl_ARRAY_push(argument_types, op_wordtype->uv.pv);
    }
  }
  subtype->argument_types = argument_types;
  
  type->uv.subtype = subtype;
  
  op_type->uv.pv = type;
  
  return op_type;
}

SPerl_OP* SPerl_OP_build_wordtype(SPerl_PARSER* parser, SPerl_OP* op_word) {
  SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_C_TYPE_TYPEOP, op_word, NULL);
  
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->type = SPerl_TYPE_C_CLASS_OR_TYPEDEF;
  type->name_word = op_word->uv.pv;
  
  op_type->uv.pv = type;
  
  return op_type;
}

SPerl_OP* SPerl_OP_build_GRAMMER(SPerl_PARSER* parser, SPerl_OP* op_packages) {
  SPerl_OP* op_grammer = SPerl_OP_newOP(parser, SPerl_OP_C_TYPE_GRAMMER, op_packages, NULL);
  parser->op_grammer = op_grammer;


  // Name and type check
  SPerl_OP_check(parser);

  // Build constant pool
  SPerl_OP_build_const_pool(parser);

  return op_grammer;
}

void SPerl_OP_check(SPerl_PARSER* parser) {
  SPerl_ARRAY* classs = parser->classs;
  SPerl_HASH* class_symtable = parser->class_symtable;
  
  // Check field info type and descripter
  for (SPerl_int i = 0; i < classs->length; i++) {
    
    SPerl_CLASS* class = SPerl_ARRAY_fetch(classs, i);
    
    // Check descripter
    if (class->type == SPerl_CLASS_C_TYPE_NORMAL) {
      SPerl_ARRAY* descripters = class->descripters;
      for (SPerl_int j = 0; j < descripters->length; j++) {
        SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, j);
        if (descripter->type != SPerl_DESCRIPTER_C_TYPE_VALUE && descripter->type != SPerl_DESCRIPTER_C_TYPE_ENUM)
        {
          SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_type_names[descripter->type]));
          sprintf(message, "Error: unknown descripter of package \"%s\" at %s line %d\n",
            SPerl_DESCRIPTER_type_names[descripter->type], descripter->op->file, descripter->op->line);
          SPerl_yyerror(parser, message);
        }
      }
    }
    
    if (class->type == SPerl_CLASS_C_TYPE_NORMAL) {
      // Check field
      SPerl_ARRAY* fields = class->fields;
      for (SPerl_int j = 0; j < fields->length; j++) {
        // Field type
        SPerl_FIELD* field = SPerl_ARRAY_fetch(fields, j);
        if (field->type->type == SPerl_TYPE_C_CLASS_OR_TYPEDEF) {
          SPerl_WORD* type_name_word = field->type->name_word;
          if (!SPerl_HASH_search(class_symtable, type_name_word->value, strlen(type_name_word->value))) {
            SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(type_name_word->value));
            sprintf(message, "Error: unknown type \"%s\" at %s line %d\n", type_name_word->value, type_name_word->op->file, type_name_word->op->line);
            SPerl_yyerror(parser, message);
          }
        }
        
        // Field descripters
        SPerl_ARRAY* descripters = field->descripters;
        for (SPerl_int k = 0; k < descripters->length; k++) {
          SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, k);
          if (descripter->type != SPerl_DESCRIPTER_C_TYPE_CONST)
          {
            SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_type_names[descripter->type]));
            sprintf(message, "Error: unknown descripter of has \"%s\" at %s line %d\n",
              SPerl_DESCRIPTER_type_names[descripter->type], descripter->op->file, descripter->op->line);
            SPerl_yyerror(parser, message);
          }
        }
      }
      
      // Check method
      SPerl_ARRAY* methods = class->methods;
      for (SPerl_int j = 0; j < methods->length; j++) {
        // Check method type
        SPerl_METHOD* method = SPerl_ARRAY_fetch(methods, j);
        if (method->return_type->type == SPerl_TYPE_C_CLASS_OR_TYPEDEF) {
          SPerl_WORD* return_type_name_word = method->return_type->name_word;
          if (!SPerl_HASH_search(class_symtable, return_type_name_word->value, strlen(return_type_name_word->value))) {
            SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(return_type_name_word->value));
            sprintf(message, "Error: unknown type \"%s\" at %s line %d\n", return_type_name_word->value, return_type_name_word->op->file, return_type_name_word->op->line);
            SPerl_yyerror(parser, message);
          }
        }
        
        // Check method descripters
        SPerl_ARRAY* descripters = method->descripters;
        SPerl_int k;
        for (SPerl_int k = 0; k < descripters->length; k++) {
          SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, k);
          if (descripter->type != SPerl_DESCRIPTER_C_TYPE_STATIC)
          {
            SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_type_names[descripter->type]));
            sprintf(message, "Error: unknown descripter of sub \"%s\" at %s line %d\n",
              SPerl_DESCRIPTER_type_names[descripter->type], descripter->op->file, descripter->op->line);
            SPerl_yyerror(parser, message);
          }
        }
        
        // Check my var information
        SPerl_ARRAY* my_vars = method->my_vars;
        for (SPerl_int k = 0; k < my_vars->length; k++) {
          SPerl_MY_VAR* my_var = SPerl_ARRAY_fetch(my_vars, k);

          if (my_var->type->type == SPerl_TYPE_C_CLASS_OR_TYPEDEF) {
            SPerl_WORD* type_name_word = my_var->type->name_word;
            if (!SPerl_HASH_search(class_symtable, type_name_word->value, strlen(type_name_word->value))) {
              SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(type_name_word->value));
              sprintf(message, "Error: unknown type \"%s\" at %s line %d\n", type_name_word->value, type_name_word->op->file, type_name_word->op->line);
              SPerl_yyerror(parser, message);
            }
          }
          
          // Check my_var descripters
          SPerl_ARRAY* descripters = my_var->descripters;
          for (SPerl_int l = 0; l < descripters->length; l++) {
            SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, l);
            if (descripter->type != SPerl_DESCRIPTER_C_TYPE_CONST)
            {
              SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(SPerl_DESCRIPTER_type_names[descripter->type]));
              sprintf(message, "Error: unknown descripter of package \"%s\" at %s line %d\n",
                SPerl_DESCRIPTER_type_names[descripter->type], descripter->op->file, descripter->op->line);
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
      case SPerl_CONST_VALUE_C_TYPE_BOOLEAN:
      case SPerl_CONST_VALUE_C_TYPE_CHAR:
      case SPerl_CONST_VALUE_C_TYPE_BYTE:
      case SPerl_CONST_VALUE_C_TYPE_SHORT:
      case SPerl_CONST_VALUE_C_TYPE_INT:
        const_value->pool_pos = parser->const_pool_pos;
        *(const_pool + parser->const_pool_pos) = const_value->uv.int_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_VALUE_C_TYPE_LONG:
        const_value->pool_pos = parser->const_pool_pos;
        *(SPerl_long*)(const_pool + parser->const_pool_pos) = const_value->uv.long_value;
        parser->const_pool_pos += 2;
        break;
      case SPerl_CONST_VALUE_C_TYPE_FLOAT:
        const_value->pool_pos = parser->const_pool_pos;
        *(SPerl_float*)(const_pool + parser->const_pool_pos) = const_value->uv.float_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_VALUE_C_TYPE_DOUBLE:
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
  
  SPerl_char* class_name = ((SPerl_WORD*)op_pkgname->uv.pv)->value;

  SPerl_HASH* typemap = parser->typemap;

  SPerl_TYPE* found_typemap = SPerl_HASH_search(parser->typemap, class_name, strlen(class_name));
  
  if (found_typemap) {
    SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(class_name));
    sprintf(message, "Error: redeclaration of package \"%s\" at %s line %d\n", class_name, op_package->file, op_package->line);
    SPerl_yyerror(parser, message);
  }
  else {
    // Class or enum
    if (op_type->type == SPerl_OP_C_TYPE_NULL) {
      SPerl_CLASS* class = SPerl_CLASS_new(parser);
      class->name = op_pkgname->uv.pv;
      class->op_block = op_block;
      class->alias = SPerl_PARSER_new_hash(parser, 0);
      
      class->descripters = SPerl_OP_create_descripters(parser, op_descripters);

      // Enum
      if (op_descripters->type == SPerl_OP_C_TYPE_ENUM) {
        class->type = SPerl_CLASS_C_TYPE_ENUM;
      }
      // Normal class
      else {
        class->type = SPerl_CLASS_C_TYPE_NORMAL;
      }
      
      // Class
      if (class->type == SPerl_CLASS_C_TYPE_NORMAL) {
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
          if (op_usehassub->type == SPerl_OP_C_TYPE_USE) {
            SPerl_USE* use = op_usehassub->uv.pv;
            SPerl_char* use_class_name = use->class_name_word->value;
            SPerl_USE* found_use
              = SPerl_HASH_search(use_symtable, use_class_name, strlen(use_class_name));
            
            if (found_use) {
              SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(use_class_name));
              sprintf(message, "Error: redeclaration of use \"%s\" at %s line %d\n", use_class_name, use->op->file, use->op->line);
              SPerl_yyerror(parser, message);
            }
            else {
              SPerl_ARRAY_push(parser->use_stack, use);
              SPerl_ARRAY_push(uses, use);
              
              if (use->alias_name_word) {
                SPerl_char* alias_name = use->alias_name_word->value;
                SPerl_HASH_insert(class->alias, alias_name, strlen(alias_name), class);
              }
              SPerl_HASH_insert(use_symtable, use_class_name, strlen(use_class_name), use);
            }
          }
          // Field
          else if (op_usehassub->type == SPerl_OP_C_TYPE_HAS) {
            SPerl_FIELD* field = (SPerl_FIELD*)op_usehassub->uv.pv;
            SPerl_char* field_name = field->name_word->value;
            SPerl_FIELD* found_field
              = SPerl_HASH_search(field_symtable, field_name, strlen(field_name));
            if (found_field) {
              SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(field_name));
              sprintf(message, "Error: redeclaration of has \"%s\" at %s line %d\n", field_name, field->op->file, field->op->line);
              SPerl_yyerror(parser, message);
            }
            else {
              field->class = class;
              field->op = op_usehassub;
              SPerl_ARRAY_push(fields, field);
              SPerl_HASH_insert(field_symtable, field_name, strlen(field_name), field);
            }
          }
        }
        // Set filed and method information
        class->fields = fields;
        class->field_symtable = field_symtable;
        class->uses = uses;
        class->use_symtable = use_symtable;

        // Method information
        SPerl_HASH* method_symtable = SPerl_PARSER_new_hash(parser, 0);
        SPerl_int i;
        for (i = 0; i < parser->current_methods->length; i++) {
          SPerl_METHOD* method = SPerl_ARRAY_fetch(parser->current_methods, i);
          
          SPerl_METHOD* found_method = NULL;
          SPerl_char* method_name;
          if (!method->anon) {
            method_name = method->name_word->value;
            SPerl_METHOD* found_method
              = SPerl_HASH_search(method_symtable, method_name, strlen(method_name));
          }
          
          if (found_method) {
            SPerl_char* message = SPerl_PARSER_new_string(parser, 200 + strlen(method_name));
            sprintf(message, "Error: redeclaration of sub \"%s\" at %s line %d\n", method_name, method->op->file, method->op->line);
            SPerl_yyerror(parser, message);
          }
          else {
            method->class = class;
            if (!method->anon) {
              SPerl_HASH_insert(method_symtable, method_name, strlen(method_name), method);
            }
          }
        }
        class->methods = parser->current_methods;
        parser->current_methods = SPerl_PARSER_new_array(parser, 0);
        class->method_symtable = method_symtable;
      }
      
      // Enum
      else if (class->type == SPerl_CLASS_C_TYPE_ENUM) {
        // Values
        SPerl_ARRAY* enum_values = SPerl_PARSER_new_array(parser, 0);
        
        // Starting value
        SPerl_long start_value = 0;
        
        SPerl_OP* op_enumvalues = op_block->first;
        SPerl_OP* op_enumvalue = op_enumvalues->first;
        while (op_enumvalue = SPerl_OP_sibling(parser, op_enumvalue)) {
          SPerl_ENUM_VALUE* enum_value = SPerl_ENUM_VALUE_new(parser);
          enum_value->name_word = op_enumvalue->first->uv.pv;
          if (op_enumvalue->last) {
            enum_value->value = op_enumvalue->last->uv.pv;
          }
          
          SPerl_CONST_VALUE* const_value;
          if (enum_value->value) {
            const_value = enum_value->value;
            start_value = const_value->uv.int_value + 1;
          }
          else {
            const_value = SPerl_CONST_VALUE_new(parser);
            const_value->type = SPerl_CONST_VALUE_C_TYPE_INT;
            const_value->uv.int_value = start_value;
            enum_value->value = const_value;
            start_value++;
          }
          SPerl_ARRAY_push(parser->const_values, const_value);
          SPerl_ARRAY_push(enum_values, enum_value);
        }
        
        class->enum_values = enum_values;
      }
      
      // Add class information
      SPerl_ARRAY_push(parser->classs, class);
      SPerl_HASH_insert(parser->class_symtable, class_name, strlen(class_name), class);

      SPerl_TYPE* type = SPerl_TYPE_new(parser);
      type->type = SPerl_TYPE_C_CLASS_OR_TYPEDEF;
      type->name_word = op_pkgname->uv.pv;
      
      SPerl_HASH_insert(parser->typemap, class_name, strlen(class_name), type);
    }
    // Typedef
    else {
      // Class name
      SPerl_WORD* class_name_word = op_pkgname->uv.pv;
      SPerl_TYPE* type = op_type->uv.pv;
      SPerl_HASH_insert(parser->typemap, class_name_word->value, strlen(class_name_word->value), type);
      
      // Add use information
      SPerl_OP* op_use = SPerl_OP_newOP(parser, SPerl_OP_C_TYPE_USE, NULL, NULL);
      op_use->file = op_type->file;
      op_use->line = op_type->line;
      SPerl_USE* use = SPerl_USE_new(parser);
      use->class_name_word = class_name_word;
      op_use->uv.pv = use;
      SPerl_ARRAY_push(parser->use_stack, use);
    }
  }
  
  return op_package;
}

SPerl_OP* SPerl_OP_build_USE(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_pkgname, SPerl_OP* op_pkgalias) {
  SPerl_OP_sibling_splice(parser, op_use, NULL, 0, op_pkgname);
  SPerl_OP_sibling_splice(parser, op_use, op_pkgname, 0, op_pkgalias);
  
  SPerl_USE* use = SPerl_USE_new(parser);
  use->class_name_word = op_pkgname->uv.pv;
  if (op_pkgalias->type != SPerl_OP_C_TYPE_NULL) {
    use->alias_name_word = op_pkgalias->uv.pv;
  }
  use->op = op_use;
  op_use->uv.pv = use;
  
  return op_use;
}

SPerl_OP* SPerl_OP_build_MY(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_descripters, SPerl_OP* op_type) {

  SPerl_OP_sibling_splice(parser, op_my, NULL, 0, op_var);
  SPerl_OP_sibling_splice(parser, op_my, op_var, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_my, op_descripters, 0, op_type);
  
  // Create my var information
  SPerl_MY_VAR* my_var = SPerl_MY_VAR_new(parser);
  SPerl_VAR* var = (SPerl_VAR*)op_var->uv.pv;
  my_var->name_word = var->name_word;
  
  // descripters
  my_var->descripters = SPerl_OP_create_descripters(parser, op_descripters);
  
  // type
  my_var->type = op_type->uv.pv;
  
  // Add my_var information to op
  op_my->uv.pv = my_var;
  
  return op_my;
}

SPerl_OP* SPerl_OP_build_HAS(SPerl_PARSER* parser, SPerl_OP* op_has, SPerl_OP* op_field_name, SPerl_OP* op_descripters, SPerl_OP* op_type) {
  
  // Build OP
  SPerl_OP_sibling_splice(parser, op_has, NULL, 0, op_field_name);
  SPerl_OP_sibling_splice(parser, op_has, op_field_name, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_has, op_descripters, 0, op_type);
  
  // Create field information
  SPerl_FIELD* field = SPerl_FIELD_new(parser);
  
  // Name
  field->name_word = op_field_name->uv.pv;

  // Descripters
  field->descripters = SPerl_OP_create_descripters(parser, op_descripters);
  
  // Type
  field->type = op_type->uv.pv;
  
  // Set field informaiton
  op_has->uv.pv = field;
  
  return op_has;
}

SPerl_OP* SPerl_OP_build_CONST_VALUE(SPerl_PARSER* parser, SPerl_OP* op_const) {

  SPerl_CONST_VALUE* const_value = op_const->uv.pv;
  SPerl_ARRAY_push(parser->const_values, const_value);
  
  return op_const;
}

SPerl_ARRAY* SPerl_OP_create_descripters(SPerl_PARSER* parser, SPerl_OP* op_descripters) {
  
  SPerl_ARRAY* descripters = SPerl_PARSER_new_array(parser, 0);

  // descripters is enum or list of descripter
  if (op_descripters->type == SPerl_OP_C_TYPE_ENUM) {
    SPerl_DESCRIPTER* descripter = SPerl_DESCRIPTER_new(parser);
    descripter->type = SPerl_DESCRIPTER_C_TYPE_ENUM;
    SPerl_ARRAY_push(descripters, descripter);
  }
  else {
    SPerl_OP* op_descripter = op_descripters->first;
    while (op_descripter = op_descripter->moresib ? op_descripter->sibparent : NULL) {
      SPerl_ARRAY_push(descripters, op_descripter->uv.pv);
    }
  }
  
  return descripters;
}

SPerl_OP* SPerl_OP_build_SUB(SPerl_PARSER* parser, SPerl_OP* op_sub, SPerl_OP* op_subname, SPerl_OP* op_subargs, SPerl_OP* op_descripters, SPerl_OP* op_type, SPerl_OP* op_block) {
  
  // Build OP_SUB
  SPerl_OP_sibling_splice(parser, op_sub, NULL, 0, op_subname);
  SPerl_OP_sibling_splice(parser, op_sub, op_subname, 0, op_subargs);
  SPerl_OP_sibling_splice(parser, op_sub, op_subargs, 0, op_descripters);
  SPerl_OP_sibling_splice(parser, op_sub, op_descripters, 0, op_type);
  SPerl_OP_sibling_splice(parser, op_sub, op_type, 0, op_block);
  
  // Create method infomation
  SPerl_METHOD* method = SPerl_METHOD_new(parser);
  if (op_subname->type == SPerl_OP_C_TYPE_NULL) {
    method->anon = 1;
  }
  else {
    method->name_word = op_subname->uv.pv;
  }
  
  // subargs
  SPerl_int argument_count = 0;
  SPerl_OP* op_subarg = op_subargs->first;
  while (op_subarg = SPerl_OP_sibling(parser, op_subarg)) {
    // Increment argument count
    argument_count++;
  }
  method->argument_count = argument_count;

  // descripters
  method->descripters = SPerl_OP_create_descripters(parser, op_descripters);
  
  // return type
  method->return_type = op_type->uv.pv;
  
  // Save block
  method->op_block = op_block;
  
  // Add method information
  SPerl_ARRAY_push(parser->current_methods, method);
  
  // my var informations
  SPerl_ARRAY* my_vars = SPerl_PARSER_new_array(parser, 0);
  SPerl_HASH* my_var_symtable = SPerl_PARSER_new_hash(parser, 0);
  
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
    if (op_cur->type == SPerl_OP_C_TYPE_VAR) {
      SPerl_VAR* var = (SPerl_VAR*)op_cur->uv.pv;
      
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
        sprintf(message, "Error: \"%s\" undeclared at %s line %d\n", var->name_word->value, op_cur->file, op_cur->line);
        SPerl_yyerror(parser, message);
      }
    }
    else if (op_cur->type == SPerl_OP_C_TYPE_MY) {
      SPerl_MY_VAR* my_var = (SPerl_MY_VAR*)op_cur->uv.pv;
      
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
        my_var->method = method;
        
        SPerl_ARRAY_push(my_var_stack, my_var);
      }
    }
    else if (op_cur->type == SPerl_OP_C_TYPE_BLOCK) {
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
            if (op_parent->type == SPerl_OP_C_TYPE_BLOCK) {
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
  method->my_vars = my_vars;
  
  op_sub->uv.pv = method;
  
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
      first->type = SPerl_OP_C_TYPE_NULL;
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
  
  if (first->type == SPerl_OP_C_TYPE_LIST) {
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
  
  SPerl_OP* op_pushmark = SPerl_OP_newOP(parser, SPerl_OP_C_TYPE_PUSHMARK, NULL, NULL);
  
  return SPerl_OP_newOP(parser, SPerl_OP_C_TYPE_LIST, op_pushmark, NULL);
}

SPerl_OP* SPerl_OP_newOP_NULL(SPerl_PARSER* parser) {
  
  return SPerl_OP_newOP(parser, SPerl_OP_C_TYPE_NULL, NULL, NULL);
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
