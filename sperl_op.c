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

SPerl_char* const SPerl_OP_C_CODE_NAMES[] = {
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
  "getfield"
};

void SPerl_OP_check(SPerl_PARSER* parser) {
  
  // Check packages
  SPerl_OP_check_packages(parser);
  
  // Check bodys
  SPerl_OP_check_bodys(parser);
  
  // Check names
  SPerl_OP_check_names(parser);
}

void SPerl_OP_check_packages(SPerl_PARSER* parser) {
  // Resolve package type
  SPerl_ARRAY* packages = parser->packages;
  SPerl_HASH* package_symtable = parser->package_symtable;
  for (SPerl_int i = 0; i < packages->length; i++) {
    SPerl_PACKAGE* package = SPerl_ARRAY_fetch(packages, i);
    SPerl_TYPE* type = package->type;
    SPerl_OP_resolve_type(parser, type);
  }
}

void SPerl_OP_check_bodys(SPerl_PARSER* parser) {
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

        // Resolve field type
        SPerl_OP_resolve_type(parser, field->type);
        
        // Check field descripters
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
      
      // Check sub
      SPerl_ARRAY* subs = body_class->subs;
      for (SPerl_int j = 0; j < subs->length; j++) {
        SPerl_SUB* sub = SPerl_ARRAY_fetch(subs, j);

        // Resolve sub return type
        SPerl_OP_resolve_type(parser, sub->return_type);
        
        // Check sub descripters
        SPerl_ARRAY* descripters = sub->descripters;
        SPerl_int k;
        for (SPerl_int k = 0; k < descripters->length; k++) {
          SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, k);
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
        for (SPerl_int k = 0; k < my_vars->length; k++) {
          SPerl_MY_VAR* my_var = SPerl_ARRAY_fetch(my_vars, k);
          SPerl_OP_resolve_type(parser, my_var->type);
          
          // Check my_var descripters
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
  }
}

void SPerl_OP_check_names(SPerl_PARSER* parser) {
  // Check names
  SPerl_ARRAY* name_checked_ops = parser->name_checked_ops;
  
  for (SPerl_int i = 0; i < name_checked_ops->length; i++) {
    SPerl_OP* op = SPerl_ARRAY_fetch(name_checked_ops, i);
    SPerl_NAME* name = op->uv.pv;
    
    
    // Check sub name
    if (op->code == SPerl_OP_C_CODE_CALLSUB) {
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
    // Check enum names
    else if (op->code == SPerl_OP_C_CODE_GETENUMVALUE) {
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
    else if (op->code == SPerl_OP_C_CODE_GETFIELD) {
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
  }
}

SPerl_OP* SPerl_OP_build_getenumvalue(SPerl_PARSER* parser, SPerl_OP* op_enumname) {
  SPerl_OP* op_getenumvalue = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_GETENUMVALUE, op_enumname, NULL);
  
  SPerl_NAME* name = SPerl_NAME_new(parser);
  name->code = SPerl_NAME_C_CODE_ENUM;
  name->abs_name_word = op_enumname->uv.pv;
  
  SPerl_ARRAY_push(parser->name_checked_ops, op_getenumvalue);
  
  op_getenumvalue->uv.pv = name;
  
  return op_getenumvalue;
}

SPerl_OP* SPerl_OP_build_getfield(SPerl_PARSER* parser, SPerl_OP* op_var, SPerl_OP* op_fieldname) {
  SPerl_OP* op_getfield = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_GETFIELD, NULL, NULL);
  SPerl_OP_sibling_splice(parser, op_getfield, NULL, 0, op_var);
  SPerl_OP_sibling_splice(parser, op_getfield, op_var, 0, op_fieldname);
  
  SPerl_NAME* name = SPerl_NAME_new(parser);
  name->code = SPerl_NAME_C_CODE_FIELD;
  name->var = op_var->uv.pv;
  name->base_name_word = op_fieldname->uv.pv;
  
  SPerl_ARRAY_push(parser->name_checked_ops, op_getfield);
  
  op_getfield->uv.pv = name;
  
  return op_getfield;
}

SPerl_OP* SPerl_OP_build_grammer(SPerl_PARSER* parser, SPerl_OP* op_packages) {
  SPerl_OP* op_grammer = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_GRAMMER, op_packages, NULL);
  parser->op_grammer = op_grammer;
  
  // Resovle types, check types and descripters.
  SPerl_OP_check(parser);
  
  // Build constant pool
  SPerl_OP_build_const_pool(parser);
  
  return op_grammer;
}

// Resolve type and index type
void SPerl_OP_resolve_type(SPerl_PARSER* parser, SPerl_TYPE* type) {
  SPerl_HASH* package_symtable = parser->package_symtable;
  
  if (type->resolved_string) {
    return;
  }
  else {
    SPerl_ARRAY* parts = type->parts;
    SPerl_ARRAY* resolved_parts = SPerl_PARSER_new_array(parser, 0);
    
    for (SPerl_int i = 0; i < parts->length; i++) {
      SPerl_TYPE_PART* part = SPerl_ARRAY_fetch(parts, i);
      if (part->code == SPerl_TYPE_PART_C_CODE_SUB) {
        type->resolved_string_length += 3;
        SPerl_ARRAY_push(type->resolved_part_names, "sub");
      }
      else if (part->code == SPerl_TYPE_PART_C_CODE_CHAR) {
        type->resolved_string_length++;
        SPerl_ARRAY_push(type->resolved_part_names, part->uv.char_name);
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
            type->resolved_string_length += strlen(found_type->uv.type_word->name_word->value);
            SPerl_char* found_part_name = found_type->uv.type_word->name_word->value;
            SPerl_ARRAY_push(type->resolved_part_names, found_part_name);
          }
          else {
            SPerl_OP_resolve_type(parser, found_type);
            type->resolved_string_length += found_type->resolved_string_length;
            for (SPerl_int j = 0; j < found_type->resolved_part_names->length; j++) {
              SPerl_char* found_part_name = SPerl_ARRAY_fetch(found_type->resolved_part_names, j);
              SPerl_ARRAY_push(type->resolved_part_names, found_part_name);
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
    SPerl_char* resolved_string = SPerl_PARSER_new_string(parser, type->resolved_string_length);
    SPerl_int cur_pos = 0;
    for (SPerl_int i = 0; i < type->resolved_part_names->length; i++) {
      SPerl_char* resolved_part_name = SPerl_ARRAY_fetch(type->resolved_part_names, i);
      SPerl_int resolved_part_name_length = strlen(resolved_part_name);
      memcpy(resolved_string + cur_pos, resolved_part_name, resolved_part_name_length);
      cur_pos += resolved_part_name_length;
    }
    type->resolved_string = resolved_string;
    
    // Create type id
    SPerl_int* id = SPerl_HASH_search(parser->type_resolved_string_symtable, resolved_string, strlen(resolved_string));
    if (id) {
      type->id = *id;
    }
    else {
      SPerl_int* new_id = SPerl_PARSER_new_int(parser);
      type->id = *new_id = parser->current_type_id++;
      SPerl_HASH_insert(parser->type_resolved_string_symtable, resolved_string, strlen(resolved_string), new_id);
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
    switch(const_value->code) {
      case SPerl_CONST_VALUE_C_CODE_BOOLEAN:
      case SPerl_CONST_VALUE_C_CODE_CHAR:
      case SPerl_CONST_VALUE_C_CODE_BYTE:
      case SPerl_CONST_VALUE_C_CODE_SHORT:
      case SPerl_CONST_VALUE_C_CODE_INT:
        const_value->pool_pos = parser->const_pool_pos;
        *(const_pool + parser->const_pool_pos) = const_value->uv.int_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_VALUE_C_CODE_LONG:
        const_value->pool_pos = parser->const_pool_pos;
        *(SPerl_long*)(const_pool + parser->const_pool_pos) = const_value->uv.long_value;
        parser->const_pool_pos += 2;
        break;
      case SPerl_CONST_VALUE_C_CODE_FLOAT:
        const_value->pool_pos = parser->const_pool_pos;
        *(SPerl_float*)(const_pool + parser->const_pool_pos) = const_value->uv.float_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_VALUE_C_CODE_DOUBLE:
        const_value->pool_pos = parser->const_pool_pos;
        *(SPerl_double*)(const_pool + parser->const_pool_pos) = const_value->uv.double_value;
        parser->const_pool_pos += 2;
        break;
    }
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
  
  SPerl_WORD* package_name_word = op_packagename->uv.pv;
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
            const_value->code = SPerl_CONST_VALUE_C_CODE_INT;
            const_value->uv.int_value = start_value;
            enum_value->value = const_value;
            start_value++;
          }
          SPerl_ARRAY_push(parser->const_values, const_value);
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
        body->code = SPerl_BODY_C_CODE_CLASS;

        SPerl_BODY_CLASS* body_class = SPerl_BODY_CLASS_new(parser);
        body_class->op_block = op_block;
        body_class->alias = SPerl_PARSER_new_hash(parser, 0);
        body_class->descripters = SPerl_OP_create_descripters(parser, op_descripters);
        body_class->code = SPerl_BODY_CLASS_C_CODE_NORMAL;
        
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
            SPerl_USE* use = op_usehassub->uv.pv;
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
        SPerl_int i;
        for (i = 0; i < parser->current_subs->length; i++) {
          SPerl_SUB* sub = SPerl_ARRAY_fetch(parser->current_subs, i);
          
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
          }
          sub->body_class = body_class;
          sub->package_name = package_name;
        }
        
        body_class->subs = parser->current_subs;
        parser->current_subs = SPerl_PARSER_new_array(parser, 0);
        
        // Set body
        body->uv.body_class = body_class;
      }
      SPerl_ARRAY_push(parser->bodys, body);
      SPerl_HASH_insert(parser->body_symtable, body->name, strlen(body->name), body);
    }
    // Typedef
    else {
      type = op_type->uv.pv;
    }
    
    SPerl_TYPE_build_parts(parser, type);
    package->type = type;
    
    // Add type information
    SPerl_ARRAY_push(parser->packages, package);
    SPerl_HASH_insert(parser->package_symtable, package_name, strlen(package_name), type);
  }
  
  return op_package;
}

SPerl_OP* SPerl_OP_build_decluse(SPerl_PARSER* parser, SPerl_OP* op_use, SPerl_OP* op_packagename, SPerl_OP* op_packagealias) {
  SPerl_OP_sibling_splice(parser, op_use, NULL, 0, op_packagename);
  SPerl_OP_sibling_splice(parser, op_use, op_packagename, 0, op_packagealias);
  
  SPerl_USE* use = SPerl_USE_new(parser);
  use->package_name_word = op_packagename->uv.pv;
  use->op = op_use;
  op_use->uv.pv = use;
  
  return op_use;
}

SPerl_OP* SPerl_OP_build_declmy(SPerl_PARSER* parser, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_descripters, SPerl_OP* op_type) {

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
  SPerl_TYPE_build_parts(parser, my_var->type);
  
  // Add my_var information to op
  op_my->uv.pv = my_var;
  
  return op_my;
}

SPerl_OP* SPerl_OP_build_declhas(SPerl_PARSER* parser, SPerl_OP* op_has, SPerl_OP* op_field_name, SPerl_OP* op_descripters, SPerl_OP* op_type) {
  
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
  
  // Create type string parts
  SPerl_TYPE_build_parts(parser, field->type);
  
  // Set field informaiton
  op_has->uv.pv = field;
  
  return op_has;
}

SPerl_OP* SPerl_OP_build_CONSTVALUE(SPerl_PARSER* parser, SPerl_OP* op_const) {

  SPerl_CONST_VALUE* const_value = op_const->uv.pv;
  SPerl_ARRAY_push(parser->const_values, const_value);
  
  return op_const;
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
    while (op_descripter = op_descripter->moresib ? op_descripter->sibparent : NULL) {
      SPerl_ARRAY_push(descripters, op_descripter->uv.pv);
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
    sub->name_word = op_subname->uv.pv;
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
  sub->return_type = op_type->uv.pv;
  SPerl_TYPE_build_parts(parser, sub->return_type);
  
  // Save block
  sub->op_block = op_block;
  
  // Save op
  sub->op = op_sub;
  
  // Add sub information
  SPerl_ARRAY_push(parser->current_subs, sub);
  SPerl_ARRAY_push(parser->subs, sub);
  
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
    if (op_cur->code == SPerl_OP_C_CODE_VAR) {
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
    else if (op_cur->code == SPerl_OP_C_CODE_MY) {
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
        my_var->sub = sub;
        
        SPerl_ARRAY_push(my_var_stack, my_var);
      }
    }
    else if (op_cur->code == SPerl_OP_C_CODE_BLOCK) {
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
            if (op_parent->code == SPerl_OP_C_CODE_BLOCK) {
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
  sub->my_vars = my_vars;
  
  // ID
  sub->id = parser->current_sub_id++;
  
  op_sub->uv.pv = sub;
  
  return op_sub;
}

SPerl_OP* SPerl_OP_build_callsub(SPerl_PARSER* parser, SPerl_OP* op_invocant, SPerl_OP* op_subname, SPerl_OP* op_terms, SPerl_boolean anon) {
  
  // Build OP_SUB
  SPerl_OP* op_callsub = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_CALLSUB, NULL, NULL);
  SPerl_OP_sibling_splice(parser, op_callsub, NULL, 0, op_invocant);
  SPerl_OP_sibling_splice(parser, op_callsub, op_invocant, 0, op_subname);
  SPerl_OP_sibling_splice(parser, op_callsub, op_subname, 0, op_terms);
  
  SPerl_NAME* name = SPerl_NAME_new(parser);
  name->code = SPerl_NAME_C_CODE_SUB;
  
  if (!anon) {
    SPerl_WORD* sub_name_word = op_subname->uv.pv;
    SPerl_char* sub_name = sub_name_word->value;
    
    if (strstr(sub_name, ":")) {
      name->abs_name_word = sub_name_word;
    }
    else {
      if (op_invocant->code == SPerl_OP_C_CODE_VAR) {
        name->var = op_invocant->uv.pv;
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
  
  name->anon = anon;
  
  // Argument count
  SPerl_int argument_count = 0;
  SPerl_OP* op_term = op_terms->first;
  while (op_term = SPerl_OP_sibling(parser, op_term)) {
    argument_count++;
  }
  name->argument_count = argument_count;
  
  if (!anon) {
    SPerl_ARRAY_push(parser->name_checked_ops, op_callsub);
  }
  
  op_callsub->uv.pv = name;
  
  return op_callsub;
}

SPerl_OP* SPerl_OP_build_subtype(SPerl_PARSER* parser, SPerl_OP* op_argument_types, SPerl_OP* op_return_type) {
  SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, op_argument_types, op_return_type);
  
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_SUB;
  
  // sub type
  SPerl_TYPE_SUB* type_sub = SPerl_TYPE_SUB_new(parser);
  type_sub->return_type = op_return_type->uv.pv;
  SPerl_ARRAY* argument_types = SPerl_PARSER_new_array(parser, 0);
  {
    SPerl_OP* op_argument_type = op_argument_types->first;
    while (op_argument_type = SPerl_OP_sibling(parser, op_argument_type)) {
      SPerl_ARRAY_push(argument_types, op_argument_type->uv.pv);
    }
  }
  type_sub->argument_types = argument_types;
  
  type->uv.type_sub = type_sub;
  
  op_type->uv.pv = type;
  
  return op_type;
}

SPerl_OP* SPerl_OP_build_wordtype(SPerl_PARSER* parser, SPerl_OP* op_wordtype) {
  SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, op_wordtype, NULL);
  
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_WORD;
  
  SPerl_TYPE_WORD* type_word = SPerl_TYPE_WORD_new(parser);
  type_word->name_word = op_wordtype->uv.pv;
  type->uv.type_word = type_word;
  
  op_type->uv.pv = type;
  
  return op_type;
}

SPerl_OP* SPerl_OP_build_arraytype(SPerl_PARSER* parser, SPerl_OP* op_type) {
  SPerl_OP* op_type_array = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, op_type, NULL);
  
  // Type array
  SPerl_TYPE_ARRAY* type_array = SPerl_TYPE_ARRAY_new(parser);
  type_array->type = op_type->uv.pv;
  
  // Type
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_ARRAY;
  type->uv.type_array = type_array;
  
  op_type_array->uv.pv = type;
  
  return op_type_array;
}

SPerl_OP* SPerl_OP_newOP(SPerl_PARSER* parser, SPerl_char type, SPerl_OP* first, SPerl_OP* last) {
  return SPerl_OP_newOP_flag(parser, type, first, last, 0, 0);
}

SPerl_OP* SPerl_OP_newOP_flag(SPerl_PARSER* parser, SPerl_char code, SPerl_OP* first, SPerl_OP* last, SPerl_char flags, SPerl_char private) {
        
  SPerl_OP *op = (SPerl_OP*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_OP));
  
  memset(op, 0, sizeof(SPerl_OP));
  
  op->code = code;
  op->first = first;
  op->flags = flags;
  op->private = private;
  
  if (last) {
    if (!first) {
      first = (SPerl_OP*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_OP));
      first->code = SPerl_OP_C_CODE_NULL;
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
